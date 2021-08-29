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

static void test_decoder_empty_extensions(void **state) {
    (void) state;

    uint8_t data[] = {0x00, 0x01};
    field_t field = {0};
    buffer_t buffer_valid = {.offset = 0, .ptr = data, .size = sizeof(data)};
    buffer_t buffer_invalid = {.offset = 0, .ptr = data, .size = 0};
    buffer_t buffer_invalid_value = {.offset = 0, .ptr = data + 1, .size = 0};

    // invalid input length
    assert_false(decoder_empty_extensions(&buffer_invalid, &field, false));

    // invalid value in buffer
    assert_false(decoder_empty_extensions(&buffer_invalid_value, &field, false));

    // valid
    assert_true(decoder_empty_extensions(&buffer_valid, &field, false));
    assert_string_equal(field.value, "[ ]");
}

static void test_decoder_empty_extensions_hashing(void **state) {
    (void) state;

    uint8_t data[] = {0x00};

    field_t field = {0};
    buffer_t buffer_valid = {.offset = 0, .ptr = data, .size = sizeof(data)};

    will_return(__wrap_cx_hash_no_throw, 0);
    will_return(__wrap_cx_hash_get_size, 0);

    // expect cx_hash to be called with string length first
    expect_value(__wrap_cx_hash_no_throw, hash, &G_context.tx_info.sha);
    expect_value(__wrap_cx_hash_no_throw, mode, 0);
    expect_memory(__wrap_cx_hash_no_throw, in, data, 1);
    expect_value(__wrap_cx_hash_no_throw, len, 1);
    expect_value(__wrap_cx_hash_no_throw, out, NULL);
    expect_value(__wrap_cx_hash_no_throw, out_len, 0);

    // expect to success
    assert_true(decoder_empty_extensions(&buffer_valid, &field, true));

    // expect it to not modify the output field, just hash data
    assert_string_equal(field.value, "");
}

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_decoder_empty_extensions), cmocka_unit_test(test_decoder_empty_extensions_hashing)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
