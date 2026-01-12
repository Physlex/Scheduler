/** @file `sched.c`
 *  @brief This file implements the `scheduler` subsystem.
 */

#include "scheduler.h"

#include <string.h>

#include "tasks.h"

#include "container/ring.h"
#include "utility/errors.h"
#include "conf/defs.h"


struct scheduler {
    ring_t *task_queue;
};


static bool _scheduler_creation_locked = false;
static scheduler_t _scheduler;


int8_t sched_init(uintptr_t queue_length) {
    if (_scheduler_creation_locked) {
        return -EC_MISUSE;
    }

    _scheduler.task_queue = ring_new(simple_task_size(), queue_length);
    _scheduler_creation_locked = true;

    return EC_SUCCESS;
}


int8_t sched_task(struct task *task) {
    return ring_enqueue(_scheduler.task_queue, task);
}


int8_t sched_run() {
    ring_t *task_queue = _scheduler.task_queue;

    do {
        simple_task_t *curr_task = nullptr;

        int8_t ec = ring_dequeue(task_queue, (void *)curr_task);
        if (ec < 0) { return ec; }

        int8_t task_state = simple_task_poll(curr_task);
        switch (task_state) {
            case TS_DONE: {
                // Don't put it back onto the scheduler queue
                break;
            }

            case TS_READY: {
                simple_task_run(curr_task);
                break;
            }

            case TS_WAITING: {
                ring_enqueue(task_queue, (void *)curr_task);
                break;
            }

            default: {
                SYS_PANIC(EC_MISUSE);
            }
        }
    // TODO: Should a program exit if the task queue is empty?
    } while (!ring_is_empty(task_queue));

    return EC_SUCCESS;
}
