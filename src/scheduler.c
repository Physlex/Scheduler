/** @file `sched.c`
 *  @brief This file implements the `scheduler` subsystem.
 */

#include "scheduler.h"

#include "container/ring.h"
#include "utility/errors.h"
#include "utility/sys.h"
#include "conf/defs.h"

#include <string.h>
#include <assert.h>


// struct scheduler {
//     ring_t *task_queue;
// };


// scheduler_t *sched_create() {
//     const uint32_t kilobyte = 1024;

//     scheduler_t *sched = malloc(sizeof(scheduler_t));
//     *sched = (scheduler_t){ ring_create(sizeof(uintptr_t), 2 * kilobyte) };

//     return sched;
// }


// int8_t sched_task(scheduler_t *ctx, task_t *task) {
//     return ring_enqueue(ctx->task_queue, task);
// }


// void sched_run(scheduler_t *ctx) {
//     ring_t *task_queue = ctx->task_queue;
    
//     do {
//         task_t curr_task;
//         int8_t ec = ring_dequeue(ctx->task_queue, (void *)&curr_task);
//         if (ec < 0) { SYS_PANIC(-ec); }

//         switch task_state(&curr_task) {
//             case TS_DONE: {
//                 break;
//             }
//             case TS_READY: {
//                 break;
//             }
//             case TS_WAITING: {
//                 break;
//             }
//             default: {
//                 SYS_PANIC(EC_MISUSE);
//             }
//         }
//     } while (ring_is_full(task_queue));
// }
