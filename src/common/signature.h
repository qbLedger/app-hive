#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "constants.h"

/**
 * Check if provided DER signature is canonical
 *
 * @param[in] der
 *  Pointer to signature candidate buffer
 * @return true if signature is canonical, false otherwise
 */
bool signature_check_canonical(uint8_t *der);

/**
 * Convert raw DER signature into compact signature supported by Hive backend
 *
 * @param[in] der
 *  Pointer to a raw DER signature buffer
 * @param[out] sig
 *  Pointer to output signature buffer
 * @param[in] sig_len
 *  Length of
 * @return true if success, false otherwise
 */
bool signature_from_der(const uint8_t *der, uint8_t *sig, size_t sig_len);