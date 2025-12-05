#ifndef SCHEDULER_CORE_UTILITY_RING_H_
#define SCHEDULER_CORE_UTILITY_RING_H_

/** @file `utility/ring.h`
 *  @brief TODO: DOCS
 */

#include "app/types.h"


#ifdef __cplusplus
extern "C" {
#endif

/** @enum ring_error
 *  @typedef ring_error_kind
 *  @brief TODO: DOCS
 */
typedef enum ring_error {
    REK_SUCCESS=0x00U,
    REK_REQS
} ring_error_kind;


/** @struct ring
 *  @typedef ring_t
 *  @brief TODO: DOCS
 */
typedef struct ring {
    void *data;
    uint32_t type_size;
    uint32_t length;
} ring_t;


/** @fn int8_t ring_create(ring_t *, uint32_t, uint32_t)
 *  @brief TODO: DOCS
 */
extern int8_t ring_create(ring_t *ctx, uint32_t size);


/** @fn int8_t ring_enqueue(ring_t *, void *)
 *  @brief TODO: DOCS
 */
extern int8_t ring_enqueue(ring_t *ctx, void *datum);


/** @fn void *ring_dequeue(ring_t *)
 *  @brief TODO: DOCS
 */
extern void *ring_dequeue(ring_t *ctx);


#ifdef __cplusplus
};
#endif


#endif  // SCHEDULER_CORE_UTILITY_RING_H_
