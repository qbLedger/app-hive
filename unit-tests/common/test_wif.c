#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>

#include "common/wif.h"

static void test_wif_from_public_key(void **state) {
    (void) state;

    uint8_t raw_public_key[PUBKEY_UNCOMPRESSED_LEN] = {0};
    char wif[PUBKEY_WIF_STR_LEN] = {0};

    // invalid raw public key
    assert_false(wif_from_public_key(raw_public_key, 1, wif, sizeof(wif)));

    // invalid raw public key
    assert_false(wif_from_public_key(NULL, sizeof(raw_public_key), wif, sizeof(wif)));

    // invalid out length
    assert_false(wif_from_public_key(raw_public_key, sizeof(raw_public_key), wif, 1));

    will_return(__wrap_cx_ripemd160_init_no_throw, 0);
    will_return(__wrap_cx_hash_no_throw, 0);
    will_return(__wrap_cx_hash_get_size, 0);

    // expect cx_hash to be called appropriate times
    expect_any(__wrap_cx_hash_no_throw, hash);
    expect_any(__wrap_cx_hash_no_throw, mode);
    expect_any(__wrap_cx_hash_no_throw, in);
    expect_any(__wrap_cx_hash_no_throw, len);
    expect_any(__wrap_cx_hash_no_throw, out);
    expect_any(__wrap_cx_hash_no_throw, out_len);

    assert_true(wif_from_public_key(raw_public_key, sizeof(raw_public_key), wif, sizeof(wif)));
    assert_string_equal(wif, "STM4tVMTu4hrMTGeAQpAEzueCYqEESJQgkaH9DVJNnzK1mzF8wuHq");
}

static void test_wif_from_compressed_public_key(void **state) {
    (void) state;

    uint8_t compressed_pub_key[PUBKEY_COMPRESSED_LEN] = {0};
    char wif[PUBKEY_WIF_STR_LEN] = {0};

    // invalid compressed public key len
    assert_false(wif_from_compressed_public_key(compressed_pub_key, 1, wif, sizeof(wif)));

    // invalid out length
    assert_false(wif_from_compressed_public_key(compressed_pub_key, sizeof(compressed_pub_key), wif, 1));

    will_return(__wrap_cx_ripemd160_init_no_throw, 0);
    will_return(__wrap_cx_hash_no_throw, 0);
    will_return(__wrap_cx_hash_get_size, 0);

    // expect cx_hash to be called appropriate times
    expect_any(__wrap_cx_hash_no_throw, hash);
    expect_any(__wrap_cx_hash_no_throw, mode);
    expect_any(__wrap_cx_hash_no_throw, in);
    expect_any(__wrap_cx_hash_no_throw, len);
    expect_any(__wrap_cx_hash_no_throw, out);
    expect_any(__wrap_cx_hash_no_throw, out_len);

    assert_true(wif_from_compressed_public_key(compressed_pub_key, sizeof(compressed_pub_key), wif, sizeof(wif)));
    assert_string_equal(wif, "STM1111111111111111111111111111111111111");
}

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_wif_from_public_key), cmocka_unit_test(test_wif_from_compressed_public_key)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}