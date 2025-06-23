#include "symbol.h"

void SymbolList::list_add(std::string ident, Symbol data) {
    sym_list_vec.back()[ident] = data;
}


Symbol SymbolList::list_get(std::string ident) {
    for (auto it = sym_list_vec.rbegin(); it != sym_list_vec.rend(); ++it) {
        if(it->find(ident) != it->end()) {
            return it->at(ident);
        }
    }
    return Symbol();
}

// void SymbolList::list_init() {
//     sym_list.clear();
// }

void SymbolList::list_new() {
    sym_list_vec.push_back(std::map<std::string, Symbol>());
}

void SymbolList::list_del() {
    sym_list_vec.pop_back();
}

int SymbolList::cur_depth() {
    return sym_list_vec.size();
}

