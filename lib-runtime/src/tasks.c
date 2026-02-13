/** @file `tasks.c`
 *  @brief This file implements various task implementations.
 */

#include "gbox/runtime/tasks.h"

#include <stdlib.h>

#include "gbox/runtime/conf/defs.h"
#include "gbox/runtime/utility/errors.h"
#include "gbox/runtime/utility/sys.h"

static inline int8_t _simple_poll(task_t* ctx) {
    ctx->state = TS_READY;
    return TS_READY;
}

struct task simple_task_create(const gen_callback_ptr_t run, void* args) {
    return task_create((task_vt_t){.run = run, .poll = _simple_poll}, args);
}

struct task task_create(const task_vt_t interface, void* args) {
    task_t task =
        (task_t){.interface = interface, .args = args, .state = TS_WAITING};

    return task;
}

int32_t task_run(task_t* ctx) {
    if (!ctx) {
        return -EC_REQUIRES;
    }

    if (ctx->state != TS_READY) {
        return -EC_MISUSE;
    }

    return ctx->interface.run(ctx->args);
}

int8_t task_poll(task_t* ctx) {
    if (!ctx) {
        return -EC_REQUIRES;
    }

    return ctx->interface.poll(ctx);
}

uintptr_t task_size() { return sizeof(struct task); }

const task_vt_t task_if(task_t* task) { return task->interface; }
