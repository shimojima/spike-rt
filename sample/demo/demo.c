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
#include "demo.h"

#include <pbsys/user_program.h>
#include <pbsys/light.h>
#include <pbio/port.h>
#include <pbio/motor_process.h>
#include <pbio/servo.h>
#include <cbricks/pup/colorsensor.h>
#include <cbricks/pup/forcesensor.h>
#include <stdio.h>

extern const uint8_t pb_font_5x5[95][5];

static pbio_servo_t *arm_motor;
static pbio_servo_t *right_motor;
static pbio_servo_t *left_motor;
static pup_device_t *color_sensor;
static pup_device_t *touch_sensor;
#if 0
static pup_device_t *sonar_sensor;
#endif

#define RETRY_COUNT 3
static pbio_servo_t *config_motor(pbio_port_id_t port, pbio_direction_t direction)
{
  pbio_servo_t *motor;
  int status = pbio_motor_process_get_servo(port, &motor);
  if (status != PBIO_SUCCESS) {
    printf("[%c] error getting access to motor.\n", port);
    slp_tsk();
  }
  fix16_t gear_ratio = F16C(1, 0);
  bool reset_angle = false;
  for (int i = 0; i < RETRY_COUNT; i++) {
    status = pbio_servo_setup(motor, direction, gear_ratio, reset_angle);
    if (status == PBIO_SUCCESS) break;
    dly_tsk(1000000);
  }
  if (status != PBIO_SUCCESS) {
    printf("[%c] error configuring motor: %d\n", port, status);
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

static pup_device_t *config_touch(pbio_port_id_t port)
{
  pup_device_t *touch = pup_force_sensor_get_device(port);
  if (touch == NULL) {
    printf("[%c] error configuring touch sensor.\n", port);
    slp_tsk();
  }
  return touch;
}

#if 0
static pup_device_t *config_sonar(pbio_port_id_t port)
{
  return NULL;
}
#endif

static void config_devices(void)
{
  dly_tsk(1000000); // 1 sec of quiet time before configuring the devices.
  right_motor  = config_motor(PBIO_PORT_ID_B, PBIO_DIRECTION_CLOCKWISE);        // forward/backward: +/-
  left_motor   = config_motor(PBIO_PORT_ID_E, PBIO_DIRECTION_COUNTERCLOCKWISE); // forward/backward: +/-
  arm_motor    = config_motor(PBIO_PORT_ID_A, PBIO_DIRECTION_COUNTERCLOCKWISE); // up/down: +/-
  color_sensor = config_color(PBIO_PORT_ID_C);
  touch_sensor = config_touch(PBIO_PORT_ID_D);
#if 0
  sonar_sensor = config_sonar(PBIO_PORT_ID_F);
#endif
}

static void wait_for_touch(void)
{
  while (!pup_force_sensor_touched(touch_sensor)) dly_tsk(100);
  while ( pup_force_sensor_touched(touch_sensor)) dly_tsk(100);
}

/*
 *  メインタスク
 */
void
main_task(intptr_t exinf)
{
  pbsys_user_program_prepare(NULL); // pbsys_processをユーザプログラム実行状態に遷移させる．
  config_devices();
  printf("Devices configured successfully...\n");
  sta_cyc(LED_TASK_CYC); // Start LED task to show all the devices are successfully configured.
  wait_for_touch();
  sta_cyc(RUN_TASK_CYC);
  wait_for_touch();
  stp_cyc(LED_TASK_CYC);
  stp_cyc(RUN_TASK_CYC);
  pbsys_user_program_unprepare();
  assert(0);
}

#include <stdio.h>
static char message[] = "SPIKE-RT ";
static char *p = message;
void led_task(intptr_t exinf)
{
  SYSTIM st;
  get_tim(&st);
  printf("%u: %s\n", (unsigned int) st, message);
  pb_assert(pbio_light_matrix_clear(pbsys_hub_light_matrix));
  pb_assert(pbio_light_matrix_set_rows(pbsys_hub_light_matrix, pb_font_5x5[*p - 32]));
  pb_assert(pbio_color_light_on(pbsys_status_light, PBIO_COLOR_YELLOW));
  p++; if (*p == '\0') p = message;
}
#define WHITE_BRIGHTNESS (100)
#define BLACK_BRIGHTNESS (  0)
#define STEERING_COEFF   (3.F)
#define BASE_SPEED       (200)
#define LEFT_EDGE        (-1)
#define RIGHT_EDGE       (+1)
void run_task(intptr_t exinf)
{
  int target      = (WHITE_BRIGHTNESS - BLACK_BRIGHTNESS) / 2;
  int reflection  = pup_color_sensor_reflection(color_sensor);
  int delta       = target - reflection;
  int steering    = (int) ((double) delta * STEERING_COEFF);

  int left_speed  = BASE_SPEED + steering * LEFT_EDGE;
  int right_speed = BASE_SPEED - steering * LEFT_EDGE;

  pbio_servo_run(right_motor, right_speed);
  pbio_servo_run( left_motor,  left_speed);
}
