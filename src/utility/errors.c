/** @file `utility/errors.c`
 *  @brief This file implements the functions defined in `utility/errors.h`
 */

#include "gbox/utility/errors.h"
#include "gbox/conf/defs.h"


static const char *requires_message =
    "API parameters were misused. Header guard triggered.";

static const char *misuse_message =
    "The method invoked was misused in some way.\n"
    "Please refer to the methods documentation and examples.";

static const char *success_message = "No error detected.";
static const char *memfull_message = "Unable to allocate more OS memory.";


const char *code_into_error_msg(error_code_k code) {
    const char *message = nullptr;
    switch (code) {
        case EC_SUCCESS: message = success_message;
        case EC_REQUIRES: message = requires_message;
        case EC_MEMFULL: message = memfull_message;
        case EC_MISUSE: message = misuse_message;
        default: error_panic(code);
    }

    return message;
}
