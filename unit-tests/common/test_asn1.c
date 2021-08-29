#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>

#include "common/asn1.h"

static void test_der_decode_tag(void **state) {
    (void) state;
    uint8_t data[] = {0x01, 0x04};
    uint8_t tag = {0};

    buffer_t empty = {.offset = 0, .ptr = data, .size = 0};
    buffer_t invalid = {.offset = 0, .ptr = data, .size = 1};
    buffer_t valid = {.offset = 0, .ptr = data + 1, .size = 1};

    // empty buffer
    assert_false(der_decode_tag(&empty, &tag));

    // invalid tag
    assert_false(der_decode_tag(&invalid, &tag));

    // valid
    assert_true(der_decode_tag(&valid, &tag));
    assert_int_equal(tag, 0x04);
}

static void test_der_decode_length(void **state) {
    (void) state;
    uint8_t data[] = {0x04, 0x85, 0x82, 0x01, 0x02, 0x02};
    uint32_t length = 0;

    buffer_t empty = {.offset = 0, .ptr = data, .size = 0};
    buffer_t single_byte_length = {.offset = 0, .ptr = data, .size = 1};
    buffer_t multi_byte_length_invalid = {.offset = 0, .ptr = data + 1, .size = 1};
    buffer_t multi_byte_length = {.offset = 0, .ptr = data + 2, .size = 4};

    // empty buffer
    assert_false(der_decode_length(&empty, &length));

    // return false while decoding invalid length tag
    assert_false(der_decode_length(&multi_byte_length_invalid, &length));

    // decode single-byte length
    assert_true(der_decode_length(&single_byte_length, &length));
    assert_int_equal(length, 0x04);

    // decode multi-byte length
    assert_true(der_decode_length(&multi_byte_length, &length));
    assert_int_equal(length, 0x102);

    // decode multi-byte length with missing buffer
    multi_byte_length.offset = 0;
    multi_byte_length.size = 2;
    assert_false(der_decode_length(&multi_byte_length, &length));
}

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_der_decode_tag), cmocka_unit_test(test_der_decode_length)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
