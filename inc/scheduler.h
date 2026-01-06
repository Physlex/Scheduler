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


//! @brief Forward declaration of ring type.
struct ring;

/** @struct scheduler
 *  @brief TODO: DOCS
 */
struct scheduler;

/** @typedef scheduler_t
 *  @brief TODO: DOCS
 */
typedef struct scheduler scheduler_t;

/** @fn int8_t sched_create(scheduler_t *)
 *  @brief Populates a scheduler struct.
 *  TODO: DOCS
 */
extern struct scheduler *sched_create();

/** @fn scheduler_t::ErrorKind sched_enqueu(scheduler_t *, task_t *)
 *  @brief Enqueue a task to the scheduler for execution.
 *  TODO: DOCS
 */
extern int8_t sched_task(struct scheduler *ctx, task_t *task);

/** @fn void sched_run(scheduler_t *self)
 *  @brief TODO: DOCS
 */
extern void sched_run(scheduler_t *ctx);


#ifdef __cplusplus
};
#endif

#endif  // SCHEDULER_CORE_SCHED_H_
