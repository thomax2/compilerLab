#include "rsicv.h"

void BaseRSICV::build(koopa_raw_program_t raw_prog) {
    Visit(raw_prog);
    out.close();
}

void BaseRSICV::Visit(const koopa_raw_program_t &program) {
    // 访问所有全局变量
    if(program.values.len > 0) {
        out << "\t.data\n";
        Visit(program.values);
    }
    // 访问所有函数
    if(program.funcs.len > 0) {
        out << "\t.text\n";
        Visit(program.funcs);
    }
}

void BaseRSICV::Visit(const koopa_raw_slice_t &slice) {
    for (size_t i = 0; i < slice.len; i++) {
        auto ptr = slice.buffer[i];

        switch (slice.kind)
        {
        case KOOPA_RSIK_FUNCTION:
            Visit(reinterpret_cast<koopa_raw_function_t>(ptr));
            break;

        case KOOPA_RSIK_BASIC_BLOCK:
            Visit(reinterpret_cast<koopa_raw_basic_block_t>(ptr));
            break;
        
        case KOOPA_RSIK_VALUE:
            Visit(reinterpret_cast<koopa_raw_value_t>(ptr));
            break;
        
        case KOOPA_RSIK_TYPE:
            // TODO
            break;

        default:
            assert(false);
        }
    }
}

void BaseRSICV::Visit(const koopa_raw_function_t &func) {
    out << "\t.global " << std::string(func->name + 1) << "\n";
    out << std::string(func->name + 1) << ":\n";
    Visit(func->bbs);
}

void BaseRSICV::Visit(const koopa_raw_basic_block_t &block) {
    Visit(block->insts);
}

// 访问指令
void BaseRSICV::Visit(const koopa_raw_value_t &value) {
    const auto &kind = value->kind;

    switch (kind.tag)
    {
    case KOOPA_RVT_INTEGER:
        Visit(kind.data.integer);
        break;

    case KOOPA_RVT_RETURN:
        Visit(kind.data.ret);
        break;

    // TODO

    default:
        break;
    }
}

void BaseRSICV::Visit(const koopa_raw_integer_t &interger) {
    out << interger.value;
}

void BaseRSICV::Visit(const koopa_raw_return_t &ret) {
    out << "\tli a0, ";
    Visit(ret.value);
    out << "\n\tret\n";
}

