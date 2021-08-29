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

static void test_decoder_weight(void **state) {
    (void) state;

    // clang-format off
    uint8_t data[] = {
        0x1A, 0x06,         // 1562
        0xE6, 0xF9          // -1562
    };
    field_t field = {0};
    buffer_t buffer_valid = {.offset = 0, .ptr = data, .size = sizeof(uint16_t)};
    buffer_t buffer_valid_negative = {.offset = 0, .ptr = data + 2, .size = sizeof(uint16_t)};
    buffer_t buffer_invalid = {.offset = 0, .ptr = data, .size = 0};

    // invalid length
    assert_false(decoder_weight(&buffer_invalid, &field, false));

    assert_true(decoder_weight(&buffer_valid, &field, false));
    assert_string_equal(field.value, "15.62%");

    assert_true(decoder_weight(&buffer_valid_negative, &field, false));
    assert_string_equal(field.value, "-15.62%");
}

static void test_decoder_weight_hashing(void **state) {
    (void) state;

    uint8_t data[] = {0x1A, 0x06};
    field_t field = {0};
    buffer_t buffer_valid = {.offset = 0, .ptr = data, .size = sizeof(data)};

    will_return(__wrap_cx_hash_no_throw, 0);
    will_return(__wrap_cx_hash_get_size, 0);

    // expect cx_hash to be called with specific values
    expect_value(__wrap_cx_hash_no_throw, hash, &G_context.tx_info.sha);
    expect_value(__wrap_cx_hash_no_throw, mode, 0);
    expect_memory(__wrap_cx_hash_no_throw, in, data, 2);
    expect_value(__wrap_cx_hash_no_throw, len, 2);
    expect_value(__wrap_cx_hash_no_throw, out, NULL);
    expect_value(__wrap_cx_hash_no_throw, out_len, 0);

    // expect to success
    assert_true(decoder_weight(&buffer_valid, &field, true));

    // expect it to not modify the output field, just hash data
    assert_string_equal(field.value, "");
}

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_decoder_weight), cmocka_unit_test(test_decoder_weight_hashing)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
