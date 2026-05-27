/*
 * mpu6050_port.h
 *
 *  Created on: Oct 9, 2025
 *      Author: danba
 */

#ifndef INC_MPU6050_PORT_H_
#define INC_MPU6050_PORT_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "cmsis_os.h"

extern I2C_HandleTypeDef hi2c1;

extern float gx;
extern float gy;
extern float gz;

extern float mag_x;
extern float mag_y;
extern float mag_z;

extern float mag_adj_x;
extern float mag_adj_y;
extern float mag_adj_z;

void mpu_init(void);
typedef struct {
    float q_angle;
    float q_bias;
    float r_measure;

    float angle;
    float bias;
    float p[2][2];
}kalman_t;


typedef struct{
	int16_t ax;
	int16_t ay;
	int16_t az;

	float Ax;
	float Ay;
	float Az;
}mpu_accel_raw;

typedef struct{

	float roll ;
    float pitch ;

}accel_roll_pitch;

typedef struct{

	float rc ;
	float pc;
}accel_roll_pitch_calib_constant;

typedef struct{
	float rc ;
	float pc ;
}gyro_roll_pitch_calib_constant;

typedef struct{
	float gx_offset ;
	float gy_offset ;
	float gz_offset ;
}gyro_calib;

typedef struct{
	float ax_offset ;
	float ay_offset ;
	float az_offset ;
}accel_calib;

typedef struct{
	float Gx;
	float Gy;
	float Gz;
}mpu_gyro_raw;

typedef struct{
	float roll ;
	float pitch ;
}gyro_roll_pitch;



void mpu_accel_read(mpu_accel_raw *param);
void mpu_roll_pitch_read_accel(accel_roll_pitch *data);
void mpu_accel_calibration(accel_calib *offset);

void mpu_gyro_read(mpu_gyro_raw *data);
void mpu_roll_pitch_read_gyro(gyro_roll_pitch *data, float dt);
void mpu_gyro_calibration(gyro_calib *data);

void mpu_roll_pitch_calibration_accel(accel_roll_pitch_calib_constant *calib);
void mpu_roll_pitch_calibration_gyro(gyro_roll_pitch_calib_constant *data);


double Kalman_get_angle(kalman_t *kalman,
                        double newAngle,
                        double newRate,
                        double dt);

void mpu_get_kalman_angles(float *roll, float *pitch);

#endif
