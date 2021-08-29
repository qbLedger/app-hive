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

static void test_decoder_date_time(void **state) {
    (void) state;

    uint8_t data[] = {0xFF, 0xC3, 0x5D, 0x60};  // 1616757759 LE

    field_t field = {0};
    buffer_t buffer_valid = {.offset = 0, .ptr = data, .size = sizeof(data)};
    buffer_t buffer_invalid = {.offset = 0, .ptr = data, .size = 0};

    // invalid length
    assert_false(decoder_date_time(&buffer_invalid, &field, false));

    assert_true(decoder_date_time(&buffer_valid, &field, false));
    assert_string_equal(field.value, "2021-03-26T11:22:39");
}

static void test_decoder_date_time_hashing(void **state) {
    (void) state;

    uint8_t data[] = {0xFF, 0xC3, 0x5D, 0x60};  // 1616757759 LE

    field_t field = {0};
    buffer_t buffer_valid = {.offset = 0, .ptr = data, .size = sizeof(data)};

    will_return(__wrap_cx_hash_no_throw, 0);
    will_return(__wrap_cx_hash_get_size, 0);

    // expect cx_hash to be called with string length first
    expect_value(__wrap_cx_hash_no_throw, hash, &G_context.tx_info.sha);
    expect_value(__wrap_cx_hash_no_throw, mode, 0);
    expect_memory(__wrap_cx_hash_no_throw, in, data, 4);
    expect_value(__wrap_cx_hash_no_throw, len, 4);
    expect_value(__wrap_cx_hash_no_throw, out, NULL);
    expect_value(__wrap_cx_hash_no_throw, out_len, 0);

    // expect to success
    assert_true(decoder_date_time(&buffer_valid, &field, true));

    // expect it to not modify the output field, just hash data
    assert_string_equal(field.value, "");
}

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_decoder_date_time), cmocka_unit_test(test_decoder_date_time_hashing)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
