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

//#include <ctl_api.h>
//#include <cross_studio_io.h>
#include <stdlib.h>

#include "global.h"
#include "watchdog_dd.h"

#include "hal.h"
#include "msg_format.h"
#include "lpcapp.h"
#include "test_msgs.h"

#include <stdio.h>

#include "cli.h"
#include "gpio_dd.hpp"
#include "ser_dd.hpp"
#include "adc_dd.hpp"
#include "protocol_msg.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "lpc18xx_cgu.h"


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
#define UN20_TASK_STACK_SIZE           ( 300 /*configMINIMAL_STACK_SIZE*/ + TASK_DEBUG_OVERHEAD )
#define UN20_TASK_PRIORITY             ( tskIDLE_PRIORITY + 1 )

#define UN20_POWERON_DELAY_MS         100

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


static struct tagLedInfo oLedInfo[ LED_MAX_LED_COUNT ] =
{
  { false, "LED_RING_0",    LED_0_GREEN, LED_0_RED },
  { false, "LED_RING_1",    LED_1_GREEN, LED_1_RED },
  { false, "LED_RING_2",    LED_2_GREEN, LED_2_RED },
  { false, "LED_RING_3",    LED_3_GREEN, LED_3_RED },
  { false, "LED_RING_4",    LED_4_GREEN, LED_4_RED },
  { true,  "!Not Valid!",   0, 0 },
  { true,  "LED_CONNECTED", LED_CONNECTION, 0 },
  { true,  "LED_BATT_RED",  LED_BATT_RED, 0 },
  { true,  "LED_BATT_GREEN",LED_BATT_GREEN, 0 }
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

static xSemaphoreHandle hSendCompleteSemaphore = NULL;

static MsgPacket sMsg;

//******************************************************************************
//******************************************************************************
// Private Functions
//******************************************************************************
//******************************************************************************

//******************************************************************************
// CLI
//******************************************************************************

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

static bool boVbus( char **papzArgs, int iInstance, int iNumArgs );
static bool boHWConfig( char **papzArgs, int iInstance, int iNumArgs );

static const tParserEntry asHalCLI[] =
{
  CLICMD("power",                 "Power button", 1, "", boPower, 0),
  CLICMD("capture",               "Capture button", 1, "", boCapture, 0),
  CLICMD("conn",                  "USB/BT connect", 1, "", boConn, 0),
  CLICMD("usb",                   "USB callback", -3, "", boUsb, 0),
  CLICMD("bt",                    "BT callback", 1, "", boBt, 0),
  CLICMD("disconn",               "Phone disconnect", 1, "", boDisconn, 0),
  CLICMD("un20",                  "Un20 power control", 2, "", boUn20Power, 0),
  CLICMD("un20r",                 "Un20 reset control", 2, "", boUn20Reset, 0),
  CLICMD("battery",               "Battery state", 2, "", boBattery, 0),
  CLICMD("off",                   "Power off", 1, "", boOff, 0),
  CLICMD("vibrate",               "Vibrate state", 2, "", boVibrate, 0),
  CLICMD("vbus",                  "Vbus state", 2, "", boVbus, 0),
  CLICMD("hwconfig",               "Configure HW", 2, "", boHWConfig, 0),
};

tParserEntry asHalMainCLI =
{
  DEBUG_MODULE_REF(HAL_FD), "HAL", "HAL commands", "", 1, 0, 0, asHalCLI, ELEMENTSOF(asHalCLI)
};

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
    pvUsbUn20Callback( pvUsbUn20Context, INTERFACE_EVENT_CONNECTED, &sEventData );
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

static bool boVbus( char **papzArgs, int iInstance, int iNumArgs )
{
  int iOn = atoi(papzArgs[1]);

  CLI_PRINT(("vBus: %d\n", iOn));

  USB_VBUS_PWR_EN->vSet( iOn );

  return true;
}

// print battery information
static bool boBattery( char **papzArgs, int iInstance, int iNumArgs )
{
  int channel = atoi(papzArgs[1]);

  CLI_PRINT(("Battery(%d): %dmV, Charging: %d, USBPresent: %d\n",
              channel, iBatteryVoltage(channel), boHalBatteryIsCharging(), boHalUSBChargePresent()));

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
  return true;
}

// Configure the hardware for reprogramming operations
static bool boHWConfig( char **papzArgs, int iInstance, int iNumArgs )
{
  int iMode = atoi(papzArgs[1]);

  CLI_PRINT(("Setting Hardware Mode:%d\n",iMode ));
  // small delay to let the message get out
  vTaskDelay(SECS_TO_TICKS(1));

  vSetHardwareConfig( iMode );

  return true;
}

// inject the message into the Phones USB RX stream
static void vUSBRx(MsgPacket *psMsg)
{
  tEventRxData sEventData;

  sEventData.iLength = psMsg->Msgheader.iLength;
  sEventData.pcData = (char *)psMsg;

  if ( pvUsbPhoneCallback != NULL )
  {
    pvUsbPhoneCallback( pvUsbPhoneContext, INTERFACE_EVENT_RX_DATA, &sEventData );
  }
}

// inject the message into the UN20's USB RX stream
static void vUN20Rx(MsgPacket *psMsg)
{
  tEventRxData sEventData;

  sEventData.iLength = psMsg->Msgheader.iLength;
  sEventData.pcData = (char *)psMsg;

  if ( pvUsbUn20Callback != NULL )
  {
    pvUsbUn20Callback( pvUsbUn20Context, INTERFACE_EVENT_RX_DATA, &sEventData );
  }
}

static bool boUsb( char **papzArgs, int iInstance, int iNumArgs )
{
  int iParam = atoi(papzArgs[1]);
  int iArg = 0;
  tEventRxData sEventData;
  
  if ( iNumArgs > 2 )
  {
     iArg = atoi(papzArgs[2]);
  }

  switch ( iParam )
  {
  case MSG_GET_SENSOR_INFO:
    {
      vSetupMessage( &sMsg, MSG_GET_SENSOR_INFO, MSG_STATUS_GOOD, NULL, 0 );
      vUSBRx( &sMsg );
    }
    break;

  case MSG_SET_SENSOR_CONFIG:
    {
      vSetupMessage( &sMsg, MSG_SET_SENSOR_CONFIG, MSG_STATUS_GOOD, &sSensorConfigPacket.oPayload, sizeof(sSensorConfigPacket.oPayload.SensorConfig) );
      vUSBRx( &sMsg );
    }
    break;

  case MSG_SET_UI:
    {
      vSetupMessage( &sMsg, MSG_SET_UI, MSG_STATUS_GOOD, &sSetuiPacket.oPayload, sizeof(sSetuiPacket.oPayload.UIControl) );
      vUSBRx( &sMsg );
    }
    break;

  case MSG_CAPTURE_IMAGE:
    {
      vSetupMessage( &sMsg, MSG_CAPTURE_IMAGE, MSG_STATUS_GOOD, &sCaptureImagePacket.oPayload, sizeof(sCaptureImagePacket.oPayload.ScanRequest) );
      vUSBRx( &sMsg );
    }

    break;

  case MSG_CAPTURE_PROGRESS:
    {
      vSetupMessage( &sMsg, MSG_CAPTURE_PROGRESS, MSG_STATUS_GOOD, &sCaptureProgressPacket.oPayload, sizeof(sCaptureProgressPacket.oPayload.ScanProgress) );
      vUN20Rx( &sMsg );
    }
    break;

  case MSG_UN20_SHUTDOWN:
    {
      vSetupMessage( &sMsg, MSG_UN20_SHUTDOWN, MSG_STATUS_GOOD, NULL, 0 );
      vUSBRx( &sMsg );
    }
    break;

  case MSG_UN20_WAKEUP:
    {
      vSetupMessage( &sMsg, MSG_UN20_WAKEUP, MSG_STATUS_GOOD, NULL, 0 );
      vUSBRx( &sMsg );
    }
    break;

  case MSG_UN20_READY:
    {
      vSetupMessage( &sMsg, MSG_UN20_READY, MSG_STATUS_GOOD, NULL, 0 );
      vUN20Rx( &sMsg );
    }
    break;

  case MSG_UN20_GET_INFO:
    {
      vSetupMessage( &sMsg, (MSG_UN20_GET_INFO | MSG_REPLY), MSG_STATUS_GOOD, &sUn20GetInfoPacket.oPayload, sizeof(sUn20GetInfoPacket.oPayload.UN20Info) );
      vUN20Rx( &sMsg );
    }
    break;

  case MSG_IMAGE_QUALITY:        // Processed by UN20 App
    {
      vSetupMessage( &sMsg, MSG_IMAGE_QUALITY, MSG_STATUS_GOOD, NULL, 0 );
      vUSBRx( &sMsg );
    }
    break;

  case MSG_GENERATE_TEMPLATE:    // Processed by UN20 App
    {
      vSetupMessage( &sMsg, MSG_GENERATE_TEMPLATE, MSG_STATUS_GOOD, NULL, 0 );
      vUSBRx( &sMsg );
    }
    break;

  case MSG_GET_IMAGE_FRAGMENT:    // Processed by UN20 App
    {
      sGetImageFragment.oPayload.FragmentRequest.iFragmentNo = iArg;
      vSetupMessage( &sMsg, MSG_GET_IMAGE_FRAGMENT, MSG_STATUS_GOOD, &sGetImageFragment.oPayload, sizeof( sGetImageFragment.oPayload.FragmentRequest ));
      vUSBRx( &sMsg );
    }
    break;

  case MSG_GET_TEMPLATE_FRAGMENT:    // Processed by UN20 App
    {
      sGetImageFragment.oPayload.FragmentRequest.iFragmentNo = iArg;
      vSetupMessage( &sMsg, MSG_GET_TEMPLATE_FRAGMENT, MSG_STATUS_GOOD, &sGetImageFragment.oPayload, sizeof( sGetImageFragment.oPayload.FragmentRequest ));
      vUSBRx( &sMsg );
    }
    break;

  case MSG_STORE_IMAGE:    // Processed by UN20 App
    {
      vSetupMessage( &sMsg, MSG_STORE_IMAGE, MSG_STATUS_GOOD, &sSaveImage.oPayload, sizeof( sSaveImage.oPayload.StoreScan ));
      vUSBRx( &sMsg );
    }
    break;

  case MSG_GET_CRASH_LOG:    // Processed by lpc App
    {
      vSetupMessage( &sMsg, MSG_GET_CRASH_LOG, MSG_STATUS_GOOD, &sCrashLog.oPayload, sizeof( sCrashLog.oPayload.CrashLogRequest ));
      vUSBRx( &sMsg );
    }
    break;

  case MSG_SET_HW_CONFIG:    // Processed by lpc App
    {
      sHwConfig.oPayload.HardwareConfig.iHwConfig = iArg;
      vSetupMessage( &sMsg, MSG_SET_HW_CONFIG, MSG_STATUS_GOOD, &sHwConfig.oPayload, sizeof( sHwConfig.oPayload.HardwareConfig ));
      vUSBRx( &sMsg );
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

//
//
//
CGU_PERIPHERAL_T eClocks[] =
{
//  CGU_PERIPHERAL_ADC0,                                          /**< ADC0 		*/
  CGU_PERIPHERAL_ADC1,						/**< ADC1 		*/
  CGU_PERIPHERAL_AES,						/**< AES		*/
  //	CGU_PERIPHERAL_ALARMTIMER_CGU_RGU_RTC_WIC,
//  CGU_PERIPHERAL_APB1_BUS,					/**< APB1 bus			*/
//  CGU_PERIPHERAL_APB3_BUS,					/**< APB3 bus			*/
  CGU_PERIPHERAL_CAN,						/**< CAN 				*/
//  CGU_PERIPHERAL_CREG,						/**< CREG				*/
  CGU_PERIPHERAL_DAC,						/**< DAC				*/
  CGU_PERIPHERAL_DMA,						/**< DMA				*/
  CGU_PERIPHERAL_EMC,						/**< EMC				*/
  CGU_PERIPHERAL_ETHERNET,					/**< Ethernet			*/
  CGU_PERIPHERAL_ETHERNET_TX, //HIDE                            /**< Ethernet transmit 	*/
//  CGU_PERIPHERAL_GPIO,						/**< GPIO				*/
  CGU_PERIPHERAL_I2C0,						/**< I2C0				*/
  CGU_PERIPHERAL_I2C1,						/**< I2C1				*/
  CGU_PERIPHERAL_I2S,						/**< I2S				*/
  CGU_PERIPHERAL_LCD,						/**< LCD				*/
//  CGU_PERIPHERAL_M3CORE,					/**< ARM Cortex-M3 Core	*/
//  CGU_PERIPHERAL_M3_BUS,					/**< ARM Cortex-M3 Bus	*/
  CGU_PERIPHERAL_MOTOCON,					/**< Motor Control 		*/
  CGU_PERIPHERAL_QEI,						/**< QEI				*/
  CGU_PERIPHERAL_RITIMER,					/**< RIT Timer			*/
//  CGU_PERIPHERAL_SCT,						/**< SCT				*/
//  CGU_PERIPHERAL_SCU,						/**< SCU				*/
  CGU_PERIPHERAL_SDIO,						/**< SDIO				*/
  CGU_PERIPHERAL_SPIFI,						/**< SPIFI				*/
  CGU_PERIPHERAL_SSP0,						/**< SSP0				*/
  CGU_PERIPHERAL_SSP1,						/**< SSP1				*/
  CGU_PERIPHERAL_TIMER0,					/**< TIMER 0 			*/
  CGU_PERIPHERAL_TIMER1,					/**< TIMER 1			*/
  CGU_PERIPHERAL_TIMER2,					/**< TIMER 2			*/
  CGU_PERIPHERAL_TIMER3,					/**< TIMER 3			*/
//  CGU_PERIPHERAL_UART0,						/**< UART0				*/
//  CGU_PERIPHERAL_UART1,						/**< UART1				*/
  CGU_PERIPHERAL_UART2,						/**< UART2				*/
  CGU_PERIPHERAL_UART3,						/**< UART3				*/
  CGU_PERIPHERAL_USB0,						/**< USB0				*/
  CGU_PERIPHERAL_USB1,						/**< USB1				*/
//  CGU_PERIPHERAL_WWDT,						/**< WWDT				*/
};

void vHalInit(void)
{
  int iClock;

  DEBUG_MODULE_INIT( HAL_FD );

#if !defined(EVAL_BOARD)

  // disable all the clocks, for things we are not using
  for ( iClock = 0; iClock < ELEMENTSOF(eClocks); iClock++ )
  {
    CGU_ConfigPWR( eClocks[ iClock ], DISABLE );
  }

  DEBUG_GPIO0->vConfigure();
  DEBUG_GPIO1->vConfigure();
  DEBUG_GPIO2->vConfigure();
  DEBUG_GPIO3->vConfigure();

  // Make sure the UN20 is reset
  UN20B_nRESET->vConfigure();
  UN20B_nRESET->vSet( true );

  // Make sure the UN20 is powered off
  UN20B_POWER->vConfigure();
  UN20B_POWER->vSet( false );

  // Configure hardware control Pins
  LPC_USB_BYPASS_EN->vConfigure();
  LPC_BOOTLDR_EN->vConfigure();
  UN20_BOOTSEL0_EN->vConfigure();
  vSetHardwareConfig( MODE_NORMAL );

  // Hardware version information
  HW_ID_0->vConfigure();
  HW_ID_1->vConfigure();
  HW_ID_2->vConfigure();

  // Battery charging monitoring
  BAT_nCHRG->vConfigure();
  BAT_nVBUS->vConfigure();

#if 0
  BAT_MON_0->vConfigure();
  BAT_MON_1->vConfigure();
  BANDGAP->vConfigure();
#endif
#endif
  // Initialise the UI elements
  vUiInit();

  boCLIregisterEntry( &asHalMainCLI );
  
}

//
// Hal functions here
//

static void vNullHandler(void)
{
  PBC_ERROR( "NULL handler" );
}

static void vNullBtCallback(void *context,  tInterfaceEvent event, void *event_data)
{
}

static void vNullUsbCallback(void *context, tInterfaceEvent event, void *event_data)
{
}

//

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
  vGPIODDsetPinInterruptHandler( BUTTON_0_POWER,  PIN_INT1_IRQn, /*ieBoth*/ ieFalling, ::vUIIsr );
  vGPIODDsetPinInterruptHandler( BUTTON_1_SCAN, PIN_INT2_IRQn, /*ieBoth*/ ieFalling,  ::vUIIsr );
#endif
}

//
//
//
void vUiLedSet(tLeds eLed, tColorOptions eColor)
{
  PBC_ASSERT(eLed < LED_MAX_LED_COUNT);

  if ( oLedInfo[ eLed ].boIsMono )
  {
    PBC_ASSERT( (eColor == OFF) || (eColor == ON) );
#if !defined(EVAL_BOARD)
    oLedInfo[ eLed ].oGreenLed->vSet( (eColor == ON));
#endif
  }
  else
  {
    PBC_ASSERT( (eColor == OFF) || (eColor == RED) || (eColor == GREEN) || (eColor == ORANGE));
    if ( (oLedInfo[ eLed ].oGreenLed) && (oLedInfo[ eLed ].oRedLed))
    {
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

void vUn20Reset( bool boReset )	// reset the UN20
{
#if !defined(EVAL_BOARD)
  UN20B_nRESET->vSet( boReset );
#endif
  DEBUGMSG(ZONE_COMMANDS,("vUn20OReset: %s\n", (boReset ? "On" : "Off")));
}

void vPowerUsbOff()
{
#if !defined(EVAL_BOARD)
#endif
  DEBUGMSG(ZONE_COMMANDS,("vPowerUsbOff()\n"));
}

void vPowerUsbOn()
{
#if !defined(EVAL_BOARD)
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
  UN20B_nRESET->vSet( true );

  USB_VBUS_PWR_EN->vSet( false );

  UN20B_POWER->vSet( false );
#endif
}

//
// turn on power to the UN20 (takes 5 seconds to become ready)
//
void vPowerUn20On()
{
  tEventConnDisconn sEventData;
  PBC_ASSERT( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING );

  DEBUGMSG(ZONE_COMMANDS,("vPowerUn20On()\n"));

  if ( pvUsbPhoneCallback != NULL )
  {
    sEventData.eInterface = USB_UN20;
    pvUsbUn20Callback( pvUsbUn20Context, INTERFACE_EVENT_CONNECTED, &sEventData );
  }
#if !defined(EVAL_BOARD)
  UN20B_nRESET->vSet( true );
  UN20B_POWER->vSet( true );
  vTaskDelay(MS_TO_TICKS(UN20_POWERON_DELAY_MS));

  USB_VBUS_PWR_EN->vSet( true );

  UN20B_nRESET->vSet( false );

#endif
}

/* Not expected to return from this call
 */
void vPowerSelfOff()	// turn the LPC1800 off (Power button wakes)
{
  DEBUGMSG(ZONE_COMMANDS,("vPowerSelfOff()\n"));
#if !defined(EVAL_BOARD)
  BUTTON_0_POWER_OFF->vConfigure();
  BUTTON_0_POWER_OFF->vSet( false );
  //CLI_PRINT(("NOTE: Hardware power off disabled\n"));
#endif
  // spin waiting for the power to decay and the CPU to stop
  // must keep kicking the watchdog to stop it going off and
  // the subsequent reset re-enabling power (via CPU pullups)
  vWDOGDDstop();
//  for (;;)
//  {
//    vWDOGDDkick();
//  }
}

void vPowerSelfOn()	// called early to latch power on
{
  DEBUGMSG(ZONE_COMMANDS,("vPowerSelfOn()\n"));
}

//
// register for notification of power button press
//
void vPowerSelfNotify(vCallback pvCallback)
{
  DEBUGMSG(ZONE_COMMANDS,("vPowerSelfNotify()\n"));
  pvPoweroffCallback = pvCallback;
}
//
// read the battery voltages (2 channels available)
//
int iBatteryVoltage(int iChannel)
{
  int iChannelValue = (iChannel + 1) * 100;

  PBC_ASSERT( (iChannel >= 0) && (iChannel <= 1) );

#if !defined(EVAL_BOARD)
  poADC0 = poADCDDgetUnit( 0 );
  // read raw value from SDC
  iChannelValue = poADC0->iReadChannel( iChannel );

  // calculate result in mV taking into account VREF and conditioning logic
  iChannelValue = ( 3260 * 2 * iChannelValue ) / ( 1023 );
#endif

  DEBUGMSG(ZONE_COMMANDS,("iBatteryVoltage(%d):%d\n", iChannel, iChannelValue));

  //iReadBandGap();
  return iChannelValue;

}

//
// read the hardware version id from the board
//
int iHardwareVersion(void)
{
  int iVersion = 0;

#if !defined(EVAL_BOARD)
  iVersion |= ( HW_ID_0->boGet() ? 0x0 : 0x01 );
  iVersion |= ( HW_ID_1->boGet() ? 0x0 : 0x02 );
  iVersion |= ( HW_ID_2->boGet() ? 0x0 : 0x04 );
#endif
  return iVersion;
}

//
// Control the switches used to permit reprogramming of devices
//
void vSetHardwareConfig( int iMode )
{
  DEBUGMSG(ZONE_COMMANDS,("vSetHardwareConfig(%d)\n", iMode));

#if !defined(EVAL_BOARD)
  switch ( iMode )
  {
    case MODE_NORMAL:
      LPC_BOOTLDR_EN->vSet(false);
      LPC_USB_BYPASS_EN->vSet(false);
      UN20_BOOTSEL0_EN->vSet(false);
      break;
    case MODE_UN20_BOOTLOADER:
      LPC_BOOTLDR_EN->vSet(false);
      LPC_USB_BYPASS_EN->vSet(true);
      UN20_BOOTSEL0_EN->vSet(true);
      break;
    case MODE_UN20_FTP:
      LPC_BOOTLDR_EN->vSet(false);
      LPC_USB_BYPASS_EN->vSet(true);
      UN20_BOOTSEL0_EN->vSet(false);
      break;
    case MODE_LPC_BOOTLOADER:
      LPC_BOOTLDR_EN->vSet(true);
      LPC_USB_BYPASS_EN->vSet(false);
      UN20_BOOTSEL0_EN->vSet(false);
      break;
  }
#endif
}

//
// Return true if battery charger indicates it is charging the battery
//
bool boHalBatteryIsCharging()
{
  return BAT_nCHRG->boGet();
}

//
// Return true if the USB cable is present (with 5v on it)
//
bool boHalUSBChargePresent()
{
  return BAT_nVBUS->boGet();
}

//
// Bluetooth
//
int iBtInit(vBtCallback pvCallback, void *pvData)
{
#if !defined(EVAL_BOARD)
  BT_RESET->vConfigure();
#endif
  DEBUGMSG(ZONE_COMMANDS,("iBtInit()\n"));
  pvBtCallback = pvCallback;

  // Create the semaphore for unblocking the send operation
  vSemaphoreCreateBinary( hSendCompleteSemaphore );
}

// used to pass data to the callback function without exposing it
void vBtCallbackFunction(tInterfaceEvent event, void *event_data)
{
  pvBtCallback( pvBtContext, event, event_data );
}

// callback used to notify the outcome of the send operation call from BT stack thread)
static void sendComplete(int result)
{
  // release the thread that was waiting for the send to complete
  xSemaphoreGive( hSendCompleteSemaphore );
}

extern "C" int sppStartSendData(void *data, int count, void (*callback)(int result));

int iBtSend(void *data, int count)
{
  int iStartedOk;
  MsgPacketheader *psHeader = (MsgPacketheader *) data;

  DEBUGMSG(ZONE_COMMANDS,("iBtSend(): Length %d, MsgId 0x%02X, Status %d\n", count, psHeader->bMsgId, psHeader->bStatus ));

  iStartedOk = sppStartSendData(data, count, sendComplete);

  if ( iStartedOk )
  {
    // suspend caller waiting for send completion notification
    xSemaphoreTake( hSendCompleteSemaphore, portMAX_DELAY );
  }

  DEBUGMSG(ZONE_COMMANDS,("iBtSend(): Complete (%d)\n", iStartedOk));

  return iStartedOk;
}

int iBtReset( bool boReset )
{
#if !defined(EVAL_BOARD)
  BT_RESET->vSet( boReset );
#endif
  DEBUGMSG(ZONE_COMMANDS,("iBtReset: %s\n", (boReset ? "On" : "Off")));
}

//
// USB
//  

// used to pass data to the callback function without exposing it
void vUN20CallbackFunction(tInterfaceEvent event, void *event_data)
{
  pvUsbUn20Callback( pvUsbUn20Context, event, event_data );
}
 
static const tLineCoding sUN20portConfig = {
  /*.dwDTERate =*/ 230400/*115200*/,                          // Data terminal rate in bits per second
  /*.bCharFormat =*/ 0,                             // Number of stop bits
  /*.bParityType =*/ 0,                             // Parity bit type
  /*.bDataBits =*/ 8                                // Number of data bits
};

ISerialPort              *poUN20Port;


static void vUN20SerialSend(void *pvData, int iCount)
{
  uint8_t *pbData = (uint8_t*)pvData;

  while ( iCount-- > 0 )
  {
    poUN20Port->iPutchar( *pbData++ );
  }
}

static void vUN20Task(void* params)
{
  tEventConnDisconn oInterface;
  oInterface.eInterface = USB_UN20;

  poUN20Port = poSERDDgetPort( UN20_UART );
  poUN20Port->vConfigurePort( (tLineCoding const *)params, 300, 64 );

  // set to blocking mode for transmit but not receive
  poUN20Port->vSetBlockingMode( ISerialPort::bmTransmitOnly );

  // flush buffers before we start
  poUN20Port->vFlush();
#if 0
  // simulate the UN20 interface coming up
  vUN20CallbackFunction( INTERFACE_EVENT_CONNECTED, &oInterface );
#endif
  for(;;)
  {
    if ( poUN20Port->iHasChars() )
    {
      int iChar = poUN20Port->iGetchar();
      if ( iChar != EOF )
      {
        tEventRxData oRxData;
        char cChar = (char)iChar;

        // deliver the data to the HAL
        oRxData.iLength = 1;
        oRxData.pcData = &cChar;
        vUN20CallbackFunction( INTERFACE_EVENT_RX_DATA, &oRxData );
      }
    }
    vTaskDelay(1);
  }
}


static void vUN20SerialInit(tLineCoding const *poConfig)
{
  /* Create the UN20 serial task. */
  xTaskCreate( vUN20Task, ( signed char * ) "UN20", UN20_TASK_STACK_SIZE, ( void * ) poConfig, UN20_TASK_PRIORITY, NULL );
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
    vUN20SerialInit(&sUN20portConfig);
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

  switch ( eWhich )
  {
  case USB_UN20:
    vUN20SerialSend(pvData, iCount);
    break;

  case USB_HOST:
    break;

  default:
    // No action
    break;

  } // End switch

}
