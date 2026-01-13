/** @file `examples/app-loop.c`
 *  @brief This file implements a simple runtime loop using the gbox core lib.
 */

#include <stdint.h>
#include <stdio.h>

#include <gbox/scheduler.h>
#include <gbox/tasks.h>
#include <gbox/utility/errors.h>


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

    simple_task_t task_0 = simple_task_create("Hello", hello_msg);
    simple_task_t task_1 = simple_task_create("World", hello_msg);

    sched_task(&task_0);
    sched_task(&task_1);

    return -sched_run();
}
