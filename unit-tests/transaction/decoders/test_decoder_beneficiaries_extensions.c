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

static void test_decoder_beneficiaries_extensions(void **state) {
    (void) state;

    uint8_t extension_empty[] = {0x00};
    // clang-format off
    uint8_t extension_beneficiaries[] = {
        0x01,                                       // extension size
        0x00,                                       // extension type - beneficiaries
        0x01,                                       // one account
        0x05,                                       // string length
        0x70, 0x6f, 0x77, 0x65, 0x72,               // "power"
        0x1A, 0x06                                  // 15.62%
    };
    // clang-format on

    field_t field = {0};
    buffer_t buffer_valid = {.offset = 0, .ptr = extension_beneficiaries, .size = sizeof(extension_beneficiaries)};
    buffer_t buffer_valid_empty = {.offset = 0, .ptr = extension_empty, .size = sizeof(extension_empty)};
    buffer_t buffer_invalid = {.offset = 0, .ptr = extension_beneficiaries, .size = 0};

    // invalid length
    assert_false(decoder_beneficiaries_extensions(&buffer_invalid, &field, false));

    // string length bigger than buffer length
    buffer_seek_set(&buffer_invalid, 0);
    buffer_invalid.size = sizeof(extension_beneficiaries) - 2;
    assert_false(decoder_beneficiaries_extensions(&buffer_invalid, &field, false));

    assert_true(decoder_beneficiaries_extensions(&buffer_valid, &field, false));
    assert_string_equal(field.value, "Beneficiaries: [power: 15.62%]");

    // accept also empty extension
    assert_true(decoder_beneficiaries_extensions(&buffer_valid_empty, &field, false));
    assert_string_equal(field.value, "[ ]");
}

static void test_decoder_beneficiaries_extensions_hashing(void **state) {
    (void) state;

    uint8_t extension_empty[] = {0x00};

    // clang-format off
    uint8_t extension_beneficiaries[] = {
        0x01,                                       // extension size
        0x00,                                       // extension type - beneficiaries
        0x01,                                       // one account
        0x05,                                       // string length
        0x70, 0x6f, 0x77, 0x65, 0x72,               // "power"
        0x1A, 0x06                                  // 15.62%
    };
    // clang-format on
    field_t field = {0};
    buffer_t buffer_valid_beneficiaries = {.offset = 0, .ptr = extension_beneficiaries, .size = sizeof(extension_beneficiaries)};
    buffer_t buffer_valid_empty = {.offset = 0, .ptr = extension_empty, .size = sizeof(extension_empty)};

    // SUPPORT EMPTY EXTENSIONS

    will_return(__wrap_cx_hash_no_throw, 0);
    will_return(__wrap_cx_hash_get_size, 0);

    // now we want to check if we hash proper data
    expect_value(__wrap_cx_hash_no_throw, hash, &G_context.tx_info.sha);
    expect_value(__wrap_cx_hash_no_throw, mode, 0);
    expect_memory(__wrap_cx_hash_no_throw, in, extension_empty, sizeof(extension_empty));
    expect_value(__wrap_cx_hash_no_throw, len, sizeof(extension_empty));
    expect_value(__wrap_cx_hash_no_throw, out, NULL);
    expect_value(__wrap_cx_hash_no_throw, out_len, 0);

    // expect to success on proper beneficiaries extension
    assert_true(decoder_beneficiaries_extensions(&buffer_valid_empty, &field, true));
    // expect it to not modify the output field, just hash data
    assert_string_equal(field.value, "");

    // SUPPORT BENEFICIARIES EXTENSIONS

    will_return(__wrap_cx_hash_no_throw, 0);
    will_return(__wrap_cx_hash_get_size, 0);

    // check if we hash proper data
    expect_value(__wrap_cx_hash_no_throw, hash, &G_context.tx_info.sha);
    expect_value(__wrap_cx_hash_no_throw, mode, 0);
    expect_memory(__wrap_cx_hash_no_throw, in, extension_beneficiaries, sizeof(extension_beneficiaries));
    expect_value(__wrap_cx_hash_no_throw, len, sizeof(extension_beneficiaries));
    expect_value(__wrap_cx_hash_no_throw, out, NULL);
    expect_value(__wrap_cx_hash_no_throw, out_len, 0);

    // expect to success with proper beneficiaries extensions
    assert_true(decoder_beneficiaries_extensions(&buffer_valid_beneficiaries, &field, true));
    // expect it to not modify the output field, just hash data
    assert_string_equal(field.value, "");
}

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_decoder_beneficiaries_extensions),
                                       cmocka_unit_test(test_decoder_beneficiaries_extensions_hashing)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
