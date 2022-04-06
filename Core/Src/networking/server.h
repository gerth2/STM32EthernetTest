#ifndef SERVER_H
#define SERVER_H

#include <FreeRTOS.h>
#include <FreeRTOS_IP.h>
#include "FreeRTOS_IP_Private.h"
#include <math.h>

#include "mongoose.h"
#include "networking.h"
#include "timeManager.h"


/** Public API */
void serverInit(void);
void serverUpdate(void);
void serverShutdown(void);

#endif
