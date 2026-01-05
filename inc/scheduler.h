#ifndef SCHEDULER_CORE_SCHED_H_
#define SCHEDULER_CORE_SCHED_H_

/** @file `scheduler.h`
 *  @brief TODO: DOCS
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "tasks.h"
#include "conf/types.h"

struct ring;

/** @enum sched_error
 *  @typedef sched_error_kind
 *  @brief Error definitions for the scheduler struct.
 */
typedef enum sched_error {
    /** @memberof sched_error_kind
     *  @brief Defines a success state. Default return path on happy execution.
     */
    SEK_SUCCESS=0x00U,

    /** @memberof sched_error_kind
     *  @brief TODO: DOCS
     */
    SEK_REQS,

    /** @memberof sched_error_kind
     *  @brief Some other error occured. From a different error kind.
     */
    SEK_OTHER
} sched_error_kind;


/** @struct scheduler
 *  @typedef scheduler_t
 *  @brief TODO: DOCS
 */
typedef struct scheduler {
    struct ring *task_queue;
} scheduler_t;


/** @fn int8_t sched_create(scheduler_t *)
 *  @brief Populates a scheduler struct.
 *  TODO: DOCS
 */
extern sched_error_kind sched_create(scheduler_t *self);


/** @fn scheduler_t::ErrorKind sched_enqueu(scheduler_t *, task_t *)
 *  @brief Enqueue a task to the scheduler for execution.
 *  TODO: DOCS
 */
extern sched_error_kind sched_enqueue(scheduler_t *self, task_t *task);


/** @fn void sched_run(scheduler_t *self)
 *  @brief TODO: DOCS
 */
[[ noreturn ]]
extern void sched_run(scheduler_t *self);


#ifdef __cplusplus
};
#endif

#endif  // SCHEDULER_CORE_SCHED_H_
