#ifndef FUSION_H
#define FUSION_H

#include <stdint.h>
#include <math.h>
#include <stdbool.h>

#include "mpu60x0.h"
#include "timeManager.h"
#include "runningCal.h"

void fusion_reset();

void fusion_update(void);

float fusion_getYaw(void);
float fusion_getPitch(void);
float fusion_getRoll(void);

bool fusion_getYawFusionActive(void);
bool fusion_getPitchFusionActive(void);
bool fusion_getRollFusionActive(void);

float fusion_getXAccel();
float fusion_getYAccel();
float fusion_getZAccel();
float fusion_getXGyro();
float fusion_getYGyro();
float fusion_getZGyro();
float fusion_getSampleTime();



#endif
