/** @file `tasks.c`
 *  @brief This file implements various task implementations.
 */

#include "tasks.h"

#include <stdlib.h>

#include "conf/defs.h"
#include "utility/sys.h"
#include "utility/errors.h"


typedef int8_t (*poll_callback_ptr_t)(struct task *);


struct task {
    gen_callback_ptr_t task_cb;
    void *task_args;
    poll_callback_ptr_t poll_cb;
    task_state_k state;
};


simple_task_t *simple_task_new(void *args, gen_callback_ptr_t cb) {
    simple_task_t *task = (simple_task_t *)malloc(sizeof(simple_task_t));
    if (!task) { SYS_PANIC(EC_MEMFULL); }

    *task = (simple_task_t){
        .task_cb=cb, .task_args=args, .poll_cb=_simple_task_poll,
        .state=TS_READY
    };

    return task;
}


int8_t simple_task_destroy(simple_task_t **ctx) {
    if (!ctx) {
        return -EC_REQUIRES;
    }

    if (*ctx) {
        free(*ctx);
        *ctx = nullptr;
    }

    return EC_SUCCESS;
}


int32_t simple_task_run(simple_task_t *ctx) {
    if (!ctx) {
        return -EC_REQUIRES;
    }

    if (ctx->state != TS_READY) {
        return -EC_MISUSE;
    }

    return _task_run((struct task *)ctx);
}


int8_t simple_task_poll(simple_task_t *ctx) {
    if (!ctx) {
        return -EC_REQUIRES;
    }

    return _task_poll((struct task *)ctx);
}


uintptr_t simple_task_size() {
    return sizeof(struct task);
}


int8_t _task_poll(struct task *ctx) {
    return ctx->poll_cb(ctx->task_args);
}


int32_t _task_run(struct task *ctx) {
    return ctx->task_cb(ctx->task_args);
}


int8_t _simple_task_poll(simple_task_t *ctx) {
    // Simple tasks are automatically invoked the moment they are ready.
    return TS_READY;
}
