#include "networking.h"
#include "FreeRTOS.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_IP_Private.h"
#include "NetworkBufferManagement.h"
#include <string.h>

uint8_t mac[] = { 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
uint8_t ip[] = { 10, 111, 76, 123 };

#define MYWWWPORT 80
#define BUFFER_SIZE 550
static uint8_t buf[BUFFER_SIZE+1];

static SPI_HandleTypeDef *spiHandle = NULL;


//////////////////////////////////
// FreeRTOS/MAC Interface

BaseType_t xNetworkInterfaceInitialise( void )
{
	BaseType_t xReturn = pdFAIL;

	if(spiHandle != NULL){
		ES_enc28j60SpiInit(spiHandle);
		ES_enc28j60Init(mac);

		uint8_t enc28j60_rev = ES_enc28j60Revision();

		if (enc28j60_rev > 0) {
			xReturn = pdPASS;
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



//////////////////////////////////
// RTOS Tasking Interface

void NetworkingInit(SPI_HandleTypeDef *spiHandle_in) {
	spiHandle = spiHandle_in;
}

void NetworkingPeriodic() {

	handlePackets();

}


