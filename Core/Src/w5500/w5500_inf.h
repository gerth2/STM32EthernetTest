

#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_IP_Private.h"
#include "main.h"
#include "debugUtils.h"
#include "shutdown.h"
#include "wn_socket.h"
#include <w5500.h>

#define debug(format, ...)     threadSafePrintf(format,##__VA_ARGS__)


void W5500_init(SPI_HandleTypeDef sh_in);
