/** @file `tasks.c`
 *  @brief This file implements unit tests for the tasks op.
 */

#include <gtest/gtest.h>

#include "gbox/tasks.h"

/** @defgroup simple task callback simpleTaskCallbacks
 *  @brief This group defines a set of callbacks to be used during tests.
 *  @{
 */

int32_t hello_world(void *args) {
    printf("Hello, World!\n");
    return 0;
}

/** @} */


TEST(simpleTaskCall, simpleTaskTests) {
    simple_task_t stask = simple_task_create(nullptr, hello_world);

    ASSERT_EQ(TS_READY, simple_task_poll(&stask));
    ASSERT_EQ(0, simple_task_run(&stask));
}
