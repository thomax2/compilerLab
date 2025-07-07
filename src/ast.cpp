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
        item->Koop(values);
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
            type_list.push_back( (*params)[params->size() - 1 - i]->Koop() );
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
            value_list.push_back( (*params)[params->size() - 1 - i]->Koop(params->size() - 1 - i) );
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
            koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
            ty->tag = KOOPA_RTT_POINTER;
            ty->data.pointer.base = ((koopa_raw_value_t)value_list[i])->ty;
            alloc->ty = ty;
            alloc->name = ((koopa_raw_value_t)value_list[i])->name;
            alloc->used_by = slice(KOOPA_RSIK_VALUE);
            alloc->kind.tag = KOOPA_RVT_ALLOC;
            blk_list.inst2block(alloc);
            if(ty->data.pointer.base->tag == KOOPA_RTT_INT32)
                sym_list.list_add(alloc->name + 1, Symbol(SymType::VAR_SYM, alloc));
            else if(ty->data.pointer.base->tag == KOOPA_RTT_POINTER)
                sym_list.list_add(alloc->name + 1, Symbol(SymType::POINTER, alloc));
            else
                assert(false);
            koopa_raw_value_data *store = new koopa_raw_value_data();
            store->ty = type_kind(KOOPA_RTT_UNIT);
            store->name = nullptr;
            store->used_by = slice(KOOPA_RSIK_VALUE);
            store->kind.tag = KOOPA_RVT_STORE;
            store->kind.data.store.dest = alloc;
            store->kind.data.store.value = (koopa_raw_value_t)value_list[i];
            blk_list.inst2block(store);
        }
    }
    block->Koop();
    blk_list.inst2block(ret_value(((koopa_raw_type_t)func_type->Koop())->tag));
    sym_list.list_del();
    blk_list.block_quit();
    ret->bbs = slice(blocks, KOOPA_RSIK_BASIC_BLOCK);
    return ret;
}

void* FuncFParamAST::Koop() const {
    // return (void *)btype->Koop();
    if(type == VAR) 
        return type_kind(KOOPA_RTT_INT32);
    else if(type == ARRAY) {
        koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
        if(ilist == nullptr)
            ty = pointer_type_kind(KOOPA_RTT_INT32);
        else {
            std::vector<int> index_vec;
            for(auto index = (*ilist).begin(); index !=  (*ilist).end(); index++) {
                index_vec.push_back((*index)->Calcu());
            }
            ty->tag = KOOPA_RTT_POINTER;
            ty->data.pointer.base = array_type_kind(KOOPA_RTT_INT32, index_vec);
        }
        return ty;
    }
    assert(false);
    return nullptr;
}

void* FuncFParamAST::Koop(size_t index) const {
    koopa_raw_value_data *ret = new koopa_raw_value_data();
    std::string s = "@" + ident;
    char* name = new char[s.size() + 1];  // 分配空间（+1 for \0）
    std::strcpy(name, s.c_str());        // 自动带上 \0
    ret->name = name;

    ret->ty = (koopa_raw_type_t)(Koop());
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

    if(type == RET && expr == nullptr) {
        ret->kind.tag = KOOPA_RVT_RETURN;
        blk_list.inst2block(ret);
    }

    if( expr != nullptr || type == BREAK || type == CONTINUE) {
        if(type == RET) {
            ret->kind.tag = KOOPA_RVT_RETURN;
            ret->kind.data.ret.value = (const koopa_raw_value_data_t *)expr->Koop();
            blk_list.inst2block(ret);
        } else if(type == VAL) {
            ret->kind.tag = KOOPA_RVT_STORE;
            LValAST *val = dynamic_cast<LValAST *>(lval.get()); 
            ret->kind.data.store.value = (const koopa_raw_value_data_t *)expr->Koop();
            ret->kind.data.store.dest = (const koopa_raw_value_data_t *)val->get_val();
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
            // std::cout << el_name << std::endl;
            el->name = el_name;
            el->params = slice(KOOPA_RSIK_VALUE);
            el->used_by = slice(ret, KOOPA_RSIK_VALUE);

            char *end_name = new char[6];
            ("\%end_" + std::to_string(t_e_num)).copy(end_name, 6);
            // std::cout << end_name << std::endl;
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
    } else if(sym.type == ARRAY_SYM) {
        std::vector<koopa_raw_value_data *> getelemptr_vec;
        if(ilist != nullptr) {
            // @arr = alloc [i32, 2]       // @arr 的类型是 *[i32, 2]
            // %ptr = getelemptr @arr, 1   // %ptr 的类型是 *i32
            // %value = load %ptr          // %value 的类型是 i32

            for(int i = 0; i < ilist->size(); i++) {
                koopa_raw_value_data *get = new koopa_raw_value_data();
                get->name = nullptr;
                get->used_by = slice(KOOPA_RSIK_VALUE);
                get->kind.tag = KOOPA_RVT_GET_ELEM_PTR;
                get->kind.data.get_elem_ptr.index = (koopa_raw_value_t)(*ilist)[ilist->size() - 1-i]->Koop();
    
                koopa_raw_type_kind *ty = new koopa_raw_type_kind();
                ty->tag = KOOPA_RTT_POINTER;
                if(i == 0) {
                    ty->data.pointer.base = sym.data.array->ty->data.pointer.base->data.array.base;
                    get->ty = ty;
                    assert(sym.data.array->ty->data.pointer.base->tag == KOOPA_RTT_ARRAY);
                    get->kind.data.get_elem_ptr.src = sym.data.array;
                } else {
                    ty->data.pointer.base = getelemptr_vec[i-1]->ty->data.pointer.base->data.array.base;
                    get->ty = ty;
                    assert(getelemptr_vec[i-1]->ty->data.pointer.base->tag == KOOPA_RTT_ARRAY);
                    get->kind.data.get_elem_ptr.src = getelemptr_vec[i-1];
                }
                getelemptr_vec.push_back(get);
                blk_list.inst2block(get);
            }

            if(getelemptr_vec.back()->ty->data.pointer.base->tag == KOOPA_RTT_ARRAY){
                // @arr = alloc [[i32, 3], 2]       // @arr 的类型是 *[[i32, 3], 2]
                // %a1 = getelemptr @arr, 1         // %ptr 的类型是 *[i32, 3]
                // %a1_ptr = getelemptr %a1, 0      // *i32，数组首地址
                // reference point  tag != KOOPA_RTT_INT32;
                koopa_raw_value_data *get = new koopa_raw_value_data();
                koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
                ty->tag = KOOPA_RTT_POINTER;
                ty->data.pointer.base = getelemptr_vec.back()->ty->data.pointer.base->data.array.base;
                
                get->ty = ty;
                get->name = nullptr;
                get->used_by = slice(KOOPA_RSIK_VALUE);
                get->kind.tag = KOOPA_RVT_GET_ELEM_PTR;
                assert(getelemptr_vec.back()->ty->data.pointer.base->tag == KOOPA_RTT_ARRAY);
                get->kind.data.get_elem_ptr.src = getelemptr_vec.back();
                NumberAST zero;
                zero.num = 0;
                get->kind.data.get_elem_ptr.index = (koopa_raw_value_t)zero.Koop();
                blk_list.inst2block(get);
                ret = get;
            } else {
                ret->kind.tag = KOOPA_RVT_LOAD;
                ret->kind.data.load.src = getelemptr_vec.back();
                blk_list.inst2block(ret);    
            }
        } else {
            // @arr = alloc [[i32, 3], 2]       // @arr 的类型是 *[[i32, 3], 2]
            // %p = getelemptr @arr, 0          ; 相当于 a → &a[0]，类型是 *[i32, 3]
            koopa_raw_value_data *get = new koopa_raw_value_data();
            koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
            ty->tag = KOOPA_RTT_POINTER;
            ty->data.pointer.base = sym.data.array->ty->data.pointer.base->data.array.base;
            
            get->ty = ty;
            get->name = nullptr;
            get->used_by = slice(KOOPA_RSIK_VALUE);
            get->kind.tag = KOOPA_RVT_GET_ELEM_PTR;
            assert(sym.data.array->ty->data.pointer.base->tag == KOOPA_RTT_ARRAY);
            get->kind.data.get_elem_ptr.src = sym.data.array;
            NumberAST zero;
            zero.num = 0;
            get->kind.data.get_elem_ptr.index = (koopa_raw_value_t)zero.Koop();
            blk_list.inst2block(get);
            ret = get;
        }
    } else if(sym.type == POINTER){
    // Key Tip: pointer need use getptr first, the arr[], arr[][1] in funcparam all pointer, so we need use getptr
    // when we use true array,  arr, arr[1], we only use getelemptr

        koopa_raw_value_data *load = new koopa_raw_value_data();
        load->name = nullptr;
        load->used_by = slice(KOOPA_RSIK_VALUE);
        load->ty = sym.data.pointer->ty->data.pointer.base;
        load->kind.tag = KOOPA_RVT_LOAD;
        load->kind.data.load.src = sym.data.pointer;
        blk_list.inst2block(load);
        std::vector<koopa_raw_value_data *> get_vec;
        if(ilist != nullptr) {
            // @arr = alloc *[i32, 3]        // @arr 的类型是 **[i32, 3]
            // %ptr1 = load @arr             // %ptr1 的类型是 *[i32, 3]
            // %ptr2 = getptr %ptr1, 1       // %ptr2 的类型是 *[i32, 3]
            // %ptr3 = getelemptr %ptr2, 2   // %ptr3 的类型是 *i32
            // %value = load %ptr3          // %value 的类型是 i32

            for(int i = 0; i < ilist->size(); i++) {
                koopa_raw_value_data *get = new koopa_raw_value_data();
                get->name = nullptr;
                get->used_by = slice(KOOPA_RSIK_VALUE);
                if(i == 0 ) {   // one getptr, many getelemptr
                    get->ty = load->ty;
                    get->kind.tag = KOOPA_RVT_GET_PTR;
                    get->kind.data.get_ptr.src = load;
                    get->kind.data.get_ptr.index = (koopa_raw_value_t)(*ilist)[ilist->size() - 1 - i]->Koop();
                } else {
                    koopa_raw_type_kind *ty = new koopa_raw_type_kind();
                    ty->tag = KOOPA_RTT_POINTER;
                    ty->data.pointer.base = get_vec[i-1]->ty->data.pointer.base->data.array.base;
                    get->ty = ty;
                    get->kind.tag = KOOPA_RVT_GET_ELEM_PTR;
                    assert(get_vec[i-1]->ty->data.pointer.base->tag == KOOPA_RTT_ARRAY);
                    get->kind.data.get_elem_ptr.src = get_vec[i-1];
                    get->kind.data.get_elem_ptr.index = (koopa_raw_value_t)(*ilist)[ilist->size() - 1 - i]->Koop();
                }
                get_vec.push_back(get);
                blk_list.inst2block(get);
            }
            
            if(get_vec.back()->ty->data.pointer.base->tag == KOOPA_RTT_ARRAY){
                // @arr = alloc [[i32, 3], 2]       // @arr 的类型是 *[[i32, 3], 2]
                // %a1 = getelemptr @arr, 1         // %ptr 的类型是 *[i32, 3]
                // %a1_ptr = getelemptr %a1, 0      // *i32，数组首地址
                // reference point  tag != KOOPA_RTT_INT32;
                koopa_raw_value_data *get = new koopa_raw_value_data();
                koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
                ty->tag = KOOPA_RTT_POINTER;
                ty->data.pointer.base = get_vec.back()->ty->data.pointer.base->data.array.base;
                
                get->ty = ty;
                get->name = nullptr;
                get->used_by = slice(KOOPA_RSIK_VALUE);
                get->kind.tag = KOOPA_RVT_GET_ELEM_PTR;
                // assert(get_vec.back)
                assert(get_vec.back()->ty->data.pointer.base->tag == KOOPA_RTT_ARRAY);
                get->kind.data.get_elem_ptr.src = get_vec.back();
                NumberAST zero;
                zero.num = 0;
                get->kind.data.get_elem_ptr.index = (koopa_raw_value_t)zero.Koop();
                blk_list.inst2block(get);
                ret = get;
            } else {
                ret->kind.tag = KOOPA_RVT_LOAD;
                ret->kind.data.load.src = get_vec.back();
                blk_list.inst2block(ret);    
            }
        } else {
            // @arr = alloc [[i32, 3], 2]       // @arr 的类型是 *[[i32, 3], 2]
            // %ptr1 = load @arr             // %ptr1 的类型是 *[i32, 3]
            // %p = getptr %ptr1, 0          ; 相当于 a → &a[0]，类型是 *[i32, 3]
            koopa_raw_value_data *get = new koopa_raw_value_data();
            koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
            ty->tag = KOOPA_RTT_POINTER;
            ty->data.pointer.base = sym.data.array->ty->data.pointer.base->data.array.base;
            
            get->ty = ty;
            get->name = nullptr;
            get->used_by = slice(KOOPA_RSIK_VALUE);
            get->kind.tag = KOOPA_RVT_GET_PTR;
            get->kind.data.get_ptr.src = load;
            NumberAST zero;
            zero.num = 0;
            get->kind.data.get_ptr.index = (koopa_raw_value_t)zero.Koop();
            blk_list.inst2block(get);
            ret = get;
        }
    } else {
        assert(false);
    }
    return ret;
}

void* LValAST::get_val() const {
    Symbol sym = (sym_list.list_get(ident));
    if(sym.type == VAR_SYM) {
        return (void *)sym.data.var_sym;
    } else if(sym.type == ARRAY_SYM) {
        // @arr = alloc [i32, 2]         // 同上
        // %ptr = getelemptr @arr, 1     // 获取第 1 个元素的地址，类型：*i32
        // store 1, %ptr                 // 把常数 1 存进该地址  <--- this step in Stmt

        std::vector<koopa_raw_value_data *> getelemptr_vec;
        if(ilist != nullptr) {
            for(int i = 0; i < ilist->size(); i++) {
                koopa_raw_value_data *get = new koopa_raw_value_data();
                get->name = nullptr;
                get->used_by = slice(KOOPA_RSIK_VALUE);
                get->kind.tag = KOOPA_RVT_GET_ELEM_PTR;
                get->kind.data.get_elem_ptr.index = (koopa_raw_value_t)(*ilist)[ilist->size() - 1-i]->Koop();
    
                koopa_raw_type_kind *ty = new koopa_raw_type_kind();
                ty->tag = KOOPA_RTT_POINTER;
                if(i == 0) {
                    ty->data.pointer.base = sym.data.array->ty->data.pointer.base->data.array.base;
                    get->ty = ty;
                    assert(sym.data.array->ty->data.pointer.base->tag == KOOPA_RTT_ARRAY);
                    get->kind.data.get_elem_ptr.src = sym.data.array;
                } else {
                    ty->data.pointer.base = getelemptr_vec[i-1]->ty->data.pointer.base->data.array.base;
                    get->ty = ty;
                    assert(getelemptr_vec[i-1]->ty->data.pointer.base->tag == KOOPA_RTT_ARRAY);
                    get->kind.data.get_elem_ptr.src = getelemptr_vec[i-1];
                }
                getelemptr_vec.push_back(get);
                blk_list.inst2block(get);
            }
            return getelemptr_vec.back();
        }
    } else if(sym.type == FUNC || sym.type == CONST_SYM) {
        std::cout << "left value type wrong" << std::endl;
        assert(false);
    } else if(sym.type == POINTER) {
        // @arr = alloc *[i32, 3]        // @arr 的类型是 **[i32, 3]
        // %ptr1 = load @arr             // %ptr1 的类型是 *[i32, 3]
        // %ptr2 = getptr %ptr1, 1       // %ptr2 的类型是 *[i32, 3]
        // %ptr3 = getelemptr %ptr2, 2   // %ptr3 的类型是 *i32
        // store 1, %ptr3               ; 将 1 存入该地址  <----- this step in Stmt

        koopa_raw_value_data *load = new koopa_raw_value_data();
        load->name = nullptr;
        load->used_by = slice(KOOPA_RSIK_VALUE);
        load->ty = sym.data.pointer->ty->data.pointer.base;
        load->kind.tag = KOOPA_RVT_LOAD;
        load->kind.data.load.src = sym.data.pointer;
        blk_list.inst2block(load);
        std::vector<koopa_raw_value_data *> get_vec;
        for(int i = 0; i < ilist->size(); i++) {
            koopa_raw_value_data *get = new koopa_raw_value_data();
            get->name = nullptr;
            get->used_by = slice(KOOPA_RSIK_VALUE);
            if(i == 0 ) {   // one getptr, many getelemptr
                get->ty = load->ty;
                get->kind.tag = KOOPA_RVT_GET_PTR;
                get->kind.data.get_ptr.src = load;
                get->kind.data.get_ptr.index = (koopa_raw_value_t)(*ilist)[ilist->size() - 1 - i]->Koop();
            } else {
                koopa_raw_type_kind *ty = new koopa_raw_type_kind();
                ty->tag = KOOPA_RTT_POINTER;
                ty->data.pointer.base = get_vec[i-1]->ty->data.pointer.base;
                get->ty = ty;
                get->kind.tag = KOOPA_RVT_GET_ELEM_PTR;
                assert(get_vec[i-1]->ty->data.pointer.base->tag == KOOPA_RTT_ARRAY);
                get->kind.data.get_elem_ptr.src = get_vec[i-1];
                get->kind.data.get_elem_ptr.index = (koopa_raw_value_t)(*ilist)[ilist->size() - 1 - i]->Koop();
            }
            get_vec.push_back(get);
            blk_list.inst2block(get);
        }
        return get_vec.back();
    }
    assert(false);
    return nullptr;
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
        // @arr = alloc *i32         // @arr 的类型是 **i32
        // %ptr1 = load @arr         // %ptr1 的类型是 *i32
        // %ptr2 = getptr %ptr1, 1   // %ptr2 的类型是 *i32
        // %value = load %ptr2       // %value 的类型是 i32
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

            for(auto func = (*plist).begin(); func != (*plist).end(); func ++) {
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

    for(auto cdef = clist->rbegin(); cdef != clist->rend(); cdef++) {
        (*cdef)->Koop();
    }
    return nullptr;
}

void* ConstDeclAST::Koop(std::vector<const void*>& insts) const {
    // koopa_raw_type_t type = (const koopa_raw_type_t)btype->Koop();

    for(auto cdef = clist->rbegin(); cdef != clist->rend(); cdef++) {
        (*cdef)->Koop(insts);
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
    if(cexp == nullptr) {
        int val = cival->Calcu();
        sym_list.list_add(ident.c_str(), Symbol(SymType::CONST_SYM, val));
        return nullptr;
    } else {
        koopa_raw_value_data *ret = new koopa_raw_value_data();
        koopa_raw_slice_t used = slice(ret, KOOPA_RSIK_VALUE);
    
        std::string full_name = "@" + ident + "_" + std::to_string(sym_list.cur_depth());
        char *name = new char[full_name.length() + 1];  // +1 for '\0'
        std::strcpy(name, full_name.c_str());          // 自动加 '\0'
        ret->name = name;
        ret->used_by = used;
        ret->kind.tag = KOOPA_RVT_ALLOC;
        blk_list.inst2block(ret);
    
        std::vector<int> index_vec;
        for(auto index = cexp->rbegin(); index != cexp->rend(); index++) {
            index_vec.push_back((*index)->Calcu());
        }
        koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
        ty->tag = KOOPA_RTT_POINTER;
        ty->data.pointer.base = array_type_kind(KOOPA_RTT_INT32, index_vec);
        ret->ty = ty;

        Symbol sym = Symbol(SymType::ARRAY_SYM, ret);
        sym_list.list_add(ident.c_str(), sym);
        int array_size = 1;
        for(int i = 0; i < index_vec.size(); i++) {
            array_size *= index_vec[i];
        }

        std::vector<const void*> val_vec;
        InitValAST *initval = dynamic_cast<InitValAST *>(cival.get());
        if(initval->type == InitValAST::EMPTY) {        // x[2] = {}
            koopa_raw_value_data *store = new koopa_raw_value_data();
            store->ty = type_kind(KOOPA_RTT_UNIT);
            store->name = nullptr;
            store->used_by = slice(KOOPA_RSIK_VALUE);
            store->kind.tag = KOOPA_RVT_STORE;
            store->kind.data.store.dest = (koopa_raw_value_t)ret;
            store->kind.data.store.value = zero_value(array_type_kind(KOOPA_RTT_INT32, index_vec));
            blk_list.inst2block(store);
        } else {
            initval->get_complete_val(val_vec, index_vec, 0);   // val_vec full of NumberAST
            
            for(int i = 0; i< val_vec.size(); i++) {            // put every entry on store
                std::vector<koopa_raw_value_data *> getelemptr_vec;
                getelemptr_vec.clear();
                int tmp_index = i;
                int tmp_size = array_size;
                for(int j = 0; j < index_vec.size() ; j++) {  // index front to back x[1][2] ---> ptr 1 + ptr 2
                    // @arr = alloc [[i32, 3], 2]    // @arr 的类型是 *[[i32, 3], 2]
                    // %ptr1 = getelemptr @arr, 1    // %ptr1 的类型是 *[i32, 3]
                    // %ptr2 = getelemptr %ptr1, 2   // %ptr2 的类型是 *i32
                    // %value = load %ptr2           // %value 的类型是 i32
                    tmp_size /= index_vec[j];
                    tmp_index = tmp_index/tmp_size;
                    int tmp = tmp_index;
                    tmp_index = tmp_index % tmp_size;

                    koopa_raw_value_data *get = new koopa_raw_value_data();
                    get->name = nullptr;
                    get->ty = type_kind(KOOPA_RTT_INT32);
                    get->used_by = slice(KOOPA_RSIK_VALUE);
                    get->kind.tag = KOOPA_RVT_GET_ELEM_PTR;
                    get->kind.data.get_elem_ptr.src = (j == 0) ? ret : getelemptr_vec[j-1];
                    NumberAST num;
                    num.num = tmp;
                    get->kind.data.get_elem_ptr.index = (koopa_raw_value_t)num.Koop();
                    getelemptr_vec.push_back(get);
                    blk_list.inst2block(get);
                }
                koopa_raw_value_data *store = new koopa_raw_value_data();
                store->ty = type_kind(KOOPA_RTT_UNIT);
                store->name = nullptr;
                store->used_by = slice(KOOPA_RSIK_VALUE);
                store->kind.tag = KOOPA_RVT_STORE;
                store->kind.data.store.dest = getelemptr_vec[index_vec.size() - 1];
                store->kind.data.store.value = (koopa_raw_value_t)val_vec[i];
                blk_list.inst2block(store);    
            }
        }
        return nullptr;
    }
    return nullptr;
}

void* ConstDefAST::Koop(std::vector<const void*>& insts) const {
    if(cexp == nullptr) {
        koopa_raw_value_data *ret = new koopa_raw_value_data();
        koopa_raw_slice_t used = slice(ret, KOOPA_RSIK_VALUE);
    
        std::string full_name = "@" + ident;
        char *name = new char[full_name.length() + 1];  // +1 for '\0'
        std::strcpy(name, full_name.c_str());          // 自动拷贝并终止
        ret->name = name;
        ret->used_by = used;
        ret->kind.tag = KOOPA_RVT_GLOBAL_ALLOC;
        ret->ty = pointer_type_kind(KOOPA_RTT_INT32);
        // assert(cival != nullptr);
        InitValAST *initval = dynamic_cast<InitValAST *>(cival.get());
        insts.push_back(ret);
        koopa_raw_value_data * init = new koopa_raw_value_data();
        init->name = nullptr;
        init->used_by = slice(KOOPA_RSIK_VALUE);
        init->ty = type_kind(KOOPA_RTT_INT32);
        init->kind.tag = KOOPA_RVT_INTEGER;
        init->kind.data.integer.value = initval->exp->Calcu();
        ret->kind.data.global_alloc.init = init;
        sym_list.list_add(ident.c_str(), Symbol(SymType::CONST_SYM, init->kind.data.integer.value));
        return nullptr;
    } else {
        koopa_raw_value_data *ret = new koopa_raw_value_data();
        koopa_raw_slice_t used = slice(ret, KOOPA_RSIK_VALUE);
    
        std::string full_name = "@" + ident;
        char *name = new char[full_name.length() + 1];  // +1 for '\0'
        std::strcpy(name, full_name.c_str());          // 自动拷贝并终止
        ret->name = name;
        ret->used_by = used;
        ret->kind.tag = KOOPA_RVT_GLOBAL_ALLOC;
        std::vector<int> index_vec;
        for(auto index = cexp->rbegin(); index != cexp->rend(); index++) {
            index_vec.push_back((*index)->Calcu());
        }
        koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
        ty->tag = KOOPA_RTT_POINTER;
        ty->data.pointer.base = array_type_kind(KOOPA_RTT_INT32, index_vec);
        ret->ty = ty;

        insts.push_back(ret);
        Symbol sym = Symbol(SymType::ARRAY_SYM, ret);
        sym_list.list_add(ident.c_str(), sym);

        // ret->kind.data.global_alloc.init = (koopa_raw_value_t)ival->Koop();
        std::vector<const void*> val_vec;
        InitValAST *initval = dynamic_cast<InitValAST *>(cival.get());
        if(initval->type == InitValAST::EMPTY) {
            ret->kind.data.global_alloc.init = zero_value(array_type_kind(KOOPA_RTT_INT32, index_vec));
        } else {
            initval->get_complete_val(val_vec, index_vec, 0); // val_vec full of NumberAST
            ret->kind.data.global_alloc.init = (koopa_raw_value_t)initval->Koop(val_vec, index_vec, 0);
        }
        return nullptr;
    }
    return nullptr;
}


void* VarDeclAST::Koop() const {
    // koopa_raw_type_t type = (const koopa_raw_type_t)btype->Koop();

    for(auto vdef = vlist->rbegin(); vdef != vlist->rend(); vdef++) {
        (*vdef)->Koop();
    }
    return nullptr;
}

void* VarDeclAST::Koop(std::vector<const void*> &insts) const {
    // koopa_raw_type_t type = (const koopa_raw_type_t)btype->Koop();

    for(auto vdef = vlist->rbegin(); vdef != vlist->rend(); vdef++) {
        (*vdef)->Koop(insts);
    }
    return nullptr;
}


void* VarDefAST::Koop() const {
    koopa_raw_value_data *ret = new koopa_raw_value_data();
    koopa_raw_slice_t used = slice(ret, KOOPA_RSIK_VALUE);

    std::string full_name = "@" + ident + "_" + std::to_string(sym_list.cur_depth());
    char *name = new char[full_name.length() + 1];  // +1 for '\0'
    std::strcpy(name, full_name.c_str());          // 复制并加终止符
    ret->name = name;
    ret->used_by = used;
    ret->kind.tag = KOOPA_RVT_ALLOC;
    blk_list.inst2block(ret);
    if(exp == nullptr) {
        ret->ty = pointer_type_kind(KOOPA_RTT_INT32);
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
    } else {
        std::vector<int> index_vec;
        for(auto index = exp->rbegin(); index != exp->rend(); index++) {
            index_vec.push_back((*index)->Calcu());
            // cout << to_string(index_vec.back()) << endl;
        }
        // cout << to_string(exp->size()) << endl;
        koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
        ty->tag = KOOPA_RTT_POINTER;
        ty->data.pointer.base = array_type_kind(KOOPA_RTT_INT32, index_vec);
        ret->ty = ty;

        Symbol sym = Symbol(SymType::ARRAY_SYM, ret);
        sym_list.list_add(ident.c_str(), sym);
        int array_size = 1;
        for(int i = 0; i < index_vec.size(); i++) {
            array_size *= index_vec[i];
        }
        if(type == VAL) {
            std::vector<const void*> val_vec;
            InitValAST *initval = dynamic_cast<InitValAST *>(ival.get());
            if(initval->type == InitValAST::EMPTY) {        // x[2] = {}
                koopa_raw_value_data *store = new koopa_raw_value_data();
                store->ty = type_kind(KOOPA_RTT_UNIT);
                store->name = nullptr;
                store->used_by = slice(KOOPA_RSIK_VALUE);
                store->kind.tag = KOOPA_RVT_STORE;
                store->kind.data.store.dest = (koopa_raw_value_t)ret;
                store->kind.data.store.value = zero_value(array_type_kind(KOOPA_RTT_INT32, index_vec));
                blk_list.inst2block(store);    
            } else {
                initval->get_complete_val(val_vec, index_vec, 0);   // val_vec full of NumberAST
                // assert(ret->ty->data.array.len != 0);
                for(int i = 0; i< val_vec.size(); i++) {            // put every entry on store
                    std::vector<koopa_raw_value_data *> getelemptr_vec;
                    getelemptr_vec.clear();
                    int tmp_index = i;
                    int tmp_size = array_size;
                    for(int j = 0; j <  index_vec.size(); j++) {  // index front to back x[1][2] ---> ptr 1 + ptr 2
                        // @arr = alloc [[i32, 3], 2]    // @arr 的类型是 *[[i32, 3], 2]
                        // %ptr1 = getelemptr @arr, 1    // %ptr1 的类型是 *[i32, 3]
                        // %ptr2 = getelemptr %ptr1, 2   // %ptr2 的类型是 *i32
                        // %value = load %ptr2           // %value 的类型是 i32

                        tmp_size /= index_vec[j];
                        // tmp_index = tmp_index/tmp_size;
                        int tmp = tmp_index/tmp_size;
                        tmp_index = tmp_index % tmp_size;

                        koopa_raw_value_data *get = new koopa_raw_value_data();
                        // koopa_raw_type_kind *ty = new koopa_raw_type_kind();
                        // if(tmp_vec.size() != 0) {
                        //     // ty->tag = KOOPA_RTT_ARRAY;
                        //     ty = array_type_kind(KOOPA_RTT_INT32, tmp_vec);    
                        // } else {
                        //     ty = type_kind(KOOPA_RTT_INT32);
                        // }
                        // if(j > 0) {
                        //     ty->tag = KOOPA_RTT_ARRAY;
                        //     ty->data.array.base = getelemptr_vec[j-1]->ty->data.array.base;
                        // }
                        // else {
                        //     ty->tag = KOOPA_RTT_ARRAY;
                        //     ty->data.array.base = ret->ty->data.pointer.base->data.array.base;
                        // }
                            
                        get->name = nullptr;
                        get->ty = type_kind(KOOPA_RTT_INT32);
                        get->used_by = slice(KOOPA_RSIK_VALUE);
                        get->kind.tag = KOOPA_RVT_GET_ELEM_PTR;
                        
                        get->kind.data.get_elem_ptr.src = (j == 0) ? ret : getelemptr_vec[j-1];
                        // if(get->ty->data.array.len == 0 && get->ty->tag == KOOPA_RTT_ARRAY){
                        //     std::cout << to_string(j) <<std::endl;
                        //     assert(get->ty->data.array.len != 0);
                        // }
                        NumberAST num;
                        num.num = tmp;
                        get->kind.data.get_elem_ptr.index = (koopa_raw_value_t)num.Koop();
                        getelemptr_vec.push_back(get);
                        blk_list.inst2block(get);
                    }
                    koopa_raw_value_data *store = new koopa_raw_value_data();
                    store->ty = type_kind(KOOPA_RTT_UNIT);
                    store->name = nullptr;
                    store->used_by = slice(KOOPA_RSIK_VALUE);
                    store->kind.tag = KOOPA_RVT_STORE;
                    store->kind.data.store.dest = getelemptr_vec[index_vec.size() - 1];
                    store->kind.data.store.value = (koopa_raw_value_t)val_vec[i];
                    blk_list.inst2block(store);    
                }
            }
        } else if(type == IDE) {
            koopa_raw_value_data *store = new koopa_raw_value_data();
            store->ty = type_kind(KOOPA_RTT_UNIT);
            store->name = nullptr;
            store->used_by = slice(KOOPA_RSIK_VALUE);
            store->kind.tag = KOOPA_RVT_STORE;
            store->kind.data.store.dest = (koopa_raw_value_t)ret;
            store->kind.data.store.value = zero_value(array_type_kind(KOOPA_RTT_INT32, index_vec));
            blk_list.inst2block(store);
        }
    }
    return nullptr;
}

void* VarDefAST::Koop(std::vector<const void*>& insts) const {
    koopa_raw_value_data *ret = new koopa_raw_value_data();
    koopa_raw_slice_t used = slice(ret, KOOPA_RSIK_VALUE);

    std::string s = "@" + ident;
    char* name = new char[s.size() + 1];
    std::strcpy(name, s.c_str());  // 自动带上 '\0'
    ret->name = name;

    ret->used_by = used;
    ret->kind.tag = KOOPA_RVT_GLOBAL_ALLOC;
    if(exp == nullptr) {
        ret->ty = pointer_type_kind(KOOPA_RTT_INT32);
        
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
    } else {       //array
        std::vector<int> index_vec;
        for(auto index = exp->rbegin(); index != exp->rend(); index++) {
            index_vec.push_back((*index)->Calcu());
        }
        koopa_raw_type_kind_t *ty = new koopa_raw_type_kind_t();
        ty->tag = KOOPA_RTT_POINTER;
        ty->data.pointer.base = array_type_kind(KOOPA_RTT_INT32, index_vec);
        ret->ty = ty;

        insts.push_back(ret);
        Symbol sym = Symbol(SymType::ARRAY_SYM, ret);
        sym_list.list_add(ident.c_str(), sym);

        if(type == VAL) {
            // ret->kind.data.global_alloc.init = (koopa_raw_value_t)ival->Koop();
            std::vector<const void*> val_vec;
            InitValAST *initval = dynamic_cast<InitValAST *>(ival.get());
            if(initval->type == InitValAST::EMPTY) {
                ret->kind.data.global_alloc.init = zero_value(array_type_kind(KOOPA_RTT_INT32, index_vec));
            } else {
                initval->get_complete_val(val_vec, index_vec, 0); // val_vec full of NumberAST
                ret->kind.data.global_alloc.init = (koopa_raw_value_t)initval->Koop(val_vec, index_vec, 0);
            }
        } else if(type == IDE) {
            ret->kind.data.global_alloc.init = zero_value(array_type_kind(KOOPA_RTT_INT32, index_vec));
        }
        return nullptr;
    }
    return nullptr;
}

void* InitValAST::Koop() const {
    if(type != EXP)
        assert(false);
    return exp->Koop();
}

int InitValAST::Calcu() const {
    if(type != EXP)
        assert(false);
    return exp->Calcu();
}

void* InitValAST::Koop(std::vector<const void*> &val_vec, std::vector<int> index_vec, int depth) const {
    std::vector<const void *> *part_val = new std::vector<const void *>();
    if(depth  == index_vec.size() - 1) {
        for(int i = 0; i < index_vec.back(); i++) {
            part_val->push_back(val_vec.front());
            val_vec.erase(val_vec.begin());
        }
    } else {
        for(int i = 0; i < index_vec[depth]; i++) {
            part_val->push_back(Koop(val_vec, index_vec, depth + 1));
        }
    }

    koopa_raw_value_data *ret = new koopa_raw_value_data_t();
    std::vector<int> part_vec;
    part_vec.clear();
    for(int i = depth; i< index_vec.size(); i++) {
        part_vec.push_back(index_vec[i]);
    }
    ret->ty = array_type_kind(KOOPA_RTT_INT32, part_vec);
    ret->name = nullptr;
    ret->used_by = slice(KOOPA_RSIK_VALUE);
    ret->kind.tag = KOOPA_RVT_AGGREGATE;
    ret->kind.data.aggregate.elems = slice(*part_val, KOOPA_RSIK_VALUE);
    return ret;
}

// [2][3] = {1,2,{3,4}} ---> { {1, 2, 0} , {3, 4, 0} }
void InitValAST::get_complete_val(std::vector<const void*> &val_vec, std::vector<int> index_vec, int depth) {
    int size = index_vec.back();
    int length = val_vec.size();
    int all_size = 1;
    for (int i = 0; i < index_vec.size(); i++) {
        all_size *= index_vec[i];
    }
    
    assert(val_vec.size() <= all_size);

    if(type != EMPTY) {
        if(depth == 0) {
            for(auto ival = (*ilist).begin(); ival != (*ilist).end() &&
                (val_vec.size() - length) <= all_size; ival ++) {
                InitValAST *initval = dynamic_cast<InitValAST *>((*ival).get());
                if(initval->type == InitValAST::EXP) {
                    NumberAST num;
                    num.num = initval->exp->Calcu();
                    // std::cout << num.num << std::endl;
                    val_vec.push_back(num.Koop());
                } else if (initval->type == InitValAST::LIST || initval->type == InitValAST::EMPTY) {
                    initval->get_complete_val(val_vec, index_vec,depth+1);
                }
            }
        } else if(depth == 1) {
            if(length % size == 0) {
                for(auto ival = (*ilist).begin(); ival != (*ilist).end() &&
                    (val_vec.size() - length) <= size; ival ++) {           // no more than size times
                    InitValAST *initval = dynamic_cast<InitValAST *>((*ival).get());
                    if(initval->type == InitValAST::EXP) {
                        NumberAST num;
                        num.num = initval->exp->Calcu();
                        // std::cout << num.num << std::endl;
                        val_vec.push_back(num.Koop());
                    } else if (initval->type == InitValAST::LIST || initval->type == InitValAST::EMPTY) {
                        initval->get_complete_val(val_vec, index_vec,depth + 1);
                    }
                }
            } else {
                auto ival = (*ilist).begin();  // one time
                InitValAST *initval = dynamic_cast<InitValAST *>((*ival).get());
                if(initval->type == InitValAST::EXP) {
                    NumberAST num;
                    num.num = initval->exp->Calcu();
                    // std::cout << num.num << std::endl;
                    val_vec.push_back(num.Koop());
                } else if (initval->type == InitValAST::LIST || initval->type == InitValAST::EMPTY) {
                    initval->get_complete_val(val_vec, index_vec,depth + 1);
                }
            }
        } else {
            auto ival = (*ilist).begin();  // one time
            InitValAST *initval = dynamic_cast<InitValAST *>((*ival).get());
            if(initval->type == InitValAST::EXP) {
                NumberAST num;
                num.num = initval->exp->Calcu();
                // std::cout << num.num << std::endl;
                val_vec.push_back(num.Koop());
            } else if (initval->type == InitValAST::LIST || initval->type == InitValAST::EMPTY) {
                initval->get_complete_val(val_vec, index_vec,depth + 1);
            }
        }
    } else {
        if(length % size == 0) {
            for(int i = 0; i<size; i++) {
                NumberAST zero;
                zero.num = 0;
                // std::cout << zero.num << std::endl;
                val_vec.push_back(zero.Koop());
            }
        } else {
            NumberAST zero;
            zero.num = 0;
            // std::cout << zero.num << std::endl;
            val_vec.push_back(zero.Koop());
        }
    }

    assert(val_vec.size() <= all_size);
    if(depth == 0) {
        while (val_vec.size() != all_size) {
            NumberAST zero;
            zero.num = 0;
            // std::cout << zero.num << std::endl;
            val_vec.push_back(zero.Koop());
        }
    } else if(depth == 1) {
        while (val_vec.size() % size != 0) {
            NumberAST zero;
            zero.num = 0;
            // std::cout << zero.num << std::endl;
            val_vec.push_back(zero.Koop());
        }
    }
    return;
}
