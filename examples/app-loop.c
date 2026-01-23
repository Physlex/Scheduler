/** @file `examples/app-loop.c`
 *  @brief This file implements a simple runtime loop using the gbox core lib.
 */

#include <stdint.h>
#include <stdio.h>

#include <gbox/runtime/scheduler.h>
#include <gbox/runtime/tasks.h>
#include <gbox/runtime/utility/errors.h>


static inline int32_t hello_msg(void *low_level_msg) {
    if (!low_level_msg) {
        return -EC_REQUIRES;
    }

    printf("Hello, %s!\n", (const char *)low_level_msg);

    return EC_SUCCESS;
}


int32_t main(int32_t argc, char** argv) {
    uint32_t maximum_num_tasks = 100;
    sched_init(maximum_num_tasks);

    task_t task_0 = simple_task_create(hello_msg, "Hello");
    task_t task_1 = simple_task_create(hello_msg, "World");

    sched_task(&task_0);
    sched_task(&task_1);

    return -sched_run();
}
