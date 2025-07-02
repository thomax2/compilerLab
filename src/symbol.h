#ifndef SYMBOL_H
#define SYMBOL_H
#include <vector>
#include <memory>
#include <cstdlib>
#include <map>
#include <iostream>
#include <assert.h>
#include "koopa.h"
#include "utils.h"

enum SymType {CONST_SYM, VAR_SYM, FUNC};
struct Symbol {
    SymType type;
    union symbol {
        int const_sym;
        koopa_raw_value_t var_sym;
        koopa_raw_function_t func;
    }data;
    Symbol() = default;
    Symbol(SymType type, int val) : type(type) {data.const_sym = val;}
    Symbol(SymType type, koopa_raw_value_t var) : type(type) {data.var_sym = var;}
    Symbol(SymType type, koopa_raw_function_t func) : type(type) {data.func = func;}
};


class SymbolList {
private:
    std::vector<std::map<std::string, Symbol>> sym_list_vec;

public:
    ~SymbolList() = default;

    void list_add(std::string ident, Symbol data);
    Symbol list_get(std::string ident);
    void list_new();
    void list_del();
    // void list_init();
    int cur_depth();
};


class BlockList {
    private:
        std::vector<const void *> *block_list;
        std::vector<const void *> insts;   // block cant embed, each independent 

    public:
        void block_init(std::vector<const void *> *bl);
        void block_add(koopa_raw_basic_block_data_t *block);
        void block_quit();
        void inst2block(const void *inst);
        bool checkBlock();
};

class WhileList {
    int while_num = 0;
    private:
        struct While
        {
            koopa_raw_basic_block_t entry;
            koopa_raw_basic_block_t end;
            int num;
            While(koopa_raw_basic_block_t entry, koopa_raw_basic_block_t end, int num)
                : entry(entry), end(end), num(num) {}
        };
        std::vector<While> while_list;

    public:
        void while_add(koopa_raw_basic_block_t entry, koopa_raw_basic_block_t end);
        koopa_raw_basic_block_t while_entry();
        koopa_raw_basic_block_t while_end();
        void while_del();
        int while_dep();
};


#endif