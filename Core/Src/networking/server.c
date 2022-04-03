#include "server.h"
#include "FreeRTOS_IP_Private.h"

uint8_t serverIsRunning = 0;

void serverInit(void){


	serverIsRunning = 1;
}

void serverUpdate(void){

}

void serverShutdown(void){


	serverIsRunning = 0;
}
