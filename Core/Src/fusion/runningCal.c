#include "runningCal.h"

circularBuffer gyroXCalBuffer;
circularBuffer gyroYCalBuffer;
circularBuffer gyroZCalBuffer;

circularBuffer accelNormCalBuffer;

unsigned long calLoops;

unsigned char calState = CAL_STATE_INACTIVE;

unsigned char calDbnc;

void calInit(){
	circBuffer_init(&gyroXCalBuffer, CAL_INIT_LOOPS, 0.0);
	circBuffer_init(&gyroYCalBuffer, CAL_INIT_LOOPS, 0.0);
	circBuffer_init(&gyroZCalBuffer, CAL_INIT_LOOPS, 0.0);
	circBuffer_init(&accelNormCalBuffer, CAL_INIT_LOOPS, 0.0);
}

void calUpdate(float gyroX_raw, float gyroY_raw, float gyroZ_raw, float accelX_raw, float accelY_raw, float accelZ_raw){

	calState = CAL_STATE_INACTIVE;


	if(calLoops < CAL_INIT_LOOPS){
		//Init/unknown sensor. Accumulate all buffers, assuming device is still.
		circBuffer_add(&gyroXCalBuffer, gyroX_raw);
		circBuffer_add(&gyroYCalBuffer, gyroY_raw);
		circBuffer_add(&gyroZCalBuffer, gyroZ_raw);
		float accelNormalize = 1.0 / (float) sqrt(accelX_raw*accelX_raw +
												accelY_raw*accelY_raw +
												accelZ_raw*accelZ_raw);
		circBuffer_add(&accelNormCalBuffer, accelNormalize);

		calState = CAL_STATE_ACTIVE;

	} else {
		//Normal rolling calibration mode.
		float accelX = cal_applyAccelX(accelX_raw);
		float accelY = cal_applyAccelX(accelY_raw);
		float accelZ = cal_applyAccelX(accelZ_raw);

		//Basic algorithm - if only gravity is impacting us, for a one-sided debounce,
		// we can assume the unit is still and able to be calibrated.
		float accelMag = (float) sqrt(accelX*accelX + accelY*accelY + accelZ*accelZ);
		if(accelMag > 0.9 && accelMag < 1.1){
			if(calDbnc > 0){
				calDbnc--;
			}
		} else {
			calDbnc = 30;
		}

		if(calDbnc == 0){
			//debounce expired, go ahead and calibrate
			circBuffer_add(&gyroXCalBuffer, gyroX_raw);
			circBuffer_add(&gyroYCalBuffer, gyroY_raw);
			circBuffer_add(&gyroZCalBuffer, gyroZ_raw);
			calState = CAL_STATE_ACTIVE;
		}
	}

	if(calState == CAL_STATE_ACTIVE){
		calLoops++;
	}

}


float cal_applyGyroX(float raw){
	return raw - circBuffer_getAvg(&gyroXCalBuffer);
}

float cal_applyGyroY(float raw){
	return raw - circBuffer_getAvg(&gyroYCalBuffer);
}

float cal_applyGyroZ(float raw){
	return raw - circBuffer_getAvg(&gyroZCalBuffer);
}

float cal_applyAccelX(float raw){
	return (raw) * circBuffer_getAvg(&accelNormCalBuffer);
}

float cal_applyAccelY(float raw){
	return (raw) * circBuffer_getAvg(&accelNormCalBuffer);
}

float cal_applyAccelZ(float raw){
	return (raw ) * circBuffer_getAvg(&accelNormCalBuffer);
}
