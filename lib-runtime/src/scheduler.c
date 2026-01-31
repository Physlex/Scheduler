/** @file `sched.c`
 *  @brief This file implements the `scheduler` subsystem.
 */

#include "gbox/runtime/scheduler.h"

#include <stdlib.h>
#include <string.h>

#include "gbox/runtime/conf/defs.h"
#include "gbox/runtime/container/ring.h"
#include "gbox/runtime/tasks.h"
#include "gbox/runtime/utility/errors.h"
#include "gbox/runtime/utility/sys.h"

struct scheduler {
    ring_t* task_queue;
};

static bool _scheduler_creation_locked = false;
static scheduler_t _scheduler;

int8_t sched_init(uintptr_t queue_length) {
    if (_scheduler_creation_locked) {
        return -EC_MISUSE;
    }

    _scheduler.task_queue = ring_new(task_size(), queue_length);
    _scheduler_creation_locked = true;

    return EC_SUCCESS;
}

int8_t sched_task(struct task* task) {
    int8_t ret = ring_enqueue(_scheduler.task_queue, task);
    return ret;
}

int8_t sched_run() {
    ring_t* task_queue = _scheduler.task_queue;

    do {
        task_t curr_task;
        int8_t ec = ring_dequeue(task_queue, &curr_task);
        if (ec < 0) {
            return -EC_MISUSE;
        }

        int8_t task_state = task_poll(&curr_task);
        switch (task_state) {
            case TS_DONE: {
                // Don't put it back onto the scheduler queue
                break;
            }

            case TS_READY: {
                task_run(&curr_task);
                break;
            }

            case TS_WAITING: {
                ring_enqueue(task_queue, (void*)&curr_task);
                break;
            }

            default: {
                SYS_PANIC(EC_MISUSE);
            }
        }
        // TODO: Should a program exit if the task queue is empty?
    } while (!ring_is_empty(task_queue));

    _scheduler_creation_locked = false;
    ring_destroy(&_scheduler.task_queue);

    return EC_SUCCESS;
}
