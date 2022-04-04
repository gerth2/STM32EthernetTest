#include "networking.h"
#include "FreeRTOS.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_IP_Private.h"
#include "NetworkBufferManagement.h"
#include "server.h"
#include <string.h>



//////////////////////////////////
// FreeRTOS/Hardware Interface
static const uint8_t ucIPAddress[ 4 ] = { 10, 111, 76, 123 };
static const uint8_t ucNetMask[ 4 ] = { 255, 255, 255, 0 };
static const uint8_t ucGatewayAddress[ 4 ] = { 10, 111, 76, 1 };
static uint8_t ucMACAddress[ 6 ] = { 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
static const uint8_t ucDNSServerAddress[ 4 ] = { 1, 1, 1, 1 };

#define MYWWWPORT 80
#define BUFFER_SIZE 550
static uint8_t buf[BUFFER_SIZE+1];

static SPI_HandleTypeDef *spiHandle = NULL;

BaseType_t xNetworkInterfaceInitialise( void )
{
	BaseType_t xReturn = pdFAIL;

	if(spiHandle != NULL){


		//Cycle the reset pin
		printf("[HARDWARE] Resetting MAC Hardware...\n");
		HAL_GPIO_WritePin(ETH_RESET_GPIO_Port, ETH_RESET_Pin, GPIO_PIN_RESET);
		HAL_Delay(250);
		HAL_GPIO_WritePin(ETH_RESET_GPIO_Port, ETH_RESET_Pin, GPIO_PIN_SET);
		HAL_Delay(250);


		// Init the Hardwaare
		printf("[HARDWARE] Starting MAC Init...\n");
		ES_enc28j60SpiInit(spiHandle);
		ES_enc28j60Init(ucMACAddress);

		uint8_t enc28j60_rev = ES_enc28j60Revision();

		// Confirm the chip responded with a valid version
		if (enc28j60_rev > 0) {
			printf("[HARDWARE] Found ENC28J60 device Revision %d\n", enc28j60_rev);
			printf("[HARDWARE] MAC Init Success.\n");
			xReturn = pdPASS;
		} else {
			printf("[HARDWARE] MAC Init FAIL!\n");
		}
	}

    return xReturn;
}

BaseType_t xNetworkInterfaceOutput( NetworkBufferDescriptor_t * const pxDescriptor,
                                    BaseType_t xReleaseAfterSend )
{
	ES_enc28j60PacketSend(pxDescriptor->xDataLength, pxDescriptor->pucEthernetBuffer );

    iptraceNETWORK_INTERFACE_TRANSMIT();

    if( xReleaseAfterSend != pdFALSE )
    {
        vReleaseNetworkBufferAndDescriptor( pxDescriptor );
    }

    return pdTRUE;
}


/* The deferred interrupt handler is a standard RTOS task.  FreeRTOS's centralised
deferred interrupt handling capabilities can also be used. */
void handlePackets(void)
{
	NetworkBufferDescriptor_t *pxBufferDescriptor;
	size_t xBytesReceived;
	/* Used to indicate that xSendEventStructToIPTask() is being called because
	of an Ethernet receive event. */
	xIPStackEvent_t xRxEvent;

	do {

		xBytesReceived=ES_enc28j60PacketReceive(BUFFER_SIZE, buf);

		if( xBytesReceived > 0 )
		{
			/* Allocate a network buffer descriptor that points to a buffer
			large enough to hold the received frame.  As this is the simple
			rather than efficient example the received data will just be copied
			into this buffer. */
			pxBufferDescriptor = pxGetNetworkBufferWithDescriptor( xBytesReceived, 0 );

			if( pxBufferDescriptor != NULL )
			{
				/* pxBufferDescriptor->pucEthernetBuffer now points to an Ethernet
				buffer large enough to hold the received data.  Copy the
				received data into pcNetworkBuffer->pucEthernetBuffer.  Here it
				is assumed ReceiveData() is a peripheral driver function that
				copies the received data into a buffer passed in as the function's
				parameter.  Remember! While is is a simple robust technique -
				it is not efficient.  An example that uses a zero copy technique
				is provided further down this page. */
				memcpy( pxBufferDescriptor->pucEthernetBuffer, buf, xBytesReceived );
				pxBufferDescriptor->xDataLength = xBytesReceived;

				/* See if the data contained in the received Ethernet frame needs
				to be processed.  NOTE! It is preferable to do this in
				the interrupt service routine itself, which would remove the need
				to unblock this task for packets that don't need processing. */
				if( eConsiderFrameForProcessing( pxBufferDescriptor->pucEthernetBuffer )
																	  == eProcessBuffer )
				{
					/* The event about to be sent to the TCP/IP is an Rx event. */
					xRxEvent.eEventType = eNetworkRxEvent;

					/* pvData is used to point to the network buffer descriptor that
					now references the received data. */
					xRxEvent.pvData = ( void * ) pxBufferDescriptor;

					/* Send the data to the TCP/IP stack. */
					if( xSendEventStructToIPTask( &xRxEvent, 0 ) == pdFALSE )
					{
						/* The buffer could not be sent to the IP task so the buffer
						must be released. */
						vReleaseNetworkBufferAndDescriptor( pxBufferDescriptor );

						/* Make a call to the standard trace macro to log the
						occurrence. */
						iptraceETHERNET_RX_EVENT_LOST();
					}
					else
					{
						/* The message was successfully sent to the TCP/IP stack.
						Call the standard trace macro to log the occurrence. */
						iptraceNETWORK_INTERFACE_RECEIVE();
					}
				}
				else
				{
					/* The Ethernet frame can be dropped, but the Ethernet buffer
					must be released. */
					vReleaseNetworkBufferAndDescriptor( pxBufferDescriptor );
				}
			}
			else
			{
				/* The event was lost because a network buffer was not available.
				Call the standard trace macro to log the occurrence. */
				iptraceETHERNET_RX_EVENT_LOST();
			}
		}

	} while(xBytesReceived > 0);

}



uint8_t serverStatus = SERVER_NOCHANGE;

//void HttpserverTask( void *pvParameters );
void vApplicationIPNetworkEventHook( eIPCallbackEvent_t eNetworkEvent )
{
    if( eNetworkEvent == eNetworkUp ) {
    	printf("[HARDWARE] Network UP\n");
    	serverStatus = SERVER_INIT;
    } else if ( eNetworkEvent == eNetworkDown) {
    	printf("[HARDWARE] Network DOWN\n");
    	serverStatus = SERVER_SHUTDOWN;
    }
}

void vApplicationPingReplyHook( ePingReplyStatus_t eStatus, uint16_t usIdentifier )
{
    switch( eStatus )
    {
        case eSuccess    :
            /* A valid ping reply has been received.  Post the sequence number
            on the queue that is read by the vSendPing() function below.  Do
            not wait more than 10ms trying to send the message if it cannot be
            sent immediately because this function is called from the TCP/IP
            RTOS task - blocking in this function will block the TCP/IP RTOS task. */
            //xQueueSend( xPingReplyQueue, &usIdentifier, 10 / portTICK_PERIOD_MS );
        	//TODO - no idea how this is used or if it matters
        	printf("[HARDWARE] Ping response success\n");
            break;

        case eInvalidChecksum :
        case eInvalidData :
            /* A reply was received but it was not valid. */
        	printf("[HARDWARE] Ping response failure\n");
            break;
    }
}

BaseType_t xApplicationGetRandomNumber( uint32_t * pulNumber ){
	*pulNumber = rand();
	return pdTRUE;
}

uint32_t seqNumCounter = 42;

uint32_t ulApplicationGetNextSequenceNumber( uint32_t ulSourceAddress,
                                             uint16_t usSourcePort,
                                             uint32_t ulDestinationAddress,
                                             uint16_t usDestinationPort ){

    return (seqNumCounter++ ^ ulSourceAddress ^ usSourcePort ^ ulDestinationAddress ^ usDestinationPort) + 1;
}



//////////////////////////////////
// RTOS Tasking Interface

void NetworkingInit(SPI_HandleTypeDef *spiHandle_in) {
	spiHandle = spiHandle_in;
    /* Initialise the TCP/IP stack. */
	printf("[HARDWARE] Starting FreeRTOS TCP-IP Stack\n");
    FreeRTOS_IPInit( ucIPAddress,
                     ucNetMask,
                     ucGatewayAddress,
                     ucDNSServerAddress,
                     ucMACAddress );
    printf("[HARDWARE] FreeRTOS TCP-IP Stack Init Complete\n");
}

void NetworkingPeriodic() {
	handlePackets();
}


