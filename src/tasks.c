/** @file `tasks.c`
 *  @brief This file implements various task implementations.
 */

#include "tasks.h"

#include "utility/sys.h"
#include "utility/errors.h"


struct task {
    gen_callback_ptr_t task_cb;
    void *task_args;
    task_state_k (*poll_cb)(struct task *ctx);
    task_state_k state;
};


simple_task_t *simple_task_new(void *args, gen_callback_ptr_t cb) {
    simple_task_t *task = (simple_task_t *)malloc(sizeof(simple_task_t));
    if (!task) { SYS_PANIC(EC_MEMFULL); }

    *task = (simple_task_t){
        .task_cb=cb, .task_args=args, .poll_cb=_simple_task_poll,
        .state=TS_READY
    };
}


int8_t simple_task_destroy(simple_task_t *ctx) {
    if (!ctx) {
        return -EC_REQUIRES;
    }

    free(ctx);
    return EC_SUCCESS;
}


int8_t simple_task_run(simple_task_t *ctx) {
    if (!ctx) {
        return -EC_REQUIRES;
    }

    if (ctx->state == TS_READY) {
        return _task_run((struct task *)ctx);
    } else {
        return EC_SUCCESS;
    }
}


int8_t simple_task_poll(simple_task_t *ctx) {
    if (!ctx) {
        return -EC_REQUIRES;
    }

    return _task_poll((struct task *)ctx);
}


int8_t _task_poll(struct task *ctx) {
    return ctx->poll_cb;
}


int8_t _task_run(struct task *ctx) {
    return ctx->task_cb(ctx->task_args);
}


int8_t _simple_task_poll(simple_task_t *ctx) {
    // Simple tasks are automatically invoked the moment they are ready.
    return TS_READY;
}
