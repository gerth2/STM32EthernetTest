#include "server.h"
#include "FreeRTOS_IP_Private.h"



uint8_t serverIsRunning = FALSE;

void serverInit(void){



	serverIsRunning = TRUE;
}


void serverUpdate(void){

}

void serverShutdown(void){
	serverIsRunning = FALSE;


}
