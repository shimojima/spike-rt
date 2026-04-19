/**
 * Application initialize task
 */
#include <kernel.h>
#define TPRI_APP_INIT_TASK (TMIN_TPRI + 7)
extern void _app_init_task(intptr_t unused);
