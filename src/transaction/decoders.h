#pragma once

#include <stdint.h>
#include "os.h"
#include "cx.h"

#include "common/buffer.h"

#include "types.h"

bool decoder_array_of_strings(buffer_t *buf, field_t *field, bool should_hash_only);
bool decoder_array_of_u64(buffer_t *buf, field_t *field, bool should_hash_only);
bool decoder_asset(buffer_t *buf, field_t *field, bool should_hash_only);
bool decoder_authority_type(buffer_t *buf, field_t *field, bool should_hash_only);
bool decoder_optional_authority_type(buffer_t *buf, field_t *field, bool should_hash_only);
bool decoder_boolean(buffer_t *buf, field_t *field, bool should_hash_only);
bool decoder_date_time(buffer_t *buf, field_t *field, bool should_hash_only);
bool decoder_operation_name(buffer_t *buf, field_t *field, bool should_hash_only);
bool decoder_public_key(buffer_t *buf, field_t *field, bool should_hash_only);
bool decoder_string(buffer_t *buf, field_t *field, bool should_hash_only);
bool decoder_uint16(buffer_t *buf, field_t *field, bool should_hash_only);
bool decoder_uint32(buffer_t *buf, field_t *field, bool should_hash_only);
bool decoder_uint64(buffer_t *buf, field_t *field, bool should_hash_only);
bool decoder_uint8(buffer_t *buf, field_t *field, bool should_hash_only);
bool decoder_weight(buffer_t *buf, field_t *field, bool should_hash_only);
bool decoder_empty_extensions(buffer_t *buf, field_t *field, bool should_hash_only);
bool decoder_beneficiaries_extensions(buffer_t *buf, field_t *field, bool should_hash_only);