
#include "fusion.h"

// Calibration-corrected imu values
rotation2d_t calAdjGyroX;
rotation2d_t calAdjGyroY;
rotation2d_t calAdjGyroZ;
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
float compFilt_accel_trust_factor = 0.02;
float compFilt_min_accel = 0.7;
float compFilt_max_accel = 2.5;

float sampleTime = 0;
float prevSampleTime = -1;


void fusion_reset(){
	rot2d_fromDegrees(&yaw, 0.0);
	rot2d_fromDegrees(&roll, 0.0);
	rot2d_fromDegrees(&pitch, 0.0);
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
	rot2d_fromDegrees(&calAdjGyroX, cal_applyGyroX(mpu60x0_getXGyro()));
	rot2d_fromDegrees(&calAdjGyroY, cal_applyGyroY(mpu60x0_getYGyro()));
	rot2d_fromDegrees(&calAdjGyroZ, cal_applyGyroZ(mpu60x0_getZGyro()));
	calAdjAccelX = cal_applyAccelX(mpu60x0_getXAccel());
	calAdjAccelY = cal_applyAccelY(mpu60x0_getYAccel());
	calAdjAccelZ = cal_applyAccelZ(mpu60x0_getZAccel());

	//Complementary Filters
	if(prevSampleTime > 0){
		//Loop Time
		float deltaTime = (sampleTime - prevSampleTime);

		//Accumulate gyro
		rot2d_integrate(&pitch, &calAdjGyroX, deltaTime);
		rot2d_integrate(&roll, &calAdjGyroY, deltaTime);
		rot2d_integrate(&yaw, &calAdjGyroZ, deltaTime);

		//Pitch Complementary Filter
		float accelForPitch = sqrtf(calAdjAccelY*calAdjAccelY + calAdjAccelZ*calAdjAccelZ);
		pitchFusionActive = (accelForPitch > compFilt_min_accel && accelForPitch < compFilt_max_accel);
		if(pitchFusionActive){
			rotation2d_t accelRot;
			rot2d_fromComponents(&accelRot, calAdjAccelZ, calAdjAccelY);
			rot2d_scale(&accelRot, compFilt_accel_trust_factor);
			rot2d_scale(&pitch, (1.0 - compFilt_accel_trust_factor));
			rot2d_rotateBy(&pitch, &accelRot);
		}

		//Roll Complementary Filter
		float accelForRoll = sqrtf(calAdjAccelX*calAdjAccelX + calAdjAccelZ*calAdjAccelZ);
		rollFusionActive = (accelForRoll > compFilt_min_accel && accelForRoll < compFilt_max_accel);
		if(rollFusionActive){
			rotation2d_t accelRot;
			rot2d_fromComponents(&accelRot, calAdjAccelZ, calAdjAccelX);
			rot2d_scale(&accelRot, -1.0 * compFilt_accel_trust_factor);
			rot2d_scale(&roll, (1.0 - compFilt_accel_trust_factor));
			rot2d_rotateBy(&roll, &accelRot);		}

		//Yaw Complementary Filter
		float accelForYaw = sqrtf(calAdjAccelX*calAdjAccelX + calAdjAccelY*calAdjAccelY);
		yawFusionActive = (accelForYaw > compFilt_min_accel && accelForYaw < compFilt_max_accel);
		if(yawFusionActive){
			rotation2d_t accelRot;
			rot2d_fromComponents(&accelRot, calAdjAccelY, calAdjAccelX);
			rot2d_scale(&accelRot, -1.0 * compFilt_accel_trust_factor);
			rot2d_scale(&yaw, (1.0 - compFilt_accel_trust_factor));
			rot2d_rotateBy(&yaw, &accelRot);
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
	return rot2d_toDegrees(&calAdjGyroX);
}

float fusion_getYGyro(){
	return rot2d_toDegrees(&calAdjGyroY);
}

float fusion_getZGyro(){
	return rot2d_toDegrees(&calAdjGyroZ);
}

float fusion_getSampleTime(){
	return sampleTime;
}

float fusion_getPitch(void){
	return rot2d_toDegrees(&pitch);
}

float fusion_getRoll(void){
	return rot2d_toDegrees(&roll);
}

float fusion_getYaw(void){
	return rot2d_toDegrees(&yaw);
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
