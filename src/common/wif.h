#pragma once

#include <stdint.h>

uint32_t wif_from_public_key(uint8_t *publicKey, uint32_t keyLength, char *out, uint32_t outLength);
uint32_t compressed_public_key_to_wif(uint8_t *publicKey,
                                      uint32_t keyLength,
                                      char *out,
                                      uint32_t outLength);