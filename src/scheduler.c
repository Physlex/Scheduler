/** @file `sched.c`
 *  @brief This file implements the `scheduler` subsystem.
 */

#include "scheduler.h"

#include "container/ring.h"
#include "utility/errors.h"
#include "conf/defs.h"

#include <string.h>
#include <assert.h>


struct scheduler {
    ring_t *task_queue;
};


scheduler_t *sched_create() {
    const uint32_t kiba = 1024;

    scheduler_t *sched = malloc(sizeof(scheduler_t));
    *sched = (scheduler_t){
        ring_create(sizeof(uintptr_t), kiba)
    };

    return sched;
}


int8_t sched_task(scheduler_t *ctx, task_t *task) {
    return ring_enqueue(ctx->task_queue, task);
}


void sched_run(scheduler_t *ctx) {
    task_t curr_task;
    ring_t *task_queue = ctx->task_queue;
    do {
        ring_dequeue(ctx->task_queue, (void *)&curr_task);
    } while (ring_is_full(*task_queue));
}
