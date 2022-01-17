#include "EtherShield.h"

#define NET_BUF_SIZE (1<<10)
#define UART_BUF_SIZE (NET_BUF_SIZE << 2)

#define MAX(a,b) ((a)>(b)?(a):(b))
#define BUF_SIZE MAX(NET_BUF_SIZE, UART_BUF_SIZE)


void NetworkingInit(SPI_HandleTypeDef * spiHandle);
void NetworkingPeriodic(void);



#ifndef PSTR
#define PSTR(str) (str)
#endif
