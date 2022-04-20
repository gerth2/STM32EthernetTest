
#ifndef _RUNNING_CAL_H
#define _RUNNING_CAL_H

#include "circBuffer.h"
#include <math.h>

#define CAL_EXPECTED_GYRO_X 0
#define CAL_EXPECTED_GYRO_Y 0
#define CAL_EXPECTED_GYRO_Z 0
#define CAL_EXPECTED_ACCEL_X 0
#define CAL_EXPECTED_ACCEL_Y 0
#define CAL_EXPECTED_ACCEL_Z 1.0
#define CAL_EXPECTED_ACCEL_NORM 1.0

#define CAL_INIT_LOOPS 50

#define CAL_STATE_INACTIVE 0
#define CAL_STATE_ACTIVE 1

void calInit(void);
void calUpdate(float gyroX_raw, float gyroY_raw, float gyroZ_raw, float accelX_raw, float accelY_raw, float accelZ_raw);

float cal_applyGyroX(float raw);
float cal_applyGyroY(float raw);
float cal_applyGyroZ(float raw);
float cal_applyAccelX(float raw);
float cal_applyAccelY(float raw);
float cal_applyAccelZ(float raw);


#endif
