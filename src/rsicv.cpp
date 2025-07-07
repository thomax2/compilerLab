#include "rsicv.h"

int BaseRSICV::func_size(const koopa_raw_function_t &func, bool *ra_flag, int *max_len) {
    int size = 0;
    int call_num = 0;
    *max_len = 0;
    for(int i = 0; i< func->bbs.len; i++) {
        size += blk_size(reinterpret_cast<koopa_raw_basic_block_t>(func->bbs.buffer[i]), &call_num, max_len);
    }
    
    if(call_num > 0) {
        size += 4;
        *ra_flag = true;
    }

    if(*max_len > 8)
        size += (*max_len -8)*4;

    
    return size;
}

int BaseRSICV::blk_size(koopa_raw_basic_block_t block, int *call_num, int *max_len) {
    int size = 0;
    for(int i = 0; i < block->insts.len; i++) {
        // assert(block->insts.buffer[i] != nullptr);
        size += inst_size(reinterpret_cast<koopa_raw_value_t>(block->insts.buffer[i]), call_num, max_len);
    }
    return size;
}

int BaseRSICV::inst_size(koopa_raw_value_t inst, int *call_num, int *max_len) {
    // assert(inst->ty != nullptr);
    switch (inst->kind.tag) {
        case KOOPA_RVT_CALL:
            *call_num = *call_num + 1;
            if(inst->kind.data.call.args.len > *max_len)
                *max_len = inst->kind.data.call.args.len;
            break;
        default:
            break;
    }
    switch (inst->ty->tag) {
    case KOOPA_RTT_INT32:
        return 4;
    case KOOPA_RTT_UNIT:
        return 0;
    case KOOPA_RTT_POINTER:
        return 4;
    case KOOPA_RTT_ARRAY:
        return array_size_cal(inst->ty);
    default:
        return 0;
    }
    return 0;
}

int BaseRSICV::type_size(koopa_raw_type_t ty) {
    switch (ty->tag) {
    case KOOPA_RTT_INT32:
        return 4;
    case KOOPA_RTT_UNIT:
        return 0;
    case KOOPA_RTT_POINTER:
        return 4;
    case KOOPA_RTT_ARRAY:
        return array_size_cal(ty);
    default:
        return 0;
    }
}

void BaseRSICV::Env::reg_init() {
    for(size_t i = 0; i< 8; i++) {
        reg_status_map[ "t" + std::to_string(i) ] = UNUSED;
        if(i != 7)
            reg_status_map[ "a" + std::to_string(i) ] = UNUSED;
    }
}


// alloc arbitrary reg to value
std::string BaseRSICV::Env::reg_alloc(koopa_raw_value_t value) {
    if( reg_insts_map.find(value) != reg_insts_map.end() ) {
        return reg_insts_map[value];
    }

    for (size_t i = 0; i < 8; i++) {
        if(reg_status_map[ "t" + std::to_string(i) ] == UNUSED) {
            reg_status_map[ "t" + std::to_string(i) ] = USED;
            reg_insts_map[value] = "t" + std::to_string(i);
            return "t" + std::to_string(i);
        }
    }
    
    for (size_t i = 0; i < 7; i++) {
        if(reg_status_map[ "a" + std::to_string(i) ] == UNUSED) {
            reg_status_map[ "a" + std::to_string(i) ] = USED;
            reg_insts_map[value] = "a" + std::to_string(i);
            return "a" + std::to_string(i);
        }
    }
    assert(false);
    return "NULL";
}


// alloc certain reg to value
std::string BaseRSICV::reg_load(koopa_raw_value_t value, std::string reg) {
    // assert(reg_status_map[reg] == UNUSED);
    std::string ret = "";
    // find
    if(value->kind.tag == KOOPA_RVT_INTEGER) {
        ret += "\tli " + reg + ", " + std::to_string(value->kind.data.integer.value) + "\n";
    } else {
        int offset = env.get_offset(value);
        if(offset < 2048 && offset >= -2048)
            ret += "\tlw " + reg + ", " + std::to_string(offset) + "(sp)\n";
        else {
            ret += "\tli t3, " + std::to_string(offset) + "\n";
            ret += "\tadd t3, sp, t3\n";
            ret += "\tlw " + reg + ", 0(t3)\n";
        }
    }
    return ret;
}

void BaseRSICV::Env::reg_free(std::string reg) {
    reg_status_map[reg] = UNUSED;
}

void BaseRSICV::Env::stack_init(int size, int max_len) {
    stack_size = size;
    if(max_len <= 8)
        cur_offset = 0;
    else
        cur_offset = (max_len-8)*4;
    stack_offset_map.clear();
}

int BaseRSICV::Env::get_offset(koopa_raw_value_t value) {
    assert(value != nullptr);
    if(stack_offset_map.find(value) != stack_offset_map.end())
        return stack_offset_map[value];
    else {
        int size;
        if(value->kind.tag == KOOPA_RVT_ALLOC)
            size = base_rsi_cv->type_size(value->ty->data.pointer.base);
        else
            size = base_rsi_cv->type_size(value->ty);
        if(size == 0)
            return -1;
        stack_offset_map[value] = cur_offset;
        cur_offset += size;
        return stack_offset_map[value];
    }
    return -1;
}

std::string BaseRSICV::store_offset(int addr, std::string reg) {
    std::string ret = "";
    if(addr < 2048 && addr >= -2048)
        ret += "\tsw " + reg + ", " + std::to_string(addr) + "(sp)\n";
    else {
        ret += "\tli t3, " + std::to_string(addr) + "\n";
        ret += "\tadd t3, sp, t3\n";
        ret += "\tsw " + reg + ", 0(t3)\n";
    }
    return ret;
}


int BaseRSICV::array_size_cal(koopa_raw_type_t ty) {
    if(ty->tag == KOOPA_RTT_ARRAY)
        return array_size_cal(ty->data.array.base) * ty->data.array.len;
    else
        return 4;
}


void BaseRSICV::build(koopa_raw_program_t raw_prog) {
    out << Visit(raw_prog);
    out.close();
}

std::string BaseRSICV::Visit(const koopa_raw_program_t &program) {
    std::string ret = "";
    // 访问所有全局变量
    if(program.values.len > 0) {
        ret += "\t.data\n";
        ret += Visit(program.values);
    }
    // 访问所有函数
    if(program.funcs.len > 0) {
        
        ret += Visit(program.funcs);
    }
    return ret;
}

std::string BaseRSICV::Visit(const koopa_raw_slice_t &slice) {
    std::string ret = "";
    for (size_t i = 0; i < slice.len; i++) {
        auto ptr = slice.buffer[i];

        switch (slice.kind)
        {
        case KOOPA_RSIK_FUNCTION:
            ret += Visit(reinterpret_cast<koopa_raw_function_t>(ptr));
            break;

        case KOOPA_RSIK_BASIC_BLOCK:
            ret += Visit(reinterpret_cast<koopa_raw_basic_block_t>(ptr));
            break;
        
        case KOOPA_RSIK_VALUE:
            ret += Visit(reinterpret_cast<koopa_raw_value_t>(ptr));
            break;
        
        case KOOPA_RSIK_TYPE:
            // TODO
            break;

        default:
            assert(false);
        }
    }
    return ret;
}

std::string BaseRSICV::Visit(const koopa_raw_function_t &func) {
    std::string ret = "";
    if(func->bbs.len == 0)
        return ret;
    env.call_in_func = false;
    bool ra_flag = false;
    int max_len = 0;
    ret += "\t.text\n";
    ret += "\t.global " + std::string(func->name + 1) + "\n";
    ret += std::string(func->name + 1) + ":\n";

    int sub_sp = func_size(func, &ra_flag,&max_len);
    if(sub_sp != 0) {
        if(sub_sp % 16 != 0)
        sub_sp += 16 - (sub_sp % 16);
        env.stack_init(sub_sp, max_len);
        if(sub_sp < 2048 && sub_sp >= -2048)
            ret += "\taddi sp, sp, -" + std::to_string(sub_sp) + "\n";
        else {
            ret += "\tli t0, " + std::to_string(sub_sp) + "\n";
            ret += "\tadd sp, sp, t0\n";
        }

        if(ra_flag == true) {
            ret += store_offset(sub_sp-4, "ra");
            env.call_in_func = true;
        }
    }
    ret += Visit(func->bbs);
    return ret;
}

std::string BaseRSICV::Visit(const koopa_raw_basic_block_t &block) {
    std::string ret = "";

    if(block->name != "\%entry")
        ret += std::string(block->name + 1) + ":\n";
    ret += Visit(block->insts);
    return ret;
}

// 访问指令
std::string BaseRSICV::Visit(const koopa_raw_value_t &value) {
    std::string ret = "";
    const auto &kind = value->kind;
    current_inst = value;
    
    switch (kind.tag)
    {
    case KOOPA_RVT_INTEGER:
        // ret += Visit(kind.data.integer);
        break;
    
    case KOOPA_RVT_BINARY:
        ret += Visit(kind.data.binary);
        break;

    case KOOPA_RVT_RETURN:
        ret += Visit(kind.data.ret);
        break;

    case KOOPA_RVT_ALLOC:
        break;

    case KOOPA_RVT_LOAD:
        ret += Visit(kind.data.load);
        break;

    case KOOPA_RVT_STORE:
        ret += Visit(kind.data.store);
        break;
    
    case KOOPA_RVT_BRANCH:
        ret += Visit(kind.data.branch);
        break;

    case KOOPA_RVT_JUMP:
        ret += Visit(kind.data.jump);
        break;
    
    case KOOPA_RVT_CALL:
        ret += Visit(kind.data.call);
        break;

    case KOOPA_RVT_GLOBAL_ALLOC:
        ret += Visit_GLOBAL(value);
        break;
    
    case KOOPA_RVT_GET_ELEM_PTR:
        ret += Visit(kind.data.get_elem_ptr);
        break;

    case KOOPA_RVT_GET_PTR:
        ret += Visit(kind.data.get_ptr);
        break;

    case KOOPA_RVT_AGGREGATE:
        ret += Visit(kind.data.aggregate);
        break;

    default:
        break;
    }
    return ret;
}

std::string BaseRSICV::Visit(const koopa_raw_integer_t &interger) {
    // out << interger.value;
    return std::to_string(interger.value);
}

std::string BaseRSICV::Visit(const koopa_raw_binary_t &binary) {
    std::string ret = "";
    
    std::string rd = "t0";
    std::string rs1 = "t0";
    std::string rs2 = "t1";
    int addr = env.get_offset(current_inst);

    ret += reg_load(binary.lhs, rs1);
    ret += reg_load(binary.rhs, rs2);

    switch (binary.op)
    {
    case KOOPA_RBO_NOT_EQ:
        ret += "\txor " + rd + ", " + rs1 + ", " + rs2 + "\n";
        ret += "\tsnez " + rd + ", " + rd + "\n";
        break;
    
    case KOOPA_RBO_EQ:
        ret += "\txor " + rd + ", " + rs1 + ", " + rs2 + "\n";
        ret += "\tseqz " + rd + ", " + rd + "\n";
        break;

    case KOOPA_RBO_GT:
        ret += "\tsgt " + rd + ", " + rs1 + ", " + rs2 + "\n"; 
        break;
    
    case KOOPA_RBO_LT:
        ret += "\tslt " + rd + ", " + rs1 + ", " + rs2 + "\n";
        break;
    
    case KOOPA_RBO_GE:
        ret += "\tslt " + rd + ", " + rs1 + ", " + rs2 + "\n";
        ret += "\tseqz " + rd + ", " + rd + "\n";
        break;
    
    case KOOPA_RBO_LE:
        ret += "\tsgt " + rd + ", " + rs1 + ", " + rs2 + "\n";
        ret += "\tseqz " + rd + ", " + rd + "\n";
        break;
    
    case KOOPA_RBO_ADD:
        ret += "\tadd " + rd + ", " + rs1 + ", " + rs2 + "\n";
        break;
    
    case KOOPA_RBO_SUB:
        ret += "\tsub " + rd + ", " + rs1 + ", " + rs2 + "\n";
        break;

    case KOOPA_RBO_MUL:
        ret += "\tmul " + rd + ", " + rs1 + ", " + rs2 + "\n";
        break;
    
    case KOOPA_RBO_DIV:
        ret += "\tdiv " + rd + ", " + rs1 + ", " + rs2 + "\n";
        break;
    
    case KOOPA_RBO_MOD:
        ret += "\trem " + rd + ", " + rs1 + ", " + rs2 + "\n";
        break;
    
    case KOOPA_RBO_AND:
        ret += "\tand " + rd + ", " + rs1 + ", " + rs2 + "\n";
        break;
    
    case KOOPA_RBO_OR:
        ret += "\tor " + rd + ", " + rs1 + ", " + rs2 + "\n";
        break;
    
    case KOOPA_RBO_XOR:
        ret += "\txor " + rd + ", " + rs1 + ", " + rs2 + "\n";
        break;

    default:
        break;
    }
    ret += store_offset(addr, rd);
    return ret;
}


std::string BaseRSICV::Visit(const koopa_raw_return_t &raw) {
    std::string ret = "";
    if(raw.value == nullptr) {
        if(env.call_in_func == true) {
            if(env.stack_size - 4 < 2048 && env.stack_size - 4 >= -2048)
                ret += "\tlw ra, " + std::to_string(env.stack_size - 4) + "(sp)\n";
            else {
                ret += "\tli t0," + std::to_string(env.stack_size - 4) + "\n";
                ret += "\tadd t0, sp, t0\n";
                ret += "\tlw ra, 0(t0)\n";
            }
        }
        if(env.stack_size > 0){
            if(env.stack_size < 2048 && env.stack_size >= -2048)
                ret += "\taddi sp, sp, " + std::to_string(env.stack_size) + "\n";
            else {
                ret += "\tli t0, " + std::to_string(env.stack_size) + "\n";
                ret += "\tadd sp, sp, t0\n";
            }
        }
        ret += "\tret\n\n";
        return ret;
    }

    // int offset = env.get_offset(raw.value);
    // if(raw.value->kind.tag == KOOPA_RVT_LOAD)
    //     ret += reg_load(raw.value->kind.data.load.src, "a0");
    // else
        ret += reg_load(raw.value, "a0");
    if(env.call_in_func == true) {
        if(env.stack_size - 4 < 2048 && env.stack_size - 4 >= -2048)
            ret += "\tlw ra, " + std::to_string(env.stack_size - 4) + "(sp)\n";
        else {
            ret += "\tli t0," + std::to_string(env.stack_size - 4) + "\n";
            ret += "\tadd t0, sp, t0\n";
            ret += "\tlw ra, 0(t0)\n";
        }
    }
    if(env.stack_size > 0){
        if(env.stack_size < 2048 && env.stack_size >= -2048)
            ret += "\taddi sp, sp, " + std::to_string(env.stack_size) + "\n";
        else {
            ret += "\tli t0, " + std::to_string(env.stack_size) + "\n";
            ret += "\tadd sp, sp, t0\n";
        }
    }
    ret += "\tret\n\n";
    return ret;
}

std::string BaseRSICV::Visit(const koopa_raw_load_t &load) {
    std::string ret = "";
    std::string rd = "t0";
    int addr = env.get_offset(current_inst);
    if(load.src->kind.tag == KOOPA_RVT_GLOBAL_ALLOC) {
        ret += "\tla t0, " + std::string(load.src->name + 1) + "\n";
        ret += "\tlw t0, 0(t0)\n";
        ret += store_offset(addr, rd);
        return ret;
    }

    ret += reg_load(load.src, rd);
    ret += store_offset(addr, rd);
    return ret;
}


std::string BaseRSICV::Visit(const koopa_raw_store_t &store) {
    std::string ret = "";
    std::string rd = "t0";

    if(store.dest->kind.tag == KOOPA_RVT_GLOBAL_ALLOC) {
        ret += "\tla t1, " + std::string(store.dest->name + 1) + "\n";
        reg_load(store.value, "t0");
        ret += "\tsw t0, 0(t1)\n";
        return ret;
    }

    int addr = env.get_offset(store.dest);

    if(store.value->kind.tag == KOOPA_RVT_FUNC_ARG_REF) {
        size_t index = store.value->kind.data.func_arg_ref.index;
        if(index < 8)
            ret += store_offset(addr, "a" + std::to_string(index));
        else {
            ret += "\tlw t0, " + std::to_string(env.stack_size + (index-8)*4) + "(sp)\n";
            ret += store_offset(addr, "t0");
        }
        return ret;
    }
    
    if(store.value->kind.tag == KOOPA_RVT_GET_ELEM_PTR || store.value->kind.tag == KOOPA_RVT_GET_PTR) {
        ret += reg_load(store.dest, "t1");
        ret += reg_load(store.value, "t0");
        ret += "\tsw t0, 0(t1)\n";
        return ret;
    }
    
    ret += reg_load(store.value, rd);
    ret += store_offset(addr, rd);
    
    return ret;
}


std::string BaseRSICV::Visit(const koopa_raw_branch_t &branch) {
    std::string ret = "";
    std::string rd = "t0";
    // int addr = env.get_offset(branch.cond);
    
    ret += reg_load(branch.cond, rd);
    ret += "\tbnez " + rd + ", " + std::string(branch.true_bb->name + 1) + "\n";
    ret += "\tj " + std::string(branch.false_bb->name + 1) + "\n";
    
    return ret;
}

std::string BaseRSICV::Visit(const koopa_raw_jump_t &jump) {
    std::string ret = "";
    std::string rd = "t0";
    // int addr = env.get_offset(branch.cond);
    
    ret += "\tj " + std::string(jump.target->name + 1) + "\n";
    
    return ret;
}

std::string BaseRSICV::Visit(const koopa_raw_call_t &call) {
    std::string ret = "";

    if (call.args.len <= 8) {
        for (int i = 0; i < call.args.len; i++) {
            ret += reg_load(reinterpret_cast<koopa_raw_value_t>(call.args.buffer[i]), "a" + std::to_string(i));
        }
    } else {
        for (int i = 0; i < 8; i++) {
            ret += reg_load(reinterpret_cast<koopa_raw_value_t>(call.args.buffer[i]), "a" + std::to_string(i));
        }

        std::string rd = "t0";
        for (int i = 8; i < call.args.len; i++) {
            ret += reg_load(reinterpret_cast<koopa_raw_value_t>(call.args.buffer[i]), rd);
            ret += store_offset((i-8) * 4, rd);
        }
    }

    ret += "\tcall " + std::string(call.callee->name + 1) + "\n";
    int addr = env.get_offset(current_inst);
    if(addr != -1)
        ret += store_offset(addr, "a0");
    return ret;
}

std::string BaseRSICV::Visit_GLOBAL(const koopa_raw_value_t &value) {
    std::string ret = "";
    ret += "\t.global " + std::string(value->name + 1) + "\n";
    ret += std::string(value->name + 1) + ":\n";
    if(value->kind.data.global_alloc.init->kind.tag == KOOPA_RVT_INTEGER)
        ret += "\t.word " + std::to_string(value->kind.data.global_alloc.init->kind.data.integer.value) + "\n\n";
    else if(value->kind.data.global_alloc.init->kind.tag == KOOPA_RVT_ZERO_INIT) {
        int size = array_size_cal(value->ty->data.pointer.base);
        ret += "\t.zero " + std::to_string(size) + "\n\n";
    } else if(value->kind.data.global_alloc.init->kind.tag == KOOPA_RVT_AGGREGATE) {
        ret += Visit(value->kind.data.global_alloc.init->kind.data.aggregate);
    }
    return ret;
}

std::string BaseRSICV::Visit(const koopa_raw_get_elem_ptr_t &ptr) {
    int src_addr = env.get_offset(ptr.src);
    std::string ret = "";
    int addr = env.get_offset(current_inst);
    if(ptr.src->kind.tag == KOOPA_RVT_GLOBAL_ALLOC) {
        // la t0 arr
        ret += "\tla t0, " + std::string(ptr.src->name + 1) + "\n";
    } else  {
        // addi t0, sp, 4
        assert(src_addr != -1);
        if(src_addr < 2048 && src_addr >= -2048) {
            ret += "\taddi t0, sp, " + std::to_string(src_addr) + "\n";
        } else {
            ret += "\tli t3, " + std::to_string(src_addr) + "\n";
            ret += "\tadd t0, sp, t3\n";
        }
    } 
    // li t1, 1
    // li t2, 4
    // mul t1, t1, t2
    // # 计算 getelemptr 的结果
    // add t0, t0, t1
    // # 保存结果到栈帧
    // sw t0 
  
    ret += reg_load(ptr.index, "t1");
    // int size = array_size_cal(ptr.src->ty->data.pointer.base->data.array.base);
    int size = 4;
    koopa_raw_value_t src = ptr.src;
    int i = 0;
    while(src->ty->tag != KOOPA_RTT_POINTER){
        src = src->kind.data.get_elem_ptr.src;
        i += 1;
    }
    assert(src->ty->tag == KOOPA_RTT_POINTER);
    std::vector<int> index_vec;
    koopa_raw_type_t ty = src->ty->data.pointer.base;
    while(ty->tag != KOOPA_RTT_ARRAY) {
        index_vec.push_back(ty->data.array.len);
    }
    if(index_vec.size() == 0)
        size = 4;
    else {
        for (int j = 0; j < i; j++)
        {
            size *= index_vec[index_vec.size() - 1 -j];
        }
    }
    
    // std::cout << std::to_string(size) << std::endl;
    ret += "\tli t2, " + std::to_string(size) + "\n";
    ret += "\tmul t1, t1, t2\n";
    ret += "\tadd t0, t0, t1\n";
    ret += store_offset(addr, "t0");
    return ret;
}

std::string BaseRSICV::Visit(const koopa_raw_get_ptr_t &ptr) {
    std::string ret = "";
    int addr = env.get_offset(current_inst);
    if(ptr.src->kind.tag == KOOPA_RVT_GLOBAL_ALLOC) {
        // la t0 arr
        ret += "\tla t0, " + std::string(ptr.src->name + 1) + "\n";
    } else {
        // addi t0, sp, 4
        int src_addr = env.get_offset(ptr.src);
        assert(src_addr != -1);
        if(src_addr < 2048 && src_addr >= -2048) {
            ret += "\taddi t0, sp, " + std::to_string(src_addr) + "\n";
        } else {
            ret += "\tli t3, " + std::to_string(src_addr) + "\n";
            ret += "\tadd t0, sp, t3\n";
        }
    }
    // li t1, 1
    // li t2, 4
    // mul t1, t1, t2
    // # 计算 getelemptr 的结果
    // add t0, t0, t1
    // # 保存结果到栈帧
    // sw t0 
  
    ret += reg_load(ptr.index, "t1");
    int size = array_size_cal(ptr.src->ty->data.pointer.base);
    ret += "\tli t2, " + std::to_string(size) + "\n";
    ret += "\tmul t1, t1, t2\n";
    ret += "\tadd t0, t0, t1\n";
    ret += store_offset(addr, "t0");
    return ret;
}

std::string BaseRSICV::Visit(const koopa_raw_aggregate_t &aggr) {
    // is called by global value
    std::string ret = "";
    for(int i = 0; i < aggr.elems.len; i++) {
        auto ptr = aggr.elems.buffer[i];
        koopa_raw_value_t value = reinterpret_cast<koopa_raw_value_t>(ptr);
        if(value->kind.tag == KOOPA_RVT_INTEGER)
            ret += "\t.word " + std::to_string(value->kind.data.integer.value) + "\n";
        else if(value->kind.tag == KOOPA_RVT_AGGREGATE)
            ret += Visit(value->kind.data.aggregate);
        else
            assert(false);
    }
    return ret;
}
