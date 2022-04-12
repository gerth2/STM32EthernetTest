
#include "fusion.h"

float gyroXBias = 0;
float gyroYBias = 0;
float gyroZBias = 0;

float yaw = 0;

#define CAL_SAMPLES 500.0
uint16_t calStep = 0;
float calAccum = 0;

float prevSampleTime = -1;

void fusion_reset(){
	yaw = 0;
	prevSampleTime = -1;
}


void fusion_update(){

	if(fusion_calIsActive()){
		//calibration mode
		calStep++;
		calAccum += mpu60x0_getZGyro();


		if(!fusion_calIsActive()){
			//Transition out of active to inactive
			fusion_reset();
			gyroZBias = calAccum / CAL_SAMPLES;
		}

	} else {
		//Normal mode

		double sampleTime = mpu60x0_getSampleTime();

		if(prevSampleTime > 0){
			yaw += (mpu60x0_getZGyro() - gyroZBias) * (sampleTime - prevSampleTime);
		} else {
			//skip - first sample, need at least two samples for delta time
		}

		prevSampleTime = sampleTime;
	}

}

void fusion_startCalibration(){
	calStep = 0;
}

uint8_t fusion_calIsActive(){
	return (calStep < CAL_SAMPLES);
}

float fusion_getYaw(void){
	return yaw;
}

