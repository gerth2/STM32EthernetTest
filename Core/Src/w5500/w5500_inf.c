
#include <w5500_inf.h>

static SPI_HandleTypeDef * SpiHandle;


void W5500_Select(void) {
	HAL_GPIO_WritePin(SPI1_ETH_CS_GPIO_Port, SPI1_ETH_CS_Pin, GPIO_PIN_RESET);
}

void W5500_Unselect(void) {
	HAL_GPIO_WritePin(SPI1_ETH_CS_GPIO_Port, SPI1_ETH_CS_Pin, GPIO_PIN_SET);
}

void W5500_init(SPI_HandleTypeDef * sh_in) {
	int i;

	threadSafePrintf("[MAC] Starting Init...\n");

	SpiHandle = sh_in;

	//Reset chip hardware
	W5500_Unselect();
	for (i = 0; i < 720000; i++) {
		HAL_GPIO_WritePin(SPI1_ETH_RESET_GPIO_Port, SPI1_ETH_RESET_Pin, GPIO_PIN_RESET);
	}
	HAL_GPIO_WritePin(SPI1_ETH_RESET_GPIO_Port, SPI1_ETH_RESET_Pin, GPIO_PIN_SET);

	//Todo - init chip hardware?

	threadSafePrintf("[MAC] Init Completed!\n");

}


void handleSPIErr(HAL_StatusTypeDef errStatus){
	switch (errStatus) {
		case HAL_OK:
			break;
		case HAL_BUSY:
			debug("PANIC: SPI: busy state occured at %s, line %d .\n", __FILE__,
					__LINE__);
			shutdown_restartUnExpected();
			break;
		case HAL_TIMEOUT:
			debug("PANIC: SPI: transmit/receive timeout occured at %s, line %d .\n",
					__FILE__, __LINE__);
			shutdown_restartUnExpected();
			break;
		case HAL_ERROR:
			debug("PANIC: SPI: transmit/receive error at %s, line %d.\n", __FILE__,
					__LINE__);
			shutdown_restartUnExpected();
			break;
		default:
			debug("PANIC: SPI: transmit/receive WUT HAPPEND at %s, line %d.\n",
					__FILE__, __LINE__);
			shutdown_restartUnExpected();
			break;
	}
}

void W5500_ReadBuff(uint8_t* buff, uint16_t len) {
	handleSPIErr(HAL_SPI_Receive(SpiHandle, buff, len, 100));
	return;
}

void W5500_WriteBuff(uint8_t* buff, uint16_t len) {
    handleSPIErr(HAL_SPI_Transmit(SpiHandle, buff, len, 100));
    return;
}

uint8_t W5500_ReadByte(void) {
    uint8_t byte;
    W5500_ReadBuff(&byte, sizeof(byte));
    return byte;
}

void W5500_WriteByte(uint8_t byte) {
    W5500_WriteBuff(&byte, sizeof(byte));
}
