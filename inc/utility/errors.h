#ifndef SCHEDULER_CORE_UTILITY_ERRORS_H_
#define SCHEDULER_CORE_UTILITY_ERRORS_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @file `conf/errors.h`
 *  @brief This file defines a set of errors, and implements means to handle them.
 */

#include <stdint.h>


/** @enum error_code
 *  @typedef error_code_k
 *  @brief This enum provides definitions for global errors.
 * 
 *         Errors are returned from function calls as negative integers. Specific error
 *         codes are designed to be primarily useful to the developer, and not
 *         necessarily useful to the application itself.
 * 
 *         The design patterns that use these definitions (ex: header guards) are used
 *         primarily for public invokations. Private implementations are reccomended to
 *         use their own means of error detection and handling.
 * 
 *         An example pattern would be "fail-fast" asserts of panics.
 */
typedef enum error_code {
    /** @memberof enum error_code
     *  @brief No error was detected.
     */
    EC_SUCCESS=0,

    /** @memberof enum error_code
     *  @brief The method has a contract for parameter passing, and the developer broke
     *         the contract.
     * 
     *         The most common occurance of this is the passing of null pointers into a
     *         function call.
     */
    EC_REQUIRES,

    /** @memberof enum error_code
     *  @brief No error was detected.
     */
    EC_MEMFULL,

    /** @memberof enum error_code
     *  @brief No error was detected.
     */
    EC_MISUSE
} error_code_k;


/** @fn int8_t error_panic(error_code_k)
 *  @brief Forces the process into a global panic, halting the application.
 *  @param code The error code provided by the developer.
 */
inline void error_panic(error_code_k code) {
    while(1) { __asm volatile ("nop"); }
}


/** @fn const char *print_error_msg(error_code_k)
 *  @brief Decodes the error number into an error message viable for printing.
 *  @param code The error code that needs decoding.
 *  @return A message corresponding to the error code.
 */
extern const char *code_into_error_msg(error_code_k code);

#ifdef __cplusplus
}
#endif

#endif  // SCHEDULER_CORE_UTILITY_ERRORS_H_
