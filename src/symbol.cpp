#include "symbol.h"

void SymbolList::list_add(std::string ident, Symbol data) {
    sym_list[ident] = data;
}


Symbol SymbolList::list_get(std::string ident) {
    return sym_list[ident];
}

void SymbolList::list_init() {
    sym_list.clear();
}