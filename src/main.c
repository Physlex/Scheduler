/** @file `main.c`
 *  @brief This file defines the entry point of the application.
 */

#include <stdlib.h>
#include <stdio.h>

#include "scheduler.h"
#include "tasks.h"


int8_t simple_task_func() {
	return 0;
}


int main() {
	// scheduler_t scheduler;
	// task_t simple_task;
	// simple_task_create(&simple_task, "simple_task", simple_task_func);
	return EXIT_SUCCESS;
}
