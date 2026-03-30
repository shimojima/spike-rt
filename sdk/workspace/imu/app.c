#include "kernel_cfg.h"
#include "app.h"
#include <stdio.h>
#include <serial/newlib.h>

#include <spike/hub/imu.h>
#include <spike/hub/button.h>
#include <spike/hub/display.h>
#include <spike/hub/speaker.h>

#define DEBUG_IMU 0  // Change this value to see (=1) / suppress (=0) debug output
#if DEBUG_IMU
#include <pbio/imu.h>
static void check_calibration_status(FILE *fp) {
  // ********************
  // To calibrate the IMU module of the SPIKE Prim Hub, install Pybricks firmware at https://code.pybricks.com
  // and run "import _imu_calibrate" on the REPL (cf. https://github.com/pybricks/support/issues/1907)
  //
  // The default values are hardcoded in drivers/spike/hub/imu.c
  // ********************
  extern int hub_imu_calibration_status;
  extern pbio_imu_persistent_settings_t hub_imu_settings;
  pbio_imu_persistent_settings_t *s = &hub_imu_settings;
  switch (hub_imu_calibration_status) {
  case  0: fprintf(fp, "IMU not calibrated\n"); break;
  case -1: fprintf(fp, "Calibration data in flash does not look good; fall back to default values\n"); break;
  case -2: fprintf(fp, "Failed to read calibration data from flash; use default values\n"); break;
  case +1: fprintf(fp, "Calibration data found in flash!\n"); break;
  }
  fprintf(fp, "Stationary thresholds: %f %f\n", s->gyro_stationary_threshold, s->accel_stationary_threshold);
  fprintf(fp, "Gravity.+: %f %f %f\n", s->gravity_pos.x, s->gravity_pos.y, s->gravity_pos.z);
  fprintf(fp, "Gravity.-: %f %f %f\n", s->gravity_neg.x, s->gravity_neg.y, s->gravity_neg.z);
  fprintf(fp, "Bias : %f %f %f\n", s->angular_velocity_bias_start.x, s->angular_velocity_bias_start.y, s->angular_velocity_bias_start.z);
  fprintf(fp, "Scale: %f %f %f\n", s->angular_velocity_scale.x, s->angular_velocity_scale.y, s->angular_velocity_scale.z);
}
#else
#define check_calibration_status(fp)
static int find_closest(int heading, int theta[], int x[], int y[], int n) {
  for (int i = 0; i < n; i++) {
    if (heading == theta[i]) return i;
    if (heading < theta[i]) {
      if (heading - theta[i-1] > theta[i] - heading) return i-1;
      return i;
    }
  }
  return 0;
}
#endif

static inline hub_button_t hub_buttons_pressed(hub_button_t buttons) {
  hub_button_t pressed;
  hub_button_is_pressed(&pressed);
  return pressed & buttons;
}

/* メインタスク(起動時にのみ関数コールされる) */
void main_task(intptr_t unused) {

  dly_tsk(5*1000*1000);

#if DEBUG_IMU
  FILE *fp = serial_open_newlib_file(SIO_USB_PORTID);
#endif

  hub_imu_init();

  check_calibration_status(fp);

  // HackSPi's hub is tilted at ~51 degrees.
  hub_imu_set_tilt(51.0f);

  // Wait for IMU to become ready
  hub_speaker_set_volume(50);
  while (!hub_imu_is_ready()) {
    hub_speaker_play_tone(220, 50);
    dly_tsk(150*1000);
  }

  // Show heading
  hub_display_orientation(PBIO_SIDE_LEFT);
  while (1) {
    if (hub_buttons_pressed(HUB_BUTTON_CENTER) != 0) {
      hub_imu_reset_heading();
      hub_speaker_play_tone(880, 50);
    }
#if DEBUG_IMU
    float w[3], a[3];
    bool stationary = hub_imu_is_stationary();
    fprintf(fp, "[%c] ", stationary ? 'T' : 'F');
    hub_imu_get_acceleration(a);
    fprintf(fp, "a = (%.0f %.0f %.0f); ", a[0], a[1], a[2]);
    hub_imu_get_angular_velocity(w);
    fprintf(fp, "ω = (%.0f %.0f %.0f); ", w[0], w[1], w[2]);
    fprintf(fp, "h = %.0f\n", hub_imu_get_heading());
#else
    int outer_t[] = { 0, 27, 45, 63, 90, 117, 135, 153, 180, 207, 225, 243, 270, 297, 315, 333, 360 };
    int outer_x[] = { 2,  3,  4,  4,  4,   4,   4,   3,   2,   1,   0,   0,   0,   0,   0,   1,   2 };
    int outer_y[] = { 0,  0,  0,  1,  2,   3,   4,   4,   4,   4,   4,   3,   2,   1,   0,   0,   0 };
    int inner_t[] = { 0,     45,     90,      135,      180,      225,      270,      315,      360 };
    int inner_x[] = { 2,      3,      3,        3,        2,        1,        1,        1,        2 };
    int inner_y[] = { 1,      1,      2,        3,        3,        3,        2,        1,        1 };
    int h = (int) hub_imu_get_heading(); while (h < 0) { h += 360; } while (h >= 360) { h -= 360; } // 0 <= h < 360
    int k;
    hub_display_off();
    k = find_closest(h, outer_t, outer_x, outer_y, sizeof(outer_t)/sizeof(outer_t[0]));
    hub_display_pixel(outer_x[k], outer_y[k], 100);
    k = find_closest(h, inner_t, inner_x, inner_y, sizeof(inner_t)/sizeof(inner_t[0]));
    hub_display_pixel(inner_x[k], inner_y[k], 100);
    hub_display_pixel(2, 2, 100);
#endif
    dly_tsk(1000*1000);
  }
  
  /* タスク終了 */
  ext_tsk();
}
