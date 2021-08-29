#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>
#include "../unit-tests/mocks.h"
#include "transaction/parsers.h"
#include "types.h"
#include "globals.h"
#include "common/format.h"

static void test_decoder_asset(void **state) {
    (void) state;

    // clang-format off
    uint8_t data[] = {
        0x39, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // int64_t amount
        0x03,                                           // uint8_t precision
        0x53, 0x54, 0x45, 0x45, 0x4d, 0x00, 0x00        // char[7] symbol
    };
    // clang-format on

    field_t field = {0};
    buffer_t buffer_valid = {.offset = 0, .ptr = data, .size = sizeof(data)};
    buffer_t buffer_invalid = {.offset = 0, .ptr = data, .size = 0};

    // invalid length
    assert_false(decoder_asset(&buffer_invalid, &field, false));

    // string length bigger than buffer length
    buffer_seek_set(&buffer_invalid, 0);
    buffer_invalid.size = sizeof(data) - 2;
    assert_false(decoder_asset(&buffer_invalid, &field, false));

    assert_true(decoder_asset(&buffer_valid, &field, false));
    assert_string_equal(field.value, "1.337 HIVE");
}

static void test_decoder_asset_hashing(void **state) {
    (void) state;

    // clang-format off
    uint8_t data[] = {
        0x39, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // int64_t amount
        0x03,                                           // uint8_t precision
        0x53, 0x54, 0x45, 0x45, 0x4d, 0x00, 0x00        // char[7] symbol
    };
    // clang-format on

    field_t field = {0};
    buffer_t buffer_valid = {.offset = 0, .ptr = data, .size = sizeof(data)};

    will_return(__wrap_cx_hash_no_throw, 0);
    will_return(__wrap_cx_hash_get_size, 0);

    // expect cx_hash to be called with string length first
    expect_value(__wrap_cx_hash_no_throw, hash, &G_context.tx_info.sha);
    expect_value(__wrap_cx_hash_no_throw, mode, 0);
    expect_memory(__wrap_cx_hash_no_throw, in, data, sizeof(asset_t));
    expect_value(__wrap_cx_hash_no_throw, len, sizeof(asset_t));
    expect_value(__wrap_cx_hash_no_throw, out, NULL);
    expect_value(__wrap_cx_hash_no_throw, out_len, 0);

    // expect to success
    assert_true(decoder_asset(&buffer_valid, &field, true));

    // expect it to not modify the output field, just hash data
    assert_string_equal(field.value, "");
}

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_decoder_asset), cmocka_unit_test(test_decoder_asset_hashing)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
