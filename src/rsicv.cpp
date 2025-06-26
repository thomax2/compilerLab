#include "rsicv.h"

int BaseRSICV::func_size(const koopa_raw_function_t &func) {
    int size = 0;
    for(int i = 0; i< func->bbs.len; i++) {
        size += blk_size(reinterpret_cast<koopa_raw_basic_block_t>(func->bbs.buffer[i]));
    }
    return size;
}

int BaseRSICV::blk_size(koopa_raw_basic_block_t block) {
    int size = 0;
    for(int i = 0; i < block->insts.len; i++) {
        // assert(block->insts.buffer[i] != nullptr);
        size += inst_size(reinterpret_cast<koopa_raw_value_t>(block->insts.buffer[i]));
    }
    return size;
}

int BaseRSICV::inst_size(koopa_raw_value_t inst) {
    // assert(inst->ty != nullptr);
    switch (inst->kind.tag) {
        case KOOPA_RVT_ALLOC:
            return 4;
        case KOOPA_RVT_LOAD:
            return 4;
        case KOOPA_RVT_BINARY:
            return 4;
        default:
            return 0;
    }

    return 0;
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
        ret += "\tlw " + reg + ", " + std::to_string(offset) + "(sp)\n";
    }
    return ret;
}

void BaseRSICV::Env::reg_free(std::string reg) {
    reg_status_map[reg] = UNUSED;
}

void BaseRSICV::Env::stack_init(int size) {
    stack_size = size;
    cur_offset = 0;
    stack_offset_map.clear();
}

int BaseRSICV::Env::get_offset(koopa_raw_value_t value) {
    assert(value != nullptr);
    if(stack_offset_map.find(value) != stack_offset_map.end())
        return stack_offset_map[value];
    else {
        stack_offset_map[value] = cur_offset;
        cur_offset += 4;
        return stack_offset_map[value];
    }
    return -1;
}

std::string BaseRSICV::store_offset(int addr, std::string reg) {
    std::string ret = "";
    ret += "\tsw " + reg + ", " + std::to_string(addr) + "(sp)\n";

    return ret;
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
        ret += "\t.text\n";
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
    ret += "\t.global " + std::string(func->name + 1) + "\n";
    ret += std::string(func->name + 1) + ":\n";

    int sub_sp = func_size(func);
    // sub_sp = sub_sp*4;
    if(sub_sp % 16 != 0)
        sub_sp += 16 - (sub_sp % 16);
    env.stack_init(sub_sp);
    ret += "\taddi sp, sp, -" + std::to_string(sub_sp) + "\n";

    ret += Visit(func->bbs);
    return ret;
}

std::string BaseRSICV::Visit(const koopa_raw_basic_block_t &block) {
    std::string ret = "";
    // std::cout << "len::" << std::to_string(block->insts.len) << std::endl;
    // int sub_sp = 0;
    // for(int i = 0; i<(block->insts.len); i++) {
    //     koopa_raw_value_data * inst = (koopa_raw_value_data *)block->insts.buffer[i];
    //     // std::cout << "tag::" << std::to_string(inst->kind.tag) << std::endl;
    //     if(inst->kind.tag == KOOPA_RVT_ALLOC || inst->kind.tag == KOOPA_RVT_LOAD || 
    //             inst->kind.tag == KOOPA_RVT_BINARY)
    //         sub_sp++;
    // }
    // sub_sp = sub_sp*4;
    // if(sub_sp % 16 != 0)
    //     sub_sp += 16 - (sub_sp % 16);
    // env.stack_init(sub_sp);
    // ret += "\taddi sp, sp, -" + std::to_string(sub_sp) + "\n";
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

    // int offset = env.get_offset(raw.value);
    ret += reg_load(raw.value, "a0");
    ret += "\tadd sp, sp, " + std::to_string(env.stack_size) + "\n";
    ret += "\tret\n";
    return ret;
}

std::string BaseRSICV::Visit(const koopa_raw_load_t &load) {
    std::string ret = "";
    std::string rd = "t0";
    int addr = env.get_offset(current_inst);

    ret += reg_load(load.src, rd);
    ret += store_offset(addr, rd);
    return ret;
}


std::string BaseRSICV::Visit(const koopa_raw_store_t &store) {
    std::string ret = "";
    std::string rd = "t0";
    int addr = env.get_offset(store.dest);

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
