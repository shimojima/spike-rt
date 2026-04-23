/*
 * libcpp/libcpp.h
 * Copyright (c) 2026 Embedded Technology Software Design Robot Contest
 */

#ifndef _LIBCPP_LIBCPP_H_
#define _LIBCPP_LIBCPP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
bool libcpp_is_ready(void);

#define libcpp_sync(delay) while (!libcpp_is_ready()) dly_tsk(delay)

#ifdef __cplusplus
}
#endif

#endif
