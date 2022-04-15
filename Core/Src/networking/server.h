#ifndef SERVER_H
#define SERVER_H

#include <FreeRTOS.h>
#include <FreeRTOS_IP.h>
#include "FreeRTOS_IP_Private.h"
#include <math.h>
#include "debugUtils.h"

#include "mongoose.h"
#include "networking.h"
#include "mdns.h"
#include "timeManager.h"
#include "stm32f4xx_hal.h"
#include "mpu60x0.h"
#include "fusion.h"
#include "perfMon.h"

/** Public API */
void serverInit(void);
void serverUpdate(void);
void serverShutdown(void);

#endif
