/** @file `ring.c`
 *  @brief This file implements the ring module.
 */

#include <stdlib.h>
#include <string.h>

#include "container/ring.h"
#include "utility/errors.h"
#include "conf/defs.h"


int8_t ring_create(ring_t *ctx, uint32_t size, uint32_t length) {
    if (!ctx) {
        return -EC_REQUIRES;
    }

    ctx->size = size;
    ctx->length = length * size;
    ctx->read_ptr = 0;
    ctx->write_ptr = 0;

    ctx->data = (uint8_t*)malloc(ring_capacity(ctx));
    if (!ctx->data) {
        return -EC_MEMFULL;
    }

    memset(ctx->data, 0, ring_capacity(ctx));

    return EC_SUCCESS;
}


int8_t ring_destroy(ring_t *ctx) {
    if (!ctx) {
        return -EC_REQUIRES;
    }

    free(ctx->data);
    ctx->data = nullptr;

    return EC_SUCCESS;
}


int8_t ring_enqueue(ring_t *ctx, const void *datum) {
    if (!ctx || !datum) {
        return -EC_REQUIRES;
    }

    const uint8_t *end = (uint8_t *)datum + ctx->size; 

    for (const uint8_t *bytes = (uint8_t *)datum; bytes < end; ++bytes) {
        ring_write(ctx, *bytes);
    }

    return EC_SUCCESS;
}


int8_t ring_dequeue(ring_t *ctx, void *read_ptr) {
    if (!ctx || !read_ptr) {
        return EC_REQUIRES;
    }

    const uint8_t *end = (uint8_t *)read_ptr + ctx->size;
    for (uint8_t *trav = read_ptr; trav < end; ++trav) {
        ring_read(ctx, (uint8_t *)trav);
    }

    return EC_SUCCESS;
}


int8_t ring_display(ring_t *ctx) {
}
