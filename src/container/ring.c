/** @file `ring.c`
 *  @brief This file implements the ring module.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "container/ring.h"
#include "utility/errors.h"
#include "conf/defs.h"


struct ring {
    const uintptr_t size;
    const uintptr_t length;

    uintptr_t read_ptr;
    uintptr_t write_ptr;
    uintptr_t capacity;

    uint8_t *data;
};


ring_t *ring_new(const uintptr_t size, const uintptr_t length) {
    ring_t *ctx = (ring_t *)malloc(sizeof(ring_t));
    if (!ctx) {
        return nullptr;
    }

    ring_t temp = (ring_t){
        .size=size, .length=length, .read_ptr=0, .write_ptr=1,
        .capacity=size * length, .data=nullptr
    };

    memcpy(ctx, &temp, sizeof(ring_t));

    ctx->data = (uint8_t*)malloc(ctx->capacity);
    if (!ctx->data) {
        return nullptr;
    }

    memset(ctx->data, 0, ctx->capacity);

    return ctx;
}


int8_t ring_destroy(ring_t **ctx) {
    if (!ctx) {
        return -EC_REQUIRES;
    }

    free((*ctx)->data);
    (*ctx)->data = nullptr;

    free(*ctx);
    *ctx = nullptr;

    return EC_SUCCESS;
}


int8_t ring_enqueue(ring_t *ctx, const void *datum) {
    if (!ctx || !datum) {
        return -EC_REQUIRES;
    }

    if (ring_is_full(ctx)) {
        return -EC_OVERRUN;
    }

    const uint8_t *end = (uint8_t *)datum + ctx->size; 
    for (const uint8_t *bytes = (uint8_t *)datum; bytes < end; ++bytes) {
        ring_write(ctx, *bytes);
    }

    return EC_SUCCESS;
}


int8_t ring_dequeue(ring_t *ctx, void *datum) {
    if (!ctx || !datum) {
        return -EC_REQUIRES;
    }

    if (ring_is_empty(ctx)) {
        return -EC_UNDERRUN;
    }

    const uint8_t *end = (uint8_t *)datum + ctx->size;
    for (uint8_t *trav = datum; trav < end; ++trav) {
        ring_read(ctx, (uint8_t *)trav);
    }

    return EC_SUCCESS;
}


uintptr_t ring_read_ptr(const ring_t *ctx) {
    return ctx->read_ptr;
}


uintptr_t ring_write_ptr(const ring_t *ctx) {
    return ctx->write_ptr;
}


bool ring_is_full(const ring_t *ctx) {
    return ctx->read_ptr == ((ctx->write_ptr + ctx->size) % ctx->capacity);
}


bool ring_is_empty(const ring_t *ctx) {
    return ctx->read_ptr == ctx->write_ptr;
}


uintptr_t ring_capacity(const ring_t *ctx) {
    return ctx->capacity;
}


void ring_read(ring_t *ctx, uint8_t *read_byte_ptr) {
    *read_byte_ptr = ctx->data[ctx->read_ptr];
    ctx->data[ctx->read_ptr] = 0;
    ctx->read_ptr = (ctx->read_ptr + 1) % ctx->capacity;
}

void ring_write(ring_t *ctx, const uint8_t byte) {
    ctx->data[ctx->write_ptr] = byte;
    ctx->write_ptr = (ctx->write_ptr + 1) % ctx->capacity;
}
