#pragma once

/**
 * Macro for the size of a specific structure field.
 */
#define MEMBER_SIZE(type, member) (sizeof(((type *) 0)->member))

/**
 * Safely calculate array length
 */
#define ARRAYLEN(array) (sizeof(array) / sizeof(array[0]))