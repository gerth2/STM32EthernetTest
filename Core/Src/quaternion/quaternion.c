

#include "quaternion.h"

// https://www.cprogramming.com/tutorial/3d/quaternions.html
void quat_mult(quaternion_t * result, quaternion_t * Q1, quaternion_t * Q2){

	quaternion_t tmp;

	tmp.w = Q1->w*Q2->w - Q1->x*Q2->x - Q1->y*Q2->y - Q1->z*Q2->z;
	tmp.x = Q1->w*Q2->w + Q1->x*Q2->x + Q1->y*Q2->y - Q1->z*Q2->z;
	tmp.y = Q1->w*Q2->w - Q1->x*Q2->x + Q1->y*Q2->y + Q1->z*Q2->z;
	tmp.z = Q1->w*Q2->w + Q1->x*Q2->x + Q1->y*Q2->y + Q1->z*Q2->z;

	quat_copy(result, &tmp);

}

void quat_scale(quaternion_t * result, quaternion_t * src, float scaleFactor){

	quaternion_t tmp;

	tmp.w = src->w * scaleFactor;
	tmp.x = src->x;
	tmp.y = src->y;
	tmp.z = src->z;

	quat_copy(result, &tmp);

}

void quat_fromGyro(quaternion_t * result, float gyroX, float gyroY, float gyroZ, float deltaT){
	float norm = sqrt(gyroX*gyroX + gyroY*gyroY + gyroZ*gyroZ);

	result->w = deltaT*norm;
	result->x = gyroX/norm;
	result->y = gyroY/norm;
	result->z = gyroZ/norm;
}

void quat_norm(quaternion_t * result, quaternion_t * Q1){
	float norm = sqrt(Q1->w*Q1->w + Q1->x*Q1->x + Q1->y*Q1->y + Q1->z*Q1->z);
	result->w = Q1->w/norm;
	result->x = Q1->x/norm;
	result->y = Q1->y/norm;
	result->z = Q1->z/norm;
}

void quat_inverse(quaternion_t * result, quaternion_t * Q1){
	quaternion_t tmp;

	quat_norm(&tmp, Q1);

	tmp.x *= -1.0;
	tmp.y *= -1.0;
	tmp.z *= -1.0;

	quat_copy(result, &tmp);
}


void quat_copy(quaternion_t * dst, quaternion_t * src){
	dst->w = src->w;
	dst->x = src->x;
	dst->y = src->y;
	dst->z = src->z;
}

// https://stackoverflow.com/questions/5782658/extracting-yaw-from-a-quaternion
float quat_getRoll(quaternion_t src){
	return atan2(2.0 * (src.z * src.y + src.w * src.x) , 1.0 - 2.0 * (src.x * src.x + src.y * src.y));
}

float quat_getPitch(quaternion_t src){
	return asin(2.0 * (src.y * src.w - src.z * src.x));
}

float quat_getYaw(quaternion_t src){
	return atan2(2.0 * (src.z * src.w + src.x * src.y) , - 1.0 + 2.0 * (src.w * src.w + src.x * src.x));
}
