#include "ast.hpp"

void CompUnitAST::lib_func_load(std::vector<const void*> &funcs) const {
    FuncFParamAST ff;
    koopa_raw_function_data_t *func = new koopa_raw_function_data_t();
    koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
    ty->tag = KOOPA_RTT_FUNCTION;
    ty->data.function.params = slice(KOOPA_RSIK_TYPE);
    ty->data.function.ret = type_kind(KOOPA_RTT_INT32);

    func->ty = ty;
    func->name = "@getint";
    func->params = slice(KOOPA_RSIK_VALUE);
    func->bbs = slice(KOOPA_RSIK_BASIC_BLOCK);
    sym_list.list_add(func->name + 1, Symbol(SymType::FUNC, func));
    funcs.push_back(func);

    func = new koopa_raw_function_data_t();
    ty = new koopa_raw_type_kind_t();
    ty->tag = KOOPA_RTT_FUNCTION;
    ty->data.function.params = slice(KOOPA_RSIK_TYPE);
    ty->data.function.ret = type_kind(KOOPA_RTT_INT32);

    func->ty = ty;
    func->name = "@getch";
    func->params = slice(KOOPA_RSIK_VALUE);
    func->bbs = slice(KOOPA_RSIK_BASIC_BLOCK);
    sym_list.list_add(func->name + 1, Symbol(SymType::FUNC, func));
    funcs.push_back(func);

    func = new koopa_raw_function_data_t();
    ty = new koopa_raw_type_kind_t();
    std::vector<const void *> plist;
    plist.clear();
    plist.push_back(pointer_type_kind(KOOPA_RTT_INT32));
    ty->tag = KOOPA_RTT_FUNCTION;
    ty->data.function.params = slice(plist, KOOPA_RSIK_TYPE);
    ty->data.function.ret = type_kind(KOOPA_RTT_INT32);

    func->ty = ty;
    func->name = "@getarray";
    func->params = slice(KOOPA_RSIK_VALUE);
    func->bbs = slice(KOOPA_RSIK_BASIC_BLOCK);
    sym_list.list_add(func->name + 1, Symbol(SymType::FUNC, func));
    funcs.push_back(func);

    func = new koopa_raw_function_data_t();
    ty = new koopa_raw_type_kind_t();
    plist.clear();
    plist.push_back(type_kind(KOOPA_RTT_INT32));
    ty->tag = KOOPA_RTT_FUNCTION;
    ty->data.function.params = slice(plist, KOOPA_RSIK_TYPE);
    ty->data.function.ret = type_kind(KOOPA_RTT_UNIT);

    func->ty = ty;
    func->name = "@putint";
    func->params = slice(KOOPA_RSIK_VALUE);
    func->bbs = slice(KOOPA_RSIK_BASIC_BLOCK);
    sym_list.list_add(func->name + 1, Symbol(SymType::FUNC, func));
    funcs.push_back(func);

    func = new koopa_raw_function_data_t();
    ty = new koopa_raw_type_kind_t();
    plist.clear();
    plist.push_back(type_kind(KOOPA_RTT_INT32));
    ty->tag = KOOPA_RTT_FUNCTION;
    ty->data.function.params = slice(plist, KOOPA_RSIK_TYPE);
    ty->data.function.ret = type_kind(KOOPA_RTT_UNIT);

    func->ty = ty;
    func->name = "@putch";
    func->params = slice(KOOPA_RSIK_VALUE);
    func->bbs = slice(KOOPA_RSIK_BASIC_BLOCK);
    sym_list.list_add(func->name + 1, Symbol(SymType::FUNC, func));
    funcs.push_back(func);

    func = new koopa_raw_function_data_t();
    ty = new koopa_raw_type_kind_t();
    plist.clear();
    plist.push_back(type_kind(KOOPA_RTT_INT32));
    plist.push_back(pointer_type_kind(KOOPA_RTT_INT32));
    ty->tag = KOOPA_RTT_FUNCTION;
    ty->data.function.params = slice(plist, KOOPA_RSIK_TYPE);
    ty->data.function.ret = type_kind(KOOPA_RTT_UNIT);

    func->ty = ty;
    func->name = "@putarray";
    func->params = slice(KOOPA_RSIK_VALUE);
    func->bbs = slice(KOOPA_RSIK_BASIC_BLOCK);
    sym_list.list_add(func->name + 1, Symbol(SymType::FUNC, func));
    funcs.push_back(func);

    func = new koopa_raw_function_data_t();
    ty = new koopa_raw_type_kind_t();
    ty->tag = KOOPA_RTT_FUNCTION;
    ty->data.function.params = slice(KOOPA_RSIK_TYPE);
    ty->data.function.ret = type_kind(KOOPA_RTT_UNIT);

    func->ty = ty;
    func->name = "@starttime";
    func->params = slice(KOOPA_RSIK_VALUE);
    func->bbs = slice(KOOPA_RSIK_BASIC_BLOCK);
    sym_list.list_add(func->name + 1, Symbol(SymType::FUNC, func));
    funcs.push_back(func);

    func = new koopa_raw_function_data_t();
    ty = new koopa_raw_type_kind_t();
    ty->tag = KOOPA_RTT_FUNCTION;
    ty->data.function.params = slice(KOOPA_RSIK_TYPE);
    ty->data.function.ret = type_kind(KOOPA_RTT_UNIT);

    func->ty = ty;
    func->name = "@stoptime";
    func->params = slice(KOOPA_RSIK_VALUE);
    func->bbs = slice(KOOPA_RSIK_BASIC_BLOCK);
    sym_list.list_add(func->name + 1, Symbol(SymType::FUNC, func));
    funcs.push_back(func);
}


void* CompUnitAST::Koop() const {
    sym_list.list_new();
    std::vector<const void*> funcs;
    std::vector<const void*> values;
    lib_func_load(funcs);

    for(auto item = (*clist).begin(); item != (*clist).end(); item ++) {
        (*item)->Koop(funcs, values);
    }

    koopa_raw_program_t *ret = new koopa_raw_program_t();
    
    // global value unused
    ret->values = slice(values, KOOPA_RSIK_VALUE);
    ret->funcs = slice(funcs, KOOPA_RSIK_FUNCTION);

    return ret;
}

void* CompItemAST::Koop(std::vector<const void*> &funcs, std::vector<const void*> &values) const {
    if(type == FUNC) {
        funcs.push_back(item->Koop());
    } else if( type == VAR) {
        item->Koop(values);
    } else if( type == CONST) {
        item->Koop();
    }

    return nullptr;
}

void* FuncDefAST::Koop() const {
    koopa_raw_function_data_t *ret = new koopa_raw_function_data_t();
    koopa_raw_basic_block_data_t *ent = new koopa_raw_basic_block_data_t();
    koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();

    
    std::vector<const void *> type_list;
    std::vector<const void *> value_list;

    sym_list.list_add(ident.c_str(), Symbol(SymType::FUNC, ret));

    ty->tag = KOOPA_RTT_FUNCTION;
    ty->data.function.ret = (koopa_raw_type_t)func_type->Koop();
    if(params != nullptr) {
        // std::vector<const void *> plist;
        for(size_t i = 0; i < params->size(); i++) {
            type_list.push_back( (*params)[i]->Koop() );
        }
        ty->data.function.params = slice(type_list, KOOPA_RSIK_TYPE);
    } else 
        ty->data.function.params = slice(KOOPA_RSIK_TYPE);

    std::string full_name = "@" + ident;
    char *name = new char[full_name.length() + 1];
    std::copy(full_name.begin(), full_name.end(), name);
    name[full_name.length()] = '\0';
        
    
    ret->ty = ty;
    ret->name = name;
    // plist.clear();

    if(params != nullptr) {
        for(size_t i = 0; i < params->size(); i++) {
            value_list.push_back( (*params)[i]->Koop(i) );
        }
        ret->params = slice(value_list, KOOPA_RSIK_VALUE);
    } else
        ret->params = slice(KOOPA_RSIK_VALUE);

    std::vector<const void *> blocks;
    blk_list.block_init(&blocks);

    // ent->insts = slice(insts,KOOPA_RSIK_VALUE); // insts insert on blocklist function
    ent->name = "\%entry";
    ent->params = slice(KOOPA_RSIK_VALUE);
    ent->used_by = slice(KOOPA_RSIK_VALUE);

    blk_list.block_add(ent);
    sym_list.list_new();
    if(params != nullptr) {
        for(size_t i = 0; i < params->size(); i++) {
            koopa_raw_value_data *alloc = new koopa_raw_value_data();
            alloc->ty = pointer_type_kind(KOOPA_RTT_INT32);
            alloc->name = ((koopa_raw_value_t)value_list[i])->name;
            alloc->used_by = slice(KOOPA_RSIK_VALUE);
            alloc->kind.tag = KOOPA_RVT_ALLOC;
            blk_list.inst2block(alloc);
            sym_list.list_add(alloc->name + 1, Symbol(SymType::VAR_SYM, alloc));

            koopa_raw_value_data *store = new koopa_raw_value_data();
            store->ty = pointer_type_kind(KOOPA_RTT_INT32);
            store->name = nullptr;
            store->used_by = slice(KOOPA_RSIK_VALUE);
            store->kind.tag = KOOPA_RVT_STORE;
            store->kind.data.store.dest = alloc;
            store->kind.data.store.value = (koopa_raw_value_t)value_list[i];
            blk_list.inst2block(store);
        }
    }
    block->Koop();
    blk_list.inst2block(return_value());
    sym_list.list_del();
    blk_list.block_quit();
    ret->bbs = slice(blocks, KOOPA_RSIK_BASIC_BLOCK);
    return ret;
}

void* FuncFParamAST::Koop() const {
    return (void *)btype->Koop();
}

void* FuncFParamAST::Koop(size_t index) const {
    koopa_raw_value_data *ret = new koopa_raw_value_data();
    char *name = new char[ident.length() + 1];
    ("@" + ident).copy(name, ident.length()+1);
    ret->name = name;
    ret->ty = (koopa_raw_type_t)(btype->Koop());
    ret->used_by = slice(KOOPA_RSIK_VALUE);

    ret->kind.tag = KOOPA_RVT_FUNC_ARG_REF;
    ret->kind.data.func_arg_ref.index = index;
    return ret;
}

void* BlockAST::Koop() const {
    if(blist != nullptr) {
        sym_list.list_new();
        
        for(auto bitem = (*blist).rbegin(); bitem != (*blist).rend(); bitem++) {
            (koopa_raw_value_t)(*bitem)->Koop();
        }

        sym_list.list_del();
    }
    return nullptr;
}


void* StmtAST::Koop() const {
    koopa_raw_value_data *ret = new koopa_raw_value_data();
    koopa_raw_slice_t used_by = slice(ret, KOOPA_RSIK_VALUE);

    ret->name = nullptr;
    ret->ty = type_kind(KOOPA_RTT_UNIT);
    ret->used_by = slice(KOOPA_RSIK_VALUE);

    if( expr != nullptr || type == BREAK || type == CONTINUE) {
        if(type == RET) {
            ret->kind.tag = KOOPA_RVT_RETURN;
            if(expr != nullptr)
                ret->kind.data.ret.value = (const koopa_raw_value_data_t *)expr->Koop();
            blk_list.inst2block(ret);
        } else if(type == VAL) {
            ret->kind.tag = KOOPA_RVT_STORE;
            ret->kind.data.store.value = (const koopa_raw_value_data_t *)expr->Koop();
            ret->kind.data.store.dest = (const koopa_raw_value_data_t *)lval->get_val();
            blk_list.inst2block(ret);
        } else if(type == EXP) {
            expr->Koop();       //dont know 
        } else if(type == BLOCK) {
            expr->Koop();
        } else if(type == IF) {
            ret = (koopa_raw_value_data *) expr->Koop();

            // else part
            koopa_raw_basic_block_data_t *el = new koopa_raw_basic_block_data_t();
            koopa_raw_basic_block_data_t *end = new koopa_raw_basic_block_data_t();

            int t_e_num = then_else_map[(koopa_raw_basic_block_data_t *)(ret->kind.data.branch.true_bb)];
            char *el_name = new char[7];
            ("\%else_" + std::to_string(t_e_num)).copy(el_name, 7);
            std::cout << el_name << std::endl;
            el->name = el_name;
            el->params = slice(KOOPA_RSIK_VALUE);
            el->used_by = slice(ret, KOOPA_RSIK_VALUE);

            char *end_name = new char[6];
            ("\%end_" + std::to_string(t_e_num)).copy(end_name, 6);
            std::cout << end_name << std::endl;
            end->name = end_name;
            end->params = slice(KOOPA_RSIK_VALUE);
            end->used_by = slice(KOOPA_RSIK_VALUE);

            if(lval == nullptr) {// no else
                ret->kind.data.branch.false_bb = end;
                ret->kind.data.branch.false_args = slice(KOOPA_RSIK_VALUE);
                blk_list.inst2block(jump_value(end));
                blk_list.block_add(end);
            } else {
                ret->kind.data.branch.false_bb = el;
                ret->kind.data.branch.false_args = slice(KOOPA_RSIK_VALUE);
                blk_list.inst2block(jump_value(end));
                blk_list.block_add(el);
                lval->Koop();
                blk_list.inst2block(jump_value(end));
                blk_list.block_add(end);
            }
        } else if(type == WHILE) {
            ret = (koopa_raw_value_data *)expr->Koop();
        } else if(type == BREAK) {
            if(whl_list.while_end() != nullptr)
                blk_list.inst2block(jump_value(whl_list.while_end()));
            else {
                std::cout << "break not in while" << std::endl;
                assert(false);
            }
        } else if(type == CONTINUE) {
            if(whl_list.while_entry() != nullptr)
                blk_list.inst2block(jump_value(whl_list.while_entry()));
            else {
                std::cout << "continue not in while" << std::endl;
                assert(false);
            }
        } else {
            assert(false);
        }    
    }
    return ret;
}

void* IfAST::Koop() const {
    koopa_raw_value_data *ret = new koopa_raw_value_data();
    ret->ty = type_kind(KOOPA_RTT_UNIT);
    ret->name = nullptr;
    ret->kind.tag = KOOPA_RVT_BRANCH;
    ret->kind.data.branch.cond = (koopa_raw_value_t)expr->Koop();
    
    blk_list.inst2block(ret);

    // if part
    koopa_raw_basic_block_data_t *then = new koopa_raw_basic_block_data_t();
    then_else_map[then] = then_else_num;
    then_else_num++;
    char *name = new char[7];
    ("%then_" + std::to_string(then_else_map[then])).copy(name, 7);
    then->name = name;
    then->params = slice(KOOPA_RSIK_VALUE);
    then->used_by = slice(KOOPA_RSIK_VALUE);
    
    blk_list.block_add(then);
    ret->kind.data.branch.true_args = slice(KOOPA_RSIK_VALUE);
    ret->kind.data.branch.true_bb = then;

    stmt->Koop();
    return ret;
}

void* WhileAST::Koop() const {
    koopa_raw_value_data *ret = new koopa_raw_value_data();

    // blk_list.inst2block(ret);

    koopa_raw_basic_block_data_t *entry = new koopa_raw_basic_block_data_t();
    koopa_raw_basic_block_data_t *body = new koopa_raw_basic_block_data_t();
    koopa_raw_basic_block_data_t *end = new koopa_raw_basic_block_data_t();
    whl_list.while_add(entry,end);
    int num = whl_list.while_dep();
    assert(num != -1);
    // int num = whl_stc.num;
    std::string entry_name = "%while_entry_" + std::to_string(num);
    std::string body_name = "%while_body_" + std::to_string(num);
    std::string end_name = "\%while_end_" + std::to_string(num);

    // then->name = strdup(then_name.c_str());


    entry->name = strdup(entry_name.c_str());
    entry->params = slice(KOOPA_RSIK_VALUE);
    entry->used_by = slice(KOOPA_RSIK_VALUE);
    blk_list.inst2block(jump_value(entry));
    blk_list.block_add(entry);

    ret->ty = type_kind(KOOPA_RTT_UNIT);
    ret->name = nullptr;
    ret->kind.tag = KOOPA_RVT_BRANCH;
    ret->kind.data.branch.cond = (koopa_raw_value_t)expr->Koop();


    body->name = strdup(body_name.c_str());
    body->params = slice(KOOPA_RSIK_VALUE);
    body->used_by = slice(KOOPA_RSIK_VALUE);

    end->name = strdup(end_name.c_str());
    end->params = slice(KOOPA_RSIK_VALUE);
    end->used_by = slice(KOOPA_RSIK_VALUE);

    ret->kind.data.branch.true_args = slice(KOOPA_RSIK_VALUE);
    ret->kind.data.branch.true_bb = body;
    ret->kind.data.branch.false_args = slice(KOOPA_RSIK_VALUE);
    ret->kind.data.branch.false_bb = end;

    blk_list.inst2block(ret);
    blk_list.block_add(body);

    stmt->Koop();
    whl_list.while_del();
    blk_list.inst2block(jump_value(entry));
    blk_list.block_add(end);
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

int NumberAST::Calcu() const { return num; }

void* ExpAST::Koop() const {
    return exp->Koop();
}

int ExpAST::Calcu() const {
    return exp->Calcu();
}

void* PrimaryExpAST::Koop() const {
    return expr->Koop();
}

int PrimaryExpAST::Calcu() const { return expr->Calcu(); }

void* LValAST::Koop() const {
    Symbol sym = (sym_list.list_get(ident));
    koopa_raw_value_data *ret = new koopa_raw_value_data();

    koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
    ty->tag = KOOPA_RTT_INT32;

    ret->name = nullptr;
    ret->ty = ty;
    ret->used_by = slice(KOOPA_RSIK_VALUE);
    if(sym.type == CONST_SYM) {
        ret->kind.tag = KOOPA_RVT_INTEGER;
        ret->kind.data.integer.value = sym.data.const_sym;
    } else if(sym.type == VAR_SYM) {
        ret->kind.tag = KOOPA_RVT_LOAD;
        ret->kind.data.load.src = sym.data.var_sym;
        blk_list.inst2block(ret);
    } else {
        assert(false);
    }
    return ret;
}

void* LValAST::get_val() const {
    return (void *)sym_list.list_get(ident).data.var_sym;
}


int LValAST::Calcu() const {
    return sym_list.list_get(ident).data.const_sym;
}


void* UnaryExpAST::Koop() const {
    if(type == EXP) {
        return primExp->Koop();
    } else if(type == OP) {
        koopa_raw_value_data *ret = new koopa_raw_value_data();
        koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();

        NumberAST zero;
        zero.num = 0;


        ty->tag = KOOPA_RTT_INT32;
    
        ret->kind.tag = KOOPA_RVT_BINARY;
        ret->name = nullptr;
        ret->ty = ty;
        ret->used_by = slice(KOOPA_RSIK_VALUE);


        ret->kind.data.binary.lhs = (koopa_raw_value_t)zero.Koop();
        ret->kind.data.binary.rhs = (koopa_raw_value_t)uexp->Koop();

        if (uop == "+") {
            ret->kind.data.binary.op = KOOPA_RBO_ADD;;
        } else if (uop == "-") {
            ret->kind.data.binary.op = KOOPA_RBO_SUB;
        } else if (uop == "!") {
            ret->kind.data.binary.op = KOOPA_RBO_EQ;
        } else {
            assert(false);
        }

        blk_list.inst2block(ret);
        return ret;
    } else if(type == FUNC) {    // function call            int add(int x, int y);
        koopa_raw_value_data *ret = new koopa_raw_value_data();
        Symbol sym = sym_list.list_get(uop);
        assert(sym.type == SymType::FUNC);
        ret->ty = sym.data.func->ty->data.function.ret;
        ret->name = nullptr;
        ret->used_by = slice(KOOPA_RSIK_VALUE);
        ret->kind.tag = KOOPA_RVT_CALL;
        ret->kind.data.call.callee = sym.data.func;
        
        if(plist != nullptr) {
            std::vector<const void *> args;

            for(auto func = (*plist).rbegin(); func != (*plist).rend(); func ++) {
                args.push_back((*func)->Koop());
            }
            ret->kind.data.call.args = slice(args, KOOPA_RSIK_VALUE);
        } else
            ret->kind.data.call.args = slice(KOOPA_RSIK_VALUE);
        blk_list.inst2block(ret);
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
  

void* MulExpAST::Koop() const {
    if(type == EXP) {
        return uexp->Koop();
    } else if(type == OP) {
        koopa_raw_value_data *ret = new koopa_raw_value_data();
        koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();

        ty->tag = KOOPA_RTT_INT32;

        ret->kind.tag = KOOPA_RVT_BINARY;
        ret->name = nullptr;
        ret->ty = ty;
        ret->used_by = slice(KOOPA_RSIK_VALUE);

        ret->kind.data.binary.lhs = (koopa_raw_value_t)mexp->Koop();
        ret->kind.data.binary.rhs = (koopa_raw_value_t)uexp->Koop();

        if (mop == "*") {
            ret->kind.data.binary.op = KOOPA_RBO_MUL;
        } else if (mop == "/") {
            ret->kind.data.binary.op = KOOPA_RBO_DIV;
        } else if (mop == "%") {
            ret->kind.data.binary.op = KOOPA_RBO_MOD;
        } else {
            assert(false);
        }
        blk_list.inst2block(ret);
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

void* AddExpAST::Koop() const {
    if(type == EXP) {
        return mexp->Koop();
    } else if(type == OP) {
        koopa_raw_value_data *ret = new koopa_raw_value_data();
        koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();

        ty->tag = KOOPA_RTT_INT32;

        ret->kind.tag = KOOPA_RVT_BINARY;
        ret->name = nullptr;
        ret->ty = ty;
        ret->used_by = slice(KOOPA_RSIK_VALUE);

        ret->kind.data.binary.lhs = (koopa_raw_value_t)mexp->Koop();
        ret->kind.data.binary.rhs = (koopa_raw_value_t)aexp->Koop();

        if (aop == "+") {
            ret->kind.data.binary.op = KOOPA_RBO_ADD;
        } else if (aop == "-") {
            ret->kind.data.binary.op = KOOPA_RBO_SUB;
        } else {
            assert(false);
        }
        blk_list.inst2block(ret);
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
  


void* RelExpAST::Koop() const {
    if(type == EXP) {
        return aexp->Koop();
    } else if(type == OP) {
        koopa_raw_value_data *ret = new koopa_raw_value_data();
        koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();

        ty->tag = KOOPA_RTT_INT32;

        ret->kind.tag = KOOPA_RVT_BINARY;
        ret->name = nullptr;
        ret->ty = ty;
        ret->used_by = slice(KOOPA_RSIK_VALUE);

        ret->kind.data.binary.lhs = (koopa_raw_value_t)rexp->Koop();
        ret->kind.data.binary.rhs = (koopa_raw_value_t)aexp->Koop();

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
        blk_list.inst2block(ret);
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

void* EqExpAST::Koop() const {
    if(type == EXP) {
        return rexp->Koop();
    } else if(type == OP) {
        koopa_raw_value_data *ret = new koopa_raw_value_data();
        koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();

        ty->tag = KOOPA_RTT_INT32;

        ret->kind.tag = KOOPA_RVT_BINARY;
        ret->name = nullptr;
        ret->ty = ty;
        ret->used_by = slice(KOOPA_RSIK_VALUE);

        ret->kind.data.binary.lhs = (koopa_raw_value_t)eexp->Koop();
        ret->kind.data.binary.rhs = (koopa_raw_value_t)rexp->Koop();

        if (eop == "==") {
            ret->kind.data.binary.op = KOOPA_RBO_EQ;
        } else if (eop == "!=") {
            ret->kind.data.binary.op = KOOPA_RBO_NOT_EQ;
        } else {
            assert(false);
        }
        blk_list.inst2block(ret);
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

void* Int2bool(const std::unique_ptr<BaseAST> &exp) {
    koopa_raw_value_data *ret = new koopa_raw_value_data();
    koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();

    ty->tag = KOOPA_RTT_INT32;

    ret->kind.tag = KOOPA_RVT_BINARY;
    ret->name = nullptr;
    ret->ty = ty;
    ret->used_by = slice(KOOPA_RSIK_VALUE);

    NumberAST zero;
    zero.num = 0;
    ret->kind.data.binary.lhs = (koopa_raw_value_t)exp->Koop();
    ret->kind.data.binary.rhs = (koopa_raw_value_t)zero.Koop();
    ret->kind.data.binary.op = KOOPA_RBO_NOT_EQ;

    blk_list.inst2block(ret);
    return ret;
}

void* LAndExpAST::Koop() const {
    if(type == EXP) 
        return eexp->Koop();
    
    // int result = 0;
    NumberAST zero;
    zero.num = 0;

    koopa_raw_value_data *result = new koopa_raw_value_data();
    result->ty = pointer_type_kind(KOOPA_RTT_INT32);
    result->name = "@r";
    result->used_by = slice(KOOPA_RSIK_VALUE);
    result->kind.tag = KOOPA_RVT_ALLOC;
    blk_list.inst2block(result);
    koopa_raw_value_data *re_store = new koopa_raw_value_data();
    re_store->ty = type_kind(KOOPA_RTT_UNIT);
    re_store->name = nullptr;
    re_store->used_by = slice(KOOPA_RSIK_VALUE);
    re_store->kind.tag = KOOPA_RVT_STORE;
    re_store->kind.data.store.dest = result;
    re_store->kind.data.store.value = (koopa_raw_value_t)zero.Koop();
    blk_list.inst2block(re_store);
    
    /***    if (lhs == 1) {
                result = rhs;
            }        ***/
    koopa_raw_value_data *branch = new koopa_raw_value_data();
    koopa_raw_basic_block_data_t *then = new koopa_raw_basic_block_data_t();
    koopa_raw_basic_block_data_t *el = new koopa_raw_basic_block_data_t();
    koopa_raw_basic_block_data_t *end = new koopa_raw_basic_block_data_t();
    std::string then_name = "\%a_then_" + std::to_string(r_te_num);
    std::string el_name = "\%a_else_" + std::to_string(r_te_num);
    std::string end_name = "\%a_end_" + std::to_string(r_te_num);
    r_te_num++;

    then->name = strdup(then_name.c_str());
    then->params = slice(KOOPA_RSIK_VALUE);
    then->used_by = slice(KOOPA_RSIK_VALUE);
    el->name = strdup(el_name.c_str());
    el->params = slice(KOOPA_RSIK_VALUE);
    el->used_by = slice(KOOPA_RSIK_VALUE);
    end->name = strdup(end_name.c_str());
    end->params = slice(KOOPA_RSIK_VALUE);
    end->used_by = slice(KOOPA_RSIK_VALUE);

    branch->ty = type_kind(KOOPA_RTT_UNIT);
    branch->name = nullptr;
    branch->used_by = slice(KOOPA_RSIK_VALUE);
    branch->kind.tag = KOOPA_RVT_BRANCH;
    branch->kind.data.branch.cond = (koopa_raw_value_t)Int2bool(laexp); // lhs == 1
    blk_list.inst2block(branch);
    

    branch->kind.data.branch.true_args = slice(KOOPA_RSIK_VALUE);
    branch->kind.data.branch.true_bb = then;
    blk_list.block_add(then);
    
    koopa_raw_value_data *re_store2 = new koopa_raw_value_data();
    re_store2->ty = type_kind(KOOPA_RTT_UNIT);
    re_store2->name = nullptr;
    re_store2->used_by = slice(KOOPA_RSIK_VALUE);
    re_store2->kind.tag = KOOPA_RVT_STORE;
    re_store2->kind.data.store.dest = result;
    re_store2->kind.data.store.value = (koopa_raw_value_t)Int2bool(eexp);
    blk_list.inst2block(re_store2);
    blk_list.inst2block(jump_value(end));

    branch->kind.data.branch.false_args = slice(KOOPA_RSIK_VALUE);
    branch->kind.data.branch.false_bb = end;
    blk_list.block_add(end);

    koopa_raw_value_data *ret = new koopa_raw_value_data();
    ret->ty = type_kind(KOOPA_RTT_INT32);
    ret->name = nullptr;
    ret->used_by = slice(KOOPA_RSIK_VALUE);
    ret->kind.tag = KOOPA_RVT_LOAD;
    ret->kind.data.load.src = result;
    blk_list.inst2block(ret);
    

    return ret;
    // return nullptr;
}

int LAndExpAST::Calcu() const {
    if(type == EXP)
        return eexp->Calcu();
    else {
        int la = laexp->Calcu();
        if(la == 0)
            return 0;
        else
            return eexp->Calcu();
    }
}

void* LOrExpAST::Koop() const {
    if(type == EXP) 
        return laexp->Koop();
    
    // int result = 1;
    NumberAST one;
    one.num = 1;

    koopa_raw_value_data *result = new koopa_raw_value_data();
    result->ty = pointer_type_kind(KOOPA_RTT_INT32);
    result->name = "@r";
    result->used_by = slice(KOOPA_RSIK_VALUE);
    result->kind.tag = KOOPA_RVT_ALLOC;
    blk_list.inst2block(result);
    koopa_raw_value_data *re_store = new koopa_raw_value_data();
    re_store->ty = type_kind(KOOPA_RTT_UNIT);
    re_store->name = nullptr;
    re_store->used_by = slice(KOOPA_RSIK_VALUE);
    re_store->kind.tag = KOOPA_RVT_STORE;
    re_store->kind.data.store.dest = result;
    re_store->kind.data.store.value = (koopa_raw_value_t)one.Koop();
    blk_list.inst2block(re_store);
    
    /***    if (lhs == 0) {
                result = rhs;
            }        ***/
    koopa_raw_value_data *branch = new koopa_raw_value_data();
    koopa_raw_basic_block_data_t *then = new koopa_raw_basic_block_data_t();
    koopa_raw_basic_block_data_t *el = new koopa_raw_basic_block_data_t();
    koopa_raw_basic_block_data_t *end = new koopa_raw_basic_block_data_t();
    std::string then_name = "\%o_then_" + std::to_string(r_te_num);
    std::string el_name = "\%o_else_" + std::to_string(r_te_num);
    std::string end_name = "\%o_end_" + std::to_string(r_te_num);
    r_te_num++;

    then->name = strdup(then_name.c_str());
    then->params = slice(KOOPA_RSIK_VALUE);
    then->used_by = slice(KOOPA_RSIK_VALUE);
    el->name = strdup(el_name.c_str());
    el->params = slice(KOOPA_RSIK_VALUE);
    el->used_by = slice(KOOPA_RSIK_VALUE);
    end->name = strdup(end_name.c_str());
    end->params = slice(KOOPA_RSIK_VALUE);
    end->used_by = slice(KOOPA_RSIK_VALUE);

    branch->ty = type_kind(KOOPA_RTT_UNIT);
    branch->name = nullptr;
    branch->used_by = slice(KOOPA_RSIK_VALUE);
    branch->kind.tag = KOOPA_RVT_BRANCH;
    branch->kind.data.branch.cond = (koopa_raw_value_t)Int2bool(loexp); // lhs == 1, do nothing
    blk_list.inst2block(branch);

    branch->kind.data.branch.true_args = slice(KOOPA_RSIK_VALUE);
    branch->kind.data.branch.true_bb = then;
    blk_list.block_add(then);
    blk_list.inst2block(jump_value(end));

    branch->kind.data.branch.false_args = slice(KOOPA_RSIK_VALUE);
    branch->kind.data.branch.false_bb = el;
    blk_list.block_add(el);
    
    koopa_raw_value_data *re_store2 = new koopa_raw_value_data();
    re_store2->ty = type_kind(KOOPA_RTT_UNIT);
    re_store2->name = nullptr;
    re_store2->used_by = slice(KOOPA_RSIK_VALUE);
    re_store2->kind.tag = KOOPA_RVT_STORE;
    re_store2->kind.data.store.dest = result;
    re_store2->kind.data.store.value = (koopa_raw_value_t)Int2bool(laexp);
    blk_list.inst2block(re_store2);
    blk_list.inst2block(jump_value(end));
    blk_list.block_add(end);

    koopa_raw_value_data *ret = new koopa_raw_value_data();
    ret->ty = type_kind(KOOPA_RTT_INT32);
    ret->name = nullptr;
    ret->used_by = slice(KOOPA_RSIK_VALUE);
    ret->kind.tag = KOOPA_RVT_LOAD;
    ret->kind.data.load.src = result;
    blk_list.inst2block(ret);

    return ret;
}

int LOrExpAST::Calcu() const {
    if(type == EXP) {
        return laexp->Calcu();
    } else {
        int lo = loexp->Calcu();
        if(lo == 1)
            return 1;
        else
            return laexp->Calcu();
    }
        return 0;
}

void* ConstDeclAST::Koop() const {
    // koopa_raw_type_t type = (const koopa_raw_type_t)btype->Koop();

    for(auto cdef = clist->begin(); cdef != clist->end(); cdef++) {
        (*cdef)->Koop();
    }
    return nullptr;
}

void* BTypeAST::Koop() const {
    koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
    if(btype == "int") {
        ty->tag = KOOPA_RTT_INT32;
    } else if(btype == "void") {
        ty->tag = KOOPA_RTT_UNIT;
    }
    return ty;
}


void* ConstDefAST::Koop() const {
    int val = cival->Calcu();
    sym_list.list_add(ident.c_str(), Symbol(SymType::CONST_SYM, val));
    return nullptr;
}


void* VarDeclAST::Koop() const {
    // koopa_raw_type_t type = (const koopa_raw_type_t)btype->Koop();

    for(auto vdef = vlist->begin(); vdef != vlist->end(); vdef++) {
        (*vdef)->Koop();
    }
    return nullptr;
}

void* VarDeclAST::Koop(std::vector<const void*> &insts) const {
    // koopa_raw_type_t type = (const koopa_raw_type_t)btype->Koop();

    for(auto vdef = vlist->begin(); vdef != vlist->end(); vdef++) {
        (*vdef)->Koop(insts);
    }
    return nullptr;
}


void* VarDefAST::Koop() const {
    koopa_raw_value_data *ret = new koopa_raw_value_data();
    koopa_raw_slice_t used = slice(ret, KOOPA_RSIK_VALUE);

    char *name = new char[ident.length() + 1 + 2];
    ("@" + ident + "_" + std::to_string(sym_list.cur_depth())).copy(name, ident.length()+1+2);
    ret->name = name;
    ret->ty = pointer_type_kind(KOOPA_RTT_INT32);
    ret->used_by = used;
    ret->kind.tag = KOOPA_RVT_ALLOC;
    blk_list.inst2block(ret);

    Symbol sym = Symbol(SymType::VAR_SYM, ret);
    sym_list.list_add(ident.c_str(), sym);

    if(type == VAL) {
        koopa_raw_value_data *store = new koopa_raw_value_data();
        store->ty = type_kind(KOOPA_RTT_INT32);
        store->name = nullptr;
        store->used_by = slice(KOOPA_RSIK_VALUE);
        store->kind.tag = KOOPA_RVT_STORE;
        store->kind.data.store.dest = (koopa_raw_value_t)ret;
        store->kind.data.store.value = (koopa_raw_value_t)ival->Koop();
        blk_list.inst2block(store);
    }
    return nullptr;
}

void* VarDefAST::Koop(std::vector<const void*>& insts) const {
    koopa_raw_value_data *ret = new koopa_raw_value_data();
    koopa_raw_slice_t used = slice(ret, KOOPA_RSIK_VALUE);

    char *name = new char[ident.length() + 1];
    ("@" + ident).copy(name, ident.length()+1);
    ret->name = name;
    ret->ty = pointer_type_kind(KOOPA_RTT_INT32);
    ret->used_by = used;
    ret->kind.tag = KOOPA_RVT_GLOBAL_ALLOC;
    // ret->kind.data.global_alloc.init = 
    insts.push_back(ret);
    Symbol sym = Symbol(SymType::VAR_SYM, ret);
    sym_list.list_add(ident.c_str(), sym);

    if(type == VAL) {
        ret->kind.data.global_alloc.init = (koopa_raw_value_t)ival->Koop();
    } else if(type == IDE) {
        NumberAST zero;
        zero.num = 0;
        ret->kind.data.global_alloc.init = (koopa_raw_value_t)(zero.Koop());
    }
    return nullptr;
}



