#include "rsicv.h"

void BaseRSICV::Reg::reg_init() {
    for(size_t i = 0; i< 8; i++) {
        reg_status_map[ "t" + std::to_string(i) ] = UNUSED;
        if(i != 7)
            reg_status_map[ "a" + std::to_string(i) ] = UNUSED;
    }
}


// alloc arbitrary reg to value
std::string BaseRSICV::Reg::reg_alloc(koopa_raw_value_t value) {
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
std::string BaseRSICV::Reg::reg_load(koopa_raw_value_t value, std::string reg) {
    // assert(reg_status_map[reg] == UNUSED);
    std::string ret = "";

    // find
    if(reg_insts_map.find(value) != reg_insts_map.end()) {
        if(value->kind.tag == KOOPA_RVT_INTEGER) {
            ret += "\tli " + reg + ", " + std::to_string(value->kind.data.integer.value) + "\n";
        } else if (reg_insts_map[value] != reg) {
            ret += "\tmv " + reg + ", " + reg_insts_map[value] + "\n";
        }
    } else {
        if(value->kind.tag == KOOPA_RVT_INTEGER) {
            ret += "\tli " + reg + ", " + std::to_string(value->kind.data.integer.value) + "\n";
        } else {
            // std::cout << std::to_string(value->kind.tag) << std::endl;
            // assert(false);
        }
    }
    return ret;
}

void BaseRSICV::Reg::reg_free(std::string reg) {
    reg_status_map[reg] = UNUSED;
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
    ret += Visit(func->bbs);
    return ret;
}

std::string BaseRSICV::Visit(const koopa_raw_basic_block_t &block) {
    return Visit(block->insts);
}

// 访问指令
std::string BaseRSICV::Visit(const koopa_raw_value_t &value) {
    std::string ret = "";
    const auto &kind = value->kind;

    switch (kind.tag)
    {
    case KOOPA_RVT_INTEGER:
        // ret += Visit(kind.data.integer);
        break;
    
    case KOOPA_RVT_BINARY:
        current_inst = value;
        ret += Visit(kind.data.binary);
        break;

    case KOOPA_RVT_RETURN:
        current_inst = value;
        ret += Visit(kind.data.ret);
        break;

    // TODO

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

    auto rd = reg_mem.reg_alloc((koopa_raw_value_data_t *)current_inst);

    std::string rs1;
    std::string rs2;

    if(binary.lhs->kind.tag == KOOPA_RVT_INTEGER && binary.lhs->kind.data.integer.value == 0) {
        rs1 = "x0";
    } else {
        rs1 = reg_mem.reg_alloc(binary.lhs);
        ret += reg_mem.reg_load(binary.lhs, rs1);
    }

    if(binary.rhs->kind.tag == KOOPA_RVT_INTEGER && binary.rhs->kind.data.integer.value == 0) {
        rs2 = "x0";
    } else {
        rs2 = reg_mem.reg_alloc(binary.rhs);
        ret += reg_mem.reg_load(binary.rhs, rs2);
    }

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
    if (rs1 != "Null" && rs1 != "x0" && rs1 != rd) {
        reg_mem.reg_free(rs1);
    }
    if (rs2 != "Null" && rs2 != "x0" && rs2 != rd) {
        reg_mem.reg_free(rs2);
    }

    return ret;
}


std::string BaseRSICV::Visit(const koopa_raw_return_t &raw) {
    std::string ret = "";

    // ret += "\tli a0, ";
    // ret += Visit(raw.value);
    // ret += "\n\tret\n";
    ret += reg_mem.reg_load(raw.value, "a0");
    ret += "\tret\n";
    return ret;
}

