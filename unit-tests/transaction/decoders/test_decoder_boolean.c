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

static void test_decoder_boolean(void **state) {
    (void) state;
    uint8_t data[] = {0x01};  // true
    field_t field = {0};
    buffer_t buffer_valid = {.offset = 0, .ptr = data, .size = sizeof(data)};
    buffer_t buffer_invalid = {.offset = 0, .ptr = data, .size = 0};

    // invalid length
    assert_false(decoder_boolean(&buffer_invalid, &field, false));

    assert_true(decoder_boolean(&buffer_valid, &field, false));
    assert_string_equal(field.value, "true");
}

static void test_decoder_boolean_hashing(void **state) {
    (void) state;

    uint8_t data[] = {0x14};  // set_withdraw_vesting_route
    field_t field = {0};
    buffer_t buffer_valid = {.offset = 0, .ptr = data, .size = sizeof(data)};

    will_return(__wrap_cx_hash_no_throw, 0);
    will_return(__wrap_cx_hash_get_size, 0);

    // expect cx_hash to be called with specific values
    expect_value(__wrap_cx_hash_no_throw, hash, &G_context.tx_info.sha);
    expect_value(__wrap_cx_hash_no_throw, mode, 0);
    expect_memory(__wrap_cx_hash_no_throw, in, data, 1);
    expect_value(__wrap_cx_hash_no_throw, len, 1);
    expect_value(__wrap_cx_hash_no_throw, out, NULL);
    expect_value(__wrap_cx_hash_no_throw, out_len, 0);

    // expect to success
    assert_true(decoder_boolean(&buffer_valid, &field, true));

    // expect it to not modify the output field, just hash data
    assert_string_equal(field.value, "");
}

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_decoder_boolean), cmocka_unit_test(test_decoder_boolean_hashing)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
