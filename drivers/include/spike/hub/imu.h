/*
 * SPDX-License-Identifier: MIT
 *
 * API for the hub built-in IMU.
 *
 * Copyright (c) 2022 Embedded and Real-Time Systems Laboratory,
 *            Graduate School of Information Science, Nagoya Univ., JAPAN
 */

/**
 * \file    spike/hub/imu.h
 * \brief   API for the hub built-in IMU.
 * \author  Shu Yoshifumi, Makoto Shimojima
 */

/**
 * \addtogroup  Hub Hub
 * @{
 */


/**
 * \~English
 * \defgroup  IMU IMU
 * \brief     API for the hub built-in IMU.
 * @{
 *
 * \~Japanese
 * \defgroup  IMU IMU
 * \brief     ハブ内蔵IMU向けAPI．
 * @{
 */


#ifndef _HUB_IMU_H_
#define _HUB_IMU_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <pbio/error.h>

/**
 * \~English
 * \brief         Initialize the IMU driver.
 * \retval PBIO_SUCCESS      Successfully initialized.
 * \retval PBIO_ERROR_FAILED Failed to initialized.
 *
 * \~Japanese
 * \brief       IMUドライバを初期化する．
 * \retval PBIO_SUCCESS      初期化が完了．
 * \retval PBIO_ERROR_FAILED 初期化に失敗した．
 */
pbio_error_t hub_imu_init(void);

/**
 * \~English
 * \brief         Check if the IMU module is "ready."
 * \retval true   Module is ready.
 * \retval false  Module still needs more quiet time.
 *
 * \~Japanese
 * \brief       IMUモジュールが使用可能かチェックする．
 * \retval true  モジュールの準備が整っている．
 * \retval false モジュールの準備がまだ整っていない．
 */
bool hub_imu_is_ready(void);

/**
 * \~English
 * \brief         Check if the IMU module is "stationary."
 * \retval true   Module is stationary.
 * \retval false  Module is in motion.
 *
 * \~Japanese
 * \brief       IMUモジュールが静止状態がチェックする．
 * \retval true  モジュールは静止している．
 * \retval false モジュールは動いている．
 */
bool hub_imu_is_stationary(void);

/**
 * \~English
 * \brief       Set the tilt angle of the IMU module.
 * \param angle Tilt angle (wrt the Y-axis) in degrees.
 *
 * \~Japanese
 * \brief       IMUモジュールの傾斜角度をセットする．
 * \param angle モジュールの（Y軸に対する）傾斜角度（度）．
 */
void hub_imu_set_tilt(float angle);

/**
 * \~English
 * \brief       Get the acceleration by the IMU.
 * \param accel   Float array to store the x/y/z axes acceleration[mm/s^2].
 *
 * \~Japanese
 * \brief       IMUから加速度を取得する．
 * \param accel   x/y/z軸の加速度を格納するためのfloat配列[mm/s^2]．
 */
void hub_imu_get_acceleration(float accel[3]);

/**
 * \~English
 * \brief       Get the angular velocity by the IMU.
 * \param angv   Float array to store the x/y/z axes angular velocity [°/s].
 *
 * \~Japanese
 * \brief       IMUから角速度を取得する．
 * \param angv   x/y/z軸の角速度を格納するためのfloat配列[°/s]．
 */
void hub_imu_get_angular_velocity(float angv[3]);

/**
 * \~English
 * \brief       Get the temperature by the IMU.
 * \retval      Temperature [℃]．
 *
 * \~Japanese
 * \brief       IMUから温度を取得する．
 * \retval      温度 [℃]．
 */
float hub_imu_get_temperature(void);

/**
 * \~English
 * \brief       Get the heading angle of the IMU.
 * \retval      Angle in degrees.
 *
 * \~Japanese
 * \brief       IMUの方位角を返す．
 * \retval      方位角 [°]．
 */
float hub_imu_get_heading(void);

/**
 * \~English
 * \brief       Reset the heading angle of the IMU.
 *
 * \~Japanese
 * \brief       IMUの方位角をリセットする．
 */
void hub_imu_reset_heading(void);

#ifdef __cplusplus
}
#endif

#endif // _HUB_IMU_H_

/**
 * @} // End of group IMU
 */

/**
 * @} // End of group Hub
 */
