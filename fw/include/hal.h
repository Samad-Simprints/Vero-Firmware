//******************************************************************************
//
// Project Index source file
//
// (c) Fen Technology Ltd. 2016. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// MODULE: hal.h
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:  Kells header file
//
//******************************************************************************

#if !defined(_HAL_H_)
#define _HAL_H_

//******************************************************************************
// Includes
//******************************************************************************

#include "global.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constants
//******************************************************************************


//******************************************************************************
// Definitions
//******************************************************************************

//
// Hardware abstraction API
//

typedef enum 
{
  LED_RING_0 = 0,
  LED_RING_1,
  LED_RING_2,
  LED_RING_3,
  LED_RING_4,
  LED_RING_5,
  LED_RING_6,
  LED_SCAN_GOOD,
  LED_SCAN_BAD,
  LED_CONNECTED,
  LED_BATTERY,
  LED_MAX_LED_COUNT
}
tLeds;

typedef enum
{
  OFF = 0,
  RED,
  GREEN,
  ORANGE,
  ON,
  MED_FLASH_ON // what is this
}
tColorOptions;

typedef enum
{
  USB_UN20 = 0,
  USB_HOST,
  BT_HOST
}
tInterface;

typedef enum
{
  INTERFACE_EVENT_NONE = 0,
  INTERFACE_EVENT_CONNECTED,
  INTERFACE_EVENT_DISCONNECTED,
  INTERFACE_EVENT_RX_DATA,
  INTERFACE_EVENT_NUM_EVENTS
}
tInterfaceEvent;

// Event data for an INTERFACE_EVENT_RX_DATA event.
PACK ( typedef struct
{
  int   iLength;
  char* pcData;
})
tEventRxData;

// Event data for an INTERFACE_EVENT_CONNECTED or INTERFACE_EVENT_DISCONNECTED event.
typedef struct
{
  tInterface eInterface;
}
tEventConnDisconn;


typedef void (*vBtCallback)(void *context,  tInterfaceEvent event, void *event_data);
typedef void (*vUsbCallback)(void *context, tInterfaceEvent event, void *event_data);
typedef void (*vCallback)(void);

extern void vBtCallbackFunction(tInterfaceEvent event, void *event_data);
extern void vUsbCallbackFunction(tInterface eWhich, tInterfaceEvent event, void *event_data);

//******************************************************************************
// Function Prototypes
//******************************************************************************
extern void  vHalInit();
extern void vPowerInit();
extern void vEmcInit();
extern void vAnalogInit();
extern void vGpioInit();
extern void vSpiInit();
extern void vDebugInit();

extern void vUiInit();
extern void vUiLedSet(tLeds eLed, tColorOptions);
extern void vUiVibrateControl(bool boOnOff);
extern void vUiButtonCapture(vCallback, void *);
  
extern int iBtInit(vBtCallback pvCallback, void *);
extern int iBtSend(void *data, int count);
extern int iBtReset( bool boReset );
  
extern int iUsbInit(tInterface eWhich, vUsbCallback pvCallback, void *pvContext);
extern int iUsbSend(tInterface eWhich, void *pvData, int iCount);
  
extern void vUn20Init();
extern void vUn20Reset( bool boReset );

extern void vPowerInit();
extern void vPowerUsbOff();
extern void vPowerUsbOn();

extern void vPowerBtOff();
extern void vPowerBtOn();
extern void vPowerUn20Off();	// turn off the UN20 (takes 2 seconds)
extern void vPowerUn20On();	// turn on the UN20 (takes 5 seconds)

extern void vPowerSelfOff();	// turn the LPC1800 off (Power button wakes)
extern void vPowerSelfOn();	// called early to latch power on
extern void vPowerSelfNotify(vCallback pvCallback); // notification of power button press

extern int iBatteryVoltage(int iChannel);

#ifdef __cplusplus
}
#endif

#endif  // _HAL_H_
