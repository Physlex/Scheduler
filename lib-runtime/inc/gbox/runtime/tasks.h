#ifndef SCHEDULER_CORE_TASKS_H_
#define SCHEDULER_CORE_TASKS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gbox/runtime/conf/types.h"
#include "gbox/runtime/scheduler.h"


/** @file `tasks.h`
 *  @brief This file defines the various task definitions used by the runtime. 
 */

/** @enum task_state
 *  @typedef task_state_k
 *  @brief This enumerator defines various return states of the `task_poll`
 *         operation.
 */
typedef enum task_state {
    /** @memberof enum task_state
     *  @brief TS_READY is returned from `poll` whenever a task is ready to
     *         execute.
     * 
     *         A ready task is the only task state in which the task is
     *         interested in being run.
     */
    TS_READY,

    /** @memberof enum task_state
     *  @brief TS_WAITING is returned from `poll` whenever a task is unable to
     *         execute, or is otherwise waiting for further completion.
     * 
     *         Typically a waiting task is not cleaned up, nor is it ran.
     */
    TS_WAITING,

    /** @memberof enum task_state
     *  @brief TS_DONE is return from `poll` whenever a task is no longer
     *         interested in being run.
     * 
     *         It is at this point that a task can be cleaned up and/or deleted.
     */
    TS_DONE
} task_state_k;

/** @typedef poll_callback_ptr_t
 *  @brief TODO: DOCS
 */
typedef int8_t (*poll_callback_ptr_t)(struct task *);

/** @struct task
 *  @brief Simple task definition. Wraps a generic callback pointer and tracks
 *         state.
 *
 *  TODO: DOCS
 */
struct task {
    gen_callback_ptr_t task_cb;
    void *task_args;
    poll_callback_ptr_t poll_cb;
    task_state_k state;
};

/** @typedef simple_task_t
 *  @brief Alias to a generic task. Simplest possible task defintion.
 */
typedef struct task simple_task_t;

/** @fn struct task simple_task_create(void *)
 *  @brief Simple task creation utility. Takes a set of args and 'hooks' into
 *         scheduler.
 *  @param args The arguments provided on startup to the simple task.
 *  @param cb The function pointer implementation used in the callback.
 * 
 *  @return A constructed simple task.
 */
extern struct task simple_task_create(void *args, gen_callback_ptr_t cb);

/** @fn int32_t simple_task_run(struct task *)
 *  @brief Runs the simple task, then marks the task as completed.
 */
extern int32_t simple_task_run(struct task *ctx);

/** @fn task_poll
 *  @brief Polls a task to query if it is ready.
 *  @return positive task_state_k. On error, negative error code.
 */
extern int8_t simple_task_poll(struct task *ctx);

/** @fn uinptr_t simple_task_size()
 *  @brief Extractor for number of bytes in a simple task struct.
 */
extern uintptr_t simple_task_size();


// PRIVATE HELPERS

/** @fn int8_t _task_poll(struct task *)
 *  @brief This method implements a generic polling structure for all tasks.
 */
static int8_t _task_poll(struct task *ctx);

/** @fn int32_t _task_run(struct task *)
 *  @brief Invokes a task.
 *  @return Any numerical value. Any nonzero value is an error code.
 */
static int32_t _task_run(struct task *ctx);

/** @fn int8_t _simple_task_poll(struct task *)
 *  @brief This method implements the polling task
 */
static int8_t _simple_task_poll(struct task *ctx);


#ifdef __cplusplus
};
#endif

#endif  // SCHEDULER_CORE_TASKS_H_
