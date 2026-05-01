/*
 * app_init.h: Application initialize task
 * Copyright (c) 2026 Embedded Technology Software Design Robot Contest
 */
#include <kernel.h>
#define TPRI_APP_INIT_TASK (4)	// = PYBRICKS_PRIORITY, but app_init_task runs after pybricks
#define TPRI_APP_SYNC_TASK (5)	// > PYBRICKS_PRIORITY, but <(=) user tasks
extern void _app_init_task(intptr_t unused);
extern void _app_sync_task(intptr_t unused);
