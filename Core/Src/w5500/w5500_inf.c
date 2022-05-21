
#include <w5500_inf.h>
#include "wizchip_conf.h"

static SPI_HandleTypeDef * SpiHandle;


volatile bool ip_assigned = false;

void Callback_IPAssigned(void) {
    UART_Printf("Callback: IP assigned! Leased time: %d sec\r\n", getDHCPLeasetime());
    ip_assigned = true;
}

void Callback_IPConflict(void) {
    UART_Printf("Callback: IP conflict!\r\n");
}

void W5500_Select(void) {
	wizchip_cs_select();
}

void W5500_Unselect(void) {
	wizchip_cs_deselect();
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
