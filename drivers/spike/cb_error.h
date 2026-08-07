// SPDX-License-Identifier: MIT
/* 
 * Copyright (c) 2022 Embedded and Real-Time Systems Laboratory,
 *                    Graduate School of Information Science, Nagoya Univ., JAPAN
 */

#ifndef _SPIKE_CB_ERROR_H_
#define _SPIKE_CB_ERROR_H_

#include <pbio/error.h>

// If you really want to bypass this check, define PUP_ERROR_IS_NOT_FATAL.
#ifdef PUP_ERROR_IS_NOT_FATAL

#define check_pbio_error_r(port, err, retval) \
  do { \
    if ((err) != PBIO_SUCCESS) { \
      return retval; \
    } \
  } while (0)

#else

#include <kernel.h>
#include <spike/hub/display.h>
#include <spike/hub/speaker.h>
#include <spike/hub/system.h>

// Stop if error is detected, unless port=='X', in which case retval is returned as before.
#define check_pbio_error_r(port, err, retval) \
  do { \
    if ((err) == PBIO_SUCCESS) break; \
    if ((port) == 'X') return retval; \
    for (int i = 0; i < 10; i++) { \
      hub_speaker_set_volume(100-10*i); \
      hub_display_char(port); \
      hub_speaker_play_tone(960.0, 500); \
      hub_display_off(); \
      dly_tsk(150*1000); \
      hub_display_char(port); \
      hub_speaker_play_tone(770.0, 500); \
      hub_display_off(); \
      dly_tsk(150*1000); \
    } \
    hub_speaker_set_volume(0); \
    hub_display_off(); \
    hub_system_shutdown(); \
  } while (0)

#endif

#define check_pbio_error(port, err) check_pbio_error_r(port, err, err)

#endif // _SPIKE_CB_ERROR_H_
