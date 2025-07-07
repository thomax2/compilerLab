#pragma once

#include <vector>
#include <koopa.h>
#include <assert.h>

koopa_raw_slice_t slice(koopa_raw_slice_item_kind_t kind = KOOPA_RSIK_UNKNOWN);

koopa_raw_slice_t slice(std::vector<const void *> &vec,
                            koopa_raw_slice_item_kind_t kind = KOOPA_RSIK_UNKNOWN);

koopa_raw_slice_t slice(const void *value, koopa_raw_slice_item_kind_t kind);

koopa_raw_type_kind_t *type_kind(koopa_raw_type_tag_t tag);
koopa_raw_type_kind_t *pointer_type_kind(koopa_raw_type_tag_t tag);
koopa_raw_type_kind_t *array_type_kind(koopa_raw_type_tag_t tag, std::vector<int> size_vec) ;

koopa_raw_value_data *jump_value(koopa_raw_basic_block_t tar);

koopa_raw_value_data *ret_value(koopa_raw_type_tag_t tag);

koopa_raw_value_data *zero_value(koopa_raw_type_kind *type);
