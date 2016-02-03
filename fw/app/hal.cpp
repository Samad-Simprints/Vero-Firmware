//******************************************************************************
//
// Project INDEX Application header file
//
// (c) Fen Technology Ltd. 2016. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// COMPONENT:    INDEX Hardware abstraction layer
// MODULE:       hal.cpp
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
#include "msg_format.h"
#include "lpcapp.h"

#include <stdio.h>

#include "cli.h"
#include "gpio_dd.hpp"
#include "adc_dd.hpp"

#include "freertos.h"
#include "task.h"

//******************************************************************************
// Private Function Prototypes
//******************************************************************************


//******************************************************************************
// Constants
//******************************************************************************
enum {
  ZONE_ERROR,
  ZONE_COMMANDS,
  ZONE_DATA
};

//******************************************************************************
// Definitions
//******************************************************************************
#define _QUOTEME(x)                              #x
#define QUOTEME(x)                               _QUOTEME(x)

#define INDEX_VERSION_MAJOR                     0
#define INDEX_VERSION_MINOR                     6
#define INDEX_VERSION_BUGFIX                    0
#define INDEX_REVISION_NUMBER                   0

#define INDEX_VERSION   QUOTEME(INDEX_VERSION_MAJOR) "."   \
                        QUOTEME(INDEX_VERSION_MINOR) "."   \
                        QUOTEME(INDEX_VERSION_BUGFIX) ":"  \
                        QUOTEME(INDEX_REVISION_NUMBER) " " \
                        "Built: " __DATE__ " " __TIME__

//******************************************************************************
// Local Storage
//******************************************************************************

DEBUG_MODULE_DEFINE( HAL_FD ) =
{
  "HAL_FD",
  { "Error", "Commands", "Data", 0 },
  0 //DEBUGZONE(ZONE_COMMANDS)
};
DEBUG_MODULE_USE( HAL_FD );

struct tagLedInfo
{
  bool boIsMono;        // true if LED on simple on/off
  char const *pszName;        // name of the LED
  tGPIOpin * const oGreenLed;  // green led GPIO (Or only led)
  tGPIOpin * const oRedLed;    // red led GPIO
};


static struct tagLedInfo oLedInfo[] =
{
  { false, "LED_RING_0",    LED_0_GREEN, LED_0_RED },
  { false, "LED_RING_1",    LED_1_GREEN, LED_1_RED },
  { false, "LED_RING_2",    LED_2_GREEN, LED_2_RED },
  { false, "LED_RING_3",    LED_3_GREEN, LED_3_RED },
  { false, "LED_RING_4",    LED_4_GREEN, LED_4_RED },
  { false, "LED_RING_5",    LED_5_GREEN, LED_5_RED },
  { false, "LED_RING_6",    LED_6_GREEN, LED_6_RED },
  { false, "LED_SCAN_GOOD", LED_SCAN_GOOD_GREEN, LED_SCAN_GOOD_RED },
  { false, "LED_SCAN_BAD",  LED_SCAN_BAD_GREEN,  LED_SCAN_BAD_RED },
  { true,  "LED_CONNECTED", LED_CONNECTION, 0 },
  { true,  "LED_BATTERY",   LED_BATTERY_RED, 0 }
};

static char const *pszLedColor[] =
{
  "OFF",
  "RED",
  "GREEN",
  "ORANGE",
  "ON"
};

static IAdcUnit *poADC0;

static void vNullHandler(void);
static void vNullBtCallback(void *context,  tInterfaceEvent event, void *event_data);
static void vNullUsbCallback(void *context,  tInterfaceEvent event, void *event_data);

static vCallback pvPoweroffCallback = vNullHandler;
static vCallback pvCaptureCallback = vNullHandler;
static vBtCallback pvBtCallback = vNullBtCallback;
static vUsbCallback pvUsbPhoneCallback = vNullUsbCallback;
static vUsbCallback pvUsbUn20Callback = vNullUsbCallback;

static void *pvBtContext = NULL;
static void *pvUsbUn20Context = NULL;
static void *pvUsbPhoneContext = NULL;

//******************************************************************************
//******************************************************************************
// Private Functions
//******************************************************************************
//******************************************************************************

//******************************************************************************
// CLI
//******************************************************************************

static bool boVersion( char **papzArgs, int iInstance, int iNumArgs );
static bool boConn( char **papzArgs, int iInstance, int iNumArgs );
static bool boCapture( char **papzArgs, int iInstance, int iNumArgs );
static bool boPower( char **papzArgs, int iInstance, int iNumArgs );
static bool boSetLed( char **papzArgs, int iInstance, int iNumArgs );
static bool boUsb( char **papzArgs, int iInstance, int iNumArgs );
static bool boBt( char **papzArgs, int iInstance, int iNumArgs );
static bool boDisconn( char **papzArgs, int iInstance, int iNumArgs );

static bool boUsbState( char **papzArgs, int iInstance, int iNumArgs );
static bool boBtState( char **papzArgs, int iInstance, int iNumArgs );

static bool boUn20Power( char **papzArgs, int iInstance, int iNumArgs );
static bool boUn20Reset( char **papzArgs, int iInstance, int iNumArgs );
static bool boBattery( char **papzArgs, int iInstance, int iNumArgs );
static bool boOff( char **papzArgs, int iInstance, int iNumArgs );
static bool boVibrate( char **papzArgs, int iInstance, int iNumArgs );


static const tParserEntry asHalCLI[] =
{
  CLICMD("version",               "Display version", 1, "", boVersion, 0),
  CLICMD("power",                 "Power button", 1, "", boPower, 0),
  CLICMD("capture",               "Capture button", 1, "", boCapture, 0),
  CLICMD("conn",                  "USB/BT connect", 1, "", boConn, 0),
  CLICMD("usb",                   "USB callback", 2, "", boUsb, 0),
  CLICMD("bt",                    "BT callback", 1, "", boBt, 0),
  CLICMD("disconn",               "Phone disconnect", 1, "", boDisconn, 0),
  CLICMD("un20",                  "Un20 power control", 2, "", boUn20Power, 0),
  CLICMD("un20r",                 "Un20 reset control", 2, "", boUn20Reset, 0),
  CLICMD("battery",               "Battery state", 2, "", boBattery, 0),
  CLICMD("off",                   "Power off", 1, "", boOff, 0),
  CLICMD("vibrate",               "Vibrate state", 2, "", boVibrate, 0),

};

tParserEntry asHalMainCLI =
{
  DEBUG_MODULE_REF(HAL_FD), "HAL", "HAL commands", "", 1, 0, 0, asHalCLI, ELEMENTSOF(asHalCLI)
};

static bool boVersion(char **papzArgs, int iInstance, int iNumArgs)
{
  CLI_PRINT(( "VERSION: %s\n", INDEX_VERSION ));
  return true;
}

static bool boDisconn( char **papzArgs, int iInstance, int iNumArgs )
{
  tEventConnDisconn sEventData;

  if ( pvUsbPhoneCallback != NULL )
  {
    sEventData.eInterface = USB_HOST;
    pvUsbPhoneCallback( pvUsbPhoneContext, INTERFACE_EVENT_DISCONNECTED, &sEventData );
  }

  if ( pvBtCallback != NULL )
  {
    sEventData.eInterface = BT_HOST;
    pvBtCallback( pvBtContext, INTERFACE_EVENT_DISCONNECTED, &sEventData );
  }

  return true;
}

static bool boConn( char **papzArgs, int iInstance, int iNumArgs )
{
  tEventConnDisconn sEventData;

  if ( pvUsbUn20Callback != NULL )
  {
    sEventData.eInterface = USB_UN20;
    pvUsbUn20Callback( pvUsbPhoneContext, INTERFACE_EVENT_CONNECTED, &sEventData );
  }

  if ( pvUsbPhoneCallback != NULL )
  {
    sEventData.eInterface = USB_HOST;
    pvUsbPhoneCallback( pvUsbPhoneContext, INTERFACE_EVENT_CONNECTED, &sEventData );
  }

  if ( pvBtCallback != NULL )
  {
    sEventData.eInterface = BT_HOST;
    pvBtCallback( pvBtContext, INTERFACE_EVENT_CONNECTED, &sEventData );
  }
  return true;
}

// simulate capture button being pressed
static bool boCapture( char **papzArgs, int iInstance, int iNumArgs )
{
  pvCaptureCallback();
  return true;
}

// simulate power button being pressed
static bool boPower( char **papzArgs, int iInstance, int iNumArgs )
{
  if ( pvPoweroffCallback != NULL )
  {
    pvPoweroffCallback();
  }
  return true;
}

// allow an led to be set
static bool boSetLed( char **papzArgs, int iInstance, int iNumArgs )
{
  int iLed = atoi(papzArgs[1]);
  int iColor = atoi(papzArgs[2]);

  if ( oLedInfo[iLed].boIsMono )
  {
    iColor = ( iColor ? ON : OFF );
  }

  vUiLedSet((tLeds)iLed, (tColorOptions)iColor);

  return true;
}

// control the UN20 power
static bool boUn20Power( char **papzArgs, int iInstance, int iNumArgs )
{
  int iOn = atoi(papzArgs[1]);

  if (iOn)
    vPowerUn20On();
  else
    vPowerUn20Off();

  return true;
}

// control the UN20 reset
static bool boUn20Reset( char **papzArgs, int iInstance, int iNumArgs )
{
  int iReset = atoi(papzArgs[1]);

  vUn20Reset(iReset);

  return true;
}

// print battery information
static bool boBattery( char **papzArgs, int iInstance, int iNumArgs )
{
  int channel = atoi(papzArgs[1]);

  CLI_PRINT(("Battery(%d): %d\n", channel, iBatteryVoltage(channel)));
  return true;
}

// remove power from system
static bool boOff( char **papzArgs, int iInstance, int iNumArgs )
{
  vPowerSelfOff();
  return true;
}

// control vibrate feedback motor
static bool boVibrate( char **papzArgs, int iInstance, int iNumArgs )
{
  bool boOnOff = (atoi(papzArgs[1]) ? true : false);

  vUiVibrateControl(boOnOff);
}

extern "C" {
  extern MsgPacket sGetSensorInfoPacket;
  extern MsgPacket sSetuiPacket;
  extern MsgPacket sSensorConfigPacket;
  extern MsgPacket sShutdownUn20Packet;
  extern MsgPacket sWakeupUn20Packet;
  extern MsgPacket sUn20ReadyPacket;
  extern MsgPacket sUn20GetInfoPacket;
  extern MsgPacket sCaptureImagePacket;
  extern MsgPacket sCaptureProgressPacket;
}

static bool boUsb( char **papzArgs, int iInstance, int iNumArgs )
{
  int iParam = atoi(papzArgs[1]);

  tEventRxData sEventData;

  switch ( iParam )
  {
  case MSG_GET_SENSOR_INFO:
    {
      sEventData.iLength = sGetSensorInfoPacket.Msgheader.iLength;
      sEventData.pcData = (char *) &sGetSensorInfoPacket;

      if ( pvUsbPhoneCallback != NULL )
      {
        pvUsbPhoneCallback( pvUsbPhoneContext, INTERFACE_EVENT_RX_DATA, &sEventData );
      }
    }
    break;

  case MSG_SET_SENSOR_CONFIG:
    {
      sEventData.iLength = sSensorConfigPacket.Msgheader.iLength;
      sEventData.pcData = (char *) &sSensorConfigPacket;

      if ( pvUsbPhoneCallback != NULL )
      {
        pvUsbPhoneCallback( pvUsbPhoneContext, INTERFACE_EVENT_RX_DATA, &sEventData );
      }
    }
    break;

  case MSG_SET_UI:
    {
      sEventData.iLength = sSetuiPacket.Msgheader.iLength;
      sEventData.pcData = (char *) &sSetuiPacket;

      if ( pvUsbPhoneCallback != NULL )
      {
        pvUsbPhoneCallback( pvUsbPhoneContext, INTERFACE_EVENT_RX_DATA, &sEventData );
      }
    }
    break;

  case MSG_CAPTURE_IMAGE:
    {
      sEventData.iLength = sCaptureImagePacket.Msgheader.iLength;
      sEventData.pcData = (char *) &sCaptureImagePacket;

      if ( pvUsbPhoneCallback != NULL )
      {
        pvUsbPhoneCallback( pvUsbPhoneContext, INTERFACE_EVENT_RX_DATA, &sEventData );
      }
    }

    break;

  case MSG_CAPTURE_PROGRESS:
    {
      sEventData.iLength = sCaptureProgressPacket.Msgheader.iLength;
      sEventData.pcData = (char *) &sCaptureProgressPacket;

      if ( pvUsbUn20Callback != NULL )
      {
        pvUsbUn20Callback( pvUsbUn20Context, INTERFACE_EVENT_RX_DATA, &sEventData );
      }
    }

    break;

  case MSG_UN20_SHUTDOWN:
    {
      sEventData.iLength = sShutdownUn20Packet.Msgheader.iLength;
      sEventData.pcData = (char *) &sShutdownUn20Packet;

      if ( pvUsbPhoneCallback != NULL )
      {
        pvUsbPhoneCallback( pvUsbPhoneContext, INTERFACE_EVENT_RX_DATA, &sEventData );
      }
    }
    break;

  case MSG_UN20_WAKEUP:
    {
      sEventData.iLength = sWakeupUn20Packet.Msgheader.iLength;
      sEventData.pcData = (char *) &sWakeupUn20Packet;

      if ( pvUsbPhoneCallback != NULL )
      {
        pvUsbPhoneCallback( pvUsbPhoneContext, INTERFACE_EVENT_RX_DATA, &sEventData );
      }
    }
    break;

  case MSG_UN20_READY:
    {
      sEventData.iLength = sUn20ReadyPacket.Msgheader.iLength;
      sEventData.pcData = (char *) &sUn20ReadyPacket;

      if ( pvUsbUn20Callback != NULL )
      {
        pvUsbUn20Callback( pvUsbUn20Context, INTERFACE_EVENT_RX_DATA, &sEventData );
      }
    }
    break;

  case MSG_UN20_GET_INFO:
    {
      sEventData.iLength = sUn20GetInfoPacket.Msgheader.iLength;
      sEventData.pcData = (char *) &sUn20GetInfoPacket;

      if ( pvUsbUn20Callback != NULL )
      {
        pvUsbUn20Callback( pvUsbUn20Context, INTERFACE_EVENT_RX_DATA, &sEventData );
      }
    }
    break;

  default:
    break;

  } // End switch

  return true;
}

static bool boBt( char **papzArgs, int iInstance, int iNumArgs )
{
  char acString[ 12 ] = { 0x00 };
  tEventRxData sEventData;
  sEventData.iLength = sizeof( acString );
  sEventData.pcData = acString;

  if ( pvBtCallback != NULL )
  {
    pvBtCallback( pvBtContext, INTERFACE_EVENT_RX_DATA, &sEventData );
  }
  return true;
}

void vHalInit(void)
{
  DEBUG_MODULE_INIT( HAL_FD );

  boCLIregisterEntry( &asHalMainCLI );
  
}

//
// Hal functions here
//
static void vNullHandler(void)
{
}

static void vNullBtCallback(void *context,  tInterfaceEvent event, void *event_data)
{
}

static void vNullUsbCallback(void *context, tInterfaceEvent event, void *event_data)
{
}

//
void vEmcInit()
{
  DEBUGMSG(ZONE_COMMANDS,("vEmcInit()\n"));
}

void vAnalogInit()
{
  DEBUGMSG(ZONE_COMMANDS,("vAnalogInit()\n"));
}

void vGpioInit()
{
  DEBUGMSG(ZONE_COMMANDS,("vGpioInit()\n"));
}

void vSpiInit()
{
  DEBUGMSG(ZONE_COMMANDS,("vSpiInit()\n"));
}

void vDebugInit()
{
  DEBUGMSG(ZONE_COMMANDS,("vDebugInit()\n"));
}

// route an interrupt to the correct handler
static void vUIIsr( const IRQn_Type eNVICchannel )
{
  if ( eNVICchannel == PIN_INT1_IRQn )
  {
    pvPoweroffCallback();
  }
  else if ( eNVICchannel == PIN_INT2_IRQn )
  {
    pvCaptureCallback();
  }
}

/* Initialise the UI elements
 */
void vUiInit()
{
  int iLed;

  DEBUGMSG(ZONE_COMMANDS,("vUiInit()\n"));

  // configure the LED's
  for (iLed = 0; iLed < ELEMENTSOF(oLedInfo); iLed++)
  {
    DEBUGMSG(ZONE_COMMANDS, ("Init: %s\n", oLedInfo[ iLed ].pszName));
#if !defined(EVAL_BOARD)
    if (oLedInfo[ iLed ].oGreenLed)
    {
      oLedInfo[ iLed ].oGreenLed->vConfigure();
    }
    if (oLedInfo[ iLed ].oRedLed)
    {
      oLedInfo[ iLed ].oRedLed->vConfigure();
    }
#endif
  }

#if !defined(EVAL_BOARD)
  // configure the vibrate motor drive
  VIBRATE_POWER->vConfigure();
  BUTTON_0_POWER->vConfigure();
  BUTTON_1_SCAN->vConfigure();

  // register callbacks for the buttons
#if 1
  vGPIODDsetPinInterruptHandler( BUTTON_0_POWER,  PIN_INT1_IRQn, /*ieBoth*/ ieFalling, ::vUIIsr );
  vGPIODDsetPinInterruptHandler( BUTTON_1_SCAN, PIN_INT2_IRQn, /*ieBoth*/ ieFalling,  ::vUIIsr );
#endif
#endif
}

void vUiLedSet(tLeds eLed, tColorOptions eColor)
{
  SIMPLEASSERT(eLed < LED_MAX_LED_COUNT);

  if ( oLedInfo[ eLed ].boIsMono )
  {
    SIMPLEASSERT( (eColor == OFF) || (eColor == ON) );
#if !defined(EVAL_BOARD)
    oLedInfo[ eLed ].oGreenLed->vSet( (eColor == ON));
#endif
  }
  else
  {
    SIMPLEASSERT( (eColor == OFF) || (eColor == RED) || (eColor == GREEN) || (eColor == ORANGE));
#if !defined(EVAL_BOARD)
    switch (eColor)
    {
      case OFF:
        oLedInfo[ eLed ].oGreenLed->vSet( false );
        oLedInfo[ eLed ].oRedLed->vSet( false );
        break;
      case RED:
        oLedInfo[ eLed ].oGreenLed->vSet( false );
        oLedInfo[ eLed ].oRedLed->vSet( true );
        break;
      case GREEN:
        oLedInfo[ eLed ].oGreenLed->vSet( true );
        oLedInfo[ eLed ].oRedLed->vSet( false );
        break;
      case ORANGE:
        oLedInfo[ eLed ].oGreenLed->vSet( true );
        oLedInfo[ eLed ].oRedLed->vSet( true );
        break;
    }
#endif
  }

  DEBUGMSG(ZONE_COMMANDS, ("Led: %s: %s\n", oLedInfo[ eLed ].pszName, pszLedColor[eColor]));
}

void vUiVibrateControl(bool boOnOff)
{
#if !defined(EVAL_BOARD)
  VIBRATE_POWER->vSet( boOnOff );
#endif
  DEBUGMSG(ZONE_COMMANDS,("Vibrate: %s\n", (boOnOff ? "On" : "Off")));
}

void vUiButtonCapture(vCallback pvCallback, void *pvData)
{
  DEBUGMSG(ZONE_COMMANDS,("vUiButtonCapture()\n"));
  pvCaptureCallback = pvCallback;
}

  
int iBtInit(vBtCallback pvCallback, void *pvData)
{
#if !defined(EVAL_BOARD)
  BT_RESET->vConfigure();
#endif
  DEBUGMSG(ZONE_COMMANDS,("iBtInit()\n"));
  pvBtCallback = pvCallback;
}

int iBtSend(void *data, int count)
{
  MsgPacketheader *psHeader = (MsgPacketheader *) data;

  DEBUGMSG(ZONE_COMMANDS,("iBtSend(): Length %d, MsgId 0x%02X, Status %d\n", count, psHeader->bMsgId, psHeader->bStatus ));
}

int iBtReset( bool boReset )
{
#if !defined(EVAL_BOARD)
  BT_RESET->vSet( boReset );
#endif
  DEBUGMSG(ZONE_COMMANDS,("iBtReset: %s\n", (boReset ? "On" : "Off")));
}
  
// eWhich - Source for which we are registering a callback
// pvCallback - Callback function for this source
// pvContext - Opaque pointer returned in the callback for this source
int iUsbInit(tInterface eWhich, vUsbCallback pvCallback, void *pvContext)
{
  DEBUGMSG(ZONE_COMMANDS,("iUsbInit()\n"));

  switch ( eWhich )
  {
  case USB_UN20:
    pvUsbUn20Callback = pvCallback;
    pvUsbUn20Context = pvContext;
    break;

  case USB_HOST:
    pvUsbPhoneCallback = pvCallback;
    pvUsbPhoneContext = pvContext;
    break;

  default:
    // No action
    break;

  } // End switch

}

int iUsbSend(tInterface eWhich, void *pvData, int iCount)
{
  MsgPacketheader *psHeader = (MsgPacketheader *) pvData;

  DEBUGMSG(ZONE_COMMANDS,("iUsbSend(): Dest %s, Length %d, MsgId 0x%02X, Status %d\n", ( eWhich == USB_UN20 ) ? "UN20" : "HOST", iCount, psHeader->bMsgId, psHeader->bStatus ));

}
  
void vUn20Init()
{
#if !defined(EVAL_BOARD)
  UN20B_nRESET->vConfigure();
#endif
  DEBUGMSG(ZONE_COMMANDS,("vUn20Init()\n"));
}

void vUn20Reset( bool boReset )	// reset the UN20
{
#if !defined(EVAL_BOARD)
  UN20B_nRESET->vSet( boReset );
#endif
  DEBUGMSG(ZONE_COMMANDS,("vUn20OReset: %s\n", (boReset ? "On" : "Off")));
}

void vPowerInit()
{
#if !defined(EVAL_BOARD)
  // latch the power to the CPU on
  nPWR_DOWN->vConfigure();
  nPWR_DOWN->vSet( false );

  UN20B_POWER->vConfigure();
  UN20B_POWER->vSet( true );

//for (volatile int i = 0; i < 18000000; i++);

#endif
  DEBUGMSG(ZONE_COMMANDS,("vPowerInit()\n"));
}

void vPowerUsbOff()
{
#if !defined(EVAL_BOARD)
  UN20B_POWER->vSet( false );
#endif
  DEBUGMSG(ZONE_COMMANDS,("vPowerUsbOff()\n"));
}

void vPowerUsbOn()
{
#if !defined(EVAL_BOARD)
  UN20B_POWER->vSet( true );
#endif
  DEBUGMSG(ZONE_COMMANDS,("vPowerUsbOn()\n"));
}

void vPowerBtOff()
{
  DEBUGMSG(ZONE_COMMANDS,("vPowerBtOff()\n"));
}

void vPowerBtOn()
{
  DEBUGMSG(ZONE_COMMANDS,("vPowerBtOn()\n"));
}

void vPowerUn20Off()	// turn off the UN20 (takes 2 seconds){
{
  tEventConnDisconn sEventData;

  DEBUGMSG(ZONE_COMMANDS,("vPowerUn20Off()\n"));

  if ( pvUsbPhoneCallback != NULL )
  {
    sEventData.eInterface = USB_UN20;
    pvUsbUn20Callback( pvUsbUn20Context, INTERFACE_EVENT_DISCONNECTED, &sEventData );
  }
#if !defined(EVAL_BOARD)
  UN20B_POWER->vSet( false );
#endif
}

void vPowerUn20On()	// turn on the UN20 (takes 5 seconds)
{
  DEBUGMSG(ZONE_COMMANDS,("vPowerUn20On()\n"));

#if !defined(EVAL_BOARD)
  UN20B_POWER->vSet( true );
#endif
}

/* Not expected to return from this call
 */
void vPowerSelfOff()	// turn the LPC1800 off (Power button wakes)
{
  DEBUGMSG(ZONE_COMMANDS,("vPowerSelfOff()\n"));
#if !defined(EVAL_BOARD)
  nPWR_DOWN->vSet( true );
  //CLI_PRINT(("NOTE: Hardware power off disabled\n"));
#endif
  for (;;);
}

void vPowerSelfOn()	// called early to latch power on
{
  DEBUGMSG(ZONE_COMMANDS,("vPowerSelfOn()\n"));
#if !defined(EVAL_BOARD)
  nPWR_DOWN->vSet( false );
#endif
}

void vPowerSelfNotify(vCallback pvCallback) // register for notification of power button press
{
  DEBUGMSG(ZONE_COMMANDS,("vPowerSelfNotify()\n"));
  pvPoweroffCallback = pvCallback;
}

// read the battery voltages (2 channels available)
int iBatteryVoltage(int iChannel)
{
  int iChannelValue = (iChannel + 1) * 100;

  SIMPLEASSERT( (iChannel >= 0) && (iChannel <= 1) );

#if !defined(EVAL_BOARD)
  poADC0 = poADCDDgetUnit( 0 );
  // read raw value from SDC
  iChannelValue = poADC0->iReadChannel( iChannel );

  // calculate result in mV taking into account VREF and conditioning logic
  iChannelValue = (((( (3300 * 100) / 1023 ) * iChannelValue) / 100) * 2);
#endif

  DEBUGMSG(ZONE_COMMANDS,("iBatteryVoltage(%d):%d\n", iChannel, iChannelValue));
  return iChannelValue;

}
