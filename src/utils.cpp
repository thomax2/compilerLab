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

