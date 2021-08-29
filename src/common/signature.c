/*****************************************************************************
 *   Ledger App Hive.
 *   Based on (c) 2020 Andrew Chaney
 *   (c) 2021 Bartłomiej (@engrave) Górnicki
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *****************************************************************************/

#include "signature.h"
#include "string.h"  // memmove

bool signature_check_canonical(uint8_t *der) {
    // Hive way to check if a signature is canonical
    return !(der[0] & 0x80) && !(der[0] == 0 && !(der[1] & 0x80)) && !(der[32] & 0x80) && !(der[32] == 0 && !(der[33] & 0x80));
}

bool signature_from_der(const uint8_t *der, uint8_t *sig, size_t sig_len) {
    if (sig_len < SIGNATURE_LEN) {
        return false;
    }

    int32_t length = 0, delta = 1, offset = 2;

    // Derive the recovery parameter, by adding 4 and 27 to stay compatible with other protocols
    sig[0] = 27 + 4 + (der[0] & 0x01);

    if (der[offset + 2] == 0) {
        length = der[offset + 1] - 1;
        offset += 3;
    } else {
        length = der[offset + 1];
        offset += 2;
    }
    if ((length < 0) || (length > 32)) {
        return false;
    }
    while ((length + delta) < 32) {
        sig[delta++] = 0;
    }

    memmove(sig + delta, der + offset, length);

    delta = 1;
    offset += length;
    if (der[offset + 2] == 0) {
        length = der[offset + 1] - 1;
        offset += 3;
    } else {
        length = der[offset + 1];
        offset += 2;
    }
    if ((length < 0) || (length > 32)) {
        return false;
    }
    while ((length + delta) < 32) {
        sig[32 + delta++] = 0;
    }

    memmove(sig + 32 + delta, der + offset, length);

    return true;
}