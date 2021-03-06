
#include "mpu60x0.h"

static I2C_HandleTypeDef imu_i2c;

static uint8_t deviceAvailable = 0;


int16_t Accel_X_RAW = 0;
int16_t Accel_Y_RAW = 0;
int16_t Accel_Z_RAW = 0;

int16_t Gyro_X_RAW = 0;
int16_t Gyro_Y_RAW = 0;
int16_t Gyro_Z_RAW = 0;

float Ax, Ay, Az, Gx, Gy, Gz;
float sampleTime;


// Private function declarations
static void checkStatus(HAL_StatusTypeDef status);
static void mpu60x0_checkWhoami(void);



void mpu60x0_init(I2C_HandleTypeDef i2cHandle){

	uint8_t Data;

	threadSafePrintf("[IMU] Starting Init...\n");

	imu_i2c = i2cHandle;

	mpu60x0_checkWhoami();

	if(deviceAvailable){

		// power management register 0X6B we should write all 0's to wake the sensor up
		Data = 0b00000000;
		checkStatus(HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, PWR_MGMT_1_REG, 1,&Data, 1, 1000));

		// Configure clock selection to follow the Z axis gyroscope
		Data = 0b00000011;
		checkStatus(HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, PWR_MGMT_1_REG, 1,&Data, 1, 1000));


		// Set DATA RATE of 1KHz by writing SMPLRT_DIV register
		Data = 0x07;
		checkStatus(HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, SMPLRT_DIV_REG, 1, &Data, 1, 1000));

		// Set accelerometer configuration in ACCEL_CONFIG Register
		// XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> 8g
		Data = 0b00100000;
		checkStatus(HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &Data, 1, 1000));

		// Set Gyroscopic configuration in GYRO_CONFIG Register
		// XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> 500 deg/sec
		Data = 0b00001000;
		checkStatus(HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, GYRO_CONFIG_REG, 1, &Data, 1, 1000));

		// filtering configuration - minimal filterning, as we'll do that on the processor
		Data = 0b00000000;
		checkStatus(HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, CONFIG_REG, 1,&Data, 1, 1000));

		threadSafePrintf("[IMU] Init Complete\n");
	} else {
		threadSafePrintf("[IMU] Init FAIL\n");
	}
	return;

}

void mpu60x0_update(){
	uint8_t Rec_Data[14];

	// Read 14 BYTES of data starting from ACCEL_XOUT_H register

	taskENTER_CRITICAL();
	checkStatus(HAL_I2C_Mem_Read(&imu_i2c, MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, Rec_Data, 14, 1000));
	sampleTime = getCurTime();
	taskEXIT_CRITICAL();

	Accel_X_RAW = ((int16_t)(Rec_Data[0]) << 8 | Rec_Data [1]);
	Accel_Y_RAW = ((int16_t)(Rec_Data[2]) << 8 | Rec_Data [3]);
	Accel_Z_RAW = ((int16_t)(Rec_Data[4]) << 8 | Rec_Data [5]);

	Ax = (float)Accel_X_RAW / ((float)(INT16_MAX)) * 8.0; //Full-scale range is 8 g
	Ay = (float)Accel_Y_RAW / ((float)(INT16_MAX)) * 8.0;
	Az = (float)Accel_Z_RAW / ((float)(INT16_MAX)) * 8.0;

	Gyro_X_RAW = ((int16_t)(Rec_Data[8])  << 8 | Rec_Data [9]);
	Gyro_Y_RAW = ((int16_t)(Rec_Data[10]) << 8 | Rec_Data [11]);
	Gyro_Z_RAW = ((int16_t)(Rec_Data[12]) << 8 | Rec_Data [13]);

	Gx = (float)Gyro_X_RAW / ((float)(INT16_MAX)) * 1000.0; //Full-scale range is 500 deg/sec
	Gy = (float)Gyro_Y_RAW / ((float)(INT16_MAX)) * 1000.0;
	Gz = (float)Gyro_Z_RAW / ((float)(INT16_MAX)) * 1000.0;

}


float mpu60x0_getXAccel(){
	return Ax;
}

float mpu60x0_getYAccel(){
	return Ay;
}

float mpu60x0_getZAccel(){
	return Az;
}

float mpu60x0_getXGyro(){
	return Gx;
}

float mpu60x0_getYGyro(){
	return Gy;
}

float mpu60x0_getZGyro(){
	return Gz;
}

double mpu60x0_getSampleTime(){
	return sampleTime;
}


static void mpu60x0_checkWhoami(){

	uint8_t check;

	// check device ID WHO_AM_I
	checkStatus(HAL_I2C_Mem_Read (&imu_i2c, MPU6050_ADDR,WHO_AM_I_REG,1, &check, 1, 1000));

	if (check == BITS_WHOAMI_EXP){  // 0x68 will be returned by the sensor if everything goes well
		deviceAvailable = 1;
	} else {
		threadSafePrintf("[IMU] unexpected WHOAMI response of %d\n", check);
	}

}


static void checkStatus(HAL_StatusTypeDef status) {
	switch (status) {
	case HAL_OK:
		break;
	case HAL_BUSY:
		threadSafePrintf("[IMU] PANIC: I2C: busy state occured at %s, line %d .\n", __FILE__,
				__LINE__);
		shutdown_restartUnExpected();
		break;
	case HAL_TIMEOUT:
		threadSafePrintf("[IMU] PANIC: I2C: transmit/receive timeout occured at %s, line %d .\n",
				__FILE__, __LINE__);
		shutdown_restartUnExpected();
		break;
	case HAL_ERROR:
		threadSafePrintf("[IMU] PANIC: I2C: transmit/receive error at %s, line %d.\n", __FILE__,
				__LINE__);
		shutdown_restartUnExpected();
		break;
	default:
		threadSafePrintf("[IMU] PANIC: I2C: transmit/receive WUT HAPPEND at %s, line %d.\n",
				__FILE__, __LINE__);
		shutdown_restartUnExpected();
		break;
	}
}

