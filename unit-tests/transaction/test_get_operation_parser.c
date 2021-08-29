#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>
#include "transaction/parsers.h"
#include "types.h"

static void test_get_operation_parser(void **state) {
    (void) state;

    uint8_t supported_ops[] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 17, 18, 19, 20, 22,
                               23, 24, 25, 26, 32, 33, 34, 36, 37, 38, 39, 40, 44, 45, 46, 47, 48, 49};
    uint8_t unsupported_ops[] = {14, 15, 16, 21, 50};

    for (uint8_t i = 0; i < sizeof(supported_ops); i++) {
        const parser_t *parser = get_operation_parser(supported_ops[i]);
        assert_in_range(parser->size, 1, 9);
    }

    for (uint8_t i = 0; i < sizeof(unsupported_ops); i++) {
        // expect it to throw
        will_return(__wrap_os_longjmp, 0);
        get_operation_parser(unsupported_ops[i]);
    }
}

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(test_get_operation_parser)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
