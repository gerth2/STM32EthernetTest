#ifndef SERVER_H
#define SERVER_H

#include <FreeRTOS.h>
#include <FreeRTOS_IP.h>

#include "mongoose.h"
#include "networking.h"


/** Public API */
void serverInit(void);
void serverUpdate(void);
void serverShutdown(void);

#endif
