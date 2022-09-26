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
#include "motor.h"

#include <pbio/servo.h>
#include <pbio/tacho.h>
#include <stdio.h>

/*
 *  メインタスク
 */
void
main_task(intptr_t exinf)
{
  pbio_servo_t *motor;
  pbio_tacho_t *tacho;
  pbio_error_t status;
  dly_tsk(1000000);
  status = pbio_motor_process_get_servo(PBIO_PORT_ID_B, &motor);
  if (status != PBIO_SUCCESS) {
    printf("Error getting access to motor: %d\n", status);
    slp_tsk();
  }
  pbio_direction_t positive = PBIO_DIRECTION_CLOCKWISE; // or PBIO_DIRECTION_COUNTERCLOCKWISE
  fix16_t gear_ratio = F16C(1, 0);
  bool reset_angle = false;
  for (int i = 0; i < 3; i++) {
    status = pbio_servo_setup(motor, positive, gear_ratio, reset_angle);
    if (status == PBIO_SUCCESS) break;
    dly_tsk(1000000);
  }
  if (status != PBIO_SUCCESS) {
    printf("Error configuring motor: %d\n", status);
    slp_tsk();
  }
  status = pbio_tacho_get(PBIO_PORT_ID_B, &tacho, positive, gear_ratio, reset_angle);
  if (status != PBIO_SUCCESS) {
    printf("Error configuring tacho meter: %d\n", status);
    slp_tsk();
  }
  int angle = 0;
  status = pbio_tacho_reset_angle(tacho, &angle, true);
  if (status != PBIO_SUCCESS) {
    printf("Error resetting tacho meter: %d\n", status);
    slp_tsk();
  }
  int speed = 360; // in deg/sec
  status = pbio_servo_run(motor, speed);
  if (status != PBIO_SUCCESS) {
    printf("Error running motor: %d\n", status);
    slp_tsk();
  }
  long int _count = 0, _angle = 0, _rate = 0, _rotation = 0;
  while (1) {
    long int  count,  angle,  rate,  rotation;
    pbio_tacho_get_count(tacho, &count);
    pbio_tacho_get_angle(tacho, &angle);
    pbio_tacho_get_rate(tacho, &rate);
    pbio_tacho_get_angular_rate(tacho, &rotation);
    printf("%ld (%ld) %ld (%ld) %ld (%ld) %ld (%ld)\n",
           count, count-_count, angle, angle-_angle, rate, rate-_rate, rotation, rotation-_rotation);
    _count = count; _angle = angle; _rate = rate; _rotation = rotation;
    dly_tsk(1000000);
  }
  status = pbio_servo_stop(motor, PBIO_DCMOTOR_COAST);
  if (status != PBIO_SUCCESS) {
    printf("Error stopping motor: %d\n", status);
    slp_tsk();
  }
  slp_tsk();
  assert(0);
}
