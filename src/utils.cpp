#include "utils.h"

koopa_raw_slice_t slice(koopa_raw_slice_item_kind_t kind) {
    koopa_raw_slice_t ret;
    ret.kind = kind;
    ret.buffer = nullptr;
    ret.len = 0;
    return ret;
}

koopa_raw_slice_t slice(std::vector<const void *> &vec, koopa_raw_slice_item_kind_t kind) {
    koopa_raw_slice_t ret;
    ret.buffer = new const void *[vec.size()];
    std::copy(vec.begin(), vec.end(), ret.buffer);
    ret.kind = kind;
    ret.len = vec.size();
    return ret;
}

koopa_raw_slice_t slice(const void *value, koopa_raw_slice_item_kind_t kind) {
    koopa_raw_slice_t ret;
    
    ret.len = 1;
    ret.buffer = new const void *[1];
    ret.buffer[0] = value;
    ret.kind = kind;

    return ret;
}

koopa_raw_type_t type_kind(koopa_raw_type_tag_t tag) {
    koopa_raw_type_kind_t *ret = new koopa_raw_type_kind_t();
    ret->tag = tag;
    return (koopa_raw_type_t)ret;
  }
  
koopa_raw_type_t pointer_type_kind(koopa_raw_type_tag_t tag) {
    koopa_raw_type_kind_t *ret = new koopa_raw_type_kind_t();
    ret->tag = KOOPA_RTT_POINTER;
    ret->data.pointer.base = type_kind(tag); 
    return (koopa_raw_type_t)ret;
}

koopa_raw_value_data *jump_value(koopa_raw_basic_block_t tar) {
    koopa_raw_value_data *jump = new koopa_raw_value_data_t();
    jump->ty = type_kind(KOOPA_RTT_UNIT);
    jump->name = nullptr;
    jump->used_by = slice(KOOPA_RSIK_VALUE);
    jump->kind.tag = KOOPA_RVT_JUMP;
    jump->kind.data.jump.args = slice(KOOPA_RSIK_VALUE);
    jump->kind.data.jump.target = tar;
    return jump;
}