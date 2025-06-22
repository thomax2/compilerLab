#ifndef SYMBOL_H
#define SYMBOL_H
#include <vector>
#include <memory>
#include <cstdlib>
#include <map>
#include "koopa.h"

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


class SymbolList
{
private:
    std::map<std::string, Symbol> sym_list;

public:
    ~SymbolList() = default;

    void list_add(std::string ident, Symbol data);
    Symbol list_get(std::string ident);
    void list_init();
};



#endif