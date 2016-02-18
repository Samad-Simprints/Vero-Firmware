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
//#define DBG_LOCAL_DEFS

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

#include "freertos.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

//******************************************************************************
// Private Function Prototypes
//******************************************************************************

// Registered HAL callback functions.
static void vCallbackPowerOffHandler(void);
static void vBtCallbackHandler(void *context, tInterfaceEvent event, void *event_data);
static void vPhoneUsbCallbackHandler(void *context, tInterfaceEvent event, void *event_data);
static void vCallbackCaptureHandler(void);
static void vUn20UsbCallbackHandler(void *context, tInterfaceEvent event, void *event_data);

static void vVibrateTask( void *pvParameters );
static void vVibTimerCallback( xTimerHandle xTimer );
static void vFlashTask( void *pvParameters );
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

static void vInterfaceConnDisconn( tInterfaceEvent event, tEventConnDisconn *psEventData );

//******************************************************************************
// Constants
//******************************************************************************

#define VIBRATE_TASK_STACK_SIZE          ( 128 + TASK_DEBUG_OVERHEAD )
#define VIBRATE_TASK_PRIORITY            ( tskIDLE_PRIORITY + 3 )
#define FLASH_TASK_STACK_SIZE            ( 128 + TASK_DEBUG_OVERHEAD )
#define FLASH_TASK_PRIORITY              ( tskIDLE_PRIORITY + 2 )

#define VIBRATE_OFF                      ( 0 )
#define VIBRATE_ON                       ( 1 )

// Flash timer tick rate in ms. This defines the minimum flash on/off time.
#define FLASH_TICK_MS                    ( 500 )

// Delay after requesting the UN20 to shutdown before we power it off.
#define UN20_SHUTDOWN_DELAY_MS           ( 10 * 1000 )

// Default UN20 idle timer value: 10 minutes in ms.
#define UN20_IDLE_TIMEOUT_MS             ( 60 * 60 * 1000 )

// Default system idle timer value: 60 minutes in ms.
#define SYSTEM_IDLE_TIMEOUT_MS             ( 60 * 60 * 1000 )

//******************************************************************************
// Definitions
//******************************************************************************

//******************************************************************************
// Local Storage
//******************************************************************************

static xQueueHandle hMsgQueue = NULL;

static xTaskHandle oVibrateTaskHandle = NULL;
static xTaskHandle oFlashTaskHandle = NULL;

static xSemaphoreHandle hVibrateSemaphore = NULL;
static xSemaphoreHandle hFlashSemaphore = NULL;

static xTimerHandle hUn20ShutdownTimer;
static xTimerHandle hUn20IdleTimer;        // Times idle seconds before UN20 power off
static xTimerHandle hInactivityTimer;      // Times idle seconds before total power off

static bool boPhoneBtConnected = false;
static bool boPhoneUsbConnected = false;
static bool boUn20UsbConnected = false;

// Our Bluetooth address.
static BT_ADDR sScannerBtAddr = { 0, 0, 0, 0, 0, 0 };

static bool boNeedUn20Info = false;        // True if we want the UN20's config info. This is set
                                           // when the UN20 reports READY (to get the UN20 version information)
                                           // and any time the number of stored images may change.

// Sensor Config items.

static int16 iPowerOffTimeoutSecs = 0; // idle seconds before total power off
static int16 iUn20IdleTimeoutSecs = 0; // idle seconds before UN20 power off
static int16 iGoodImageThreshold = 0;  // good image quality threshold for LED
static int16 iSmileThresholds[MAX_SMILE_STAGES]; // thesholds used to drive the smile
static int16 iScanTimeout = 0;         // default scan timeout
static int16 iScanBrightness = 0;      // default scan brightness
static int16 iRetryLimit = 0;          // default scan retry limit

// Set UI items.

static bool boEnableTrigger = false;       // True if the trigger button is enabled
static bool boSetLeds = false;             // LED setting
static bool boTriggerVibrate = false;      // True if vibrate requested
static uint8 bLedState[LED_MAX_LED_COUNT]; // off, red, green, orange, on, <flash choice>
static int16 iVibrateMs = 100;             // 0 = off, > 0 trigger vibrate for Ms (then stop)

static bool boFlashConnectionLed = true;   // True if we are flashing the blue Connection LED
static bool boFlashConnLedIsOn = true;     // True if the flashing Connection LED is currently ON.
static xTimerHandle hFlashTimer;

// UN20 Config items.

static int16 iUn20Version = 0;	           // UN20 client firmware version
static int16 iUn20StoreCount = 0;          // Number of stored images and templates

// This app operates mostly stateless (except for some hardware peripheral states such as
// the vibrate motor, LED flash state etc.
// Other state information is defined here:

static bool boUn20ShuttingDown = false;    // True if we have told the UN20 to shutdown
                                           // and are timing 3s before pulling its power.
static bool boUn20PoweredUp = false;       // True if the UN20 is powered.

//******************************************************************************
//******************************************************************************
// Private Functions
//******************************************************************************
//******************************************************************************

// Callback for the UN20 Shutdown timer.
static void vUn20ShutdownTimerCallback( xTimerHandle xTimer )
{
  CLI_PRINT(("vUn20ShutdownTimerCallback\n"));

  vPowerUn20Off();

  boUn20ShuttingDown = false;
  boUn20PoweredUp = false;

  return;
}


// Callback for the UN20 inactivity (idle) timer.
static void vUn20IdleTimerCallback( xTimerHandle xTimer )
{
  MsgPacket sResponseMsg;
  MsgDummyPayload sDummy;

  CLI_PRINT(("vUn20IdleTimerCallback\n"));

  // We just switch the UN20 off without telling the phone app.

  // Cleanly shut down the UN20 app by sending it a MSG_UN20_SHUTDOWN message.
  sResponseMsg.Msgheader.uMsgHeaderSyncWord = MSG_PACKET_HEADER_SYNC_WORD;
  sResponseMsg.Msgheader.bMsgId = MSG_UN20_SHUTDOWN;
  sResponseMsg.Msgheader.bStatus = MSG_STATUS_GOOD;

  sDummy.uMsgFooterSyncWord = MSG_PACKET_FOOTER_SYNC_WORD;

  sResponseMsg.oPayload.DummyPayload = sDummy;

  sResponseMsg.Msgheader.iLength = sizeof( MsgPacketheader ) + sizeof( MsgDummyPayload );

  // We send the message to the UN20 USB if it's connected.
  if ( boUn20UsbConnected == true )
  {
    (void) iUsbSend( USB_UN20, (void *) &sResponseMsg, sResponseMsg.Msgheader.iLength );
  }

  // Start the 3s UN20 shutdown timer.
  xTimerStart( hUn20ShutdownTimer, 0 );

  boUn20ShuttingDown = true;
#if 0    
  // We also send a MSG_UN20_ISSHUTDOWN message to the phone.
  //sResponseMsg.Msgheader.uMsgHeaderSyncWord = MSG_PACKET_HEADER_SYNC_WORD;
  sResponseMsg.Msgheader.bMsgId = MSG_UN20_ISSHUTDOWN;
  //sResponseMsg.Msgheader.bStatus = MSG_STATUS_GOOD;

  // sDummy.uMsgFooterSyncWord = MSG_PACKET_FOOTER_SYNC_WORD;

  //sResponseMsg.oPayload.DummyPayload = sDummy;

  //sResponseMsg.Msgheader.iLength = sizeof( MsgPacketheader ) + sizeof( MsgDummyPayload );

  // We send the message to the phone Bluetooth if it's connected,
  // else we send it to the phone USB (if connected).
  if ( boPhoneBtConnected == true )
  {
    (void) iBtSend( (void *) &sResponseMsg, sResponseMsg.Msgheader.iLength );
  }
  else if ( boPhoneUsbConnected == true )
  {
    (void) iUsbSend( USB_HOST, (void *) &sResponseMsg, sResponseMsg.Msgheader.iLength );
  }
  // Else no valid destination - don't send the message.
#endif
  return;
}


// Callback for system inactivity (Idle) timeout.
static void vSystemIdleTimerCallback( xTimerHandle xTimer )
{
  CLI_PRINT(("vSystemIdleTimerCallback\n"));

  // We take the same actions as when the user presses the power button.
  vCallbackPowerOffHandler();

  return;
}


// Callback for the LED-flash timer.
static void vFlashTimerCallback( xTimerHandle xTimer )
{
  // Re-evaluate LEDs needing to be flashed.
  if ( boFlashConnectionLed == true )
  {
    if ( boFlashConnLedIsOn == true )
    {
      // Next an OFF flash.
      vUiLedSet( LED_CONNECTED, OFF );

      boFlashConnLedIsOn = false;
    }
    else
    {
      // Next an ON flash.
      vUiLedSet( LED_CONNECTED, ON );

      boFlashConnLedIsOn = true;
    }
  }
  // Else flash not required. The flash task will have left the LED state as required.

  return;
}


// LED Flash task.
// At the moment this only flashes the blue Connection LED but could be
// generalised to flash any LED in the future.
static void vFlashTask( void *pvParameters )
{
  const char acTimerName[] = "Flash timer";

  CLI_PRINT(("vFlashTask: Starting\n"));

  hFlashTimer = xTimerCreate( acTimerName,
                         MS_TO_TICKS(FLASH_TICK_MS),
                         pdTRUE,    // Continuous timeouts
                         NULL,
                         vFlashTimerCallback );

  // Evaluate LEDs needing to be flashed.

  if ( boFlashConnectionLed == true )
  {
    // Start the timer.
    xTimerStart( hFlashTimer, 0 );

    // Start with an ON flash.
    bLedState[ LED_CONNECTED ] = MED_FLASH_ON;
    vUiLedSet( LED_CONNECTED, ON );

    boFlashConnLedIsOn = true;
  }

  // Flash task main loop.
  while ( 1 )
  {
    // Wait to be prodded.
    if ( hFlashSemaphore != NULL )
    {
      xSemaphoreTake( hFlashSemaphore, portMAX_DELAY );
    }

    // Re-evaluate LEDs needing to be flashed.
    if ( boFlashConnectionLed == true )
    {
      // Start the timer.
      xTimerStart( hFlashTimer, 0 );

      // Start with an ON flash.
      bLedState[ LED_CONNECTED ] = MED_FLASH_ON;
      vUiLedSet( LED_CONNECTED, ON );

      boFlashConnLedIsOn = true;
    }
    else
    {
      // Flash not required.

      // Stop the timer.
      xTimerStop( hFlashTimer, 0 );

      // Check for stopping a flashing LED.
      if ( bLedState[ LED_CONNECTED ] == MED_FLASH_ON )
      {
        // Stop flashing but leave the LED on. The phone app may change it if required.
        bLedState[ LED_CONNECTED ] = ON;
        vUiLedSet( LED_CONNECTED, ON );
      }
    }

  } // End while
}

static void vVibTimerCallback( xTimerHandle xTimer )
{
  // We simply turn the vibrate motor off.
  vUiVibrateControl ( VIBRATE_OFF );

  return;
}


static void vVibrateTask( void *pvParameters )
{
  xTimerHandle hTimer;
  const char acTimerName[] = "Vibrate timer";

  CLI_PRINT(("vVibrateTask: Starting\n"));

  hTimer = xTimerCreate( acTimerName,
                         MS_TO_TICKS( iVibrateMs ),
                         pdFALSE, // One-shot timeout
                         NULL,
                         vVibTimerCallback );

  // Vibrate task main loop.
  while ( 1 )
  {

    // Wait to be kicked off.
    if ( hVibrateSemaphore != NULL )
    {
      xSemaphoreTake( hVibrateSemaphore, portMAX_DELAY );
    }

    if( iVibrateMs > 0 )
    {
      // Set the new period.
      xTimerChangePeriod( hTimer,( iVibrateMs / portTICK_RATE_MS ), 0 );
     
      // Start the timer.
      xTimerStart( hTimer, 0 );
    
      // Start the vibrate motor.
      vUiVibrateControl ( VIBRATE_ON );
    }

  } // End while
}

static void vReturnSensorInfo( MsgPacket *psMsg, int iMsglength )
{
  CLI_PRINT(("vReturnSensorInfo\n"));

  psMsg->Msgheader.uMsgHeaderSyncWord = MSG_PACKET_HEADER_SYNC_WORD;
  psMsg->Msgheader.iLength = sizeof( MsgPacketheader ) + sizeof( MsgSensorInfo );
  psMsg->Msgheader.bMsgId = MSG_GET_SENSOR_INFO | MSG_REPLY;
  psMsg->Msgheader.bStatus = MSG_STATUS_GOOD;

  // Build up the payload.
  memcpy((void *) &psMsg->oPayload.SensorInfo.btAddr, (void *) sScannerBtAddr, sizeof( sScannerBtAddr ));
  psMsg->oPayload.SensorInfo.iUCVersion = 0 /* TODO */;
  psMsg->oPayload.SensorInfo.iUN20Version = iUn20Version;
  psMsg->oPayload.SensorInfo.iBatteryLevel1 = iBatteryVoltage( 0 );
  psMsg->oPayload.SensorInfo.iBatteryLevel2 = iBatteryVoltage( 1 );
  psMsg->oPayload.SensorInfo.iStoreCount = iUn20StoreCount;
  psMsg->oPayload.SensorInfo.boPowerOn = boUn20PoweredUp;
  psMsg->oPayload.SensorInfo.uMsgFooterSyncWord = MSG_PACKET_FOOTER_SYNC_WORD;

  CLI_PRINT(("Status: UN20:%d, Bat1:%d, Bat2:%d\n",
                  psMsg->oPayload.SensorInfo.boPowerOn,
                  psMsg->oPayload.SensorInfo.iBatteryLevel1,
                  psMsg->oPayload.SensorInfo.iBatteryLevel2));

  // We send the message to the phone Bluetooth if it's connected,
  // else we send it to the phone USB (if connected).
  if ( boPhoneBtConnected == true )
  {
    (void) iBtSend( (void *) psMsg, psMsg->Msgheader.iLength );
  }
  else if ( boPhoneUsbConnected == true )
  {
    (void) iUsbSend( USB_HOST, (void *) psMsg, psMsg->Msgheader.iLength );
  }
  // Else no valid destination - discard the message.

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

  CLI_PRINT(( "vSetSensorConfig: PwrOffTO=%d, IdleTO=%d, GoodThr=%d, ScanTO=%d, ScanBr=%d, Retry=%d\n",
              iPowerOffTimeoutSecs, iUn20IdleTimeoutSecs, iGoodImageThreshold, iScanTimeout, iScanBrightness, iRetryLimit ));

  // Put the timeout items into action.
  xTimerStop( hUn20IdleTimer, 0 );
  xTimerStop( hInactivityTimer, 0 );
  xTimerChangePeriod( hUn20IdleTimer, ( iUn20IdleTimeoutSecs * 1000 ) / portTICK_RATE_MS, 0 );
  xTimerChangePeriod( hInactivityTimer, ( iPowerOffTimeoutSecs * 1000 ) / portTICK_RATE_MS, 0 );
  xTimerStart( hUn20IdleTimer, 0 );
  xTimerStart( hInactivityTimer, 0 );

  return;
}


static void vSetUi( MsgPacket *psMsg, int iMsglength )
{
  MsgUIControl *psEventData;
  int iLoop;

  psEventData = (MsgUIControl *) &(psMsg->oPayload);

  boEnableTrigger = psEventData->boEnableTrigger;           // enable the trigger button
  boSetLeds = psEventData->boSetLeds;                       // set LED's according to bLedState
  boTriggerVibrate = psEventData->boTriggerVibrate;         // set vibrate according to boVibrateState
  iVibrateMs = psEventData->iVibrateMs;                     // 0 = off, > 0 trigger vibrate for Ms (then stop)

  for ( iLoop = 0; iLoop < LED_MAX_LED_COUNT; iLoop++ )
  {
    bLedState[ iLoop ] = psEventData->bLedState[ iLoop ];
  }

  CLI_PRINT(( "vSetUi: EnTrig=%d, SetLeds=%d, TrigVib=%d, VibMs=%d\n",
              boEnableTrigger, boSetLeds, boTriggerVibrate, iVibrateMs ));

  // Put these items into action.

  if ( boSetLeds == true )
  {
    for ( iLoop = 0; iLoop < LED_MAX_LED_COUNT; iLoop++ )
    {
      //CLI_PRINT(( "vSetUi: LED %d state %d\n", iLoop, bLedState[ iLoop ] ));

      vUiLedSet ((tLeds) iLoop, (tColorOptions) bLedState[ iLoop ] );
    }
  }

  if ( boTriggerVibrate == true )
  {
    // Notify the vibrate period to our vibrate task and let it run the vibration.
    if ( hVibrateSemaphore != NULL )
    {
      xSemaphoreGive( hVibrateSemaphore );
    }
  }

  // setup an ACK message and send it
  vSetupACK( psMsg );

  // We send the message to the phone Bluetooth if it's connected,
  // else we send it to the phone USB (if connected).
  if ( boPhoneBtConnected == true )
  {
    (void) iBtSend( (void *)psMsg, psMsg->Msgheader.iLength );
  }
  else if ( boPhoneUsbConnected == true )
  {
    (void) iUsbSend( USB_HOST, (void *)psMsg, psMsg->Msgheader.iLength );
  }
  // Else no valid destination - discard the message.

  return;
}


static void vBtPair( MsgPacket *psMsg, int iMsglength )
{
  CLI_PRINT(("vBtPair\n"));

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

  CLI_PRINT(( "vSetUn20Info: Ver 0x%04X, StoreCount %d\n",
              iUn20Version, iUn20StoreCount ));

  return;
}

// Queue the completed message for processing. NOTE: DOES NOT TAKE A COPY OF THE MESSAGE
static void vQueueMessageCompleteCallback( MsgInternalPacket *psMsg )
{
  xQueueSendToBack( hMsgQueue, &psMsg, RTOS_FOREVER );
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

  CLI_PRINT(( "vMessageCompleteCallback: From %s, length %d, MsgId 0x%02X\n",
              ( bSource == MSG_SOURCE_PHONE_BT ) ? "HostBT" : ( bSource == MSG_SOURCE_PHONE_USB ) ? "HostUSB" : "UN20USB",
              iMsglength, psMsg->oMsg.Msgheader.bMsgId ));

  // Reset the system inactivity timeout.
  xTimerReset( hInactivityTimer, 0 );

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

    // We generate a response to the phone to say that the UN20 is powering up.
    // When it has finished powering up, it will send a MSG_UN20_READY message which we
    // just pass through.
    if (( bSource == MSG_SOURCE_PHONE_BT ) || ( bSource == MSG_SOURCE_PHONE_USB ))
    {
      MsgPacket sResponseMsg;
      MsgDummyPayload sDummy;

      CLI_PRINT(("Waking up UN20\n"));

      // Start the UN20 powering up.
      vPowerUn20On();

      vSetupACK( psPacket );
#if 0
      // Generate a MSG_UN20_WAKINGUP message back to the phone.
      sResponseMsg.Msgheader.uMsgHeaderSyncWord = MSG_PACKET_HEADER_SYNC_WORD;
      sResponseMsg.Msgheader.bMsgId = MSG_UN20_WAKINGUP;
      sResponseMsg.Msgheader.bStatus = MSG_STATUS_GOOD;

      sDummy.uMsgFooterSyncWord = MSG_PACKET_FOOTER_SYNC_WORD;

      sResponseMsg.oPayload.DummyPayload = sDummy;

      sResponseMsg.Msgheader.iLength = sizeof( MsgPacketheader ) + sizeof( MsgDummyPayload );
#endif
      // We send the message to the phone Bluetooth if it's connected,
      // else we send it to the phone USB (if connected).
      if ( boPhoneBtConnected == true )
      {
        (void) iBtSend( (void *) psPacket, psPacket->Msgheader.iLength );
      }
      else if ( boPhoneUsbConnected == true )
      {
        (void) iUsbSend( USB_HOST, (void *) psPacket, psPacket->Msgheader.iLength );
      }
      // Else no valid destination - discard the message.
    }
    break;

  case MSG_UN20_READY:

    // This is a UN20 state message to say the UN20 is powered up and ready

    if ( bSource == MSG_SOURCE_UN20_USB )
    {
      boUn20ShuttingDown = false;
      boUn20PoweredUp = true;

      // Now is a good time to request the UN20's current config info.
      boNeedUn20Info = true;
#if 0
      // We also pass the message on to the phone.

      // We send the message to the phone Bluetooth if it's connected,
      // else we send it to the phone USB (if connected).
      if ( boPhoneBtConnected == true )
      {
        (void) iBtSend( (void *) &(psMsg->oMsg), iMsglength );
      }
      else if ( boPhoneUsbConnected == true )
      {
        (void) iUsbSend( USB_HOST, (void *) &(psMsg->oMsg), iMsglength );
      }
      // Else no valid destination - discard the message.
#endif
    }
          
    break;
    
  case MSG_UN20_SHUTDOWN:
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
      if (( boUn20ShuttingDown == true ) || ( boUn20PoweredUp == false ))
      {
        MsgPacket sResponseMsg;
        MsgDummyPayload sDummy;

        // The UN20 is in the wrong state to process this message.
        // We just return an error response.
      
        CLI_PRINT(("Sending UN20-shutdown error response to phone\n"));

        vSetupNACK( psPacket, MSG_STATUS_UN20_STATE_ERROR );
#if 0
        sResponseMsg.Msgheader.uMsgHeaderSyncWord = MSG_PACKET_HEADER_SYNC_WORD;
        sResponseMsg.Msgheader.bMsgId = psMsg->oMsg.Msgheader.bMsgId | MSG_REPLY; // Original message id with reply bit set
        sResponseMsg.Msgheader.bStatus = MSG_STATUS_UN20_STATE_ERROR;             // Error state

        sDummy.uMsgFooterSyncWord = MSG_PACKET_FOOTER_SYNC_WORD;

        sResponseMsg.oPayload.DummyPayload = sDummy;

        sResponseMsg.Msgheader.iLength = sizeof( MsgPacketheader ) + sizeof( MsgDummyPayload );
#endif
        // We send the message to the phone Bluetooth if it's connected,
        // else we send it to the phone USB (if connected).
        if ( boPhoneBtConnected == true )
        {
          (void) iBtSend( (void *) psPacket, psPacket->Msgheader.iLength );
        }
        else if ( boPhoneUsbConnected == true )
        {
          (void) iUsbSend( USB_HOST, (void *) psPacket, psPacket->Msgheader.iLength );
        }
        // Else no valid destination - discard the message.

        // No further action is taken for this message
        break;
      }
    }

    // The UN20 is powered up.

    // If this is the outbound message, set shutdown flag to protect against further messages
    if ( psMsg->oMsg.Msgheader.bMsgId == MSG_UN20_SHUTDOWN )
    {
      // Start the UN20 shutdown timer - this gives the UN20 time to halt.
      xTimerStart( hUn20ShutdownTimer, 0 );

      boUn20ShuttingDown = true;
    } // if this is the acknowledgment, then the UN20 is now logically disconnected
    else if ( psMsg->oMsg.Msgheader.bMsgId == ( MSG_UN20_SHUTDOWN | MSG_REPLY) )
    {
      boUn20PoweredUp = false;
    }

#if 0
    // Now is a good time to re-request the UN20's current config info.
    boNeedUn20Info = true;
#endif
    // We also restart the UN20 inactivity timeout.
    xTimerReset( hUn20IdleTimer, 0 );

    // By default we just echo these message types to the other end.
    
    if (( bSource == MSG_SOURCE_PHONE_BT ) || ( bSource == MSG_SOURCE_PHONE_USB ))
    {
      // The only possible destination is the UN20 USB (if it's connected).
      if ( boUn20UsbConnected == true )
      {
        (void) iUsbSend( USB_UN20, (void *) &(psMsg->oMsg), iMsglength );
      }
      // Else no valid destination - discard the message.
    }
    else if ( bSource == MSG_SOURCE_UN20_USB )
    {
      // We send the message to the phone Bluetooth if it's connected,
      // else we send it to the phone USB (if connected).
      if ( boPhoneBtConnected == true )
      {
        (void) iBtSend( (void *) &(psMsg->oMsg), iMsglength );
      }
      else if ( boPhoneUsbConnected == true )
      {
        (void) iUsbSend( USB_HOST, (void *) &(psMsg->oMsg), iMsglength );
      }
      // Else no valid destination - discard the message.
    }
    // Else invalid source - discard the message.
    break;

  case MSG_REPORT_UI:
  case MSG_UN20_WAKINGUP:
  default:
    CLI_PRINT(("*** Ignoring message\n"));

    // Invalid or unexpected message type - discard the message.
    break;

  } // End switch

  return;
}

static void vMessageErrorCallback( tMsgError eErrorCode )
{
  CLI_PRINT(( "vMessageErrorCallback: error %d\n", eErrorCode ));

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
         CLI_PRINT(( "UN20 USB Connected\n" ));

         boUn20UsbConnected = true;
      }
      else if( psEventData->eInterface == USB_HOST )
      {
        CLI_PRINT(( "Phone USB Connected\n" ));

        boPhoneUsbConnected = true;
      }
      else if ( psEventData->eInterface == BT_HOST )
      {
        CLI_PRINT(( "Phone BT Connected\n" ));

        boPhoneBtConnected = true;
      }
      // Else unknown interface.
    }

    // Check for a connection to the phone.
    if (( boPhoneBtConnected == true ) || ( boPhoneUsbConnected == true ))
    {
      boFlashConnectionLed = false;

      // Notify our flash task to stop the flash.
      if ( hFlashSemaphore != NULL )
      {
        xSemaphoreGive( hFlashSemaphore );
      }
    }
    break;

  case INTERFACE_EVENT_DISCONNECTED:
    {
      if( psEventData->eInterface == USB_UN20 )
      {
        CLI_PRINT(( "UN20 USB Disconnected\n" ));

        boUn20UsbConnected = false;
      }
      else if( psEventData->eInterface == USB_HOST )
      {
        CLI_PRINT(( "Phone USB Disconnected\n" ));

        boPhoneUsbConnected = false;
      }
      else if ( psEventData->eInterface == BT_HOST )
      {
        CLI_PRINT(( "Phone BT Disconnected\n" ));

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
      boFlashConnectionLed = true;

      // Notify our flash task and let it run the flash.
      if ( hFlashSemaphore != NULL )
      {
        xSemaphoreGive( hFlashSemaphore );
      }
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
  CLI_PRINT(("vBtCallbackHandler: event %d\n", event));

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

    CLI_PRINT(("vUn20UsbCallbackHandler: event %d\n", event));
    vInterfaceConnDisconn( event, (tEventConnDisconn *) event_data );
    break;

  case INTERFACE_EVENT_RX_DATA:
    {
      tEventRxData *psEventData = event_data;
      if ( psEventData->iLength > 1 )
      {
        CLI_PRINT(("vUn20UsbCallbackHandler: event %d\n", event));
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
  
  CLI_PRINT(("vPhoneUsbCallbackHandler: event %d\n", event));

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

// Power button pressed.
static void vCallbackPowerOffHandler(void)
{
  MsgPacket sResponseMsg;
  MsgDummyPayload sDummy;

  CLI_PRINT(("vCallbackPowerOffHandler\n"));

  // We just switch off without telling the phone app.

  // Cleanly shut down the UN20 app.
  sResponseMsg.Msgheader.uMsgHeaderSyncWord = MSG_PACKET_HEADER_SYNC_WORD;
  sResponseMsg.Msgheader.bMsgId = MSG_UN20_SHUTDOWN;
  sResponseMsg.Msgheader.bStatus = MSG_STATUS_GOOD;

  sDummy.uMsgFooterSyncWord = MSG_PACKET_FOOTER_SYNC_WORD;

  sResponseMsg.oPayload.DummyPayload = sDummy;

  sResponseMsg.Msgheader.iLength = sizeof( MsgPacketheader ) + sizeof( MsgDummyPayload );

  // We send the message to the UN20 USB if it's connected.
  if ( boUn20UsbConnected == true )
  {
    (void) iUsbSend( USB_UN20, (void *) &sResponseMsg, sResponseMsg.Msgheader.iLength );
  }

  // Pause here for a short while to allow the UN20 to cleanly shut down.
  vTaskDelay( MS_TO_TICKS( UN20_SHUTDOWN_DELAY_MS ));
  
  // Power down the UN20.
  vPowerUn20Off();

  // boUn20ShuttingDown = false;
  // boUn20PoweredUp = false;

  // Now power down the whole scanner.
  vPowerSelfOff();

  // Should never be reached!
  return;
}

// Capture button pressed.
static void vCallbackCaptureHandler(void)
{
  CLI_PRINT(("vCallbackCaptureHandler\n"));

  // Reset the system inactivity timeout.
  xTimerReset( hInactivityTimer, 0 );

  // If the capture button is enabled, we pass this indication on to the phone.
  if ( boEnableTrigger == true )
  {
    MsgPacket sResponseMsg;
    MsgUINotification sNotification;

    sResponseMsg.Msgheader.uMsgHeaderSyncWord = MSG_PACKET_HEADER_SYNC_WORD;
    sResponseMsg.Msgheader.bMsgId = MSG_REPORT_UI;
    sResponseMsg.Msgheader.bStatus = MSG_STATUS_GOOD;

    sNotification.boTriggerPressed = true;

    sNotification.uMsgFooterSyncWord = MSG_PACKET_FOOTER_SYNC_WORD;

    sResponseMsg.oPayload.UINotification = sNotification;

    sResponseMsg.Msgheader.iLength = sizeof( MsgPacketheader ) + sizeof( MsgUINotification );

    // We send the message to the phone Bluetooth if it's connected,
    // else we send it to the phone USB (if connected).
    if ( boPhoneBtConnected == true )
    {
      (void) iBtSend( (void *) &sResponseMsg, sResponseMsg.Msgheader.iLength );
    }
    else if ( boPhoneUsbConnected == true )
    {
      (void) iUsbSend( USB_HOST, (void *) &sResponseMsg, sResponseMsg.Msgheader.iLength );
    }
    // Else no valid destination - discard the message.
  }
  // Else ignore this button press.

  return;
}


//******************************************************************************
//******************************************************************************
// Public Functions
//******************************************************************************
//******************************************************************************

void vLpcAppTask( void *pvParameters )
{
  const char acShutdownTimerName[] = "UN20 shutdown timer";
  const char acUn20IdleTimerName[] = "UN20 idle timer";
  const char acInactivityTimerName[] = "System idle timer";

  CLI_PRINT(("vLpcAppTask: Starting\n"));
#if 0
  // Initialise HAL items.
  vHalInit();
  // Called early to latch power on.
  vPowerSelfOn();
#endif
  vProtocolInit();

  // Perform a start-up dance on the LEDs.
  vUiLedSet(LED_RING_0, OFF);
  vUiLedSet(LED_RING_1, OFF);
  vUiLedSet(LED_RING_2, OFF);
  vUiLedSet(LED_RING_3, OFF);
  vUiLedSet(LED_RING_4, OFF);
  vUiLedSet(LED_RING_5, OFF);
  vUiLedSet(LED_RING_6, OFF);
  vUiLedSet(LED_SCAN_GOOD, OFF);
  vUiLedSet(LED_SCAN_BAD, OFF);

  vUiLedSet(LED_CONNECTED, OFF);
  vUiLedSet(LED_BATTERY, OFF);
#if 0
  vTaskDelay( 1000 );

    // Perform a start-up dance on the LEDs.
  vUiLedSet(LED_RING_0, GREEN);
  vUiLedSet(LED_RING_1, GREEN);
  vUiLedSet(LED_RING_2, ORANGE);
  vUiLedSet(LED_RING_3, RED);
  vUiLedSet(LED_RING_4, ORANGE);
  vUiLedSet(LED_RING_5, GREEN);
  vUiLedSet(LED_RING_6, GREEN);
  vUiLedSet(LED_SCAN_GOOD, GREEN);
  vUiLedSet(LED_SCAN_BAD, RED);

  vUiLedSet(LED_CONNECTED, ON);
  vUiLedSet(LED_BATTERY, ON);

  vTaskDelay( 1000 );

    // Perform a start-up dance on the LEDs.
  vUiLedSet(LED_RING_0, OFF);
  vUiLedSet(LED_RING_1, OFF);
  vUiLedSet(LED_RING_2, OFF);
  vUiLedSet(LED_RING_3, OFF);
  vUiLedSet(LED_RING_4, OFF);
  vUiLedSet(LED_RING_5, OFF);
  vUiLedSet(LED_RING_6, OFF);
  vUiLedSet(LED_SCAN_GOOD, OFF);
  vUiLedSet(LED_SCAN_BAD, OFF);

  vUiLedSet(LED_CONNECTED, OFF);
  vUiLedSet(LED_BATTERY, OFF);
#endif
  // Register callbacks.
  
  vUiButtonCapture( vCallbackCaptureHandler, NULL );

  vPowerSelfNotify( vCallbackPowerOffHandler ); // notification of power button press

  vProtocolMsgNotify( vQueueMessageCompleteCallback );
  vProtocolMsgError( vMessageErrorCallback );

  vUiVibrateControl( false );

  // Create the semaphore for unblocking the vibrate task.
  vSemaphoreCreateBinary( hVibrateSemaphore );

    // Create the semaphore for prodding the flash task.
  vSemaphoreCreateBinary( hFlashSemaphore );

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

  // Create the Vibrate task
  xTaskCreate( vVibrateTask, ( signed char * ) "Vibrate", VIBRATE_TASK_STACK_SIZE, ( void * ) NULL, VIBRATE_TASK_PRIORITY, &oVibrateTaskHandle );

  // Create the flash task
  xTaskCreate( vFlashTask, ( signed char * ) "Flash", FLASH_TASK_STACK_SIZE, ( void * ) NULL, FLASH_TASK_PRIORITY, &oFlashTaskHandle );

#define LPCAPP_MSG_QUEUE_SIZE 10
  hMsgQueue = xQueueCreate( LPCAPP_MSG_QUEUE_SIZE, sizeof(MsgInternalPacket*) );

  // Initialise the transport callbacks after everything is ready as we may get immediate callbacks.
  iBtInit( vBtCallbackHandler, NULL );
  
  iUsbInit (USB_UN20, vUn20UsbCallbackHandler, NULL );
  
  iUsbInit( USB_HOST, vPhoneUsbCallbackHandler, NULL );

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
      if ( boUn20UsbConnected == true )
      {
        // Connected to the UN20 and need its config. Fire off a request.

        MsgPacket sRequestMsg;
        MsgDummyPayload sDummy;

        sRequestMsg.Msgheader.uMsgHeaderSyncWord = MSG_PACKET_HEADER_SYNC_WORD;
        sRequestMsg.Msgheader.bMsgId = MSG_UN20_GET_INFO;
        sRequestMsg.Msgheader.bStatus = MSG_STATUS_GOOD;

        sDummy.uMsgFooterSyncWord = MSG_PACKET_FOOTER_SYNC_WORD;

        sRequestMsg.oPayload.DummyPayload = sDummy;

        sRequestMsg.Msgheader.iLength = sizeof( MsgPacketheader ) + sizeof( MsgDummyPayload );

        // We send the message to the UN20 USB if it's connected.
        (void) iUsbSend( USB_UN20, (void *) &sRequestMsg, sRequestMsg.Msgheader.iLength );
      }
    }
  }
}


