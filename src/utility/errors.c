/** @file `utility/errors.c`
 *  @brief This file implements the functions defined in `utility/errors.h`
 */

#include "utility/errors.h"


static const char *requires_message =
    "API parameters were misused. Header guard triggered.";

static const char *misuse_message =
    "The method invoked was misused in some way.\n"
    "Please refer to the methods documentation and examples.";

static const char *success_message = "No error detected.";
static const char *memfull_message = "Unable to allocate more OS memory.";


const char *code_into_error_msg(error_code_k code) {
    switch (code) {
        case EC_SUCCESS: return success_message;
        case EC_REQUIRES: return requires_message;
        case EC_MEMFULL: return memfull_message;
        case EC_MISUSE: return misuse_message;
        default: error_panic(code);
    }
}
