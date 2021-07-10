#include "signature.h"
#include "string.h"  // memmove

/**
 * Hive way to check if a signature is canonical :/
 */
uint8_t check_canonical(uint8_t *rs) {
    return !(rs[0] & 0x80) && !(rs[0] == 0 && !(rs[1] & 0x80)) && !(rs[32] & 0x80) &&
           !(rs[32] == 0 && !(rs[33] & 0x80));
}

void ecdsa_der_to_signature(const uint8_t *der, uint8_t *sig) {
    int length;
    int offset = 2;
    int delta = 0;
    if (der[offset + 2] == 0) {
        length = der[offset + 1] - 1;
        offset += 3;
    } else {
        length = der[offset + 1];
        offset += 2;
    }
    if ((length < 0) || (length > 32)) {
        return;
    }
    while ((length + delta) < 32) {
        sig[delta++] = 0;
    }
    memmove(sig + delta, der + offset, length);

    delta = 0;
    offset += length;
    if (der[offset + 2] == 0) {
        length = der[offset + 1] - 1;
        offset += 3;
    } else {
        length = der[offset + 1];
        offset += 2;
    }
    if ((length < 0) || (length > 32)) {
        return;
    }
    while ((length + delta) < 32) {
        sig[32 + delta++] = 0;
    }
    memmove(sig + 32 + delta, der + offset, length);

    return;
}