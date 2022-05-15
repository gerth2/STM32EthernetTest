

#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "main.h"
#include "debugUtils.h"
#include "shutdown.h"
#include "socket.h"
#include <w5500.h>

#define debug(format, ...)     threadSafePrintf(format,##__VA_ARGS__)


void W5500_init(SPI_HandleTypeDef * sh_in);
