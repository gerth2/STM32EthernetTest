#ifndef __TIME_MANAGER_H
#define __TIME_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif


#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"

#define SEC_PER_TIMER_TICK 0.0001f //prescaler set such that timer is 100 us per bit

extern TIM_HandleTypeDef htim11;

void timeInit(void);
double getCurTime(void);


#ifdef __cplusplus
}
#endif

#endif /* __TIME_MANAGER_H */
