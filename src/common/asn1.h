#pragma once

#include "buffer.h"

/**
 * Each DER tag consist of a class, type and a nonnegative tag number
 */
typedef struct {
    uint8_t class;
    uint8_t type;
    uint8_t number;
} tag_t;

/**
 * Decode and validate DER tag from encoded field
 *
 * @param[in] input
 * Pointer to input string buffer.
 * @param[out] tag
 * Pointer to 8-bit unsigned integer read from buffer
 * @return true if success, false otherwise.
 */
bool der_decode_tag(buffer_t *input, uint8_t *tag);

/**
 * Decode and validate DER encoded field length which may use up to 4 bytes
 *
 * @param[in] input
 * Pointer to input string buffer
 * @param[out] length
 * Pointer to 32-bit unsigned integer read from buffer
 * @return true if success, false otherwise.
 */
bool der_decode_length(buffer_t *input, uint32_t *length);