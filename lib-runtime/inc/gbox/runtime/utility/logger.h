#ifndef SCHEDULER_CORE_UTILITY_LOGGER_H_
#define SCHEDULER_CORE_UTILITY_LOGGER_H_

/** @file `utility/logger.h`
 *  @brief This file defines a simple logging utility.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "gbox/runtime/container/ring.h"

/** @enum log_level
 *  @typedef log_level_k
 *  @brief This enum defines the various log levels.
 */
typedef enum log_level {
    /** @memberof log_level
     *  @brief TODO: DOCS
     */
    LL_INFO,

    /** @memberof log_level
     *  @brief TODO: DOCS
     */
    LL_DEBUG,

    /** @memberof log_level
     *  @brief TODO: DOCS
     */
    LL_WARN,

    /** @memberof log_level
     *  @brief TODO: DOCS
     */
    LL_CRITICAL
} log_level_k;

/** @fn int8_t logger_set_level(log_level_k)
 *  @brief Sets the current logging level.
 *
 *         The logging level dictates the minimum loggable level for the logger
 * to print.
 *
 *  @param level The minimum logging level cutoff threshhold.
 *  @return A negative error code or 0.
 */
extern int8_t logger_set_level(log_level_k level);

/** @fn int8_t logger_set_fdout(int32_t)
 *  @brief Sets the loggers file descriptor output (stderr, stdout, etc).
 *  @param fd The file descriptor we log to.
 *  @return A negative error code or 0.
 */
extern int8_t logger_set_fdout(int32_t fd);

/** @fn int8_t logger_vlog(log_level_k, const char *, ...)
 *  @brief Variadic logging utility. Used to print via format as in printf.
 *  @param level The level of logging.
 *  @return A negative error code or 0.
 */
extern int8_t logger_vlog(log_level_k level, const char* format, ...);

//! @brief Logging utility. Log as a debug level.
#define LOG_DEBUG (format, vargs) logger_vlog(LL_DEBUG, format, vargs);

//! @brief Logging Utility. Log as info level.
#define LOG_INFO (format, vargs) logger_vlog(LL_INFO, format, vargs);

//! @brief Logging Utility. Log as warning level.
#define LOG_WARN (format, vargs) logger_vlog(LL_WARN, format, vargs);

//! @brief Logging Utility. Log as critical level.
#define LOG_CRITICAL (format, vargs) logger_vlog(LL_CRITICAL, format, vargs);

#ifdef __cplusplus
}
#endif

#endif  // SCHEDULER_CORE_UTILITy
