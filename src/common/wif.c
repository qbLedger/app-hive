#include "wif.h"
#include "os.h"
#include "cx.h"
#include "common/base58.h"
#include "errors.h"
#include <stdint.h>
#include <string.h>  //memmove, memset

// TODO understand WIF creation process and refactor
uint32_t wif_from_public_key(uint8_t *publicKey, size_t keyLength, char *out, uint32_t outLength) {
    if (publicKey == NULL || keyLength < 33) {
        THROW(INVALID_PARAMETER);
    }
    if (outLength < 40) {
        THROW(EXCEPTION_OVERFLOW);
    }

    uint8_t temp[33];
    // is even?
    temp[0] = (publicKey[64] & 0x1) ? 0x03 : 0x02;
    memmove(temp + 1, publicKey + 1, 32);
    return compressed_public_key_to_wif(temp, sizeof(temp), out, outLength);
}

// TODO refactor
uint32_t compressed_public_key_to_wif(uint8_t *publicKey,
                                      uint32_t keyLength,
                                      char *out,
                                      size_t outLength) {
    if (keyLength < 33) {
        THROW(INVALID_PARAMETER);
    }
    if (outLength < 40) {
        THROW(EXCEPTION_OVERFLOW);
    }

    uint8_t temp[37];
    memset(temp, 0, sizeof(temp));
    memmove(temp, publicKey, 33);

    uint8_t check[20];
    cx_ripemd160_t riprip;
    cx_ripemd160_init(&riprip);
    cx_hash(&riprip.header, CX_LAST, temp, 33, check, sizeof(check));
    memmove(temp + 33, check, 4);

    memset(out, 0, outLength);
    strcpy(out, "STM");
    int addressLen = base58_encode(temp, sizeof(temp), out + 3, outLength);

    if (addressLen == -1) {
        THROW(EXCEPTION_OVERFLOW);
    }

    PRINTF("WIF: %s \n", out);

    return addressLen + 3;
}