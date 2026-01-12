/** @file `utility/logger.c`
 *  @brief Logging utility implementation.
 */

#include "gbox/utility/logger.h"
#include "gbox/container/ring.h"
#include "gbox/utility/errors.h"


/** @struct logger
 *  @typedef logger_t
 *  @brief This struct defines a logger struct. TODO: DOCS
 */
typedef struct logger {
    log_level_k level;
    int32_t fd;
    ring_t *data;
} logger_t;


//! @brief Memory allocation for logging.
static logger_t log;


int8_t logger_set_level(log_level_k level) {
    if (level < LL_DEBUG || level > LL_CRITICAL) {
        return -EC_MISUSE;
    }

    log.level = level;

    return EC_SUCCESS;
}


int8_t logger_set_fdout(int32_t fd) {
    // TODO: IMPLEMENTATION
    return EC_SUCCESS;
}


int8_t logger_vlog(log_level_k level, const char *format, ...) {
    if (log.level < level) {
    }

    // TODO: IMPLEMENTATION
    return EC_SUCCESS;
}
