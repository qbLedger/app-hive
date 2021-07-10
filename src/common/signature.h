#pragma once

#include <stdint.h>

uint8_t check_canonical(uint8_t *rs);
void ecdsa_der_to_signature(const uint8_t *der, uint8_t *sig);