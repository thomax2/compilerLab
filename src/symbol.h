#ifndef SYMBOL_H
#define SYMBOL_H
#include <vector>
#include <memory>
#include <cstdlib>
#include <map>
#include "koopa.h"
#include "utils.h"

enum SymType {CONST_SYM, VAR_SYM};
struct Symbol {
    SymType type;
    union symbol {
        int const_sym;
        koopa_raw_value_t var_sym;
    }data;
    Symbol() = default;
    Symbol(SymType type, int val) : type(type) {data.const_sym = val;}
    Symbol(SymType type, koopa_raw_value_t var) : type(type) {data.var_sym = var;}
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



#endif