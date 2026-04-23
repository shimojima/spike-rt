/*
 * app_init.h: Application initialize task
 * Copyright (c) 2026 Embedded Technology Software Design Robot Contest
 */
#include <kernel.h>
#define TPRI_APP_INIT_TASK (TMIN_TPRI + 7)
extern void _app_init_task(intptr_t unused);
