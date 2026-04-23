/*
 * app_init.c
 * Copyright (c) 2026 Embedded Technology Software Design Robot Contest
 */

#include "app_init.h"
#include <libcpp/libcpp.h>

static bool _app_init_task_completed = false;

void _app_init_task(intptr_t unused) {

  // Call C++ global constructors
  typedef void (* constructor_t)(void);
  extern constructor_t _init_array_start[]; // cf. linker script
  extern constructor_t _init_array_end[];   // in asp3/target/primehub_gcc/stm32f413xx.ld
  constructor_t *fn = _init_array_start;
  while (fn < _init_array_end) {
    (*fn++)();
  }
  _app_init_task_completed = true;
  ext_tsk();
}

bool libcpp_is_ready(void) {
  return _app_init_task_completed;
}

#if 0
// cf. http://wiki.osdev.org/Calling_Global_Constructors
void __attribute__((weak)) _fini() {}
void *__dso_handle __attribute__((weak))=0;
#endif
