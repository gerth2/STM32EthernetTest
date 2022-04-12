#ifndef FUSION_H
#define FUSION_H

#include "mpu60x0.h"
#include <stdint.h>
#include "timeManager.h"

void fusion_reset();

void fusion_update(void);

void fusion_startCalibration(void);

uint8_t fusion_calIsActive(void);

float fusion_getYaw(void);


#endif
