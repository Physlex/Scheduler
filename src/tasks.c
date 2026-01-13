/** @file `tasks.c`
 *  @brief This file implements various task implementations.
 */

#include "gbox/tasks.h"

#include <stdlib.h>

#include "gbox/conf/defs.h"
#include "gbox/utility/sys.h"
#include "gbox/utility/errors.h"


struct task simple_task_create(void *args, gen_callback_ptr_t cb) {
    simple_task_t task = (simple_task_t){
        .task_cb=cb, .task_args=args, .poll_cb=_simple_task_poll,
        .state=TS_READY
    };

    return task;
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
