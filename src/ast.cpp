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

    // std::vector<const void *> insts{stmt->Koop()};
    std::vector<const void *> insts;
    stmt->Koop(insts);

    ret->insts = slice(insts,KOOPA_RSIK_VALUE);
    ret->name = "\%entry";
    ret->params = slice(KOOPA_RSIK_VALUE);
    ret->used_by = slice(KOOPA_RSIK_VALUE);

    return ret;
}

void* StmtAST::Koop(std::vector<const void *> &insts) const {
    koopa_raw_value_data *ret = new koopa_raw_value_data();
    koopa_raw_slice_t used_by = slice(ret, KOOPA_RSIK_VALUE);

    koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
    ty->tag = KOOPA_RTT_INT32;

    ret->kind.tag = KOOPA_RVT_RETURN;
    ret->kind.data.ret.value = (const koopa_raw_value_data_t *)expr->Koop(used_by, insts);
    ret->name = nullptr;
    ret->ty = ty;
    ret->used_by = slice(KOOPA_RSIK_VALUE);
    insts.push_back(ret);
    return ret;
}

void* NumberAST::Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const {
    koopa_raw_value_data *ret = new koopa_raw_value_data();

    koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
    ty->tag = KOOPA_RTT_INT32;

    ret->kind.data.integer.value = num;
    ret->kind.tag = KOOPA_RVT_INTEGER;
    ret->name = nullptr;
    ret->ty = ty;
    ret->used_by = used_by;
    return ret;
}

void* ExpAST::Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const {
    return exp->Koop(used_by, insts);
}

void* PrimaryExpAST::Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const {
    return expr->Koop(used_by, insts);
}

void* UnaryExpAST::Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const {
    if(type == EXP) {
        return primExp->Koop(used_by, insts);
    } else if(type == OP) {
        koopa_raw_value_data *ret = new koopa_raw_value_data();
        koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
        NumberAST zero;
        zero.num = 0;

        ty->tag = KOOPA_RTT_INT32;
    
        ret->kind.tag = KOOPA_RVT_BINARY;
        ret->name = nullptr;
        ret->ty = ty;
        ret->used_by = used_by;


        ret->kind.data.binary.lhs = (koopa_raw_value_t)zero.Koop(used_by, insts);
        ret->kind.data.binary.rhs = (koopa_raw_value_t)uexp->Koop(used_by, insts);

        if (uop == "+") {
            ret->kind.data.binary.op = KOOPA_RBO_ADD;;
        } else if (uop == "-") {
            ret->kind.data.binary.op = KOOPA_RBO_SUB;
        } else if (uop == "!") {
            ret->kind.data.binary.op = KOOPA_RBO_EQ;
        } else {
            assert(false);
        }
        insts.push_back(ret);
        return ret;
    }
    return nullptr;
}

void* MulExpAST::Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const {
    if(type == EXP) {
        return uexp->Koop(used_by, insts);
    } else if(type == OP) {
        koopa_raw_value_data *ret = new koopa_raw_value_data();
        koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
        ty->tag = KOOPA_RTT_INT32;

        ret->kind.tag = KOOPA_RVT_BINARY;
        ret->name = nullptr;
        ret->ty = ty;
        ret->used_by = used_by;

        ret->kind.data.binary.lhs = (koopa_raw_value_t)mexp->Koop(used_by, insts);
        ret->kind.data.binary.rhs = (koopa_raw_value_t)uexp->Koop(used_by, insts);

        if (mop == "*") {
            ret->kind.data.binary.op = KOOPA_RBO_MUL;
        } else if (mop == "/") {
            ret->kind.data.binary.op = KOOPA_RBO_DIV;
        } else if (mop == "%") {
            ret->kind.data.binary.op = KOOPA_RBO_MOD;
        } else {
            assert(false);
        }
        insts.push_back(ret);
        return ret;
    }
    return nullptr;
}

void* AddExpAST::Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const {
    if(type == EXP) {
        return mexp->Koop(used_by, insts);
    } else if(type == OP) {
        koopa_raw_value_data *ret = new koopa_raw_value_data();
        koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
        ty->tag = KOOPA_RTT_INT32;

        ret->kind.tag = KOOPA_RVT_BINARY;
        ret->name = nullptr;
        ret->ty = ty;
        ret->used_by = used_by;

        ret->kind.data.binary.lhs = (koopa_raw_value_t)mexp->Koop(used_by, insts);
        ret->kind.data.binary.rhs = (koopa_raw_value_t)aexp->Koop(used_by, insts);

        if (aop == "+") {
            ret->kind.data.binary.op = KOOPA_RBO_ADD;
        } else if (aop == "-") {
            ret->kind.data.binary.op = KOOPA_RBO_SUB;
        } else {
            assert(false);
        }

        insts.push_back(ret);
        return ret;
    }
    return nullptr;
}

void* RelExpAST::Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const {
    if(type == EXP) {
        return aexp->Koop(used_by, insts);
    } else if(type == OP) {
        koopa_raw_value_data *ret = new koopa_raw_value_data();
        koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
        ty->tag = KOOPA_RTT_INT32;

        ret->kind.tag = KOOPA_RVT_BINARY;
        ret->name = nullptr;
        ret->ty = ty;
        ret->used_by = used_by;

        ret->kind.data.binary.lhs = (koopa_raw_value_t)rexp->Koop(used_by, insts);
        ret->kind.data.binary.rhs = (koopa_raw_value_t)aexp->Koop(used_by, insts);

        if (rop == "<") {
            ret->kind.data.binary.op = KOOPA_RBO_LT;
        } else if (rop == ">") {
            ret->kind.data.binary.op = KOOPA_RBO_GT;
        } else if (rop == "<=") {
            ret->kind.data.binary.op = KOOPA_RBO_LE;
        } else if (rop == ">=") {
            ret->kind.data.binary.op = KOOPA_RBO_GE;
        } else {
            assert(false);
        }
        insts.push_back(ret);
        return ret;
    }
    return nullptr;
}

void* EqExpAST::Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const {
    if(type == EXP) {
        return rexp->Koop(used_by, insts);
    } else if(type == OP) {
        koopa_raw_value_data *ret = new koopa_raw_value_data();
        koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
        ty->tag = KOOPA_RTT_INT32;

        ret->kind.tag = KOOPA_RVT_BINARY;
        ret->name = nullptr;
        ret->ty = ty;
        ret->used_by = used_by;

        ret->kind.data.binary.lhs = (koopa_raw_value_t)eexp->Koop(used_by, insts);
        ret->kind.data.binary.rhs = (koopa_raw_value_t)rexp->Koop(used_by, insts);

        if (eop == "==") {
            ret->kind.data.binary.op = KOOPA_RBO_EQ;
        } else if (eop == "!=") {
            ret->kind.data.binary.op = KOOPA_RBO_NOT_EQ;
        } else {
            assert(false);
        }
        insts.push_back(ret);
        return ret;
    }
    return nullptr;
}

void* Int2bool(koopa_raw_slice_t used_by, std::vector<const void*>& insts, const std::unique_ptr<BaseAST> &exp) {
    koopa_raw_value_data *ret = new koopa_raw_value_data();
    koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
    ty->tag = KOOPA_RTT_INT32;

    ret->kind.tag = KOOPA_RVT_BINARY;
    ret->name = nullptr;
    ret->ty = ty;
    ret->used_by = used_by;

    NumberAST zero;
    zero.num = 0;
    ret->kind.data.binary.lhs = (koopa_raw_value_t)exp->Koop(used_by, insts);
    ret->kind.data.binary.rhs = (koopa_raw_value_t)zero.Koop(used_by, insts);
    ret->kind.data.binary.op = KOOPA_RBO_NOT_EQ;

    insts.push_back(ret);
    return ret;
}

void* LAndExpAST::Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const {
    if(type == EXP) {
        return eexp->Koop(used_by, insts);
    } else if(type == OP) {
        koopa_raw_value_data *ret = new koopa_raw_value_data();
        koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
        ty->tag = KOOPA_RTT_INT32;

        ret->kind.tag = KOOPA_RVT_BINARY;
        ret->name = nullptr;
        ret->ty = ty;
        ret->used_by = used_by;

        ret->kind.data.binary.lhs = (koopa_raw_value_t)Int2bool(used_by,insts,laexp);
        ret->kind.data.binary.rhs = (koopa_raw_value_t)Int2bool(used_by,insts,eexp);
        ret->kind.data.binary.op = KOOPA_RBO_AND;

        insts.push_back(ret);
        return ret;
    }
    return nullptr;
}


void* LOrExpAST::Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const {
    if(type == EXP) {
        return laexp->Koop(used_by, insts);
    } else if(type == OP) {
        koopa_raw_value_data *ret = new koopa_raw_value_data();
        koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
        ty->tag = KOOPA_RTT_INT32;

        ret->kind.tag = KOOPA_RVT_BINARY;
        ret->name = nullptr;
        ret->ty = ty;
        ret->used_by = used_by;

        ret->kind.data.binary.lhs = (koopa_raw_value_t)Int2bool(used_by,insts,loexp);
        ret->kind.data.binary.rhs = (koopa_raw_value_t)Int2bool(used_by,insts,laexp);
        ret->kind.data.binary.op = KOOPA_RBO_OR;

        insts.push_back(ret);
        return ret;
    }
    return nullptr;
}

