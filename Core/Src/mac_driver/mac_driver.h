#ifndef NETWORKING_H
#define NETWORKING_H

#define NET_BUF_SIZE (1<<10)
#define UART_BUF_SIZE (NET_BUF_SIZE << 2)

#define BUF_SIZE MAX(NET_BUF_SIZE, UART_BUF_SIZE)

#include <stdint.h>
#include <string.h>

#include "main.h"
#include "FreeRTOS.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_IP_Private.h"
#include "NetworkBufferManagement.h"
#include "server.h"
#include "settings.h"
#include "debugUtils.h"
#include "enc28j60.h"
#include "statusLED.h"

void MAC_Init();

#define SERVER_NOCHANGE 0
#define SERVER_INIT 1
#define SERVER_SHUTDOWN 2
extern uint8_t serverStatus;


#ifndef PSTR
#define PSTR(str) (str)
#endif

#endif
