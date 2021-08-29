#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// [x-coordinate (32)][y-coordinate (32)]
#define PUBKEY_UNCOMPRESSED_LEN 64

// [prefix (1)][x-coordinate (32)]
#define PUBKEY_COMPRESSED_LEN 33

// [wif (53)][\0]
#define PUBKEY_WIF_STR_LEN 54

/**
 * Convert raw public key into WIF representation supported by Hive backend
 *
 * @param[in] raw_public_key
 *  Pointer to 8-bit, raw public key buffer
 * @param[in] key_len
 *  Input buffer length
 * @param[out] out
 *  Pointer to output string
 * @param[in] out_len
 *  Length of output string buffer
 * @return true if success, false otherwise
 */
bool wif_from_public_key(uint8_t raw_public_key[static PUBKEY_UNCOMPRESSED_LEN], size_t key_len, char *out, size_t out_len);

/**
 * Convert compressed public key into WIF representation supported by Hive backend
 *
 * @param[in] public_key
 *  Pointer to 8-bit, raw public key buffer
 * @param[in] key_len
 *  Input buffer length
 * @param[out] out
 *  Pointer to output string
 * @param[in] out_len
 *  Length of output string buffer
 * @return true if success, false otherwise
 */
bool wif_from_compressed_public_key(uint8_t public_key[static PUBKEY_COMPRESSED_LEN], size_t key_len, char *out, size_t out_len);