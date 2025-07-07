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

koopa_raw_type_kind_t *type_kind(koopa_raw_type_tag_t tag) {
    koopa_raw_type_kind_t *ret = new koopa_raw_type_kind_t();
    ret->tag = tag;
    return ret;
  }
  
  koopa_raw_type_kind_t *pointer_type_kind(koopa_raw_type_tag_t tag) {
    koopa_raw_type_kind_t *ret = new koopa_raw_type_kind_t();
    ret->tag = KOOPA_RTT_POINTER;
    ret->data.pointer.base = type_kind(tag); 
    return ret;
}

koopa_raw_type_kind_t *array_type_kind(koopa_raw_type_tag_t tag, std::vector<int> size_vec) {
    std::vector<koopa_raw_type_kind *> array_vec;
    for(int i = 0; i < size_vec.size(); i++) {
        koopa_raw_type_kind_t *ret = new koopa_raw_type_kind_t();
        ret->tag = KOOPA_RTT_ARRAY;
        ret->data.array.len = size_vec[i];
        assert(ret->data.array.len != 0);
        array_vec.push_back(ret);
    }
    for(int i = 0; i < size_vec.size(); i++) {
        array_vec[i]->data.array.base = (i == (size_vec.size() - 1)) ? type_kind(tag) : array_vec[i+1];
    }
    return array_vec[0];
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

koopa_raw_value_data *ret_value(koopa_raw_type_tag_t tag) {
    koopa_raw_value_data *ret = new koopa_raw_value_data_t();
    ret->ty = type_kind(KOOPA_RTT_UNIT);
    ret->name = nullptr;
    ret->used_by = slice(KOOPA_RSIK_VALUE);
    ret->kind.tag = KOOPA_RVT_RETURN;
    if (tag == KOOPA_RTT_UNIT) {
      ret->kind.data.ret.value = nullptr;
    } else {
      koopa_raw_value_data *zero = new koopa_raw_value_data_t();
      zero->ty = type_kind(KOOPA_RTT_INT32);
      zero->name = nullptr;
      zero->used_by = slice(KOOPA_RSIK_VALUE);
      zero->kind.tag = KOOPA_RVT_INTEGER;
      zero->kind.data.integer.value = 0;
      ret->kind.data.ret.value = zero;
    }
    return ret;
}

koopa_raw_value_data *zero_value(koopa_raw_type_kind *type) {
    koopa_raw_value_data *ret = new koopa_raw_value_data_t();
    ret->ty = type;
    ret->name = nullptr;
    ret->used_by = slice(KOOPA_RSIK_VALUE);
    ret->kind.tag = KOOPA_RVT_ZERO_INIT;
    return ret;
}
