

#include "timeManager.h"

double curTime_sec;

uint16_t prevTimerVal;


void timeInit(){

	  // Start timer from
	  curTime_sec = 0;
	  prevTimerVal = 0;
	  HAL_TIM_Base_Start(&htim11);

}

double getCurTime(){

	taskENTER_CRITICAL();
	uint16_t curTimerVal = __HAL_TIM_GET_COUNTER(&htim11);

	double deltaTime_sec = 0.0;

	if(curTimerVal > prevTimerVal){
		//normal case
		deltaTime_sec = ((double)(curTimerVal - prevTimerVal)) * SEC_PER_TIMER_TICK;
	} else {
		//Timer rolled over
		deltaTime_sec = ((double)(65535 - prevTimerVal + curTimerVal + 1)) * SEC_PER_TIMER_TICK;
	}

	curTime_sec += deltaTime_sec;

	prevTimerVal = curTimerVal;
	taskEXIT_CRITICAL();
	return curTime_sec;

}
