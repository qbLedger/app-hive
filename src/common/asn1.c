/*****************************************************************************
 *   Ledger App Hive
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

#include <stdint.h>
#include <stdbool.h>

#include "asn1.h"
#include "buffer.h"

/**
 * To read more about DER encoding, visit
 *  - http://luca.ntop.org/Teaching/Appunti/asn1.html
 *  - http://directory.apache.org/api/internal-design-guide/4.1-asn1-tlv.html
 */

/**
 * Each DER encoded field should consist following parts:
 * [ der tag (1) ] [ length (1-4) ] [ value (var) ]
 */

#define DER_OCTET_STRING_TAG_NUMBER 0x04

/**
 * Each DER tag consist of a class, type and a nonnegative tag number
 */
static void der_parse_tag(const uint8_t byte, tag_t *tag) {
    tag->class = byte & 0xc0;
    tag->type = byte & 0x20;
    tag->number = byte & 0x1f;
}

bool der_decode_tag(buffer_t *input, uint8_t *tag) {
    tag_t parsed_tag = {0};

    if (!buffer_read_u8(input, tag)) {
        return false;
    }

    der_parse_tag(*tag, &parsed_tag);

    /** We only accept Octet Strings */
    if (parsed_tag.number != DER_OCTET_STRING_TAG_NUMBER) {
        return false;
    }
    return true;
}

bool der_decode_length(buffer_t *input, uint32_t *length) {
    uint8_t byte;

    if (!buffer_read_u8(input, &byte)) {
        return false;
    }

    *length = 0;

    /**
     * If the first byte is > 0x7F, that means it’s a multi-bytes length, and we have to process
     * the following bytes to get the real length. In this case, the first byte contains the
     * number of expected following bytes.
     */
    if (byte > 0x7f) {
        uint8_t count = byte & 0x7f;

        if (count > 4) {
            /** Properly encoded DER string cannot use more than 4 bytes to encode it's length */
            return false;
        }

        /** Decode multi-byte length */
        for (uint8_t i = 0; i < count; ++i) {
            if (!buffer_read_u8(input, &byte)) {
                return false;
            }
            *length = (*length << 8) | byte;
        }

    } else {
        *length = byte;
    }

    return true;
}