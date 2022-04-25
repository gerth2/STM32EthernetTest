#include "mahony.h"

// GLOBALLY DECLARED, required for Mahony filter
// vector to hold quaternion
quaternion_t curPoseEst;

// Free parameters in the Mahony filter and fusion scheme,
// Kp for proportional feedback, Ki for integral
float Kp = 1.0;
float Ki = 0.0;

void mahony_init(){
	curPoseEst.w = 1.0;
	curPoseEst.x = 0.0;
	curPoseEst.y = 0.0;
	curPoseEst.z = 0.0;
}

//See also - https://nitinjsanket.github.io/tutorials/attitudeest/mahony.html

//--------------------------------------------------------------------------------------------------
// Mahony scheme uses proportional and integral filtering on
// the error between estimated reference vector (gravity) and measured one.
// Madgwick's implementation of Mayhony's AHRS algorithm.
// See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
//
// Date      Author      Notes
// 29/09/2011 SOH Madgwick    Initial release
// 02/10/2011 SOH Madgwick  Optimised for reduced CPU load
// last update 07/09/2020 SJR minor edits
//--------------------------------------------------------------------------------------------------
// IMU algorithm update

void mahony_update(float ax, float ay, float az, float gx, float gy, float gz, float deltat) {
  float recipNorm;
  float vx, vy, vz;
  float ex, ey, ez;  //error terms
  float qa, qb, qc;
  static float ix = 0.0, iy = 0.0, iz = 0.0;  //integral feedback terms
  float tmp;

  // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
  tmp = ax * ax + ay * ay + az * az;
  if (tmp > 0.0)
  {

    // Estimated direction of gravity in the body frame (factor of two divided out)
    vx = curPoseEst.x * curPoseEst.z - curPoseEst.w * curPoseEst.y;
    vy = curPoseEst.w * curPoseEst.x + curPoseEst.y * curPoseEst.z;
    vz = curPoseEst.w * curPoseEst.w - 0.5f + curPoseEst.z * curPoseEst.z;

    // Error is cross product between estimated and measured direction of gravity in body frame
    // (half the actual magnitude)
    ex = (ay * vz - az * vy);
    ey = (az * vx - ax * vz);
    ez = (ax * vy - ay * vx);

    // Compute and apply to gyro term the integral feedback, if enabled
    if (Ki > 0.0f) {
      ix += Ki * ex * deltat;  // integral error scaled by Ki
      iy += Ki * ey * deltat;
      iz += Ki * ez * deltat;
      gx += ix;  // apply integral feedback
      gy += iy;
      gz += iz;
    }

    // Apply proportional feedback to gyro term
    gx += Kp * ex;
    gy += Kp * ey;
    gz += Kp * ez;
  }

  // Integrate rate of change of quaternion, q cross gyro term
  deltat = 0.5 * deltat;
  gx *= deltat;   // pre-multiply common factors
  gy *= deltat;
  gz *= deltat;
  qa = curPoseEst.w;
  qb = curPoseEst.x;
  qc = curPoseEst.y;
  curPoseEst.w += (-qb * gx - qc * gy - curPoseEst.z * gz);
  curPoseEst.x += (qa * gx + qc * gz - curPoseEst.z * gy);
  curPoseEst.y += (qa * gy - qb * gz + curPoseEst.z * gx);
  curPoseEst.z += (qa * gz + qb * gy - qc * gx);

  // renormalise quaternion
  recipNorm = 1.0 / sqrtf(curPoseEst.w * curPoseEst.w + curPoseEst.x * curPoseEst.x + curPoseEst.y * curPoseEst.y + curPoseEst.z * curPoseEst.z);
  curPoseEst.w = curPoseEst.w * recipNorm;
  curPoseEst.x = curPoseEst.x * recipNorm;
  curPoseEst.y = curPoseEst.y * recipNorm;
  curPoseEst.z = curPoseEst.z * recipNorm;
}

float mahoney_getPitch(void){
	  return asinf(2.0 * (curPoseEst.w * curPoseEst.y - curPoseEst.x * curPoseEst.z)) * 180.0 / M_PI;
}
float mahoney_getRoll(void){
	return atan2f((curPoseEst.w * curPoseEst.x + curPoseEst.y * curPoseEst.z), 0.5 - (curPoseEst.x * curPoseEst.x + curPoseEst.y * curPoseEst.y)) * 180.0 / M_PI;
}
float mahoney_getYaw(void){
	  return -atan2f((curPoseEst.x * curPoseEst.y + curPoseEst.w * curPoseEst.z), 0.5 - (curPoseEst.y * curPoseEst.y + curPoseEst.z * curPoseEst.z)) * 180.0 / M_PI;
}
