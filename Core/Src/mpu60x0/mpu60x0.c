
#include "mpu60x0.h"

static SPI_HandleTypeDef imu_spi;

static uint8_t deviceAvailable = 0;


// Private function declarations
static void checkStatus(HAL_StatusTypeDef status);
static uint8_t readSingleByte(uint8_t addr);
static void mpu60x0_checkWhoami(void);



void mpu60x0_init(SPI_HandleTypeDef spiHandle){

	printf("[IMU] Starting Init...\n");

	imu_spi = spiHandle;

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
	uint8_t response;

	response = readSingleByte(MPU6000_WHOAMI);
	if(response == BITS_WHOAMI_EXP){
		deviceAvailable = 1;
	} else {
		printf("[IMU] unexpected WHOAMI response of %d\n", response);
		return;
	}
}

static uint8_t readSingleByte(uint8_t addr){
	uint8_t sendBuf;
	uint8_t recvBuf;

	sendBuf = addr | 0x80;

	taskENTER_CRITICAL();
	checkStatus(HAL_SPI_Transmit(&imu_spi, &sendBuf, 1, 100));
	checkStatus(HAL_SPI_Receive(&imu_spi, &recvBuf, 1, 100));
	taskEXIT_CRITICAL();

	return recvBuf;
}


static void checkStatus(HAL_StatusTypeDef status) {
	switch (status) {
	case HAL_OK:
		break;
	case HAL_BUSY:
		printf("[IMU] PANIC: SPI: busy state occured at %s, line %d .\n", __FILE__,
				__LINE__);
		while (1)
			;
		break;
	case HAL_TIMEOUT:
		printf("[IMU] PANIC: SPI: transmit/receive timeout occured at %s, line %d .\n",
				__FILE__, __LINE__);
		while (1)
			;
		break;
	case HAL_ERROR:
		printf("[IMU] PANIC: SPI: transmit/receive error at %s, line %d.\n", __FILE__,
				__LINE__);
		while (1)
			;
		break;
	default:
		printf("[IMU] PANIC: SPI: transmit/receive WUT HAPPEND at %s, line %d.\n",
				__FILE__, __LINE__);
		while (1)
			;
		break;
	}
}

