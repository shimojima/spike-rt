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

extern const uint8_t pb_font_5x5[95][5];

/*
 *  メインタスク
 */
void
main_task(intptr_t exinf)
{
  pbsys_user_program_prepare(NULL); // pbsys_processをユーザプログラム実行状態に遷移させる．
  sta_cyc(LED_TASK_CYC);
  slp_tsk();
  stp_cyc(LED_TASK_CYC);
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
