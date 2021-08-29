#pragma once

#include <stdbool.h>  // bool

#include "os.h"
#include "ux.h"
#include "glyphs.h"

#include "constants.h"
#include "globals.h"
#include "io.h"
#include "sw.h"
#include "common/bip32.h"
#include "common/format.h"
#include "common/wif.h"
#include "common/macros.h"
#include "ui/action/validate.h"
#include "transaction/transaction_parse.h"

enum e_state {
    STATIC_SCREEN,
    DYNAMIC_SCREEN,
};

/**
 * Display transaction information on the device and ask confirmation before signing
 *
 * @return 0 if success, negative integer otherwise.
 *
 */
int ui_display_transaction(void);

/**
 * Traverse DER encoded operation up to g_tx_field_position and copy it's value and title to specified field.
 *
 * @param[out] field
 *  Output field that will be used to display information (title of the operation field and its value) on the screen
 * @param[in] reverse_order
 *  Traverse the operation in reverse order to handle FLOW_LOOP
 * @param start_from_last_operation
 *  Handle edge case when we want to display the last operation when entering from the first screen, moving left
 * @return true if success, false otherwise
 */
bool parse_field(field_t *field, bool reverse_order, bool start_from_last_operation);

/**
 * Parse operation and determine if there is more data to show, to handle dynamic flow of screens
 *
 * @param[in] is_upper_delimiter
 *  Determine if we're entering upper or lower delimiter in dynamic flow
 */
void display_next_state(bool is_upper_delimiter);

/**
 * Initialize "Signing transaction" display when TX got accepted
 *
 */
void ui_display_signing_message(void);