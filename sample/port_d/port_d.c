// SPDX-License-Identifier: MIT
/*
 * Copyright (c) 2022-2023 Embedded and Real-Time Systems Laboratory,
 *                         Graduate School of Information Science, Nagoya Univ., JAPAN
 */

#include <syssvc/syslog.h>
#include <spike/hub/system.h>
#include <serial/serial.h>
#include <stdio.h>
#include "port_d.h"
extern int   serial_opn_por(ID portid);
extern int   serial_wri_dat(ID portid, char *message, int size);
extern FILE *serial_open_newlib_file(ID portid);

/*
 *  メインタスク
 */
void
main_task(intptr_t exinf)
{
  dly_tsk(1000000);

  char str[] = "*** Testing USART serial on port D ***";
  int status = serial_opn_por(SIO_UART_D_PORTID);
  if (status != E_OK) {
    syslog(LOG_ERROR, "Error opening serial port D.");
    hub_system_shutdown();
  }
  int n = serial_wri_dat(SIO_UART_D_PORTID, str, sizeof(str));
  if (n < 0) {
    syslog(LOG_ERROR, "Error writing to serial port D.");
    hub_system_shutdown();
  }

  FILE *fd = serial_open_newlib_file(SIO_UART_D_PORTID);
  while (1) {
    SYSTIM st;
    get_tim(&st);
    fprintf(fd, "%u\n", (unsigned int) st);
    dly_tsk(1000000); // 1 sec
  }
  hub_system_shutdown();
}
