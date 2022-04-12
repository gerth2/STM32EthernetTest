
#include "fusion.h"

//Cal result biases for transforming from raw sensor to calibrated sensor
float gyroXBias = 0;
float gyroYBias = 0;
float gyroZBias = 0;

float accelXBias = 0;
float accelYBias = 0;
float accelZBias = 0;
float accelNormalize = 1.0;

// Calibration-corrected imu values
float calAdjGyroX = 0;
float calAdjGyroY = 0;
float calAdjGyroZ = 0;
float calAdjAccelX = 0;
float calAdjAccelY = 0;
float calAdjAccelZ = 0;

//Fused pose estimates
float yaw = 0;


//Calibration infrastructure
#define CAL_SAMPLES 500.0
uint16_t calStep = 0;

float calGyroXAccum = 0;
float calGyroYAccum = 0;
float calGyroZAccum = 0;

float calAccelXAccum = 0;
float calAccelYAccum = 0;
float calAccelZAccum = 0;

float sampleTime = 0;

float prevSampleTime = -1;

#define CAL_EXPECTED_GYRO_X 0
#define CAL_EXPECTED_GYRO_Y 0
#define CAL_EXPECTED_GYRO_Z 0
#define CAL_EXPECTED_ACCEL_X 0
#define CAL_EXPECTED_ACCEL_Y 0
#define CAL_EXPECTED_ACCEL_Z 1.0
#define CAL_EXPECTED_ACCEL_NORM 1.0

void fusion_reset(){
	yaw = 0;
	prevSampleTime = -1;
}


void fusion_update(){

	sampleTime = mpu60x0_getSampleTime();

	if(fusion_calIsActive()){
		//calibration mode
		calStep++;
		calGyroXAccum += mpu60x0_getXGyro()/ CAL_SAMPLES;
		calGyroYAccum += mpu60x0_getYGyro()/ CAL_SAMPLES;
		calGyroZAccum += mpu60x0_getZGyro()/ CAL_SAMPLES;

		calAccelXAccum += mpu60x0_getXAccel()/ CAL_SAMPLES;
		calAccelYAccum += mpu60x0_getYAccel()/ CAL_SAMPLES;
		calAccelZAccum += mpu60x0_getZAccel()/ CAL_SAMPLES;


		if(!fusion_calIsActive()){
			//Transition out of active to inactive
			fusion_reset();
			gyroXBias = CAL_EXPECTED_GYRO_X - calGyroXAccum;
			gyroYBias = CAL_EXPECTED_GYRO_Y - calGyroYAccum;
			gyroZBias = CAL_EXPECTED_GYRO_Z - calGyroZAccum;

			accelXBias = CAL_EXPECTED_ACCEL_X - calAccelXAccum;
			accelYBias = CAL_EXPECTED_ACCEL_Y - calAccelYAccum;
			accelZBias = CAL_EXPECTED_ACCEL_Z - calAccelZAccum;
			accelNormalize = CAL_EXPECTED_ACCEL_NORM / (float) sqrt(calAccelXAccum*calAccelXAccum +
																	calAccelYAccum*calAccelYAccum +
																	calAccelZAccum*calAccelZAccum);
		}

	} else {
		//Normal mode

		if(prevSampleTime > 0){

			//Correct Gyro Readings
			float deltaTime = (sampleTime - prevSampleTime);
			calAdjGyroX = (mpu60x0_getXGyro() + gyroXBias);
			calAdjGyroY = (mpu60x0_getYGyro() + gyroYBias);
			calAdjGyroZ = (mpu60x0_getZGyro() + gyroZBias);
			calAdjAccelX = (mpu60x0_getXAccel() + accelXBias) * accelNormalize;
			calAdjAccelY = (mpu60x0_getYAccel() + accelYBias) * accelNormalize;
			calAdjAccelZ = (mpu60x0_getZAccel() + accelZBias) * accelNormalize;

			//Fuse
			yaw += calAdjGyroZ * deltaTime;;

		} else {
			//skip - first sample, need at least two samples for delta time
		}

		prevSampleTime = sampleTime;
	}

}

float fusion_getXAccel(){
	return calAdjAccelX;
}

float fusion_getYAccel(){
	return calAdjAccelY;
}

float fusion_getZAccel(){
	return calAdjAccelZ;
}

float fusion_getXGyro(){
	return calAdjGyroX;
}

float fusion_getYGyro(){
	return calAdjGyroY;
}

float fusion_getZGyro(){
	return calAdjGyroZ;
}

float fusion_getSampleTime(){
	return sampleTime;
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

