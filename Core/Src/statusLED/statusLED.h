
#include "stm32f4xx_hal.h"
#include "main.h"

void statusLED_init();
void statusLED_periodic();

void statusLED_setNetworkUp(uint8_t in);
void statusLED_clientConnected(uint8_t in);
