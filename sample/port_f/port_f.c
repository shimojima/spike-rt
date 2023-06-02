// SPDX-License-Identifier: MIT
/*
 * Copyright (c) 2022-2023 Embedded and Real-Time Systems Laboratory,
 *                         Graduate School of Information Science, Nagoya Univ., JAPAN
 */

#include <syssvc/syslog.h>
#include <spike/hub/system.h>
#include <spike/pup/ultrasonicsensor.h>
#include <serial/serial.h>
#include <stdio.h>
#include "port_f.h"

/*
 *  メインタスク
 */
void
main_task(intptr_t exinf)
{
  dly_tsk(1000000);

  pup_device_t *eyes = pup_ultrasonic_sensor_get_device(PBIO_PORT_ID_F);
  if (eyes == NULL) {
    syslog(LOG_ERROR, "Error configuring ultrasonic sensor.");
    hub_system_shutdown();
  }

  while (1) {
    int distance = pup_ultrasonic_sensor_distance(eyes);
    syslog(LOG_NOTICE, "Distance: %d mm.", distance);
    dly_tsk(1000000);
  }
  hub_system_shutdown();
}
