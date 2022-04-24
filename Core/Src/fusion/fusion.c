
#include "fusion.h"

// Calibration-corrected imu values
float calAdjGyroX = 0;
float calAdjGyroY = 0;
float calAdjGyroZ = 0;
float calAdjAccelX = 0;
float calAdjAccelY = 0;
float calAdjAccelZ = 0;

quaternion_t curOrientation;

//Fused pose estimates
float yaw = 0;
float roll = 0;
float pitch = 0;
bool yawFusionActive = false;
bool rollFusionActive = false;
bool pitchFusionActive = false;

bool accelFusionActive = false;

//Complimentary filter calibrations
float compFilt_accel_trust_factor = 0.02;
float compFilt_min_accel = 0.7;
float compFilt_max_accel = 2.5;

float sampleTime = 0;
float prevSampleTime = -1;


void fusion_reset(){
	yaw = 0;
	prevSampleTime = -1;
	calInit();

	curOrientation.w = 0;
	curOrientation.x = 0;
	curOrientation.y = 0;
	curOrientation.z = 1.0;
}

float wrapAngle (float in){
	if(in > 180.0){
		return in - 360;
	} else if (in < -180.0) {
		return in + 360.0;
	} else {
		return in;
	}
}

void fusion_update(){

	sampleTime = mpu60x0_getSampleTime();

	calUpdate(mpu60x0_getXGyro(), mpu60x0_getYGyro(), mpu60x0_getZGyro(), mpu60x0_getXAccel(), mpu60x0_getYAccel(), mpu60x0_getZAccel());

	//Apply sensor calibration to readings
	calAdjGyroX = cal_applyGyroX(mpu60x0_getXGyro());
	calAdjGyroY = cal_applyGyroY(mpu60x0_getYGyro());
	calAdjGyroZ = cal_applyGyroZ(mpu60x0_getZGyro());
	calAdjAccelX = cal_applyAccelX(mpu60x0_getXAccel());
	calAdjAccelY = cal_applyAccelY(mpu60x0_getYAccel());
	calAdjAccelZ = cal_applyAccelZ(mpu60x0_getZAccel());


	//Complementary Filters
	if(prevSampleTime > 0){
		//Loop Time
		float deltaTime = (sampleTime - prevSampleTime);

		//Accumulate gyro
		quaternion_t gyroQuat;
		quat_fromGyro(&gyroQuat, calAdjGyroX, calAdjGyroY, calAdjGyroZ, deltaTime);
		quat_mult(&curOrientation, &curOrientation, &gyroQuat);

		double accelMag =  sqrt(calAdjAccelX*calAdjAccelX  + calAdjAccelY*calAdjAccelY + calAdjAccelZ*calAdjAccelZ);
		accelFusionActive = (accelMag > compFilt_min_accel && accelMag < compFilt_max_accel);
		if(accelFusionActive){
			quaternion_t gyroCompTemp;
			quaternion_t accelCompTemp;

			accelCompTemp.w = 0.0;
			accelCompTemp.x = calAdjAccelX;
			accelCompTemp.y = calAdjAccelY;
			accelCompTemp.z = calAdjAccelZ;

			quat_norm(&accelCompTemp, &accelCompTemp);
			quat_scale(&accelCompTemp, &accelCompTemp, compFilt_accel_trust_factor);

			quat_copy(&gyroCompTemp, &curOrientation);
			quat_scale(&gyroCompTemp, &gyroCompTemp, (1.0 - compFilt_accel_trust_factor));

			quat_mult(&curOrientation, &gyroCompTemp, &accelCompTemp);
		}

		pitch = quat_getPitch(curOrientation) * 180/M_PI;
		roll = quat_getRoll(curOrientation) * 180/M_PI;
		yaw = quat_getYaw(curOrientation) * 180/M_PI;


	} else {
		//skip - first sample, need at least two samples for delta time
	}

	prevSampleTime = sampleTime;

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

float fusion_getPitch(void){
	return pitch;
}

float fusion_getRoll(void){
	return roll;
}

float fusion_getYaw(void){
	return yaw;
}

bool fusion_getYawFusionActive(void){
	return yawFusionActive;
}
bool fusion_getPitchFusionActive(void){
	return pitchFusionActive;
}
bool fusion_getRollFusionActive(void){
	return rollFusionActive;
}
