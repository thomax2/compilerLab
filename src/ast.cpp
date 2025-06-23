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
    sym_list.list_new();

    // std::vector<const void *> insts{stmt->Koop()};
    std::vector<const void *> insts;
    // blist->Koop(insts);

    for(auto bitem = blist->end() - 1; bitem > blist->begin() - 1; bitem--) {
        (koopa_raw_value_t)(*bitem)->Koop(slice(KOOPA_RSIK_VALUE),insts);
        if (insts.size() > 0) {
            auto inst = (koopa_raw_value_t)insts.back();
            if (inst->kind.tag == KOOPA_RVT_RETURN) {
              break;
            }
        }
    }
    // ((koopa_raw_value_data_t *)insts[1])->used_by = slice(((koopa_raw_value_t)insts[0]),KOOPA_RSIK_VALUE);
    // ((koopa_raw_value_data_t *)insts[0])->used_by = slice(((koopa_raw_value_t)insts[1]),KOOPA_RSIK_VALUE);
    
    
    
    ret->insts = slice(insts,KOOPA_RSIK_VALUE);
    ret->name = "\%entry";
    ret->params = slice(KOOPA_RSIK_VALUE);
    ret->used_by = slice(KOOPA_RSIK_VALUE);

    sym_list.list_del();
    return ret;
}

void* BlockAST::Koop(koopa_raw_slice_t b, std::vector<const void *> &insts) const {
    if(blist == nullptr) 
        return nullptr;

    sym_list.list_new();

    for(auto bitem = blist->end() - 1; bitem > blist->begin() - 1; bitem--) {
        (koopa_raw_value_t)(*bitem)->Koop(slice(KOOPA_RSIK_VALUE),insts);
        if (insts.size() > 0) {
            auto inst = (koopa_raw_value_t)insts.back();
            if (inst->kind.tag == KOOPA_RVT_RETURN) {
              break;
            }
        }
    }
    sym_list.list_del();
    return nullptr;
}

void* StmtAST::Koop(koopa_raw_slice_t b, std::vector<const void *> &insts) const {
    koopa_raw_value_data *ret = new koopa_raw_value_data();
    koopa_raw_slice_t used_by = slice(ret, KOOPA_RSIK_VALUE);

    ret->name = nullptr;
    ret->ty = type_kind(KOOPA_RTT_UNIT);
    ret->used_by = slice(KOOPA_RSIK_VALUE);

    if( expr != nullptr ) {
        if(type == RET) {
            ret->kind.tag = KOOPA_RVT_RETURN;
            if(expr != nullptr)
                ret->kind.data.ret.value = (const koopa_raw_value_data_t *)expr->Koop(used_by, insts);  
            insts.push_back(ret);  
        } else if(type == VAL) {
            ret->kind.tag = KOOPA_RVT_STORE;
            ret->kind.data.store.value = (const koopa_raw_value_data_t *)expr->Koop(used_by, insts);
            ret->kind.data.store.dest = (const koopa_raw_value_data_t *)lval->Koop();
            insts.push_back(ret);
        } else if(type == EXP) {
            expr->Koop(b, insts); //dont know 
        } else if(type == BLOCK) {
            expr->Koop(b, insts);
        } else {
            assert(false);
        }    
    }

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

int NumberAST::Calcu() const { return num; }

void* ExpAST::Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const {
    return exp->Koop(used_by, insts);
}

int ExpAST::Calcu() const {
    return exp->Calcu();
}

void* PrimaryExpAST::Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const {
    return expr->Koop(used_by, insts);
}

int PrimaryExpAST::Calcu() const { return expr->Calcu(); }

void* LValAST::Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const {
    Symbol sym = (sym_list.list_get(ident));
    koopa_raw_value_data *ret = new koopa_raw_value_data();

    koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
    ty->tag = KOOPA_RTT_INT32;

    ret->name = nullptr;
    ret->ty = ty;
    ret->used_by = used_by;
    if(sym.type == CONST_SYM) {
        ret->kind.tag = KOOPA_RVT_INTEGER;
        ret->kind.data.integer.value = sym.data.const_sym;
    } else if(sym.type == VAR_SYM) {
        ret->kind.tag = KOOPA_RVT_LOAD;
        ret->kind.data.load.src = sym.data.var_sym;
        insts.push_back(ret);
    } else {
        assert(false);
    }
    return ret;
}

void* LValAST::Koop() const {
    return (void *)sym_list.list_get(ident).data.var_sym;
}


int LValAST::Calcu() const {
    return sym_list.list_get(ident).data.const_sym;
}


void* UnaryExpAST::Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const {
    if(type == EXP) {
        return primExp->Koop(used_by, insts);
    } else if(type == OP) {
        koopa_raw_value_data *ret = new koopa_raw_value_data();
        koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
        koopa_raw_slice_t used = slice(ret, KOOPA_RSIK_VALUE);

        NumberAST zero;
        zero.num = 0;


        ty->tag = KOOPA_RTT_INT32;
    
        ret->kind.tag = KOOPA_RVT_BINARY;
        ret->name = nullptr;
        ret->ty = ty;
        ret->used_by = used_by;


        ret->kind.data.binary.lhs = (koopa_raw_value_t)zero.Koop(used, insts);
        ret->kind.data.binary.rhs = (koopa_raw_value_t)uexp->Koop(used, insts);

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

int UnaryExpAST::Calcu() const {
    if (type == EXP)
      return primExp->Calcu();
    if (uop == "-")
      return -uexp->Calcu();
    if (uop == "!")
      return !uexp->Calcu();
    assert(false);
    return 0;
}
  

void* MulExpAST::Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const {
    if(type == EXP) {
        return uexp->Koop(used_by, insts);
    } else if(type == OP) {
        koopa_raw_value_data *ret = new koopa_raw_value_data();
        koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
        koopa_raw_slice_t used = slice(ret, KOOPA_RSIK_VALUE);

        ty->tag = KOOPA_RTT_INT32;

        ret->kind.tag = KOOPA_RVT_BINARY;
        ret->name = nullptr;
        ret->ty = ty;
        ret->used_by = used_by;

        ret->kind.data.binary.lhs = (koopa_raw_value_t)mexp->Koop(used, insts);
        ret->kind.data.binary.rhs = (koopa_raw_value_t)uexp->Koop(used, insts);

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

int MulExpAST::Calcu() const {
    if (type == EXP)
      return uexp->Calcu();
    if (mop == "*")
      return mexp->Calcu() * uexp->Calcu();
    if (mop == "/")
      return mexp->Calcu() / uexp->Calcu();
    if (mop == "%")
      return mexp->Calcu() % uexp->Calcu();
    assert(false);
    return 0;
  }

void* AddExpAST::Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const {
    if(type == EXP) {
        return mexp->Koop(used_by, insts);
    } else if(type == OP) {
        koopa_raw_value_data *ret = new koopa_raw_value_data();
        koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
        koopa_raw_slice_t used = slice(ret, KOOPA_RSIK_VALUE);

        ty->tag = KOOPA_RTT_INT32;

        ret->kind.tag = KOOPA_RVT_BINARY;
        ret->name = nullptr;
        ret->ty = ty;
        ret->used_by = used_by;

        ret->kind.data.binary.lhs = (koopa_raw_value_t)mexp->Koop(used, insts);
        ret->kind.data.binary.rhs = (koopa_raw_value_t)aexp->Koop(used, insts);

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

int AddExpAST::Calcu() const {
    if (type == EXP)
      return mexp->Calcu();
    if (aop == "+")
      return mexp->Calcu() + aexp->Calcu();
    if (aop == "-")
      return mexp->Calcu() - aexp->Calcu();
    assert(false);
    return 0;
  }
  


void* RelExpAST::Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const {
    if(type == EXP) {
        return aexp->Koop(used_by, insts);
    } else if(type == OP) {
        koopa_raw_value_data *ret = new koopa_raw_value_data();
        koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
        koopa_raw_slice_t used = slice(ret, KOOPA_RSIK_VALUE);

        ty->tag = KOOPA_RTT_INT32;

        ret->kind.tag = KOOPA_RVT_BINARY;
        ret->name = nullptr;
        ret->ty = ty;
        ret->used_by = used_by;

        ret->kind.data.binary.lhs = (koopa_raw_value_t)rexp->Koop(used, insts);
        ret->kind.data.binary.rhs = (koopa_raw_value_t)aexp->Koop(used, insts);

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

int RelExpAST::Calcu() const {
    if (type == EXP)
      return aexp->Calcu();
    if (rop == "<")
      return rexp->Calcu() < aexp->Calcu();
    if (rop == ">")
      return rexp->Calcu() > aexp->Calcu();
    if (rop == "<=")
      return rexp->Calcu() <= aexp->Calcu();
    if (rop == ">=")
      return rexp->Calcu() >= aexp->Calcu();
      assert(false);
    return 0;
  }

void* EqExpAST::Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const {
    if(type == EXP) {
        return rexp->Koop(used_by, insts);
    } else if(type == OP) {
        koopa_raw_value_data *ret = new koopa_raw_value_data();
        koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
        koopa_raw_slice_t used = slice(ret, KOOPA_RSIK_VALUE);

        ty->tag = KOOPA_RTT_INT32;

        ret->kind.tag = KOOPA_RVT_BINARY;
        ret->name = nullptr;
        ret->ty = ty;
        ret->used_by = used_by;

        ret->kind.data.binary.lhs = (koopa_raw_value_t)eexp->Koop(used, insts);
        ret->kind.data.binary.rhs = (koopa_raw_value_t)rexp->Koop(used, insts);

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

int EqExpAST::Calcu() const {
    if(type == EXP)
        return rexp->Calcu();
    else {
        if(eop == "==")
            return eexp->Calcu() == rexp->Calcu();
        else if(eop == "!=")
            return eexp->Calcu() != rexp->Calcu();
    }
    assert(false);
    return 0;
}

void* Int2bool(koopa_raw_slice_t used_by, std::vector<const void*>& insts, const std::unique_ptr<BaseAST> &exp) {
    koopa_raw_value_data *ret = new koopa_raw_value_data();
    koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
    koopa_raw_slice_t used = slice(ret, KOOPA_RSIK_VALUE);

    ty->tag = KOOPA_RTT_INT32;

    ret->kind.tag = KOOPA_RVT_BINARY;
    ret->name = nullptr;
    ret->ty = ty;
    ret->used_by = used_by;

    NumberAST zero;
    zero.num = 0;
    ret->kind.data.binary.lhs = (koopa_raw_value_t)exp->Koop(used, insts);
    ret->kind.data.binary.rhs = (koopa_raw_value_t)zero.Koop(used, insts);
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
        koopa_raw_slice_t used = slice(ret, KOOPA_RSIK_VALUE);

        ty->tag = KOOPA_RTT_INT32;

        ret->kind.tag = KOOPA_RVT_BINARY;
        ret->name = nullptr;
        ret->ty = ty;
        ret->used_by = used_by;

        ret->kind.data.binary.lhs = (koopa_raw_value_t)Int2bool(used,insts,laexp);
        ret->kind.data.binary.rhs = (koopa_raw_value_t)Int2bool(used,insts,eexp);
        ret->kind.data.binary.op = KOOPA_RBO_AND;

        insts.push_back(ret);
        return ret;
    }
    return nullptr;
}

int LAndExpAST::Calcu() const {
    if(type == EXP)
        return eexp->Calcu();
    else
        return laexp->Calcu() && eexp->Calcu();
}

void* LOrExpAST::Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const {
    if(type == EXP) {
        return laexp->Koop(used_by, insts);
    } else if(type == OP) {
        koopa_raw_value_data *ret = new koopa_raw_value_data();
        koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
        koopa_raw_slice_t used = slice(ret, KOOPA_RSIK_VALUE);

        ty->tag = KOOPA_RTT_INT32;

        ret->kind.tag = KOOPA_RVT_BINARY;
        ret->name = nullptr;
        ret->ty = ty;
        ret->used_by = used_by;

        ret->kind.data.binary.lhs = (koopa_raw_value_t)Int2bool(used,insts,loexp);
        ret->kind.data.binary.rhs = (koopa_raw_value_t)Int2bool(used,insts,laexp);
        ret->kind.data.binary.op = KOOPA_RBO_OR;

        insts.push_back(ret);
        return ret;
    }
    return nullptr;
}

int LOrExpAST::Calcu() const {
    if(type == EXP) {
        return laexp->Calcu();
    } else
        return loexp->Calcu() || laexp->Calcu();
        assert(false);
    return 0;
}

void* ConstDeclAST::Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const {
    koopa_raw_type_t type = (const koopa_raw_type_t)btype->Koop(used_by, insts);

    for(auto cdef = clist->begin(); cdef != clist->end(); cdef++) {
        (*cdef)->Koop(used_by, insts);
    }
    return nullptr;
}

void* BTypeAST::Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const {
    koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
    if(btype == "int") {
        ty->tag = KOOPA_RTT_INT32;
    }
    return ty;    
}


void* ConstDefAST::Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const {
    int val = cival->Calcu();
    sym_list.list_add(ident, Symbol(SymType::CONST_SYM, val));
    return nullptr;
}


void* VarDeclAST::Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const {
    koopa_raw_type_t type = (const koopa_raw_type_t)btype->Koop(used_by, insts);

    for(auto vdef = vlist->begin(); vdef != vlist->end(); vdef++) {
        (*vdef)->Koop(used_by, insts);
    }
    return nullptr;

}

void* VarDefAST::Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const {
    koopa_raw_value_data *ret = new koopa_raw_value_data();
    koopa_raw_slice_t used = slice(ret, KOOPA_RSIK_VALUE);

    char *name = new char[ident.length() + 1 + 2];
    ("@" + ident + "_" + std::to_string(sym_list.cur_depth())).copy(name, ident.length()+1+2);
    // ("@" + ident + "_" + std::to_string(sym_list.cur_depth())).copy(name, ident.length()+1+2);
    ret->name = name;
    ret->ty = pointer_type_kind(KOOPA_RTT_INT32);
    ret->used_by = used;
    ret->kind.tag = KOOPA_RVT_ALLOC;
    insts.push_back(ret);

    Symbol sym = Symbol(SymType::VAR_SYM, ret);
    sym_list.list_add(ident.c_str(), sym);

    if(type == VAL) {
        koopa_raw_value_data *store = new koopa_raw_value_data();
        koopa_raw_slice_t used = slice(store, KOOPA_RSIK_VALUE);
        store->name = nullptr;
        store->used_by = slice(KOOPA_RSIK_VALUE);
        store->kind.tag = KOOPA_RVT_STORE;
        store->kind.data.store.dest = (koopa_raw_value_t)ret;
        store->kind.data.store.value = (koopa_raw_value_t)ival->Koop(used,insts);
        insts.push_back(store);
    }
    return nullptr;
}




