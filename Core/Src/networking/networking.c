#include "networking.h"
#include "FreeRTOS.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_IP_Private.h"
#include "NetworkBufferManagement.h"
#include "server.h"
#include <string.h>

//////////////////////////////////
// FreeRTOS/Hardware Interface
static const uint8_t ucIPAddress[4] = { 10, 111, 76, 123 };
static const uint8_t ucNetMask[4] = { 255, 255, 255, 0 };
static const uint8_t ucGatewayAddress[4] = { 10, 111, 76, 1 };
static uint8_t ucMACAddress[6] = { 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
static const uint8_t ucDNSServerAddress[4] = { 1, 1, 1, 1 };

#define MYWWWPORT 80
#define BUFFER_SIZE 550
static uint8_t buf[BUFFER_SIZE + 1];

void vReleaseNetworkBufferAndDescriptor( xNetworkBufferDescriptor_t * const pxNetworkBuffer );

BaseType_t xNetworkInterfaceInitialise( void )
{
    extern uint8_t ucMACAddress[ 6 ];
    if ( enc28j60_init(ucMACAddress, hspi1 ) == 0) {
        return pdPASS;
    } else {
        return pdFAIL;
    }
}

BaseType_t xNetworkInterfaceOutput( xNetworkBufferDescriptor_t * const pxDescriptor,
                                    BaseType_t xReleaseAfterSend )
{
    enc28j60_send_packet(pxDescriptor->pucEthernetBuffer, pxDescriptor->xDataLength );

    printf("FreeRTOS: Packet forwarded to driver for transmiting...\n");
    /* Call the standard trace macro to log the send event. */
    iptraceNETWORK_INTERFACE_TRANSMIT();

    if( xReleaseAfterSend != pdFALSE )
    {
        vReleaseNetworkBufferAndDescriptor( pxDescriptor );
    }

    return pdTRUE;
}



uint8_t serverStatus = SERVER_NOCHANGE;

//void HttpserverTask( void *pvParameters );
void vApplicationIPNetworkEventHook(eIPCallbackEvent_t eNetworkEvent) {
	if (eNetworkEvent == eNetworkUp) {
		printf("[HARDWARE] Network UP\n");
		serverStatus = SERVER_INIT;
	} else if (eNetworkEvent == eNetworkDown) {
		printf("[HARDWARE] Network DOWN\n");
		serverStatus = SERVER_SHUTDOWN;
	}
}

void vApplicationPingReplyHook(ePingReplyStatus_t eStatus,
		uint16_t usIdentifier) {
	switch (eStatus) {
	case eSuccess:
		/* A valid ping reply has been received.  Post the sequence number
		 on the queue that is read by the vSendPing() function below.  Do
		 not wait more than 10ms trying to send the message if it cannot be
		 sent immediately because this function is called from the TCP/IP
		 RTOS task - blocking in this function will block the TCP/IP RTOS task. */
		//xQueueSend( xPingReplyQueue, &usIdentifier, 10 / portTICK_PERIOD_MS );
		//TODO - no idea how this is used or if it matters
		printf("[HARDWARE] Ping response success\n");
		break;

	case eInvalidChecksum:
	case eInvalidData:
		/* A reply was received but it was not valid. */
		printf("[HARDWARE] Ping response failure\n");
		break;
	}
}

BaseType_t xApplicationGetRandomNumber(uint32_t *pulNumber) {
	*pulNumber = rand();
	return pdTRUE;
}

uint32_t seqNumCounter = 42;

uint32_t ulApplicationGetNextSequenceNumber(uint32_t ulSourceAddress,
		uint16_t usSourcePort, uint32_t ulDestinationAddress,
		uint16_t usDestinationPort) {

	return (seqNumCounter++ ^ ulSourceAddress ^ usSourcePort
			^ ulDestinationAddress ^ usDestinationPort) + 1;
}

//////////////////////////////////
// RTOS Tasking Interface

void NetworkingInit() {
	/* Initialise the TCP/IP stack. */
	printf("[HARDWARE] Starting FreeRTOS TCP-IP Stack\n");
	FreeRTOS_IPInit(ucIPAddress, ucNetMask, ucGatewayAddress,
			ucDNSServerAddress, ucMACAddress);
	printf("[HARDWARE] FreeRTOS TCP-IP Stack Init Complete\n");
}

void NetworkingPeriodic() {

}

