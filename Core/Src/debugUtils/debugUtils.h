#ifndef _DEBUG_UTILS_H
#define _DEBUG_UTILS_H


#include <stdarg.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx_hal.h"


void threadSafePrintf(const char *fmt, ...);
void RetargetInit(UART_HandleTypeDef *huart);


#endif
