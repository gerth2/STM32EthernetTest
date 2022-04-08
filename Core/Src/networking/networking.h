#ifndef NETWORKING_H
#define NETWORKING_H

#include "stm32f4xx_hal.h"

#define NET_BUF_SIZE (1<<10)
#define UART_BUF_SIZE (NET_BUF_SIZE << 2)

#define BUF_SIZE MAX(NET_BUF_SIZE, UART_BUF_SIZE)


void NetworkingInit(SPI_HandleTypeDef * spiHandle);
void NetworkingPeriodic(void);

#define SERVER_NOCHANGE 0
#define SERVER_INIT 1
#define SERVER_SHUTDOWN 2
extern uint8_t serverStatus;


#ifndef PSTR
#define PSTR(str) (str)
#endif

#endif
