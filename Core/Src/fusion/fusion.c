
#include "fusion.h"

// Calibration-corrected imu values
float calAdjGyroX;
float calAdjGyroY;
float calAdjGyroZ;
float calAdjAccelX = 0;
float calAdjAccelY = 0;
float calAdjAccelZ = 0;

//Fused pose estimates
rotation2d_t yaw;
rotation2d_t roll;
rotation2d_t pitch;
bool yawFusionActive = false;
bool rollFusionActive = false;
bool pitchFusionActive = false;

//Complimentary filter calibrations
float compFilt_accel_trust_factor = 0.2;
float compFilt_min_accel = 0.9;
float compFilt_max_accel = 1.25;

float sampleTime = 0;
float prevSampleTime = -1;


void fusion_reset(){
	rot2d_fromDegrees(&yaw, 0.0);
	rot2d_fromDegrees(&roll, 0.0);
	rot2d_fromDegrees(&pitch, 0.0);
	prevSampleTime = -1;
	calInit();
	mahony_init();
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

		mahony_update(calAdjAccelX, calAdjAccelY, calAdjAccelZ, calAdjGyroX, calAdjGyroY, calAdjGyroZ, deltaTime);

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
	return mahoney_getPitch();
}

float fusion_getRoll(void){
	return mahoney_getRoll();
}

float fusion_getYaw(void){
	return mahoney_getYaw();
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
