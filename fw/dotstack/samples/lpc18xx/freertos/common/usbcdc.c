/*
	This module is derived from the USB_CDC.c module provided with the FreeRTOS
	example for LPC1768. It implements a USB serial port using the CDC class.

	Windows:
	Extract the usbser.sys file from .cab file in C:\WINDOWS\Driver Cache\i386
	and store it somewhere (C:\temp is a good place) along with the usbser.inf
	file. Then plug in the LPC176x and direct windows to the usbser driver.
	Windows then creates an extra COMx port that you can open in a terminal
	program, like hyperterminal. [Note for FreeRTOS users - the required .inf
	file is included in the project directory.]

	Linux:
	The device should be recognised automatically by the cdc_acm driver,
	which creates a /dev/ttyACMx device file that acts just like a regular
	serial port.

*/

/*
	LPCUSB, an USB device driver for LPC microcontrollers	
	Copyright (C) 2006 Bertrik Sikken (bertrik@sikken.nl)

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright
	   notice, this list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright
	   notice, this list of conditions and the following disclaimer in the
	   documentation and/or other materials provided with the distribution.
	3. The name of the author may not be used to endorse or promote products
	   derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
	IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
	NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
	THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "usbcdc.h"

#include "usbapi.h"
#include "usbdebug.h"

#include <stdio.h>
#include <string.h>

#define usbMAX_SEND_BLOCK		( 20 / portTICK_RATE_MS )
#define usbBUFFER_LEN			( 20 )

#define INCREMENT_ECHO_BY 1
#define BAUD_RATE	115200

#define INT_IN_EP		0x81
#define BULK_OUT_EP		0x05
#define BULK_IN_EP		0x82

#define MAX_PACKET_SIZE	64

#define LE_WORD(x)		((x)&0xFF),((x)>>8)

// CDC definitions
#define CS_INTERFACE			0x24
#define CS_ENDPOINT				0x25

#define	SET_LINE_CODING			0x20
#define	GET_LINE_CODING			0x21
#define	SET_CONTROL_LINE_STATE	0x22

// data structure for GET_LINE_CODING / SET_LINE_CODING class requests
typedef struct {
	unsigned long		dwDTERate;
	unsigned char		bCharFormat;
	unsigned char		bParityType;
	unsigned char		bDataBits;
} TLineCoding;

static TLineCoding LineCoding = {115200, 0, 0, 8};
static unsigned char abBulkBuf[64];
static unsigned char abClassReqData[8];

static const char *mTransferData;
static       int   mTransferSize;
static usbcdc_transfer_callback_t mCallback;

static const unsigned char abDescriptors[] = {

// device descriptor
	0x12,
	DESC_DEVICE,
	LE_WORD(0x0101),			// bcdUSB
	0x02,						// bDeviceClass
	0x00,						// bDeviceSubClass
	0x00,						// bDeviceProtocol
	MAX_PACKET_SIZE0,			// bMaxPacketSize
	LE_WORD(0xFFFF),			// idVendor
	LE_WORD(0x0005),			// idProduct
	LE_WORD(0x0100),			// bcdDevice
	0x01,						// iManufacturer
	0x02,						// iProduct
	0x03,						// iSerialNumber
	0x01,						// bNumConfigurations

// configuration descriptor
	0x09,
	DESC_CONFIGURATION,
	LE_WORD(67),				// wTotalLength
	0x02,						// bNumInterfaces
	0x01,						// bConfigurationValue
	0x00,						// iConfiguration
	0xC0,						// bmAttributes
	0x32,						// bMaxPower
// control class interface
	0x09,
	DESC_INTERFACE,
	0x00,						// bInterfaceNumber
	0x00,						// bAlternateSetting
	0x01,						// bNumEndPoints
	0x02,						// bInterfaceClass
	0x02,						// bInterfaceSubClass
	0x01,						// bInterfaceProtocol, linux requires value of 1 for the cdc_acm module
	0x00,						// iInterface
// header functional descriptor
	0x05,
	CS_INTERFACE,
	0x00,
	LE_WORD(0x0110),
// call management functional descriptor
	0x05,
	CS_INTERFACE,
	0x01,
	0x01,						// bmCapabilities = device handles call management
	0x01,						// bDataInterface
// ACM functional descriptor
	0x04,
	CS_INTERFACE,
	0x02,
	0x02,						// bmCapabilities
// union functional descriptor
	0x05,
	CS_INTERFACE,
	0x06,
	0x00,						// bMasterInterface
	0x01,						// bSlaveInterface0
// notification EP
	0x07,
	DESC_ENDPOINT,
	INT_IN_EP,					// bEndpointAddress
	0x03,						// bmAttributes = intr
	LE_WORD(8),					// wMaxPacketSize
	0x0A,						// bInterval
// data class interface descriptor
	0x09,
	DESC_INTERFACE,
	0x01,						// bInterfaceNumber
	0x00,						// bAlternateSetting
	0x02,						// bNumEndPoints
	0x0A,						// bInterfaceClass = data
	0x00,						// bInterfaceSubClass
	0x00,						// bInterfaceProtocol
	0x00,						// iInterface
// data EP OUT
	0x07,
	DESC_ENDPOINT,
	BULK_OUT_EP,				// bEndpointAddress
	0x02,						// bmAttributes = bulk
	LE_WORD(MAX_PACKET_SIZE),	// wMaxPacketSize
	0x00,						// bInterval
// data EP IN
	0x07,
	DESC_ENDPOINT,
	BULK_IN_EP,					// bEndpointAddress
	0x02,						// bmAttributes = bulk
	LE_WORD(MAX_PACKET_SIZE),	// wMaxPacketSize
	0x00,						// bInterval
	
	// string descriptors
	0x04,
	DESC_STRING,
	LE_WORD(0x0409),

	0x0E,
	DESC_STRING,
	'L', 0, 'P', 0, 'C', 0, 'U', 0, 'S', 0, 'B', 0,

	0x14,
	DESC_STRING,
	'U', 0, 'S', 0, 'B', 0, 'S', 0, 'e', 0, 'r', 0, 'i', 0, 'a', 0, 'l', 0,

	0x12,
	DESC_STRING,
	'D', 0, 'E', 0, 'A', 0, 'D', 0, 'C', 0, '0', 0, 'D', 0, 'E', 0,

// terminating zero
	0
};


/**
	Local function to handle incoming bulk data
		
	@param [in] bEP
	@param [in] bEPStatus
 */
static void BulkOut(unsigned char bEP, unsigned char bEPStatus)
{
	int /* i,*/ iLen;
	long lHigherPriorityTaskWoken = pdFALSE;

	(void) bEPStatus;
	
	// get data from USB into intermediate buffer
	iLen = USBHwEPRead(bEP, abBulkBuf, sizeof(abBulkBuf));
#if 0
	for (i = 0; i < iLen; i++) {
		// put into queue
		xQueueSendFromISR( xRxedChars, &( abBulkBuf[ i ] ), &lHigherPriorityTaskWoken );
	}
#else
	(void)iLen;
#endif
	portEND_SWITCHING_ISR( lHigherPriorityTaskWoken );
}


/**
	Local function to handle outgoing bulk data
		
	@param [in] bEP
	@param [in] bEPStatus
 */
static void BulkIn(unsigned char bEP, unsigned char bEPStatus)
{
	int i, len;

	(void) bEPStatus;

	len = 0;
	
	while (mTransferSize > 0 && len < MAX_PACKET_SIZE)
	{
		// get bytes from transmit FIFO into intermediate buffer
		for (i = 0; len < MAX_PACKET_SIZE && i < mTransferSize; i++, len++)
		{
			abBulkBuf[len] = mTransferData[i];
		}
		
		mTransferData += i;
		mTransferSize -= i;
	
		// Report partial transfer progress to the user
		mCallback(i);

		// Indicate end of the last transfer operation if no data is pending
		// anymore. We can do this here instead of when the transfer actually
		// completes because we have copied data into out abBulkBuf and user
		// is free to reuse own buffer space as he sees fit. Callback might
		// have re-primed us with more data, so continue until we still have
		// a space to fill in the packet.
		if (mTransferSize == 0)
			mCallback(0);
	}

	// send over USB
	if (len > 0)
	{
		USBHwEPWrite(bEP, abBulkBuf, len);
	}
	else
	{
		// no more data, disable further NAK interrupts until next USB frame
		USBHwNakIntEnable(0);
	}
}


/**
	Local function to handle the USB-CDC class requests
		
	@param [in] pSetup
	@param [out] piLen
	@param [out] ppbData
 */
static BOOL HandleClassRequest(TSetupPacket *pSetup, int *piLen, unsigned char **ppbData)
{
	switch (pSetup->bRequest) {

	// set line coding
	case SET_LINE_CODING:
		DBG("SET_LINE_CODING\n");
		memcpy((unsigned char *)&LineCoding, *ppbData, 7);
		*piLen = 7;
		DBG("dwDTERate=%u, bCharFormat=%u, bParityType=%u, bDataBits=%u\n",
		LineCoding.dwDTERate,
		LineCoding.bCharFormat,
		LineCoding.bParityType,
		LineCoding.bDataBits);
		break;

	// get line coding
	case GET_LINE_CODING:
		DBG("GET_LINE_CODING\n");
		*ppbData = (unsigned char *)&LineCoding;
		*piLen = 7;
		break;

	// set control line state
	case SET_CONTROL_LINE_STATE:
		// bit0 = DTR, bit = RTS
		DBG("SET_CONTROL_LINE_STATE %X\n", pSetup->wValue);
		break;

	default:
		return FALSE;
	}
	return TRUE;
}


/**
	Interrupt handler
	
	Simply calls the USB ISR
 */
void USB_IRQHandler(void)
{
	USBHwISR();
}


static void USBFrameHandler(unsigned short wFrame)
{
	if (mTransferSize > 0)
	{
		// data available, enable NAK interrupt on bulk in
		USBHwNakIntEnable(INACK_BI);
	}
}

static void USBTask(void* parameters)
{
#if 0
	DBG("Initialising USB stack\n");

	// initialise stack
	USBInit();

	// register descriptors
	USBRegisterDescriptors(abDescriptors);

	// register class request handler
	USBRegisterRequestHandler(REQTYPE_TYPE_CLASS, HandleClassRequest, abClassReqData);

	// register endpoint handlers
	USBHwRegisterEPIntHandler(INT_IN_EP, NULL);
	USBHwRegisterEPIntHandler(BULK_IN_EP, BulkIn);
	USBHwRegisterEPIntHandler(BULK_OUT_EP, BulkOut);
	
	// register frame handler
	USBHwRegisterFrameHandler(USBFrameHandler);

	// enable bulk-in interrupts on NAKs
	USBHwNakIntEnable(INACK_BI);

	DBG("Starting USB communication\n");

	NVIC_SetPriority( USB_IRQn, configUSB_INTERRUPT_PRIORITY );
	NVIC_EnableIRQ( USB_IRQn );
		
	// connect to bus
		
	DBG("Connecting to USB bus\n");
	USBHwConnect(TRUE);
	
	for (;;)
	{
		vTaskDelay(portMAX_DELAY);
	}
#endif
}


void usbcdc_init(void)
{
#if 0
    // Configure the clock for the USB.
    if( LPC_SC->PLL1STAT & ( 1 << 9 ) )
    {
            // Enable PLL, disconnected.
            LPC_SC->PLL1CON = 1;			
            LPC_SC->PLL1FEED = 0xAA;
            LPC_SC->PLL1FEED = 0x55;
    }
    
    // Disable PLL, disconnected.
    LPC_SC->PLL1CON = 0;				
    LPC_SC->PLL1FEED = 0xAA;
    LPC_SC->PLL1FEED = 0x55;
    
    LPC_SC->PLL1CFG = 0x23;
    LPC_SC->PLL1FEED = 0xAA;
    LPC_SC->PLL1FEED = 0x55;
    
    // Enable PLL, disconnected.
    LPC_SC->PLL1CON = 1;				
    LPC_SC->PLL1FEED = 0xAA;
    LPC_SC->PLL1FEED = 0x55;
    while (((LPC_SC->PLL1STAT & (1 << 10)) == 0));
    
    // Enable and connect.
    LPC_SC->PLL1CON = 3;
    LPC_SC->PLL1FEED = 0xAA;
    LPC_SC->PLL1FEED = 0x55;
    while (((LPC_SC->PLL1STAT & (1 << 9)) == 0 ));
    
    // Create the USB task.
    xTaskCreate(&USBTask, "USB", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
 #endif
}


void usbcdc_transmit(const char *data, unsigned int len,
					 usbcdc_transfer_callback_t callback)
{
	mTransferData = data;
	mTransferSize = len;
	
	mCallback = callback;
}

