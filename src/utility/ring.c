/** @file `ring.c`
 *  @brief This file implements the ring module.
 */

#include "utility/ring.h"

#include <stdlib.h>


int8_t ring_create(ring_t *ctx, uint32_t length, uint32_t size) {
    if (!ctx) {
        return -REK_REQS;
    }

    ctx->data = (uint8_t*)malloc(size * length);
    if (!ctx->data) {
        return -REK_MEMORY;
    }

    ctx->size = size;
    ctx->length = length * size;

    return REK_SUCCESS;
}


int8_t ring_enqueue(ring_t *ctx, uint8_t *datum) {
    if (!ctx || !datum) {
        return -REK_REQS;
    }

    for (uint32_t bytes_written; bytes_written < ctx->size; ++bytes_written) {
        ctx->data[ctx->write_ptr + bytes_written] = datum[bytes_written];
    }

    (ctx->write_ptr += ctx->size) % ctx->length;

    return REK_SUCCESS;
}


int8_t *ring_dequeue(ring_t *ctx) {
}


int8_t ring_destroy(ring_t *ctx) {
    if (!ctx) {
        return -REK_REQS;
    }

    free(ctx->data);
}
