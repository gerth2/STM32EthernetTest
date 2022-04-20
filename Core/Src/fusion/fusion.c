
#include "fusion.h"

// Calibration-corrected imu values
float calAdjGyroX = 0;
float calAdjGyroY = 0;
float calAdjGyroZ = 0;
float calAdjAccelX = 0;
float calAdjAccelY = 0;
float calAdjAccelZ = 0;

//Fused pose estimates
float yaw = 0;
float roll = 0;
float pitch = 0;

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
		pitch += calAdjGyroX * deltaTime;
		roll += calAdjGyroY * deltaTime;
		yaw += calAdjGyroZ * deltaTime;

		//Wrap Angles
		pitch = wrapAngle(pitch);
		roll = wrapAngle(roll);
		yaw = wrapAngle(yaw);

		//Pitch Complementary Filter
		double accelForPitch = sqrt(calAdjAccelY*calAdjAccelY + calAdjAccelZ*calAdjAccelZ);
		if(accelForPitch > compFilt_min_accel && accelForPitch < compFilt_max_accel){
			pitch = pitch * (1.0 - compFilt_accel_trust_factor) + atan2(calAdjAccelY, calAdjAccelZ) * 180/3.14159 * compFilt_accel_trust_factor;
		}

		//Roll Complementary Filter
		double accelForRoll = sqrt(calAdjAccelX*calAdjAccelX + calAdjAccelZ*calAdjAccelZ);
		if(accelForRoll > compFilt_min_accel && accelForRoll < compFilt_max_accel){
			roll = roll * (1.0 - compFilt_accel_trust_factor) + atan2(calAdjAccelX, calAdjAccelZ) * 180/3.14159 * compFilt_accel_trust_factor;
		}

		//Yaw Complementary Filter
		double accelForYaw = sqrt(calAdjAccelX*calAdjAccelX + calAdjAccelY*calAdjAccelY);
		if(accelForYaw > compFilt_min_accel && accelForYaw < compFilt_max_accel){
			yaw = yaw * (1.0 - compFilt_accel_trust_factor) + atan2(calAdjAccelX, calAdjAccelY) * 180/3.14159 * compFilt_accel_trust_factor;
		}


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

