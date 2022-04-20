#ifndef FUSION_H
#define FUSION_H

#include "mpu60x0.h"
#include <stdint.h>
#include "timeManager.h"
#include "math.h"
#include "runningCal.h"

void fusion_reset();

void fusion_update(void);

float fusion_getYaw(void);
float fusion_getPitch(void);
float fusion_getRoll(void);

float fusion_getXAccel();
float fusion_getYAccel();
float fusion_getZAccel();
float fusion_getXGyro();
float fusion_getYGyro();
float fusion_getZGyro();
float fusion_getSampleTime();



#endif
