//******************************************************************************
//
// Project INDEX Application header file
//
// (c) Fen Technology Ltd. 2014. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// COMPONENT:    KELLS cli handling functions
// MODULE:       kells.c
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:  see Header file for details
//
//******************************************************************************

//******************************************************************************
// Compilation switches
//******************************************************************************
// Indicate we have local debug zones. Disable the default legacy ones
#define DBG_LOCAL_DEFS

//******************************************************************************
// Includes
//******************************************************************************

#include <ctl_api.h>
#include <cross_studio_io.h>
#include <stdlib.h>

#include "global.h"
#include "hal.h"

#include <stdio.h>

#include "cli.h"
#include "gpio_dd.hpp"

#include "FreeRTOS.h"
#include "task.h"

//******************************************************************************
// Private Function Prototypes
//******************************************************************************


//******************************************************************************
// Constants
//******************************************************************************

//******************************************************************************
// Definitions
//******************************************************************************

//******************************************************************************
// Local Storage
//******************************************************************************

//******************************************************************************
//******************************************************************************
// Private Functions
//******************************************************************************
//******************************************************************************

//******************************************************************************
// CLI
//******************************************************************************

void vBtCallbackHandler(void *context,  tInterfaceEvent event, void *event_data)
{
  CLI_PRINT(("vBtCallbackHandler: %d\n", event));
}

void vUsbCallbackHandler(void *context,  tInterfaceEvent event, void *event_data)
{
  CLI_PRINT(("vUsbCallbackHandler: %d\n", event));
}

void vCallbackPowerOffHandler(void)
{
  CLI_PRINT(("vCallbackPowerOffHandler\n"));
}

void vCallbackCaptureHandler(void)
{
  CLI_PRINT(("vCallbackCaptureHandler\n"));
}

void vLedFlash()
{
  while (1)
  {
    vUiLedSet(LED_CONNECTED, ON);
    vTaskDelay(SECS_TO_TICKS(1));
    vUiLedSet(LED_CONNECTED, OFF);
    vTaskDelay(SECS_TO_TICKS(1));
  }
}

void vHalTest()
{
int iDelay, iSec;

  vGpioInit();
  vEmcInit();
  vAnalogInit();
  vSpiInit();
  vDebugInit();

  vPowerInit();
  vUn20Init();

  vUn20Reset( true );
  vUn20Reset( false );

  iBtInit(vBtCallbackHandler, NULL);
  iBtSend("Hello World", 8);

  iBtReset( true );
  iBtReset( false );

  vPowerUn20Off();
  vPowerUn20On();

  iBatteryVoltage(0);
  iBatteryVoltage(1);

  //vPowerSelfOff();	// turn the LPC1800 off (Power button wakes)
  vPowerSelfOn();	// turn the LPC1800 off (Power button wakes)

  vUiInit();

  vUiLedSet(LED_RING_0, OFF);
  vUiLedSet(LED_RING_0, RED);
  vUiLedSet(LED_RING_0, GREEN);
  vUiLedSet(LED_RING_0, ORANGE);
  vUiLedSet(LED_RING_0, OFF);

  vUiLedSet(LED_RING_1, OFF);
  vUiLedSet(LED_RING_1, RED);
  vUiLedSet(LED_RING_1, GREEN);
  vUiLedSet(LED_RING_1, ORANGE);
  vUiLedSet(LED_RING_1, OFF);

  vUiLedSet(LED_RING_2, OFF);
  vUiLedSet(LED_RING_2, RED);
  vUiLedSet(LED_RING_2, GREEN);
  vUiLedSet(LED_RING_2, ORANGE);
  vUiLedSet(LED_RING_2, OFF);

  vUiLedSet(LED_RING_3, OFF);
  vUiLedSet(LED_RING_3, RED);
  vUiLedSet(LED_RING_3, GREEN);
  vUiLedSet(LED_RING_3, ORANGE);
  vUiLedSet(LED_RING_3, OFF);

  vUiLedSet(LED_RING_4, OFF);
  vUiLedSet(LED_RING_4, RED);
  vUiLedSet(LED_RING_4, GREEN);
  vUiLedSet(LED_RING_4, ORANGE);
  vUiLedSet(LED_RING_4, OFF);
  vUiLedSet(LED_CONNECTED, OFF);
  vUiLedSet(LED_CONNECTED, ON);
  vUiLedSet(LED_CONNECTED, OFF);

  vUiLedSet(LED_BATTERY_RED, OFF);
  vUiLedSet(LED_BATTERY_RED, ON);
  vUiLedSet(LED_BATTERY_RED, OFF);

  vUiLedSet(LED_BATTERY_GREEN, OFF);
  vUiLedSet(LED_BATTERY_GREEN, ON);
  vUiLedSet(LED_BATTERY_GREEN, OFF);

  vUiVibrateControl(false);
  vUiVibrateControl(true);
  vUiVibrateControl(false);

  vUiButtonCapture(vCallbackCaptureHandler,NULL);
  
  
  iUsbInit(USB_UN20, vUsbCallbackHandler, NULL);
  iUsbSend(USB_UN20, "Hello World\n", 8);
  
  vUn20Init();

  vPowerUsbOff();
  vPowerUsbOn();

  vPowerBtOff();
  vPowerBtOn();
  vPowerUn20Off();	// turn off the UN20 (takes 2 seconds)
  vPowerUn20On();	// turn on the UN20 (takes 5 seconds)

  vPowerSelfOn();	// called early to latch power on
  vPowerSelfNotify(vCallbackPowerOffHandler); // notification of power button press

  iBatteryVoltage(0);
  iBatteryVoltage(1);

  vUiLedSet(LED_RING_0, ORANGE);
  vUiLedSet(LED_RING_1, ORANGE);
  vUiLedSet(LED_RING_2, ORANGE);
  vUiLedSet(LED_RING_3, ORANGE);
  vUiLedSet(LED_RING_4, ORANGE);
#if 0
  vUiLedSet(LED_RING_5, ORANGE);
  vUiLedSet(LED_RING_6, ORANGE);
  vUiLedSet(LED_SCAN_GOOD, ORANGE);
  vUiLedSet(LED_SCAN_BAD, ORANGE);
#endif
  vUiLedSet(LED_CONNECTED, ON);
  vUiLedSet(LED_BATTERY_RED, ON);
  vUiLedSet(LED_BATTERY_GREEN, ON);

  vPowerSelfOff();	// turn the LPC1800 off (Power button wakes)

}
