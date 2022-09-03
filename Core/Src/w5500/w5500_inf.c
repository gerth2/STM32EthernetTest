
#include <w5500_inf.h>
#include "wizchip_conf.h"

static SPI_HandleTypeDef * SpiHandle;


volatile bool ip_assigned = false;

void Callback_IPAssigned(void) {
    threadSafePrintf("Callback: IP assigned! \r\n");
    ip_assigned = true;
}

void Callback_IPConflict(void) {
	threadSafePrintf("Callback: IP conflict!\r\n");
}

void W5500_EnterCrit(void) {
    taskENTER_CRITICAL();
}

void W5500_ExitCrit(void) {
    taskEXIT_CRITICAL();
}

void W5500_Select(void) {
	HAL_GPIO_WritePin(SPI1_ETH_CS_GPIO_Port, SPI1_ETH_CS_Pin, GPIO_PIN_RESET);
}

void W5500_Unselect(void) {
	HAL_GPIO_WritePin(SPI1_ETH_CS_GPIO_Port, SPI1_ETH_CS_Pin, GPIO_PIN_SET);
}

void handleSPIErr(HAL_StatusTypeDef errStatus){
	switch (errStatus) {
		case HAL_OK:
			break;
		case HAL_BUSY:
			debug("PANIC: SPI: busy state occurred at %s, line %d .\n", __FILE__,
					__LINE__);
			shutdown_restartUnExpected();
			break;
		case HAL_TIMEOUT:
			debug("PANIC: SPI: transmit/receive timeout occurred at %s, line %d .\n",
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


void W5500_init(SPI_HandleTypeDef * sh_in) {
	int i;

	threadSafePrintf("[MAC] Starting Init...\n");

	SpiHandle = sh_in;

	reg_wizchip_cris_cbfunc(W5500_EnterCrit, W5500_ExitCrit);
    reg_wizchip_cs_cbfunc(W5500_Select, W5500_Unselect);
    reg_wizchip_spi_cbfunc(W5500_ReadByte, W5500_WriteByte);
    reg_wizchip_spiburst_cbfunc(W5500_ReadBuff, W5500_WriteBuff);


	//Reset chip hardware
	W5500_Unselect();
	for (i = 0; i < 720000; i++) {
		HAL_GPIO_WritePin(SPI1_ETH_RESET_GPIO_Port, SPI1_ETH_RESET_Pin, GPIO_PIN_RESET);
	}
	HAL_GPIO_WritePin(SPI1_ETH_RESET_GPIO_Port, SPI1_ETH_RESET_Pin, GPIO_PIN_SET);

	wizchip_sw_reset();
	wizchip_init(NULL, NULL); //TODO - default 2kb buffer size?

	uint8_t ver = getVERSIONR();
	if(ver != 0x04){
		threadSafePrintf("[MAC] Unexpected chip version %d!\n", ver);
		while(1);
	}

    wiz_NetInfo net_info = {
        .mac  = { 0xEA, 0x11, 0x22, 0x33, 0x44, 0xEA },
        .dhcp = NETINFO_STATIC,
		.ip   = {10, 111, 76, 123},
		.sn   = {255, 255, 255, 0},
		.gw   = {10, 111, 76, 1},
		.dns  = {1, 1, 1, 1},

    };

    // set MAC and IP config
    setSHAR(net_info.mac);
    wizchip_setnetinfo(&net_info);


	threadSafePrintf("[MAC] Init Completed!\n");

}
