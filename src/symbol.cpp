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

void SymbolList::list_new() {
    sym_list_vec.push_back(std::map<std::string, Symbol>());
}

void SymbolList::list_del() {
    sym_list_vec.pop_back();
}

int SymbolList::cur_depth() {
    return sym_list_vec.size();
}

void BlockList::block_init(std::vector<const void *> *bl) {
    this->block_list = bl;
    insts.clear();
}

bool BlockList::checkBlock() {
    if (block_list->size() > 0 && insts.size() > 0) {
      for (size_t i = 0; i < insts.size(); i++) {
        koopa_raw_value_t inst = (koopa_raw_value_t)insts[i];
        if (inst->kind.tag == KOOPA_RVT_RETURN ||
            inst->kind.tag == KOOPA_RVT_JUMP ||
            inst->kind.tag == KOOPA_RVT_BRANCH) {
          return true;
        }
      }
    }
    return false;
  }


void BlockList::block_add(koopa_raw_basic_block_data_t *block) {
    block_quit();
    block->insts.buffer = nullptr;
    block->insts.len = 0;
    block_list->push_back(block);
}

void BlockList::block_quit() {
    if(block_list->size() > 0) {
        if(insts.size() > 0) {
            koopa_raw_basic_block_data_t *cur_blk = (koopa_raw_basic_block_data_t *)block_list->back();
            cur_blk->insts = slice(insts, KOOPA_RSIK_VALUE);
            insts.clear();
        } else {
            block_list->pop_back();
        }
    }
}

void BlockList::inst2block(const void *inst) {
    if(!checkBlock())
        insts.push_back(inst);
}

void WhileList::while_add(koopa_raw_basic_block_t entry, koopa_raw_basic_block_t end) {
    while_list.push_back(While(entry, end, while_num++));
}

void WhileList::while_del() {
    while_list.pop_back();
}

koopa_raw_basic_block_t WhileList::while_end() {
    if(while_list.size() == 0)
        return nullptr;
    return while_list.back().end;
}

koopa_raw_basic_block_t WhileList::while_entry() {
    if(while_list.size() == 0)
        return nullptr;
    return while_list.back().entry;
}

int WhileList::while_dep() {
    if(while_list.size() == 0)
        return -1;
    return while_list.back().num;
}

