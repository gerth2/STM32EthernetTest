#ifndef SERVER_H
#define SERVER_H

#include <FreeRTOS.h>
#include <FreeRTOS_IP.h>
#include "FreeRTOS_IP_Private.h"
#include <math.h>
#include "debugUtils.h"

#include "mongoose.h"
#include "mjson.h"
#include "mdns.h"
#include "timeManager.h"
#include "stm32f4xx_hal.h"
#include "mpu60x0.h"
#include "fusion.h"
#include "perfMon.h"
#include "statusLED.h"
#include "mac_driver.h"

/** Public API */
void serverInit(void);
void serverUpdate(void);
void serverShutdown(void);
void gzip_http_reply(struct mg_connection *c, int code, const char *headers, const unsigned char * data, size_t len) ;


#endif
