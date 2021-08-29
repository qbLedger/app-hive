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

static void test_decoder_authority_type(void **state) {
    (void) state;

    // clang-format off
    uint8_t data[] = {
        0x01, 0x00, 0x00, 0x00,                     // uint32_t weight_threshold
        0x01,                                       // uint8_t account_auths length
        0x07,                                       // uint8_t string length
        0x65, 0x6e, 0x67, 0x72, 0x61, 0x76, 0x65,   // string "engrave"
        0x01, 0x00,                                 // uint16_t account weight
        0x01,                                       // uint8_t key_auths length                                                    
        0x02, 0x7e, 0x40,                           // uint8_t[33] compressed public key
        0x35, 0x7c, 0xba, 
        0x6d, 0x9f, 0x35, 
        0x43, 0x92, 0x69, 
        0x4a, 0xb4, 0xaf, 
        0x20, 0x21, 0x8f, 
        0x5a, 0x10, 0x8f, 
        0xc8, 0xdc, 0xec, 
        0x28, 0xc1, 0xe1, 
        0x66, 0x70, 0x8c, 
        0x82, 0x40, 0x67,
        0x01, 0x00                                  // uint16_t key weight
    };
    // clang-format on

    uint8_t optional_data[] = {0x00};

    field_t field = {0};
    buffer_t buffer_valid = {.offset = 0, .ptr = data, .size = sizeof(data)};
    buffer_t buffer_invalid = {.offset = 0, .ptr = data, .size = 0};
    buffer_t buffer_field_optional = {.offset = 0, .ptr = optional_data, .size = sizeof(optional_data)};

    will_return(__wrap_cx_ripemd160_init_no_throw, 0);
    will_return(__wrap_cx_hash_no_throw, 0);
    will_return(__wrap_cx_hash_get_size, 0);

    // we dont really care because wif module is already tested
    expect_any(__wrap_cx_hash_no_throw, hash);
    expect_any(__wrap_cx_hash_no_throw, mode);
    expect_any(__wrap_cx_hash_no_throw, in);
    expect_any(__wrap_cx_hash_no_throw, len);
    expect_any(__wrap_cx_hash_no_throw, out);
    expect_any(__wrap_cx_hash_no_throw, out_len);

    // invalid length
    assert_false(decoder_authority_type(&buffer_invalid, &field, false));

    // invalid length (cannot read key auths)
    buffer_valid.size = 15;
    assert_true(buffer_seek_set(&buffer_valid, 0));
    assert_false(decoder_authority_type(&buffer_valid, &field, false));

    buffer_valid.size = sizeof(data);
    assert_true(buffer_seek_set(&buffer_valid, 0));
    assert_true(decoder_authority_type(&buffer_valid, &field, false));
    assert_string_equal(field.value, "Weight: 1, [ [ engrave, 1 ] ], [ [ STM5r6G7EsPUUPYojYhd9nt8dEZ4fwBNUbz2nQyxXHf56ccp8v9j5, 1 ] ]");

    assert_true(decoder_authority_type(&buffer_field_optional, &field, false));
    assert_string_equal(field.value, "no changes");
}

static void test_decoder_authority_type_hashing(void **state) {
    (void) state;

    // clang-format off
    uint8_t data[] = {
        0x01, 0x00, 0x00, 0x00,                     // uint32_t weight_threshold
        0x01,                                       // uint8_t account_auths length
        0x07,                                       // uint8_t string length
        0x65, 0x6e, 0x67, 0x72, 0x61, 0x76, 0x65,   // string "engrave"
        0x01, 0x00,                                 // uint16_t account weight
        0x01,                                       // uint8_t key_auths length                                                    
        0x02, 0x7e, 0x40,                           // uint8_t[33] compressed public key
        0x35, 0x7c, 0xba, 
        0x6d, 0x9f, 0x35, 
        0x43, 0x92, 0x69, 
        0x4a, 0xb4, 0xaf, 
        0x20, 0x21, 0x8f, 
        0x5a, 0x10, 0x8f, 
        0xc8, 0xdc, 0xec, 
        0x28, 0xc1, 0xe1, 
        0x66, 0x70, 0x8c, 
        0x82, 0x40, 0x67,
        0x01, 0x00                                  // uint16_t key weight
    };
    // clang-format on

    field_t field = {0};
    buffer_t buffer_valid = {.offset = 0, .ptr = data, .size = sizeof(data)};

    will_return(__wrap_cx_ripemd160_init_no_throw, 0);
    will_return(__wrap_cx_hash_no_throw, 0);
    will_return(__wrap_cx_hash_get_size, 0);

    will_return(__wrap_cx_hash_no_throw, 0);
    will_return(__wrap_cx_hash_get_size, 0);

    // we dont really care because first call is to convert pub key into wif and this module is already tested
    expect_any(__wrap_cx_hash_no_throw, hash);
    expect_any(__wrap_cx_hash_no_throw, mode);
    expect_any(__wrap_cx_hash_no_throw, in);
    expect_any(__wrap_cx_hash_no_throw, len);
    expect_any(__wrap_cx_hash_no_throw, out);
    expect_any(__wrap_cx_hash_no_throw, out_len);

    // now we want to check if we hash proper data
    expect_value(__wrap_cx_hash_no_throw, hash, &G_context.tx_info.sha);
    expect_value(__wrap_cx_hash_no_throw, mode, 0);
    expect_memory(__wrap_cx_hash_no_throw, in, data, sizeof(data));
    expect_value(__wrap_cx_hash_no_throw, len, sizeof(data));
    expect_value(__wrap_cx_hash_no_throw, out, NULL);
    expect_value(__wrap_cx_hash_no_throw, out_len, 0);

    // expect to success
    assert_true(decoder_authority_type(&buffer_valid, &field, true));

    // expect it to not modify the output field, just hash data
    assert_string_equal(field.value, "");
}

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_decoder_authority_type), cmocka_unit_test(test_decoder_authority_type_hashing)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
