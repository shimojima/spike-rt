
#include <spike/hub/system.h>
#include <spike/hub/display.h>
#include "bluetooth.h"

#include <syssvc/syslog.h>
#include <serial/serial.h>
#include <stdio.h>
extern FILE *serial_open_newlib_file(ID portid);
/*
 *  メインタスク
 */
void
main_task(intptr_t exinf)
{
#ifdef USE_NEWLIB
  FILE *USB = serial_open_newlib_file(SIO_USB_PORTID);
  if (USB == NULL) hub_system_shutdown();
#endif
  // Connect to BT
  pbio_error_t err = serial_opn_por(SIO_BLUETOOTH_PORTID);
  if (err != E_OK) hub_system_shutdown();
#ifdef USE_NEWLIB
  FILE *BT  = serial_open_newlib_file(SIO_BLUETOOTH_PORTID);
  if (BT == NULL) hub_system_shutdown();
#endif
  hub_display_orientation(PBIO_SIDE_TOP);
  for (int i = 0; ; i++) {
    int a = (i%10000)/1000;
    int b = (i%1000)/100;
    int c = (i%100)/10;
    int d = i%10;
    char buffer[6];
    buffer[0] = '0'+a;
    buffer[1] = '0'+b;
    buffer[2] = '0'+c;
    buffer[3] = '0'+d;
    buffer[4] = '\n';
    buffer[5] = '\0';
#ifdef USE_NEWLIB
    fprintf(BT , "%d\n", i);
    fprintf(USB, "%d\n", i);
#else
    serial_wri_dat(SIO_BLUETOOTH_PORTID, buffer, sizeof(buffer));
    serial_wri_dat(SIO_USB_PORTID, buffer, sizeof(buffer));
#endif
    hub_display_char(buffer[0]); dly_tsk(100000); hub_display_off(); dly_tsk(100000);
    hub_display_char(buffer[1]); dly_tsk(100000); hub_display_off(); dly_tsk(100000);
    hub_display_char(buffer[2]); dly_tsk(100000); hub_display_off(); dly_tsk(100000);
    hub_display_char(buffer[3]); dly_tsk(600000);
  }
  hub_display_off();
  hub_system_shutdown();
}
