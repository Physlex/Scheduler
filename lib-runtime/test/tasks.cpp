/** @file `tasks.c`
 *  @brief This file implements unit tests for the tasks op.
 */

#include "gbox/runtime/tasks.h"

#include <gtest/gtest.h>

/** @defgroup simple task callback simpleTaskCallbacks
 *  @brief This group defines a set of callbacks to be used during tests.
 *  @{
 */

int32_t hello_world(void* args) {
    printf("Hello, World!\n");
    return 0;
}

/** @} */

TEST(simpleTaskCall, simpleTaskTests) {
    task_t stask = simple_task_create(hello_world, nullptr);

    ASSERT_EQ(TS_READY, task_poll(&stask));
    ASSERT_EQ(0, task_run(&stask));
}
