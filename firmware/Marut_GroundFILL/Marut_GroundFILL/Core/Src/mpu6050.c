#include "mpu6050.h"

#define MPU6050_I2C_ADDR    ((uint8_t)(0x68u << 1u))
#define MPU9250_MAG_ADDRESS 0x0C

#define WHO_AM_I_REG        0x75
#define PWR_MGMT_1_REG      0x6B
#define SMPLRT_DIV_REG      0x19
#define GYRO_CONFIG_REG     0x1B
#define ACCEL_CONFIG_REG    0x1C
#define CONFIG_REG          0x1A  
#define ACCEL_XOUT_H_REG    0x3B
#define GYRO_XOUT_H_REG     0x43


extern float calibration_const_global_roll_accel;
extern float calibration_const_global_pitch_accel;

extern float calibration_const_global_gx;
extern float calibration_const_global_gy;
extern float calibration_const_global_gz;

extern I2C_HandleTypeDef hi2c1;

static kalman_t kalmanX = { .q_angle = 0.001f, .q_bias = 0.003f, .r_measure = 1.25f, };

static kalman_t kalmanY = { .q_angle = 0.001f, .q_bias = 0.003f, .r_measure = 0.80f, };

typedef struct{
	int16_t ax;
	int16_t ay;
	int16_t az;

	float Ax; 
	float Ay; 
	float Az;
}mpu_accel_raw; 

typedef struct{

	float roll = 0.0f;
    float pitch = 0.0f;

}accel_roll_pitch;

typedef struct{
	float r = 0; 
	float p = 0;
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
	float roll = 0.0f;
	float pitch = 0.0f;
}gyro_roll_pitch;


void Kalman_Init(kalman_t *K) {
	K->angle = 0.0f;
	K->bias = 0.0f;

	K->p[0][0] = 1.0f;
	K->p[0][1] = 0.0f;
	K->p[1][0] = 0.0f;
	K->p[1][1] = 1.0f;
}

void mpu_init(void) {

	uint8_t who = 0;
	HAL_I2C_Mem_Read(&hi2c1,MPU6050_I2C_ADDR,
	WHO_AM_I_REG, 1, &who, 1, 100);

	printf("WHO_AM_I = 0x%02X\n", who);

	uint8_t data;
	data = 0x00;
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_I2C_ADDR, PWR_MGMT_1_REG, 1, &data, 1, 50);
	
	data = 0x00;
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_I2C_ADDR, GYRO_CONFIG_REG, 1, &data, 1, 50);
	
	data = 0x00;
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_I2C_ADDR, ACCEL_CONFIG_REG, 1, &data, 1, 50);
	
	data = 0x05;
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_I2C_ADDR, CONFIG_REG, 1, &data, 1, 100);

	data = 0x03;
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_I2C_ADDR, SMPLRT_DIV_REG, 1, &data, 1, 100);
	Kalman_Init(&kalmanX);
	Kalman_Init(&kalmanY);
}

void mpu_accel_read(mpu_accel_raw *param) {
	uint8_t Rec_Data[6];
	
	HAL_I2C_Mem_Read(&hi2c1, MPU6050_I2C_ADDR, ACCEL_XOUT_H_REG, 1, Rec_Data, 6,
			50);

	param->ax = (int16_t) (Rec_Data[0] << 8 | Rec_Data[1]);
	param->ay = (int16_t) (Rec_Data[2] << 8 | Rec_Data[3]);
	param->az = (int16_t) (Rec_Data[4] << 8 | Rec_Data[5]);

	param->Ax = param->ax / 16384.0f;
	param->Ay = param->ay / 16384.0f;
	param->Az = param->az / 16384.0f;
}

void mpu_roll_pitch_read_accel( accel_roll_pitch *data) {
	mpu_accel_raw accel ;
	mpu_accel_read(&accel);

	data->Roll = atan2f(accel.Ay, sqrtf(accel.Ax * accel.Ax + accel.Az * accel.Az)) * 180.0f / M_PI;
	data->Pitch = atan2f((-accel.Ax), sqrtf(accel.Ay * accel.Ay + accel.Az * accel.Az)) * 180.0f / M_PI;
}

void mpu_roll_pitch_calibration_accel(accel_roll_pitch_calib_constant *calib) {

	accel_roll_pitch angels ;
	mpu_roll_pitch_read_accel(&angels);

	for (int i = 0; i < 2000; i++) {
		calib->r += angels.roll;
		calib->p += angels.pitch;
		HAL_Delay(1);

	}
	calib->rc = r / 2000.0f;
	calib->pc = p / 2000.0f;
}

float mpu_roll_pitch_calibration_gyro(gyro_roll_pitch_calib_constant *data) {

	float r = 0; 
	float p = 0;

	gyro_roll_pitch angle ;
	mpu_roll_pitch_read_gyro(&angle,0.001f);

	for (int i = 0; i < 2000; i++) {
		r += angle.roll;
		p += angle.pitch;
		HAL_Delay(1);
	}

	data->rc = r / 2000.0f;
	data->pc = p / 2000.0f;

}

float mpu_gyro_calibration(gyro_calib *data) {

	float gx = 0; 
	float gy = 0; 
	float gz = 0;

	mpu_gyro_raw g_raw_values;
	mpu_gyro_read(&g_raw_values);

	for (int i = 0; i < 2000; i++) {
		gx += g_raw_values.Gx;
		gy += g_raw_values.Gy;
		gz += g_raw_values.Gz;
		HAL_Delay(1);
	}

	data->gx_offset = gx / 2000.0f;
	data->gy_offset = gy / 2000.0f;
	data->gz_offset = gz / 2000.0f;

}

float mpu_accel_calibration(accel_calib *offset) {

	float ax = 0; 
	float ay = 0; 
	float az = 0;

	mpu_accel_raw raw_values;
	mpu_accel_read(&raw_values);


	for (int i = 0; i < 2000; i++) {
		ax += raw_values.Ax;
		ay += raw_values.Ay;
		az += raw_values.Az;
		HAL_Delay(1);
	}

	offset->ax_offset = ax / 2000.0f;
	offset->ay_offset = ay / 2000.0f;
	offset->az_offset = az / 2000.0f;

}

float mpu_gyro_read(mpu_gyro_raw *data) {

	uint8_t Rec_Data[6];

	int16_t gx; 
	int16_t gy; 
	int16_t gz;

	HAL_I2C_Mem_Read(&hi2c1,MPU6050_I2C_ADDR, GYRO_XOUT_H_REG, 1, Rec_Data, 6, 50);
	
	gx = (int16_t) (Rec_Data[0] << 8 | Rec_Data[1]);
	gy = (int16_t) (Rec_Data[2] << 8 | Rec_Data[3]);
	gz = (int16_t) (Rec_Data[4] << 8 | Rec_Data[5]);
	
	data->Gx = gx / 131.0f;
	data->Gy = gy / 131.0f;
	data->Gz = gz / 131.0f;
}

float mpu_roll_pitch_read_gyro(gyro_roll_pitch *data,float dt) {

	mpu_gyro_raw gyro; 
	mpu_gyro_read(&gyro);

	float Gx = gyro.Gx;
	float Gy = gyro.Gy;

	data->roll += Gx * dt;
	data->pitch += Gy * dt;

}

double Kalman_get_angle(kalman_t *kalman, double newAngle, double newRate,
		double dt) {

	
	double rate = newRate - kalman->bias;
	kalman->angle += dt * rate;
	
	kalman->p[0][0] += dt
			* (dt * kalman->p[1][1] - kalman->p[0][1] - kalman->p[1][0]
					+ kalman->q_angle);
	kalman->p[0][1] -= dt * kalman->p[1][1];
	kalman->p[1][0] -= dt * kalman->p[1][1];
	kalman->p[1][1] += kalman->q_bias * dt;

	double y = newAngle - kalman->angle;  

	double S = kalman->p[0][0] + kalman->r_measure; 
    
	double K[2]; 

	K[0] = kalman->p[0][0] / S;
	K[1] = kalman->p[1][0] / S;

	kalman->angle += K[0] * y;
	kalman->bias += K[1] * y;
	
	double tempA = kalman->p[0][0];
	double tempB = kalman->p[0][1];

	kalman->p[0][0] -= K[0] * tempA;
	kalman->p[0][1] -= K[0] * tempB;
	kalman->p[1][0] -= K[1] * tempA;
	kalman->p[1][1] -= K[1] * tempB;

	return kalman->angle;
}

void mpu_get_kalman_angles(float *roll, float *pitch) {

	static uint32_t lastTick = 0;
	uint32_t now = osKernelGetTickCount();

	float dt = (now - lastTick) / 1000.0f;
	lastTick = now;

	float accRoll = mpu_roll_pitch_read_accel(0)
			- calibration_const_global_roll_accel;
	float accPitch = mpu_roll_pitch_read_accel(1)
			- calibration_const_global_pitch_accel;

	float gyroRollRate = mpu_gyro_read(0) - calibration_const_global_gx;
	float gyroPitchRate = mpu_gyro_read(1) - calibration_const_global_gy;

	*roll = Kalman_get_angle(&kalmanX, accRoll, gyroRollRate, dt);
	*pitch = Kalman_get_angle(&kalmanY, accPitch, gyroPitchRate, dt);
}

