#ifndef _QUATERNION_H
#define _QUATERNION_H

#include <math.h>

typedef struct quaternion {
	float w;
	float x;
	float y;
	float z;

} quaternion_t;

void quat_mult(quaternion_t * result, quaternion_t * Q1, quaternion_t * Q2);
void quat_fromGyro(quaternion_t * result, float gyroX, float gyroY, float gyroZ, float deltaT);
void quat_norm(quaternion_t * result, quaternion_t * Q1);
void quat_inverse(quaternion_t * result, quaternion_t * Q1);
void quat_copy(quaternion_t * dst, quaternion_t * src);
float quat_getRoll(quaternion_t src);
float quat_getPitch(quaternion_t src);
float quat_getYaw(quaternion_t src);

#endif
