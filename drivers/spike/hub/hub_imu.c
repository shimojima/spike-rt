/*
 * SPDX-License-Identifier: MIT
 *
 * API for the hub built-in IMU.
 *
 * Copyright (c) 2022 Embedded and Real-Time Systems Laboratory,
 *            Graduate School of Information Science, Nagoya Univ., JAPAN
 */
#include <t_syslog.h>
#include <spike/hub/imu.h>

#include <pbio/imu.h>
#include <math.h>

// (Undocumented) hooks to poke around calibration status. See sdk/workspace/imu.c
int hub_imu_calibration_status = 0;    // not calibrated
pbio_imu_persistent_settings_t hub_imu_settings;

/*
 * We implement the IMU API by just wrapping functions in external/libpybricks/lib/pbio/src/imu.c.
 */

// Use threshold values, which are more or less constants, to see if data in flash looks OK
static bool looks_OK(pbio_imu_persistent_settings_t *settings) {
  if (settings->gyro_stationary_threshold < 0.0f) return false;
  if (settings->gyro_stationary_threshold > 5.0f) return false;
  if (settings->accel_stationary_threshold < 2000.0f) return false;
  if (settings->accel_stationary_threshold > 3000.0f) return false;
  return true;
}

pbio_error_t hub_imu_init(void) {
  pbio_imu_persistent_settings_t *settings_in_flash = NULL;
  pbio_imu_init();
  if (pbio_imu_get_settings(&settings_in_flash) == PBIO_SUCCESS) {
    if (looks_OK(settings_in_flash)) {
      hub_imu_calibration_status = +1; // calibration data in flash looks good!
      hub_imu_settings = *settings_in_flash; // keep a local, exportable, copy
      pbio_imu_apply_loaded_settings(settings_in_flash);
      return PBIO_SUCCESS;
    } else {
      hub_imu_calibration_status = -1; // calibration data in flash does not look good; fall back to default...
    }
  } else {
    hub_imu_calibration_status = -2;   // failed to read calibration data from flash
  }
#if 0
  pbio_imu_set_default_settings(&hub_imu_settings);
#else
  hub_imu_settings.flags = 0;
  hub_imu_settings.gyro_stationary_threshold  =    2.0f;
  hub_imu_settings.accel_stationary_threshold = 2500.0f;
  hub_imu_settings.gravity_pos.x =  +9969.83984375f;
  hub_imu_settings.gravity_neg.x =  -9739.06640625f;
  hub_imu_settings.gravity_pos.y =  +9923.37402344f;
  hub_imu_settings.gravity_neg.y =  -9842.00488281f;
  hub_imu_settings.gravity_pos.z =  +9666.05957031f;
  hub_imu_settings.gravity_neg.z = -10125.87890625f;
  hub_imu_settings.angular_velocity_bias_start.x = -1.07564986f;
  hub_imu_settings.angular_velocity_bias_start.y = -2.09562278f;
  hub_imu_settings.angular_velocity_bias_start.z = -0.96552324f;
  hub_imu_settings.angular_velocity_scale.x = 363.33685303f;
  hub_imu_settings.angular_velocity_scale.y = 358.36773682f;
  hub_imu_settings.angular_velocity_scale.z = 359.43572998f;
  pbio_imu_apply_loaded_settings(&hub_imu_settings);
#endif
  return PBIO_SUCCESS;
}

bool hub_imu_is_ready(void) {
  return pbio_imu_is_ready();
}

bool hub_imu_is_stationary(void) {
  return pbio_imu_is_stationary();
}

void hub_imu_set_tilt(float angle) {
  double T = (double) angle * M_PI / 180.0f;
  double sinT = sin(T);
  double cosT = cos(T);
  pbio_geometry_xyz_t front = { .x = +cosT, .y = 0.0f, .z = +sinT };
  pbio_geometry_xyz_t top   = { .x = -sinT, .y = 0.0f, .z = +cosT };
  pbio_imu_set_base_orientation(&front, &top);
}

void hub_imu_get_acceleration(float accel[3]) {
  pbio_imu_get_acceleration((pbio_geometry_xyz_t *) accel, true);
}

void hub_imu_get_angular_velocity(float angv[3]) {
  pbio_imu_get_angular_velocity((pbio_geometry_xyz_t *) angv, true);
}

float hub_imu_get_temperature(void) {
  return 0.0;
}

float hub_imu_get_heading(void) {
  return pbio_imu_get_heading(PBIO_IMU_HEADING_TYPE_3D);
}

void hub_imu_reset_heading(void) {
  pbio_imu_set_heading(0.0f);
}
