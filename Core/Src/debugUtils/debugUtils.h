#ifndef _DEBUG_UTILS_H
#define _DEBUG_UTILS_H


#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx_hal.h"
#include <string.h>



int threadSafeSPrintf(char * buf,const char *fmt, ...);
int threadSafePrintf(const char *fmt, ...);
void RetargetInit(UART_HandleTypeDef *huart);


#endif
