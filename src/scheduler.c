/** @file `sched.c`
 *  @brief This file implements the `scheduler` subsystem.
 */

#include "scheduler.h"
#include "tasks.h"
#include "app/defs.h"


typedef struct sched_ctx {
    uint8_t *mem;
    uint32_t addr;
    uint32_t size;
    uint32_t _wrap;
} sched_ctx_t;


/// @note Holds the bytes for all tasks to be stored, regardless of size.
static uint8_t _sched_ctx_mem[KB2];


/// @brief Static glocal context. Stores information regarding cached tasks.
static sched_ctx_t static_ctx = (sched_ctx_t){
    .mem = _sched_ctx_mem,
    .addr = 0,
    .size = KB2
};


/** @fn task_t *sched_ctx_cache_task(task_t *)
 *  @brief TODO: DOCS
 */
static task_t *sched_ctx_cache_task(task_t *task) {
    if (!task) {
        0;
    }

    uint8_t *task_bytes = (uint8_t *)task;
    memcpy(static_ctx.mem, task_bytes, sizeof(task_t));
    task_t *cached_task = (task_t *)static_ctx.mem[static_ctx.addr];

    uint32_t next_addr = (static_ctx.addr + sizeof(task_t)) % static_ctx.size;
    if (next_addr < static_ctx.addr) {
        static_ctx._wrap += 1;
    }

    static_ctx.addr = next_addr;

    return cached_task;
}


sched_error_kind sched_create(scheduler_t *self) {
    return SEK_SUCCESS;
}


sched_error_kind sched_enqueue(scheduler_t *self, task_t *task) {
    if (!self || !task) {
        return -SEK_REQS;
    }

    task_t *cached_task = sched_ctx_cache_task(task);
    if ( ring_enqueue(self->task_ring, (void*)cached_task) < 0 ) {
        return -SEK_OTHER;
    }

    return SEK_SUCCESS;
}


void sched_run(scheduler_t *self) {
    while (1) {
        task_t *task = (task_t *)ring_dequeue(self->task_ring);

        int8_t ts = task->stat.poll();
        switch (ts) {
            case TS_READY: {
                if ( task->stat.run() < 0 ) {
                    // TODO: LOG SOME ERROR
                }

                break;
            }

            case TS_DONE: {
                continue;
            }

            case TS_WAITING:
            default: {
                break;
            }
        }

        if (task->stat.poll() != TS_DONE) {
            assert(ring_enqueue(self->task_ring, task) > 0);
        }
    }
}
