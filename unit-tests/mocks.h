#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>
#include "cx.h"

int __wrap_os_longjmp(int fd);
void *__wrap_pic(void *link_address);
cx_err_t __wrap_cx_hash_no_throw(cx_hash_t *hash, uint32_t mode, const uint8_t *in, size_t len, uint8_t *out, size_t out_len);
size_t __wrap_cx_hash_get_size(int fd);
cx_err_t __wrap_cx_ripemd160_init_no_throw(cx_ripemd160_t *hash);