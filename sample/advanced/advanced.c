/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2019 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 * 
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  $Id: sample1.c 1237 2019-07-10 01:00:30Z ertl-hiro $
 */

#include <t_syslog.h>
#include "kernel_cfg.h"
#include "advanced.h"
#include <cbricks/hub/button.h>
#include <cbricks/hub/display.h>
#include <cbricks/pup/colorsensor.h>
#include <cbricks/pup/forcesensor.h>
#include <pbio/port.h>
#include <pbio/motor_process.h>
#include <pbio/servo.h>
#include <pbdrv/battery.h>
#include <stdio.h>

//#include <pbsys/user_program.h>

typedef struct {
  pbio_servo_t *servo;
  pbio_tacho_t *tacho;
} pbio_motor_t;
static pbio_motor_t arm_motor;
static pbio_motor_t right_motor;
static pbio_motor_t left_motor;
static pup_device_t *color_sensor;
//static pup_device_t *touch_sensor;

#define RETRY_COUNT 4
static pbio_motor_t config_motor(pbio_port_id_t port, pbio_direction_t direction)
{
  pbio_motor_t motor;
  int status = pbio_motor_process_get_servo(port, &(motor.servo));
  if (status != PBIO_SUCCESS) {
    printf("[%c] error getting access to motor.\n", port);
    slp_tsk();
  }
  fix16_t gear_ratio = F16C(1, 0);
  bool reset_angle = false;
  for (int i = 0; i < RETRY_COUNT; i++) {
    status = pbio_servo_setup(motor.servo, direction, gear_ratio, reset_angle);
    if (i == 0 && status == PBIO_ERROR_NO_DEV) {
      // try one more time
    } else if (status != PBIO_ERROR_AGAIN) break;
    dly_tsk(1000000);
  }
  if (status != PBIO_SUCCESS) {
    printf("[%c] error configuring motor: %s\n", port, pbio_error_str(status));
    slp_tsk();
  }
  status = pbio_tacho_get(port, &(motor.tacho), direction, gear_ratio, reset_angle);
  if (status != PBIO_SUCCESS) {
    printf("[%c] error configuring tacho: %s\n", port, pbio_error_str(status));
    slp_tsk();
  }
  long int angle = 0;
  status = pbio_tacho_reset_angle(motor.tacho, &angle, true);
  if (status != PBIO_SUCCESS) {
    printf("[%c] error resetting tacho: %s\n", port, pbio_error_str(status));
    slp_tsk();
  }
  return motor;
}

static pup_device_t *config_color(pbio_port_id_t port)
{
  pup_device_t *color = pup_color_sensor_get_device(port);
  if (color == NULL) {
    printf("[%c] error configuring color sensor.\n", port);
    slp_tsk();
  }
  return color;
}

#if 0
static pup_device_t *config_touch(pbio_port_id_t port)
{
  pup_device_t *touch = pup_force_sensor_get_device(port);
  if (touch == NULL) {
    printf("[%c] error configuring touch sensor.\n", port);
    slp_tsk();
  }
  return touch;
}
#endif

static void config_devices(void)
{
  dly_tsk(1000000); // Allow 1 sec of quiet time before configuring the devices.
  right_motor  = config_motor(PBIO_PORT_ID_B, PBIO_DIRECTION_CLOCKWISE);        // forward/backward: +/-
  left_motor   = config_motor(PBIO_PORT_ID_E, PBIO_DIRECTION_COUNTERCLOCKWISE); // forward/backward: +/-
  arm_motor    = config_motor(PBIO_PORT_ID_A, PBIO_DIRECTION_COUNTERCLOCKWISE); // up/down: +/-
  color_sensor = config_color(PBIO_PORT_ID_C);
//  touch_sensor = config_touch(PBIO_PORT_ID_D);
}

#if 0
static void wait_for_touch(void)
{
  while (!pup_force_sensor_touched(touch_sensor)) dly_tsk(100);
  while ( pup_force_sensor_touched(touch_sensor)) dly_tsk(100);
}
#endif

static inline hub_button_t hub_buttons_pressed(hub_button_t buttons)
{
  hub_button_t pressed;
  hub_button_is_pressed(&pressed);
  return pressed & buttons;
}

static hub_button_t wait_for_hub_buttons(hub_button_t buttons_to_watch)
{
  hub_button_t buttons = 0;
  hub_button_t pressed;
  while (hub_buttons_pressed(buttons_to_watch) == 0) dly_tsk(10000);
  while (pressed = hub_buttons_pressed(buttons_to_watch), pressed != 0) {
    buttons |= pressed;
    dly_tsk(10000);
  }
  return buttons;
}

#define WHITE_BRIGHTNESS (100)
#define BLACK_BRIGHTNESS (  0)
#define STEERING_KP      (4.F)
#define STEERING_KD      (-4.F)
#define BASE_SPEED       (200)
#define LEFT_EDGE        (-1)
#define RIGHT_EDGE       (+1)
static int old_reflection = (WHITE_BRIGHTNESS + BLACK_BRIGHTNESS) / 2;
void linetrace(int edge)
{
  int target      = (WHITE_BRIGHTNESS + BLACK_BRIGHTNESS) / 2;
  int reflection  = pup_color_sensor_reflection(color_sensor);
  int delta       = target - reflection;
  int diff        = reflection - old_reflection;
  int steering    = (int) ((double) delta * STEERING_KP + diff * STEERING_KD);

  int left_speed  = BASE_SPEED + steering * edge;
  int right_speed = BASE_SPEED - steering * edge;

  pbio_servo_run(right_motor.servo, right_speed);
  pbio_servo_run( left_motor.servo,  left_speed);

  old_reflection = reflection;
}

/*
 *  メインタスク
 */

static int edge = LEFT_EDGE;
#include <stdio.h>
void
main_task(intptr_t exinf)
{
  uint16_t volt, curr;
  char buffer[80];
  pbsys_user_program_prepare(NULL); // pbsys_processをユーザプログラム実行状態に遷移させる．
  config_devices();
  printf("Devices configured successfully...\n");
  pbdrv_battery_get_voltage_now(&volt);
  pbdrv_battery_get_current_now(&curr);
  printf("Battery: %d mV @ %d mA\n", volt, curr);
  snprintf(buffer, sizeof(buffer), "%u mV %u mA ", volt, curr);
  hub_display_text(buffer, 240, 10);
  printf("ADVANCED\n");
  dly_tsk(1000000);
  hub_display_char('A');
  while (1) {
    hub_button_t buttons = wait_for_hub_buttons(HUB_BUTTON_CENTER|HUB_BUTTON_RIGHT|HUB_BUTTON_LEFT);
    if (buttons&HUB_BUTTON_LEFT) {
      hub_display_char('L'); printf("L\n");
      edge = LEFT_EDGE;
    } else if (buttons&HUB_BUTTON_RIGHT) {
      hub_display_char('R'); printf("R\n");
      edge = RIGHT_EDGE;
    } else if (buttons&HUB_BUTTON_CENTER) {
      break;
    }
  }
  sta_cyc(LED_TASK_CYC);
  wait_for_hub_buttons(HUB_BUTTON_CENTER);
  dly_tsk(100000); // 0.1 sec
  sta_cyc(RUN_TASK_CYC);
  wait_for_hub_buttons(HUB_BUTTON_CENTER);
  stp_cyc(RUN_TASK_CYC);
  pbio_servo_stop(right_motor.servo, PBIO_DCMOTOR_COAST);
  pbio_servo_stop( left_motor.servo, PBIO_DCMOTOR_COAST);
  stp_cyc(LED_TASK_CYC);
  pbsys_user_program_unprepare();
}

void run_task(intptr_t exinf)
{
  linetrace(edge);
}

static char message[] = "SPIKE-RT ";
static char *p = message;
void led_task(intptr_t exinf)
{
  hub_display_char(*p);
  p++; if (*p == '\0') p = message;
}
