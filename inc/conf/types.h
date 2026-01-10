#ifndef SCHED_CORE_APP_TYPES_H_
#define SCHED_CORE_APP_TYPES_H_

/** @file `conf/types.h`
 *  @brief This file defines a set of types.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>


/** @defgroup floating_point_types Floating Point Types 
 *  @brief This group defines the various floating point aliases in gbox.
 *  @{
 */

typedef float float32_t;
typedef double float64_t;

/** @} */

/** @defgroup type_aliases
 *  @brief This group defines various generic type aliases that better define
 *         developer intent.
 *  @{
 */

/** @typedef void *(*gen_callback_ptr_t)(void*)
 *  @brief Generic callback alias.
 *  
 *         This function alias defines a better semantic interpretation of the
 *         concept of a fully generic function definition.
 */
typedef void *(*gen_callback_ptr_t)(void*);

/** @} */


#ifdef __cplusplus
}
#endif

#endif  // SCHED_CORE_APP_TYPES_H_
