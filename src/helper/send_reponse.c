/*****************************************************************************
 *   Ledger App Boilerplate.
 *   (c) 2020 Ledger SAS.
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

#include <stddef.h>  // size_t
#include <stdint.h>  // uint*_t
#include <string.h>  // memmove

#include "send_response.h"
#include "../constants.h"
#include "../globals.h"
#include "../sw.h"
#include "common/buffer.h"

// uint32_t get_public_key_and_set_result() {
//     uint32_t tx = 0;
//     // publickey length
//     G_io_apdu_buffer[tx++] = 65;

//     // copy publickey
//     os_memmove(G_io_apdu_buffer + tx, tmpCtx.publicKeyContext.publicKey.W, 65);
//     tx += 65;

//     uint32_t addressLength = strlen(tmpCtx.publicKeyContext.address);

//     // wif length
//     G_io_apdu_buffer[tx++] = addressLength;

//     // copy wif
//     os_memmove(G_io_apdu_buffer + tx, tmpCtx.publicKeyContext.address, addressLength);
//     tx += addressLength;
//     // copy chain code
//     if (tmpCtx.publicKeyContext.getChaincode) {
//         os_memmove(G_io_apdu_buffer + tx, tmpCtx.publicKeyContext.chainCode, 32);
//         tx += 32;
//     }
//     return tx;
// }

int helper_send_response_pubkey() {
    uint8_t resp[1 + PUBKEY_LEN + 1 + WIF_LEN + CHAINCODE_LEN] = {0};
    size_t offset = 0;

    resp[offset++] = PUBKEY_LEN;
    // resp[offset++] = 0x04;
    memmove(resp + offset, G_context.pk_info.raw_public_key, PUBKEY_LEN);
    offset += PUBKEY_LEN;

    uint32_t wifLength = strlen(G_context.pk_info.wif);

    resp[offset++] = wifLength;
    memmove(resp + offset, G_context.pk_info.wif, wifLength);

    offset += wifLength;
    memmove(resp + offset, G_context.pk_info.chain_code, CHAINCODE_LEN);
    offset += CHAINCODE_LEN;

    return io_send_response(&(const buffer_t){.ptr = resp, .size = offset, .offset = 0}, SW_OK);
}

int helper_send_response_sig() {
    uint8_t resp[1 + MAX_DER_SIG_LEN + 1] = {0};
    size_t offset = 0;

    resp[offset++] = G_context.tx_info.signature_len;
    memmove(resp + offset, G_context.tx_info.signature, G_context.tx_info.signature_len);
    offset += G_context.tx_info.signature_len;
    resp[offset++] = (uint8_t) G_context.tx_info.v;

    return io_send_response(&(const buffer_t){.ptr = resp, .size = offset, .offset = 0}, SW_OK);
}
