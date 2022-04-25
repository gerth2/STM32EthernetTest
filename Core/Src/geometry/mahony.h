#ifndef _MAHONY_H
#define _MAHONY_H

#include <math.h>
#include "quaternion.h"

void mahony_init(void);
void mahony_update(float ax, float ay, float az, float gx, float gy, float gz, float deltat);

float mahoney_getPitch(void);
float mahoney_getRoll(void);
float mahoney_getYaw(void);

#endif
