#ifndef SCHEDULER_CORE_UTILITY_RING_H_
#define SCHEDULER_CORE_UTILITY_RING_H_

/** @file `utility/ring.h`
 *  @brief TODO: DOCS
 */

#ifdef __cplusplus
extern "C" {
#endif


#include "conf/types.h"

/** @struct ring
 *  @typedef ring_t
 *  @brief TODO: DOCS
 */
typedef struct ring {
    uint8_t *data;
    uint32_t size;
    uint32_t length;
    uint32_t write_ptr;
    uint32_t read_ptr;
} ring_t;

/** @fn int8_t ring_create(ring_t *, uint32_t, uint32_t)
 *  @brief This function allocates resources for the ring buffer.
 * 
 *  TODO: DOCS
 */
extern int8_t ring_create(ring_t *ctx, uint32_t length, uint32_t size);

/** @fn void ring_destroy(ring_t *)
 *  @brief This function releases the ring buffer resources.
 */
extern int8_t ring_destroy(ring_t *ctx);

/** @fn int8_t ring_enqueue(ring_t *, void *)
 *  @brief TODO: DOCS
 */
extern int8_t ring_enqueue(ring_t *ctx, uint8_t *datum);

/** @fn void *ring_dequeue(ring_t *)
 *  @brief TODO: DOCS
 */
extern int8_t *ring_dequeue(ring_t *ctx);


#ifdef __cplusplus
};
#endif


#endif  // SCHEDULER_CORE_UTILITY_RING_H_
