#ifndef SCHEDULER_CORE_TASKS_H_
#define SCHEDULER_CORE_TASKS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

typedef enum task_state {
    TS_READY,
    TS_WAITING,
    TS_DONE
} task_state_kind;

typedef enum task_error {
    TEK_SUCCESS=0x00U,
} task_error_kind;

typedef struct task_stat {
    const char *name;
    int8_t (*poll)();
    int8_t (*run)();
    task_state_kind state;
} task_stat_t;

typedef struct task {
    task_stat_t stat;
} task_t;

typedef struct periodic_task {
    task_stat_t stat;
    uint32_t period_ms;
} periodic_task_t;


// /** @fn task_create(task_t *, task_generic_handle_t)
//  *  @brief TODO: DOCS
//  */
// extern task_error_kind task_create(
//     task_t *self, const char *name, int8_t (*poll)(void), int8_t (*run)(void)
// );

// extern task_error_kind simple_task_create(
//     task_t *ctx, const char *name, int8_t (*run)(void)
// );

// extern task_error_kind periodic_task_create(
//     task_t *ctx, const char *name, int8_t (*run)(void), uint32_t period_ms
// );


#ifdef __cplusplus
};
#endif

#endif  // SCHEDULER_CORE_TASKS_H_
