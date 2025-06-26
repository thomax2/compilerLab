#pragma once

#include <vector>
#include <koopa.h>

koopa_raw_slice_t slice(koopa_raw_slice_item_kind_t kind = KOOPA_RSIK_UNKNOWN);

koopa_raw_slice_t slice(std::vector<const void *> &vec,
                            koopa_raw_slice_item_kind_t kind = KOOPA_RSIK_UNKNOWN);

koopa_raw_slice_t slice(const void *value, koopa_raw_slice_item_kind_t kind);

koopa_raw_type_t type_kind(koopa_raw_type_tag_t tag);

koopa_raw_type_t pointer_type_kind(koopa_raw_type_tag_t tag);
koopa_raw_value_data *jump_value(koopa_raw_basic_block_t tar);