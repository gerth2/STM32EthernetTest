
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


// Private function declarations
static void checkStatus(HAL_StatusTypeDef status);
static void mpu60x0_checkWhoami(void);



void mpu60x0_init(I2C_HandleTypeDef i2cHandle){

	printf("[IMU] Starting Init...\n");

	imu_i2c = i2cHandle;

	mpu60x0_checkWhoami();

	if(deviceAvailable){
		printf("[IMU] Init Complete\n");
	} else {
		printf("[IMU] Init FAIL\n");
	}
	return;

}

void mpu60x0_update(){

}


float mpu60x0_getXAccel(){
	return 0.0; //todo
}

static void mpu60x0_checkWhoami(){

	uint8_t check;
	uint8_t Data;

	// check device ID WHO_AM_I

	checkStatus(HAL_I2C_Mem_Read (&imu_i2c, MPU6050_ADDR,WHO_AM_I_REG,1, &check, 1, 1000));

	if (check == BITS_WHOAMI_EXP)  // 0x68 will be returned by the sensor if everything goes well
	{
		deviceAvailable = 1;

		// power management register 0X6B we should write all 0's to wake the sensor up
		Data = 0;
		checkStatus(HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, PWR_MGMT_1_REG, 1,&Data, 1, 1000));

		// Set DATA RATE of 1KHz by writing SMPLRT_DIV register
		Data = 0x07;
		checkStatus(HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, SMPLRT_DIV_REG, 1, &Data, 1, 1000));

		// Set accelerometer configuration in ACCEL_CONFIG Register
		// XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> � 2g
		Data = 0x00;
		checkStatus(HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, ACCEL_CONFIG_REG, 1, &Data, 1, 1000));

		// Set Gyroscopic configuration in GYRO_CONFIG Register
		// XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> � 250 �/s
		Data = 0x00;
		checkStatus(HAL_I2C_Mem_Write(&imu_i2c, MPU6050_ADDR, GYRO_CONFIG_REG, 1, &Data, 1, 1000));
	} else {
		printf("[IMU] unexpected WHOAMI response of %d\n", check);

	}

}


static void checkStatus(HAL_StatusTypeDef status) {
	switch (status) {
	case HAL_OK:
		break;
	case HAL_BUSY:
		printf("[IMU] PANIC: I2C: busy state occured at %s, line %d .\n", __FILE__,
				__LINE__);
		while (1)
			;
		break;
	case HAL_TIMEOUT:
		printf("[IMU] PANIC: I2C: transmit/receive timeout occured at %s, line %d .\n",
				__FILE__, __LINE__);
		while (1)
			;
		break;
	case HAL_ERROR:
		printf("[IMU] PANIC: I2C: transmit/receive error at %s, line %d.\n", __FILE__,
				__LINE__);
		while (1)
			;
		break;
	default:
		printf("[IMU] PANIC: I2C: transmit/receive WUT HAPPEND at %s, line %d.\n",
				__FILE__, __LINE__);
		while (1)
			;
		break;
	}
}

