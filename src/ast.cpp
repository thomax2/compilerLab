#include "ast.hpp"


void* CompUnitAST::Koop() const {
    std::vector<const void*> funcs{func_def->Koop()};

    koopa_raw_program_t *ret = new koopa_raw_program_t();
    
    // global value unused
    ret->values = slice(KOOPA_RSIK_VALUE);
    ret->funcs = slice(funcs, KOOPA_RSIK_FUNCTION);

    return ret;
}


void* FuncDefAST::Koop() const {

    koopa_raw_function_data_t *ret = new koopa_raw_function_data_t();

    koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
    ty->tag = KOOPA_RTT_FUNCTION;
    ty->data.function.params = slice(KOOPA_RSIK_TYPE);
    ty->data.function.ret = (const struct koopa_raw_type_kind *)func_type->Koop();
    
    std::vector<const void *> blocks{block->Koop()};
    char *name = new char[ident.length() + 1];
    ("@" + ident).copy(name, ident.length() + 1);


    ret->ty = ty;
    ret->bbs = slice(blocks,KOOPA_RSIK_BASIC_BLOCK);
    ret->name = name;
    ret->params = slice(KOOPA_RSIK_VALUE);

    return ret;
}


void* FuncTypeAST::Koop() const {
    koopa_raw_type_kind_t *ret = new koopa_raw_type_kind_t();
    if(func_type == "int") {
        ret->tag = KOOPA_RTT_INT32;
        return ret;
    }
    return nullptr;  // TODO
}

void* BlockAST::Koop() const {
    koopa_raw_basic_block_data_t *ret = new koopa_raw_basic_block_data_t();

    std::vector<const void *> insts{stmt->Koop()};

    ret->insts = slice(insts,KOOPA_RSIK_VALUE);
    ret->name = "\%entry";
    ret->params = slice(KOOPA_RSIK_VALUE);
    ret->used_by = slice(KOOPA_RSIK_VALUE);

    return ret;
}

void* StmtAST::Koop() const {
    koopa_raw_value_data *ret = new koopa_raw_value_data();
    
    koopa_raw_value_kind_t *kind = new koopa_raw_value_kind_t();
    kind->tag = KOOPA_RVT_RETURN;
    kind->data.ret.value = (const koopa_raw_value_data_t *)number->Koop();

    koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
    ty->tag = KOOPA_RTT_INT32;

    ret->kind = *kind;
    ret->name = nullptr;
    ret->ty = ty;
    ret->used_by = slice(KOOPA_RSIK_VALUE);
    return ret;
}

void* NumberAST::Koop() const {
    koopa_raw_value_data *ret = new koopa_raw_value_data();

    koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
    ty->tag = KOOPA_RTT_INT32;

    ret->kind.data.integer.value = num;
    ret->kind.tag = KOOPA_RVT_INTEGER;
    ret->name = nullptr;
    ret->ty = ty;
    ret->used_by = slice(KOOPA_RSIK_VALUE);
    return ret;
}

