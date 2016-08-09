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
// COMPONENT:    INDEX cli handling functions
// MODULE:       lpcapp.c
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
#include "lpcapp.h"
#include "msg_format.h"
#include "protocol_msg.h"

#include <stdio.h>

#include "cli.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

extern byte *pbGetBluetoothAddress(void);

//******************************************************************************
// Private Function Prototypes
//******************************************************************************

// Registered HAL callback functions.
static void vCallbackPowerOffHandler(void);
static void vBtCallbackHandler(void *context, tInterfaceEvent event, void *event_data);
static void vPhoneUsbCallbackHandler(void *context, tInterfaceEvent event, void *event_data);
static void vCallbackCaptureHandler(void);
static void vUn20UsbCallbackHandler(void *context, tInterfaceEvent event, void *event_data);

static void vVibTimerCallback( xTimerHandle xTimer );
static void vFlashTimerCallback( xTimerHandle xTimer );

static void vUn20ShutdownTimerCallback( xTimerHandle xTimer );
static void vUn20IdleTimerCallback( xTimerHandle xTimer );
static void vSystemIdleTimerCallback( xTimerHandle xTimer );

static void vMessageCompleteCallback( MsgInternalPacket *psMsg );
static void vMessageErrorCallback( tMsgError eErrorCode );

static void vReturnSensorInfo( MsgPacket *psMsg, int iMsglength );
static void vSetSensorConfig( MsgPacket *psMsg, int iMsglength );
static void vSetUi( MsgPacket *psMsg, int iMsglength );
static void vBtPair( MsgPacket *psMsg, int iMsglength );
static void vSetUn20Info( MsgPacket *psMsg, int iMsglength );

static void vUIReset();

static void vInterfaceConnDisconn( tInterfaceEvent event, tEventConnDisconn *psEventData );

//******************************************************************************
// Constants
//******************************************************************************
enum {
  ZONE_ERROR,
  ZONE_TRACE,
  ZONE_DATA
};

#define VIBRATE_OFF                      ( 0 )
#define VIBRATE_ON                       ( 1 )

// Flash timer tick rate in ms. This defines the minimum flash on/off time.
#define FLASH_TICK_MS                    ( 500 )

// Delay after requesting the UN20 to shutdown before we power it off.
#define UN20_SHUTDOWN_DELAY_MS           ( 4 * 1000 )

// Default UN20 idle timer value: 10 minutes in ms.
#define UN20_IDLE_TIMEOUT_MS             ( 10 * 60 * 1000 )

// Default system idle timer value: 60 minutes in ms.
#define SYSTEM_IDLE_TIMEOUT_MS             ( 60 * 60 * 1000 )

// Default delay before actioning a hardware configuration change
#define LPC_HARDWARE_CONFIG_DELAY_MS        ( 2 * 1000 )

// Minimum time that a button has to be pressed to be actioned (x2)
#define BUTTON_DEBOUNCE_DELAY_MS      ( 2 )

typedef enum
{
  // Possible Scanner states
  SFS_OFF,              // SFS is powered off
  SFS_CHARGING,         // SFS in charge mode
  SFS_SHUTTING_DOWN,    // SFS is shutting down (to deal with double power off events)
  SFS_ON,               // SFS is in operating mode
}
tScannerState;

//******************************************************************************
// Definitions
//******************************************************************************
#define BATT_UN20_REJECT_MV 3575      // refuse to start UN20 if below threshold
#define BATT_WARN_LOW_MV    3680      // flash RED LED below 3.68V
#define BATT_SHUTOFF_MV     3500      // power unit off below 3.5V
#define BATT_AVG_SAMPLES    5         // number of samples to average over

#define LPCAPP_MSG_QUEUE_SIZE 10

//******************************************************************************
// Local Storage
//******************************************************************************

DEBUG_MODULE_DEFINE( LPC_FD ) =
{
  "LPC_FD",
  { "Error", "Trace", "Data", 0 },
#ifdef DEBUG
  DEBUGZONE(ZONE_ERROR) | DEBUGZONE(ZONE_TRACE) | DEBUGZONE(ZONE_DATA)
#else
  DEBUGZONE(ZONE_ERROR)
#endif
};
DEBUG_MODULE_USE( LPC_FD );

static xQueueHandle hMsgQueue = NULL;

static xTimerHandle hUn20ShutdownTimer;
static xTimerHandle hUn20IdleTimer;         // Times idle seconds before UN20 power off
static xTimerHandle hInactivityTimer;       // Times idle seconds before total power off

static bool boPhoneBtConnected = false;
static bool boPhoneUsbConnected = false;
static bool boUn20UsbConnected = false;

static bool boNeedUn20Info = false;         // True if we want the UN20's config info. This is set
                                            // when the UN20 reports READY (to get the UN20 version information)

// Power off led sequence state
static int16 iPowerOffSequenceValue = 0;
static int16 iPowerOffSequenceStep = 0;

// Sensor Config items.

static int16 iPowerOffTimeoutSecs = 0;      // idle seconds before total power off
static int16 iUn20IdleTimeoutSecs = 0;      // idle seconds before UN20 power off
static int16 iGoodImageThreshold = 0;       // good image quality threshold for LED
static int16 iSmileThresholds[MAX_SMILE_STAGES]; // thesholds used to drive the smile
static int16 iScanTimeout = 0;              // default scan timeout
static int16 iScanBrightness = 0;           // default scan brightness
static int16 iRetryLimit = 0;               // default scan retry limit

// Set UI items.

static bool boEnableTrigger = false;        // True if the trigger button is enabled
static bool boSetLeds = false;              // LED setting
static bool boTriggerVibrate = false;       // True if vibrate requested
static uint8 bLedState[LED_MAX_USER_COUNT]; // off, red, green, orange, on, <flash choice>

static bool boFlashConnectionLed = false;   // True if we are flashing the blue Connection LED
static bool boFlashBatteryLed = false;      // True if we are flashing the red battery LED
static bool boCharging = false;             // True if battery is charging (forces charge led on)
static bool boVBUSPresent = false;          // True if USB cable connected
static bool boPowerButtonAtStartup = false; // True if Power button pressed at powerup
static int iLedState = 0;                   // current state of flashing LED


static int iCurrentBatteryVoltage = 0;
static int iAverageBatteryVoltage = 0;
static int iVoltageAccumulator = ((BATT_WARN_LOW_MV+1) * BATT_AVG_SAMPLES);

static xTimerHandle hFlashTimer;
static xTimerHandle hTimer;

// UN20 Config items.

static int16 iUn20Version = 0;	           // UN20 client firmware version (0 means not recovered yet)
static int16 iUn20StoreCount = 0;          // Number of stored images and templates

// message structure used for internally generated messages to the UN20
static MsgPacket sInternalMsg;

// message structures for power, scan button and UN20 shutdown processing
static MsgInternalPacket sPowerButtonMsg  = { .eSource = MSG_SOURCE_INTERNAL };
static MsgInternalPacket sScanButtonMsg   = { .eSource = MSG_SOURCE_INTERNAL };
static MsgInternalPacket sUN20ShutdownMsg = { .eSource = MSG_SOURCE_INTERNAL };
static MsgInternalPacket sPowerStateMsg   = { .eSource = MSG_SOURCE_INTERNAL };
static MsgInternalPacket sPowerToggleMsg  = { .eSource = MSG_SOURCE_INTERNAL };

// This app operates mostly stateless (except for some hardware peripheral states such as
// the vibrate motor, LED flash state etc.
// Other state information is defined here:

// current state of the UN20 reported to the outside world
static tUN20State eUN20State = UN20_STATE_SHUTDOWN;

// current state of the main scanner board
static tScannerState eScannerState = SFS_OFF;

enum
{
  // Internal message ID's used to process button events
  MSG_POWER_BUTTON = (MSG_NUM_MSGS + 0),
  MSG_SCAN_BUTTON  = (MSG_NUM_MSGS + 1),
  MSG_POWER_STATE  = (MSG_NUM_MSGS + 2),
  MSG_POWER_TOGGLE = (MSG_NUM_MSGS + 3)
};

enum
{
  IF_UN20 = (1 << MSG_SOURCE_UN20_USB),  // Send message to UN20
  IF_USB  = (1 << MSG_SOURCE_PHONE_USB), // Send message to Phone via USB
  IF_BT   = (1 << MSG_SOURCE_PHONE_BT)   // Send message to Phone via Bluetooth
};

//******************************************************************************
//******************************************************************************
// Private Functions
//******************************************************************************
//******************************************************************************
static bool boTest( char **papzArgs, int iInstance, int iNumArgs );

static const tParserEntry asLpcCLI[] =
{
  CLICMD("test",                  "Test", 1, "", boTest, 0),
};

tParserEntry asLpcMainCLI =
{
  DEBUG_MODULE_REF(LPC_FD), "LPC", "LPC commands", "", 1, 0, 0, asLpcCLI, ELEMENTSOF(asLpcCLI)
};

static bool boTest( char **papzArgs, int iInstance, int iNumArgs )
{
  CLI_PRINT(("Test\n"));

  return true;
}

// Send a message to one or more connected interfaces
static int iIfSend(int iWhere, void *pvData, int iLength)
{
  int iResult = 0;
  MsgPacket *psPacket = (MsgPacket*)pvData;

  DEBUGMSG(ZONE_DATA,
              ( "iIfSend: To %s%s%s length %d, MsgId 0x%02x, Status %02d\n",
              (( iWhere & IF_UN20) ? "UN20USB," : ""),
              (( iWhere & IF_USB)  ? "HostUSB," : ""),
              (( iWhere & IF_BT)   ? "HostBT," : ""),
              psPacket->Msgheader.iLength, psPacket->Msgheader.bMsgId, psPacket->Msgheader.bStatus ));

  if ( (iWhere & IF_UN20) && boUn20UsbConnected )
  {
    iResult |= iUsbSend( USB_UN20, pvData, iLength );
  }

  if ( (iWhere & IF_USB) && boPhoneUsbConnected )
  {
    iResult |= iUsbSend( USB_HOST, pvData, iLength );
  }

  if ( (iWhere & IF_BT) && boPhoneBtConnected )
  {
    iResult |=iBtSend( pvData, iLength );
  }
  return iResult;
}

// Callback for the LED-flash timer.
static void vFlashTimerCallback( xTimerHandle xTimer )
{
  // Toggle the LED state for this cycle
  iLedState ^= 1;

  // Re-evaluate LEDs needing to be flashed.

  // control state of the Bluetooth connection LED
  if ( eScannerState == SFS_ON )
  {
    if ( boFlashConnectionLed )
    {
      vUiLedSet( LED_CONNECTED, (iLedState ? ON : OFF) );
    }
    else
    {
      vUiLedSet( LED_CONNECTED, ON );
    }
  }
  else
  {
    vUiLedSet( LED_CONNECTED, OFF );
  }

  // control state of the charge indication LED's
  if ( boVBUSPresent ) 
  {
      if ( boCharging )
      {
        vUiLedSet( LED_BATTERY_RED, OFF );
        vUiLedSet( LED_BATTERY_GREEN, (iLedState ? ON : OFF) );
      }
      else
      {
        vUiLedSet( LED_BATTERY_RED, OFF );
        vUiLedSet( LED_BATTERY_GREEN, ON );
      }
  }
  else
  {
    if ( boFlashBatteryLed )
    {
      vUiLedSet( LED_BATTERY_RED, (iLedState ? ON : OFF) );
      vUiLedSet( LED_BATTERY_GREEN, OFF );
    }
    else
    {
      vUiLedSet( LED_BATTERY_RED, OFF );
      vUiLedSet( LED_BATTERY_GREEN, OFF );
    }
  }

  // Power off Sequence
  if (iPowerOffSequenceStep != 0) {
    for (int iLoop = 0 ; iLoop < LED_MAX_USER_COUNT; iLoop++) {
      vUiLedSet((tLeds) iLoop, (iLoop < iPowerOffSequenceValue/2) ? GREEN : OFF);
    }
    if (iLedState) {
      iPowerOffSequenceValue += iPowerOffSequenceStep;
    }
  }

  return;
}


static void vVibTimerCallback( xTimerHandle xTimer )
{
  // We simply turn the vibrate motor off.
  vUiVibrateControl ( VIBRATE_OFF );

  return;
}

static void vVibrateTrigger(int iVibrateMs)
{
  if( iVibrateMs > 0 )
  {
    // Start the vibrate motor.
    vUiVibrateControl ( VIBRATE_ON );

    // Set the new period.
    xTimerChangePeriod( hTimer,( iVibrateMs / portTICK_RATE_MS ), 0 );
   
    // Start the timer.
    xTimerStart( hTimer, 0 );
  }
}


static void vReturnSensorInfo( MsgPacket *psMsg, int iMsglength )
{
  MsgSensorInfo sInfo;
  tExceptionRecord *poException;
  word wLogEventLength;

  DEBUGMSG(ZONE_TRACE,("vReturnSensorInfo\n"));

  // Build up the payload.
  memcpy((void *) &sInfo.btAddr, (void *) pbGetBluetoothAddress, 6);
  sInfo.iUCVersion = INDEX_REVISION_NUMBER;
  sInfo.iUN20Version = iUn20Version;
  sInfo.iBatteryLevel1 = iBatteryVoltage( 0 );
  sInfo.iBatteryLevel2 = iBatteryVoltage( 1 );
  sInfo.eUN20State = eUN20State;
  sInfo.iCrashLogValid = ( boLogCacheGet( &poException, &wLogEventLength ) && poException->sHeader.dwReason != ERROR_REQUESTED );
  sInfo.iHwVersion = iHardwareVersion();

  vSetupMessage( psMsg, (MSG_GET_SENSOR_INFO | MSG_REPLY), MSG_STATUS_GOOD, &sInfo, sizeof( sInfo ) );

  DEBUGMSG(ZONE_TRACE,("Status: UN20:%d, Bat1:%d, Bat2:%d, Clog:%d, Hw:%d, UN20Ver:%d, LPCVer:%d\n",
                        psMsg->oPayload.SensorInfo.eUN20State,
                        psMsg->oPayload.SensorInfo.iBatteryLevel1,
                        psMsg->oPayload.SensorInfo.iBatteryLevel2,
                        psMsg->oPayload.SensorInfo.iCrashLogValid,
                        psMsg->oPayload.SensorInfo.iHwVersion,
                        psMsg->oPayload.SensorInfo.iUN20Version,
                        psMsg->oPayload.SensorInfo.iUCVersion));

  // Send the response message from the UN20 on to the phone
  iIfSend((IF_USB | IF_BT), psMsg, psMsg->Msgheader.iLength);

  return;
}

static void vSetSensorConfig( MsgPacket *psMsg, int iMsglength )
{
  MsgSensorConfig *psEventData;
  int iLoop;

  psEventData = (MsgSensorConfig *) &(psMsg->oPayload);

  iPowerOffTimeoutSecs = psEventData->iPowerOffTimeoutSecs; // idle seconds before total power off
  iUn20IdleTimeoutSecs = psEventData->iUn20IdleTimeoutSecs; // idle seconds before UN20 power off
  iGoodImageThreshold = psEventData->iGoodImageThreshold;  // good image quality threshold for LED - NOT USED

  for ( iLoop = 0; iLoop < MAX_SMILE_STAGES; iLoop++ )
  {
    iSmileThresholds[ iLoop ] = psEventData->iSmileThresholds[ iLoop ];
  }

  iScanTimeout = psEventData->iScanTimeout;         // default scan timeout - NOT USED
  iScanBrightness = psEventData->iScanBrightness;   // default scan brightness - NOT USED
  iRetryLimit = psEventData->iRetryLimit;           // default scan retry limit - NOT USED

  DEBUGMSG(ZONE_TRACE,( "vSetSensorConfig: PwrOffTO=%d, IdleTO=%d, GoodThr=%d, ScanTO=%d, ScanBr=%d, Retry=%d\n",
              iPowerOffTimeoutSecs, iUn20IdleTimeoutSecs, iGoodImageThreshold, iScanTimeout, iScanBrightness, iRetryLimit ));

  // Put the timeout items into action.
  xTimerStop( hUn20IdleTimer, 0 );
  xTimerStop( hInactivityTimer, 0 );
  xTimerChangePeriod( hUn20IdleTimer, SECS_TO_TICKS( iUn20IdleTimeoutSecs ), 0 );
  xTimerChangePeriod( hInactivityTimer, SECS_TO_TICKS( iPowerOffTimeoutSecs ), 0 );
  xTimerStart( hUn20IdleTimer, 0 );
  xTimerStart( hInactivityTimer, 0 );

  return;
}


static void vSetUi( MsgPacket *psMsg, int iMsglength )
{
  MsgUIControl *psEventData;
  int16 iVibrateMs;
  int iLoop;

  psEventData = (MsgUIControl *) &(psMsg->oPayload);

  boEnableTrigger = psEventData->boEnableTrigger;           // enable the trigger button
  boSetLeds = psEventData->boSetLeds;                       // set LED's according to bLedState
  boTriggerVibrate = psEventData->boTriggerVibrate;         // set vibrate according to boVibrateState
  iVibrateMs = psEventData->iVibrateMs;                     // 0 = off, > 0 trigger vibrate for Ms (then stop)

  for ( iLoop = 0; iLoop < LED_MAX_USER_COUNT; iLoop++ )
  {
    bLedState[ iLoop ] = psEventData->bLedState[ iLoop ];
  }

  DEBUGMSG(ZONE_TRACE,( "vSetUi: EnTrig=%d, SetLeds=%d, TrigVib=%d, VibMs=%d\n",
                      boEnableTrigger, boSetLeds, boTriggerVibrate, iVibrateMs ));

  // Put these items into action.

  if ( boSetLeds == true )
  {
    for ( iLoop = 0; iLoop < LED_MAX_USER_COUNT; iLoop++ )
    {
      //CLI_PRINT(( "vSetUi: LED %d state %d\n", iLoop, bLedState[ iLoop ] ));

      vUiLedSet ((tLeds) iLoop, (tColorOptions) bLedState[ iLoop ] );
    }
  }

  if ( boTriggerVibrate == true )
  {
    // Notify the vibrate period to our vibrate handler and let it run the vibration.
    vVibrateTrigger( iVibrateMs );
  }

  // setup an ACK message and send it
  vSetupACK( psMsg );

  // Send the response message from the UN20 on to the phone
  iIfSend((IF_USB | IF_BT), psMsg, psMsg->Msgheader.iLength);

  return;
}


static void vBtPair( MsgPacket *psMsg, int iMsglength )
{
  DEBUGMSG(ZONE_TRACE,("vBtPair\n"));

  return;
}

// Called for a received MSG_UN20_GET_INFO response message from the UN20.
static void vSetUn20Info( MsgPacket *psMsg, int iMsglength )
{
  MsgUN20Info *psEventData;

  psEventData = (MsgUN20Info *) &(psMsg->oPayload);

  iUn20Version = psEventData->iVersion;
  iUn20StoreCount = psEventData->iStoreCount;

  // We don't immediately need to re-get the config info.
  boNeedUn20Info = false;

  DEBUGMSG(ZONE_TRACE,("vSetUn20Info: Ver 0x%04X, StoreCount %d\n", iUn20Version, iUn20StoreCount));

  return;
}

// Queue the completed message for processing. NOTE: DOES NOT TAKE A COPY OF THE MESSAGE
static void vQueueMessageCompleteCallback( MsgInternalPacket *psMsg )
{
  xQueueSendToBack( hMsgQueue, &psMsg, RTOS_FOREVER );
}

// Queue the ISR generated message for processing. NOTE: DOES NOT TAKE A COPY OF THE MESSAGE
static void vQueueMessageCompleteCallbackISR( MsgInternalPacket *psMsg )
{
  portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
  xQueueSendToBackFromISR( hMsgQueue, &psMsg, &xHigherPriorityTaskWoken );
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// Kick off power-off led dance switch
static void vKickOffShutdownLEDs( void )
{
  // The power off sequence value will go from LED_MAX_USER_COUNT*2 to 0
  // by power off step
  if (iPowerOffSequenceStep == 0) {
    iPowerOffSequenceValue = LED_MAX_USER_COUNT*2;
    iPowerOffSequenceStep = (eUN20State != UN20_STATE_SHUTDOWN) ? -1 : -2;
    }
}

// Called when a protocol message has been received from the UN20 or phone.
static void vMessageProcess( MsgInternalPacket *psMsg )
{
  uint8 bSource;
  MsgPacket *psPacket;
  uint16 iMsglength;
  MsgPacketheader *psHeader;

  // By the time this callback is called, we know that the message:
  // - has a valid source
  // - has valid header and footer syncwords
  // - has a valid length
  // - has a valid message-id

  bSource = psMsg->eSource;
  psPacket = &(psMsg->oMsg);
  psHeader = &(psMsg->oMsg.Msgheader);
  iMsglength = psHeader->iLength;

  DEBUGMSG(ZONE_DATA,
                ( "vMessageCompleteCallback: From %s, length %d, MsgId 0x%02X\n",
                ( bSource == MSG_SOURCE_PHONE_BT )  ? "HostBT"  :
                ( bSource == MSG_SOURCE_PHONE_USB ) ? "HostUSB" :
                ( bSource == MSG_SOURCE_UN20_USB )  ? "UN20USB" : "Internal",
                iMsglength, psMsg->oMsg.Msgheader.bMsgId ));

  // Reset the system inactivity timeout.
  xTimerReset( hInactivityTimer, 0 );

  // if scanner is not "on" reject commands from USB and Bluetooth
  if ( ( bSource != MSG_SOURCE_INTERNAL ) && ( eScannerState != SFS_ON ) )
  {
    DEBUGMSG(ZONE_TRACE,("Rejecting command - scanner is in charge mode\n"));
    vSetupNACK( psPacket, MSG_STATUS_CHARGING );

    // send NACK and exit quickly
    iIfSend((IF_USB | IF_BT), psPacket, psPacket->Msgheader.iLength);
    return;
  }

  // Decode the message type (ignoring the Response bit).
  switch ( psMsg->oMsg.Msgheader.bMsgId & ~MSG_REPLY )
  {
  // Messages sent to scanner application.
  case MSG_GET_SENSOR_INFO:

    if (( bSource == MSG_SOURCE_PHONE_BT ) || ( bSource == MSG_SOURCE_PHONE_USB ))
    {
      vReturnSensorInfo( &(psMsg->oMsg), iMsglength );
    }
    // Else invalid source - discard the message.
    break;

  case MSG_SET_SENSOR_CONFIG:

    if (( bSource == MSG_SOURCE_PHONE_BT ) || ( bSource == MSG_SOURCE_PHONE_USB ))
    {
      vSetSensorConfig( &(psMsg->oMsg), iMsglength );
    }
    // Else invalid source - discard the message.
    break;


  case MSG_SET_UI:

    if (( bSource == MSG_SOURCE_PHONE_BT ) || ( bSource == MSG_SOURCE_PHONE_USB ))
    {
      vSetUi( &(psMsg->oMsg), iMsglength );
    }
    // Else invalid source - discard the message.
    break;

  case MSG_PAIR:

    if (( bSource == MSG_SOURCE_PHONE_BT ) || ( bSource == MSG_SOURCE_PHONE_USB ))
    {
      vBtPair( &(psMsg->oMsg), iMsglength );
    }
    // Else invalid source - discard the message.
    break;

  // Messages sent to/from UN20 application.

  case MSG_UN20_GET_INFO:

    if ( bSource == MSG_SOURCE_UN20_USB )
    {
      vSetUn20Info( &(psMsg->oMsg), iMsglength );
    }
    // Else invalid source - discard the message.

    break;

  case MSG_UN20_WAKEUP:
    if (( bSource == MSG_SOURCE_PHONE_BT ) || ( bSource == MSG_SOURCE_PHONE_USB ))
    {
      if ( iAverageBatteryVoltage > BATT_UN20_REJECT_MV )
      {
        DEBUGMSG(ZONE_TRACE,("%sWaking up UN20\n", (eUN20State == UN20_STATE_SHUTDOWN ? "" :"Not ")));
        switch ( eUN20State )
        {
          case UN20_STATE_READY:
          case UN20_STATE_STARTING_UP:
            // already actioned required state change
            vSetupACK( psPacket );
            break;

          case UN20_STATE_SHUTTING_DOWN:
            // in the process of shutting down so cant stop it
            vSetupNACK( psPacket, MSG_STATUS_UN20_STATE_ERROR );
            break;

          case UN20_STATE_SHUTDOWN:
            // Start the UN20 powering up.
            eUN20State = UN20_STATE_STARTING_UP;
            vPowerUn20On();
            vSetupACK( psPacket );

            // restart the UN20 inactivity timer.
            xTimerReset( hUn20IdleTimer, 0 );

            break;
        }
      }
      else
      {
        DEBUGMSG(ZONE_TRACE,("Not Waking up UN20 - battery voltage too low\n"));
        vSetupNACK( psPacket, MSG_STATUS_UN20_VOLTAGE );
      }

      // send ACK or NACK on behalf of the UN20 as it cannot respond
      iIfSend((IF_USB | IF_BT), psPacket, psPacket->Msgheader.iLength);
    }
    break;

  case MSG_UN20_READY:

    // This is a UN20 state message to say the UN20 is powered up and ready

    if ( bSource == MSG_SOURCE_UN20_USB )
    {
      eUN20State = UN20_STATE_READY;

      // Now is a good time to request the UN20's current config info.
      boNeedUn20Info = true;
    }
          
    break;
    
  case MSG_UN20_SHUTDOWN:
  case MSG_UN20_SHUTDOWN_NO_ACK:
    if ( bSource == MSG_SOURCE_UN20_USB )
    {
      // Send the response message from the UN20 on to the phone
      iIfSend((IF_USB | IF_BT), psPacket, psPacket->Msgheader.iLength);
    }
    else
    {
      DEBUGMSG(ZONE_TRACE,("%sShutting down UN20\n", (eUN20State == UN20_STATE_READY ? "" : "Not ")));

      switch ( eUN20State )
      {
        case UN20_STATE_READY:
          // Start the UN20 powering off.
          eUN20State = UN20_STATE_SHUTTING_DOWN;
          // response will come from the UN20 which we forward on
          iIfSend(IF_UN20, psPacket, psPacket->Msgheader.iLength);

          // stop the UN20 inactivity timer.
          xTimerStop( hUn20IdleTimer, 0 );

          // Start the UN20 shutdown timer - this gives the UN20 time to halt.
          xTimerStart( hUn20ShutdownTimer, 0 );
          break;

        case UN20_STATE_STARTING_UP:
          // starting up so cant stop it, reject request
          vSetupNACK( psPacket, MSG_STATUS_UN20_STATE_ERROR );
          if ( bSource != MSG_SOURCE_INTERNAL )
          { // dont forward responses for internal messages
            iIfSend((IF_USB | IF_BT), psPacket, psPacket->Msgheader.iLength);
          }
          break;

        case UN20_STATE_SHUTDOWN:
        case UN20_STATE_SHUTTING_DOWN:
          // already actioned required state change
          vSetupACK( psPacket );
          if ( bSource != MSG_SOURCE_INTERNAL )
          { // dont forward responses for internal messages
            iIfSend((IF_USB | IF_BT), psPacket, psPacket->Msgheader.iLength);
          }
          break;
      }
    }
    break;

  case MSG_CAPTURE_IMAGE:
  case MSG_CAPTURE_PROGRESS:
  case MSG_CAPTURE_ABORT:
  case MSG_RECOVER_IMAGE:
  case MSG_IMAGE_FRAGMENT:
  case MSG_STORE_IMAGE:
  case MSG_IMAGE_QUALITY:
  case MSG_GENERATE_TEMPLATE:
  case MSG_RECOVER_TEMPLATE:
  case MSG_COMPARE_TEMPLATE:
  case MSG_GET_IMAGE_FRAGMENT:
  case MSG_GET_TEMPLATE_FRAGMENT:

    // These are UN20 image activity messages.

    // We don't process phone messages if a UN20 shutdown in progress (or completed).
    if (( bSource == MSG_SOURCE_PHONE_BT ) || ( bSource == MSG_SOURCE_PHONE_USB ))
    {
      if ( eUN20State != UN20_STATE_READY )
      {
        // The UN20 is in the wrong state to process this message.
        // We just return an error response.
      
        DEBUGMSG(ZONE_TRACE,("Sending UN20-shutdown error response to phone\n"));

        vSetupNACK( psPacket, MSG_STATUS_UN20_STATE_ERROR );

        // Send the response message from the UN20 on to the phone
        iIfSend((IF_USB | IF_BT), psPacket, psPacket->Msgheader.iLength);

        // No further action is taken for this message
        break;
      }
    }

    // We also restart the UN20 inactivity timeout.
    xTimerReset( hUn20IdleTimer, 0 );

    // By default we just echo these message types to the other end.
    
    if (( bSource == MSG_SOURCE_PHONE_BT ) || ( bSource == MSG_SOURCE_PHONE_USB ))
    {
      iIfSend(IF_UN20, psPacket, psPacket->Msgheader.iLength);
    }
    else if ( bSource == MSG_SOURCE_UN20_USB )
    {
      // Send the response message from the UN20 on to the phone
      iIfSend((IF_USB | IF_BT), psPacket, psPacket->Msgheader.iLength);
    }
    // Else invalid source - discard the message.
    break;

  case MSG_POWER_STATE:
    // Either the battery charge state or the state of the USB charge has changed
    CLI_PRINT(("*** Power state change ***\n"));
 
    switch ( eScannerState )
    {
      case SFS_OFF:
        if ( boVBUSPresent )
        {
          // powered up by USB charge source go to charging mode
          CLI_PRINT(("*** Power up: Enter charging state ***\n"));
          eScannerState = SFS_CHARGING;
        }
        else
        {
          // assume powered up with button pressed request entry to full on mode
          CLI_PRINT(("*** Power up: Enter power-on state ***\n"));
          boPowerButtonAtStartup = false;
          vSystemIdleTimerCallback( NULL );
        }
        break;

      case SFS_CHARGING:
        if ( !boVBUSPresent )
        {
          CLI_PRINT(("*** Charging: charge source lost - turning off ***\n"));
          eScannerState = SFS_SHUTTING_DOWN;
          // Reset led power off values
          iPowerOffSequenceValue = 0;
          iPowerOffSequenceStep = 0;
          vKickOffShutdownLEDs();
          vPowerSelfOff();
        }
        break;

      case SFS_ON:
        // no action required. Pressing power button will cause reevaluation of power state
        break;
    }
    break;

  case MSG_POWER_BUTTON:
    {
      bool boButtonSample0;
      bool boButtonSample1;

      // Require the power button to remain pressed for 2 consecutive sample periods
      vTaskDelay( MS_TO_TICKS( BUTTON_DEBOUNCE_DELAY_MS ) );
      boButtonSample0 = boHalPowerButtonPressed();

      vTaskDelay( MS_TO_TICKS( BUTTON_DEBOUNCE_DELAY_MS ) );
      boButtonSample1 = boHalPowerButtonPressed();

      // if button did not remain pressed then ignore this notification
      if ( !(boButtonSample0 && boButtonSample1) )
      {
        CLI_PRINT(("*** Ignorning spurious power button event ***\n"));
        return;
      }
    }
    // fall-thru to normal button processing code.
  case MSG_POWER_TOGGLE:
    // power button has been pressed to either turn us on or off
    CLI_PRINT(("*** Turning %s ***\n", (eScannerState == SFS_ON) ? "Off" : "On"));

    if ( eScannerState == SFS_SHUTTING_DOWN )
      eScannerState == SFS_OFF;
    else if ( eScannerState == SFS_ON )
    {
      // cancel any pending transfers and disconnect from remote device (if any)
      {
        extern int sppCancelAndDisconnect();
        sppCancelAndDisconnect();
      }

      // now determine if we enter charging mode or turn off
      // do this before shutting down the UN20 because the time delay causes mixed led output (shutting down + BT)
      eScannerState = boVBUSPresent?SFS_CHARGING:SFS_SHUTTING_DOWN;

      vKickOffShutdownLEDs();

      if ( eUN20State != UN20_STATE_SHUTDOWN )
      {
        // Cleanly shut down the UN20 app if it is running (UN20 powered up).
        vSetupMessage( &sInternalMsg, MSG_UN20_SHUTDOWN_NO_ACK, MSG_STATUS_GOOD, NULL, 0 );

        // send shutdown message to UN20
        iIfSend(IF_UN20, (void *) &sInternalMsg, sInternalMsg.Msgheader.iLength);

        // Pause here for a short while to allow the UN20 to cleanly shut down.
        vTaskDelay( MS_TO_TICKS( UN20_SHUTDOWN_DELAY_MS ));
      }

      // Power down the UN20.
      vPowerUn20Off();

      // now determine if we enter charging mode or turn off
      if ( eScannerState == SFS_SHUTTING_DOWN )
      {
        // no charge source present so shutdown
        vPowerSelfOff();
      }
    }
    else
    {
      eScannerState = SFS_ON;

      // Reset led power off values
      iPowerOffSequenceValue = 0;
      iPowerOffSequenceStep = 0;

      // Perform a start-up dance on the LEDs.
      vUiLedSet(LED_RING_0, GREEN);
      vUiLedSet(LED_RING_1, GREEN);
      vUiLedSet(LED_RING_2, GREEN);
      vUiLedSet(LED_RING_3, GREEN);
      vUiLedSet(LED_RING_4, GREEN);

      vTaskDelay( 1000 );

      // restore startup UI
      vUIReset();
    }

    break;

  case MSG_SCAN_BUTTON:
  {
      bool boButtonSample0;
      bool boButtonSample1;

      // Require the power button to remain pressed for 2 consecutive sample periods
      vTaskDelay( MS_TO_TICKS( BUTTON_DEBOUNCE_DELAY_MS ) );
      boButtonSample0 = boHalScanButtonPressed();

      vTaskDelay( MS_TO_TICKS( BUTTON_DEBOUNCE_DELAY_MS ) );
      boButtonSample1 = boHalScanButtonPressed();

      // if button did not remain pressed then ignore this notification
      if ( !(boButtonSample0 && boButtonSample1) )
      {
        CLI_PRINT(("*** Ignorning spurious scan button event ***\n"));
        return;
      }

      DEBUGMSG(ZONE_TRACE,("*** Scan request ***\n"));

      // If the capture button is enabled, we pass this indication on to the phone.
      if ( boEnableTrigger == true )
      {
        MsgUINotification sUINotification;
        sUINotification.boTriggerPressed = true;

        vSetupMessage( &sInternalMsg, MSG_REPORT_UI, MSG_STATUS_GOOD, &sUINotification, sizeof( sUINotification ) );

        // Send the response message from the UN20 on to the phone
        iIfSend((IF_USB | IF_BT), (void *) &sInternalMsg, sInternalMsg.Msgheader.iLength);
      }
    }
    // Else ignore this button press.
    break;

  case MSG_GET_CRASH_LOG:
  {
    tExceptionRecord *poException;
    word wLogEventLength;

    // if the log is valid send it and then clear it, otherwise reject the request
    if ( boLogCacheGet( &poException, &wLogEventLength ) )
    {
      sInternalMsg.oPayload.CrashLogResponse.iSize = wLogEventLength;
      memcpy(&sInternalMsg.oPayload.CrashLogResponse.bData, poException, wLogEventLength);

      vSetupMessage( psPacket, (MSG_GET_CRASH_LOG | MSG_REPLY), MSG_STATUS_GOOD,
                     &sInternalMsg.oPayload, sizeof(sInternalMsg.oPayload.CrashLogResponse.iSize) + wLogEventLength );
#ifdef DEBUG
    vPrintCrashRecord( poException );
#endif
      vLogCacheClear();
    }
    else
    {
      vSetupNACK( psPacket, MSG_STATUS_NO_CRASH_LOG );
    }

    // Send the response message  on to the phone
    iIfSend((IF_USB | IF_BT), (void *) psPacket, psPacket->Msgheader.iLength);
  }
    break;

  case MSG_SET_HW_CONFIG:
  {
    int iHwConfig = psPacket->oPayload.HardwareConfig.iHwConfig;

    if ( eUN20State != UN20_STATE_SHUTDOWN )
    {
      vSetupNACK( psPacket, MSG_STATUS_UN20_STATE_ERROR );
    }
    else if ( (iHwConfig != HW_MODE_NORMAL ) &&
              (iHwConfig != HW_MODE_UN20_BOOTLOADER ) &&
              (iHwConfig != HW_MODE_UN20_FTP ) &&
              (iHwConfig != HW_MODE_LPC_BOOTLOADER ) )
    {
      vSetupNACK( psPacket, MSG_STATUS_BAD_PARAMETER );
    }
    else
    {
      vSetupACK( psPacket );
    }

    // Send the response message from to the phone
    iIfSend((IF_USB | IF_BT), (void *) psPacket, psPacket->Msgheader.iLength);

    // small delay before we action the hardware changes
    vTaskDelay( MS_TO_TICKS( LPC_HARDWARE_CONFIG_DELAY_MS ));

    switch ( iHwConfig )
    {
      case HW_MODE_NORMAL:
        vSetHardwareConfig( MODE_NORMAL );
        vPowerUn20Off();
        break;
      case HW_MODE_UN20_BOOTLOADER:
        vSetHardwareConfig( MODE_UN20_BOOTLOADER );
        vPowerUn20On();
        break;
      case HW_MODE_UN20_FTP:
        vSetHardwareConfig( MODE_UN20_FTP );
        vPowerUn20On();
        break;
      case HW_MODE_LPC_BOOTLOADER:
        vSetHardwareConfig( MODE_LPC_BOOTLOADER );
        vLogGeneral( ERROR_REQUESTED, 0, 0 );
        break;
      default:
        break;
    }
    break;
  }
  case MSG_REPORT_UI:
  case MSG_UN20_WAKINGUP:
  default:
    // Invalid or unexpected message type - discard the message.
    DEBUGMSG(ZONE_ERROR,("*** Ignoring message\n"));

    if (( bSource == MSG_SOURCE_PHONE_BT ) || ( bSource == MSG_SOURCE_PHONE_USB ))
    {
      vSetupNACK( psPacket, MSG_STATUS_UNSUPPORTED );
      // Send the response message to the phone
      iIfSend((IF_USB | IF_BT), psPacket, psPacket->Msgheader.iLength);
    }
    break;

  } // End switch

  return;
}

static void vMessageErrorCallback( tMsgError eErrorCode )
{
  DEBUGMSG(ZONE_ERROR,( "vMessageErrorCallback: error %d\n", eErrorCode ));

  return;
}

static void vInterfaceConnDisconn( tInterfaceEvent event, tEventConnDisconn *psEventData )
{
  switch( event )
  {
  case INTERFACE_EVENT_CONNECTED:
    {
      if( psEventData->eInterface == USB_UN20 )
      {
         DEBUGMSG(ZONE_TRACE,( "UN20 USB Connected\n" ));

         boUn20UsbConnected = true;
      }
      else if( psEventData->eInterface == USB_HOST )
      {
        DEBUGMSG(ZONE_TRACE,( "Phone USB Connected\n" ));

        boPhoneUsbConnected = true;
      }
      else if ( psEventData->eInterface == BT_HOST )
      {
        DEBUGMSG(ZONE_TRACE,( "Phone BT Connected\n" ));

        boPhoneBtConnected = true;
      }
      // Else unknown interface.
    }

    // Check for a connection to the phone.
    if (( boPhoneBtConnected == true ) || ( boPhoneUsbConnected == true ))
    {
      boFlashConnectionLed = false;
    }
    break;

  case INTERFACE_EVENT_DISCONNECTED:
    {
      if( psEventData->eInterface == USB_UN20 )
      {
        DEBUGMSG(ZONE_TRACE,( "UN20 USB Disconnected\n" ));

        boUn20UsbConnected = false;
      }
      else if( psEventData->eInterface == USB_HOST )
      {
        DEBUGMSG(ZONE_TRACE,( "Phone USB Disconnected\n" ));

        boPhoneUsbConnected = false;
      }
      else if ( psEventData->eInterface == BT_HOST )
      {
        DEBUGMSG(ZONE_TRACE,( "Phone BT Disconnected\n" ));

        boPhoneBtConnected = false;
      }
      // Else unknown interface.
    }
    break;

    default:
      break;

    } // End switch

    // Check for total disconnection from the phone.
    if (( boPhoneBtConnected == false ) && ( boPhoneUsbConnected == false ))
    {
      vUIReset();
      boFlashConnectionLed = true;
    }

  // Reset the interfaces packet assembler when the interface connects and disconnects
  switch (psEventData->eInterface)
  {
    case BT_HOST:
      vProtocolReset( MSG_SOURCE_PHONE_BT );
      break;
    case USB_HOST:
      vProtocolReset( MSG_SOURCE_PHONE_USB );
      break;
    case USB_UN20:
      vProtocolReset( MSG_SOURCE_UN20_USB );
      break;
  }
  return;
}


//******************************************************************************
// Private HAL callback functions
//******************************************************************************

static void vBtCallbackHandler(void *context, tInterfaceEvent event, void *event_data)
{
  DEBUGMSG(ZONE_DATA,("vBtCallbackHandler: event %d\n", event));

  // Reset the system inactivity timeout.
  xTimerReset( hInactivityTimer, 0 );

  switch( event )
  {
  case INTERFACE_EVENT_CONNECTED:
  case INTERFACE_EVENT_DISCONNECTED:

    vInterfaceConnDisconn( event, (tEventConnDisconn *) event_data );
    break;

  case INTERFACE_EVENT_RX_DATA:
    {
      tEventRxData *psEventData = event_data;
      vIncomingBytes( MSG_SOURCE_PHONE_BT, (char *) psEventData->pcData, psEventData->iLength );
    }
    break;

  default:
    break;

  } // End switch
}

static void vUn20UsbCallbackHandler(void *context, tInterfaceEvent event, void *event_data)
{
  PRECOND( event_data != NULL );
  
  //CLI_PRINT(("vUn20UsbCallbackHandler: event %d\n", event));

  // Reset the system inactivity timeout.
  xTimerReset( hInactivityTimer, 0 );

  switch( event )
  {
  case INTERFACE_EVENT_CONNECTED:
  case INTERFACE_EVENT_DISCONNECTED:

    DEBUGMSG(ZONE_DATA,("vUn20UsbCallbackHandler: event %d\n", event));
    vInterfaceConnDisconn( event, (tEventConnDisconn *) event_data );
    break;

  case INTERFACE_EVENT_RX_DATA:
    {
      tEventRxData *psEventData = event_data;
      if ( psEventData->iLength > 1 )
      {
        DEBUGMSG(ZONE_DATA,("vUn20UsbCallbackHandler: event %d\n", event));
      }
      vIncomingBytes( MSG_SOURCE_UN20_USB, (char *) psEventData->pcData, psEventData->iLength );
    }
    break;

  default:
    break;

  } // End switch
}

static void vPhoneUsbCallbackHandler(void *context, tInterfaceEvent event, void *event_data)
{
  PRECOND( event_data != NULL );
  
  DEBUGMSG(ZONE_DATA,("vPhoneUsbCallbackHandler: event %d\n", event));

  // Reset the system inactivity timeout.
  xTimerReset( hInactivityTimer, 0 );

  switch( event )
  {
  case INTERFACE_EVENT_CONNECTED:
  case INTERFACE_EVENT_DISCONNECTED:

    vInterfaceConnDisconn( event, (tEventConnDisconn *) event_data );
    break;

  case INTERFACE_EVENT_RX_DATA:
    {
      tEventRxData *psEventData = event_data;
      vIncomingBytes( MSG_SOURCE_PHONE_USB, (char *) psEventData->pcData, psEventData->iLength );
      }
    break;

  default:
    break;

  } // End switch
}

// Callback for the UN20 poweroff timer.
static void vUn20ShutdownTimerCallback( xTimerHandle xTimer )
{
  DEBUGMSG(ZONE_TRACE,("vUn20ShutdownTimerCallback\n"));

  eUN20State = UN20_STATE_SHUTDOWN;

  vPowerUn20Off();

  return;
}


// Callback for the UN20 inactivity (idle) timer.
static void vUn20IdleTimerCallback( xTimerHandle xTimer )
{
  DEBUGMSG(ZONE_TRACE,("vUn20IdleTimerCallback\n"));

  // Send a special UN20 shutdown message that generates no ACK to the phone
  vSetupMessage( &sUN20ShutdownMsg.oMsg, MSG_UN20_SHUTDOWN_NO_ACK, MSG_STATUS_GOOD, NULL, 0 );
  vQueueMessageCompleteCallback( &sUN20ShutdownMsg  );
  return;
}

// Callback for system inactivity (Idle) timeout.
static void vSystemIdleTimerCallback( xTimerHandle xTimer )
{
  DEBUGMSG(ZONE_TRACE,("vSystemIdleTimerCallback\n"));

  // We take the same actions as when the user presses the power button.
  sPowerToggleMsg.oMsg.Msgheader.bMsgId = MSG_POWER_TOGGLE;
  vQueueMessageCompleteCallback( &sPowerToggleMsg  );
  return;
}

// Post message indicating change of charge state.
static void vSystemPowerStateChange(void)
{
  DEBUGMSG(ZONE_TRACE,("vSystemPowerStateChange\n"));

  // Notify the system that the charge state has changed.
  sPowerStateMsg.oMsg.Msgheader.bMsgId = MSG_POWER_STATE;
  vQueueMessageCompleteCallback( &sPowerStateMsg  );
  return;
}

// Power button pressed. Called from ISR context
static void vCallbackPowerOffHandler(void)
{
  sPowerButtonMsg.oMsg.Msgheader.bMsgId = MSG_POWER_BUTTON;
  vQueueMessageCompleteCallbackISR( &sPowerButtonMsg );
}

// Capture button pressed. Called from ISR context
static void vCallbackCaptureHandler(void)
{
  sScanButtonMsg.oMsg.Msgheader.bMsgId = MSG_SCAN_BUTTON;
  vQueueMessageCompleteCallbackISR( &sScanButtonMsg );
}

// reset the UI elements controlled by the App
static void vUIReset()
{
  vUiLedSet(LED_RING_0, OFF);
  vUiLedSet(LED_RING_1, OFF);
  vUiLedSet(LED_RING_2, OFF);
  vUiLedSet(LED_RING_3, OFF);
  vUiLedSet(LED_RING_4, OFF);
}

//******************************************************************************
//******************************************************************************
// Public Functions
//******************************************************************************
//******************************************************************************
void vLpcAppInit( bool boPowerButtonState )
{
  const char acShutdownTimerName[] = "UN20 shutdown timer";
  const char acUn20IdleTimerName[] = "UN20 idle timer";
  const char acInactivityTimerName[] = "System idle timer";
  const char acFlashTimerName[] = "Flash timer";
  const char acVibrateTimerName[] = "Vibrate timer";

  boPowerButtonAtStartup = boPowerButtonState;

  DEBUG_MODULE_INIT( LPC_FD );

  CLI_PRINT(("vLpcAppInit: Initialising LpcTask\n"));

  boCLIregisterEntry( &asLpcMainCLI );

  vProtocolInit();

  // reset the UI elements controlled by the App
  vUIReset();

  // reset the UI controlled by the system
  vUiLedSet(LED_CONNECTED, OFF);
  vUiLedSet(LED_BATTERY_RED, OFF);
  vUiLedSet(LED_BATTERY_GREEN, OFF);

  // Register callbacks.
  
  vUiButtonCapture( vCallbackCaptureHandler, NULL );

  vPowerSelfNotify( vCallbackPowerOffHandler ); // notification of power button press

  vProtocolMsgNotify( vQueueMessageCompleteCallback );
  vProtocolMsgError( vMessageErrorCallback );

  vUiVibrateControl( false );

  // Create a timer for timing UN20 shutdown.
  hUn20ShutdownTimer = xTimerCreate( acShutdownTimerName,
                                     MS_TO_TICKS( UN20_SHUTDOWN_DELAY_MS ),
                                     pdFALSE,    // One-shot timeout
                                     NULL,
                                     vUn20ShutdownTimerCallback );

  // Create a timer for timing idle seconds before UN20 power off.
  hUn20IdleTimer = xTimerCreate( acUn20IdleTimerName,
                                 MS_TO_TICKS( UN20_IDLE_TIMEOUT_MS ),
                                 pdFALSE,    // One-shot timeout
                                 NULL,
                                 vUn20IdleTimerCallback );

  // Start the timer.
  xTimerStart( hUn20IdleTimer, 0 );

  // Create a timer for timing idle seconds before total power off.
  hInactivityTimer = xTimerCreate( acInactivityTimerName,
                                   MS_TO_TICKS( SYSTEM_IDLE_TIMEOUT_MS ),
                                   pdFALSE,    // One-shot timeout
                                   NULL,
                                   vSystemIdleTimerCallback );

  // Start the timer.
  xTimerStart( hInactivityTimer, 0 );

  // Create the vibrate control timer
  hTimer = xTimerCreate( acVibrateTimerName,
                         MS_TO_TICKS( 100 ),
                         pdFALSE, // One-shot timeout
                         NULL,
                         vVibTimerCallback );

  // Create the LED flash timer
  hFlashTimer = xTimerCreate( acFlashTimerName,
                         MS_TO_TICKS(FLASH_TICK_MS),
                         pdTRUE,    // Continuous timeouts
                         NULL,
                         vFlashTimerCallback );

  // Start the flash timer.
  xTimerStart( hFlashTimer, 0 );

  hMsgQueue = xQueueCreate( LPCAPP_MSG_QUEUE_SIZE, sizeof(MsgInternalPacket*) );

  // Initialise the transport callbacks after everything is ready as we may get immediate callbacks.
  iBtInit( vBtCallbackHandler, NULL );
  
  iUsbInit (USB_UN20, vUn20UsbCallbackHandler, NULL );
  
  iUsbInit( USB_HOST, vPhoneUsbCallbackHandler, NULL );

  /* Create the LPC App task. */
  xTaskCreate( vLpcAppTask, ( signed char * ) "LPC", LPCAPP_TASK_STACK_SIZE, ( void * ) NULL, LPCAPP_TASK_PRIORITY, NULL );

}

void vLpcAppTask( void *pvParameters )
{
  bool boLastCharging = false;
  bool boLastVBUSPresent = false;
  bool boNotify = true;
  bool boFirst = true;

  CLI_PRINT(("vLpcAppTask: Starting\n"));

  // Main loop.
  while ( 1 )
  {
    MsgInternalPacket *psMsg;

    // wait for a message to process
    if ( xQueueReceive( hMsgQueue, &psMsg, MS_TO_TICKS(500) ) == pdTRUE )
    {
      vMessageProcess( psMsg );
    }

    // See if we need to retrieve config info from the UN20.
    if ( boNeedUn20Info == true )
    {
      if ( ( boUn20UsbConnected == true ) && ( eUN20State == UN20_STATE_READY ) )
      {
        // Connected to the UN20 and need its config. Fire off a request.
        vSetupMessage( &sInternalMsg, MSG_UN20_GET_INFO, MSG_STATUS_GOOD, NULL, 0 );

        iIfSend(IF_UN20, (void *) &sInternalMsg, sInternalMsg.Msgheader.iLength);
      }
    }

    // get the current battery voltage
    iCurrentBatteryVoltage = iBatteryVoltage( 0 );

    // roll this reading into a rolling average to avoid responding to short spikes
    iVoltageAccumulator = ( iVoltageAccumulator - ( iVoltageAccumulator / BATT_AVG_SAMPLES ) ) + iCurrentBatteryVoltage;
    iAverageBatteryVoltage = ( iVoltageAccumulator / BATT_AVG_SAMPLES );

    // turn on the power led if  less than threshold
    boFlashBatteryLed = ( iAverageBatteryVoltage < BATT_WARN_LOW_MV );

    // turn on charge led if we are charging
    boCharging = boHalBatteryIsCharging();

    // record if VBUS is present (i.e. USB cable connected)
    boVBUSPresent = boHalUSBChargePresent();

    // if there is a change of charge state notify the system
    if ( ( boLastVBUSPresent != boVBUSPresent ) || boFirst )
    {
      CLI_PRINT(("*** Charge Source: %s ***\n", (boVBUSPresent ? "Present" : "Lost")));
      boNotify = true;
    }

    if ( ( boLastCharging != boCharging ) || boFirst )
    {
      CLI_PRINT(("*** Charge Status: %s ***\n", boCharging ? "Charging" : "Charged"));
      boNotify = true;
    }

    // update previous state cache
    boFirst = false;
    boLastCharging = boCharging;
    boLastVBUSPresent = boVBUSPresent;

    if ( boNotify )
    {
      vSystemPowerStateChange();
      boNotify = false;
    }

    // if the battery voltage has reached critical level action a shutdown
    if ( iAverageBatteryVoltage < BATT_SHUTOFF_MV )
    {
      vSystemIdleTimerCallback( NULL );
    }
  }
}


