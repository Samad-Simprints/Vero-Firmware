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

#include <stdlib.h>

#include "global.h"
#include "msg_format.h"
#include <stdio.h>

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

  //  uint32 uMsgHeaderSyncWord;   // Message header sync word - used to detect unsynchronisation
  //  uint8  bMsgId;               // The message id
  //  uint16 iLength;              // Message length including all header and footer bytes
  //  uint8  bStatus;              // Status for responses
  // <Payload>
  //  uint32 uMsgFooterSyncWord;  // Message footer sync word - used to detect unsynchronisation
  const MsgPacket sGetSensorInfoPacket = {
  .Msgheader = { 0xFAFAFAFA, sizeof( MsgPacketheader ) + sizeof( MsgDummyPayload ), MSG_GET_SENSOR_INFO, 0x00 },
  .oPayload.DummyPayload = { 0xF5F5F5F5 }
  };

  //  MsgUIControl        UIControl;
  //    bool boEnableTrigger;               // enable the trigger button
  //    bool boSetLeds;                     // set LED's according to bLedState
  //    bool boTriggerVibrate;              // set vibrate according to boVibrateState
  //    uint8 bLedState[LED_MAX_USER_COUNT]; // off, red, green, orange, on
  //    int16 iVibrateMs;                   // 0 = off, > 0 trigger vibrate for Ms (then stop)
  const MsgPacket sSetuiPacket = {
  .Msgheader = { 0xFAFAFAFA, sizeof( MsgPacketheader ) + sizeof( MsgUIControl ), MSG_SET_UI, 0x00 },
  .oPayload.UIControl = { true, true, true, GREEN, ORANGE, RED, ORANGE, GREEN, 3000, 0xF5F5F5F5 }
  };

  // MsgSensorConfig     SensorConfig;
  //    int16 iPowerOffTimeoutSecs; // idle seconds before total power off
  //    int16 iUn20IdleTimeoutSecs; // idle seconds before UN20 power off
  //    int16 iGoodImageThreshold;  // good image quality threshold for LED
  //    int16 iSmileThresholds[MAX_SMILE_STAGES]; // thesholds used to drive the smile
  //    these may be unnecessary as Capture message contains them
  //    int16 iScanTimeout;         // default scan timeout
  //    int16 iScanBrightness;      // default scan brightness
  //    int16 iRetryLimit;          // default scan retry limit
  const MsgPacket sSensorConfigPacket = {
  .Msgheader = { 0xFAFAFAFA, sizeof( MsgPacketheader ) + sizeof( MsgSensorConfig ), MSG_SET_SENSOR_CONFIG, 0x00 },
  .oPayload.SensorConfig = { 60, 30, 0x30, 0x0A, 0x20, 0x40, 0x64, 0x0A, 0x50, 0x08, 0xF5F5F5F5 }
  };

  const MsgPacket sShutdownUn20Packet = {
  .Msgheader = { 0xFAFAFAFA, sizeof( MsgPacketheader ) + sizeof( MsgDummyPayload ), MSG_UN20_SHUTDOWN, 0x00 },
  .oPayload.DummyPayload = { 0xF5F5F5F5 }
  };

  const MsgPacket sWakeupUn20Packet = {
  .Msgheader = { 0xFAFAFAFA, sizeof( MsgPacketheader ) + sizeof( MsgDummyPayload ), MSG_UN20_WAKEUP, 0x00 },
  .oPayload.DummyPayload = { 0xF5F5F5F5 }
  };

  const MsgPacket sUn20ReadyPacket = {
  .Msgheader = { 0xFAFAFAFA, sizeof( MsgPacketheader ) + sizeof( MsgDummyPayload ), MSG_UN20_READY, 0x00 },
  .oPayload.DummyPayload = { 0xF5F5F5F5 }
  };

  const MsgPacket sUn20GetInfoPacket = {
  .Msgheader = { 0xFAFAFAFA, sizeof( MsgPacketheader ) + sizeof( MsgUN20Info ), MSG_UN20_GET_INFO | MSG_REPLY, 0x00 },
  .oPayload.UN20Info = {0x1234, 7, 0xF5F5F5F5 }
  };

  const MsgPacket sCaptureImagePacket = {
  .Msgheader = { 0xFAFAFAFA, sizeof( MsgPacketheader ) + sizeof( MsgScanRequest ), MSG_CAPTURE_IMAGE, 0x00 },
  .oPayload.ScanRequest = { 0, 5, 8, 80, 75, 0xF5F5F5F5 }
  };

  const MsgPacket sCaptureProgressPacket = {
  .Msgheader = { 0xFAFAFAFA, sizeof( MsgPacketheader ) + sizeof( MsgScanProgress ), MSG_CAPTURE_PROGRESS, 0x00 },
  .oPayload.ScanProgress = { 2, 55, 0xF5F5F5F5 }
  };

  const MsgPacket sGetQualityPacket = {
  .Msgheader = { 0xFAFAFAFA, sizeof( MsgPacketheader ) + sizeof( MsgDummyPayload ), MSG_IMAGE_QUALITY, 0x00 },
  .oPayload.DummyPayload = { 0xF5F5F5F5 }
 };

  const MsgPacket sGetGenerateTemplatePacket = {
  .Msgheader = { 0xFAFAFAFA, sizeof( MsgPacketheader ) + sizeof( MsgDummyPayload ), MSG_GENERATE_TEMPLATE, 0x00 },
  .oPayload.DummyPayload = { 0xF5F5F5F5 }
  };

  MsgPacket sGetImageFragment = {
  .Msgheader = { 0xFAFAFAFA, sizeof( MsgPacketheader ) + sizeof( MsgRequestFragment ), MSG_GET_IMAGE_FRAGMENT, 0x00 },
  .oPayload.FragmentRequest = { 0x0, 0xF5F5F5F5 }
  };

  MsgPacket sGetTemplateFragment = {
  .Msgheader = { 0xFAFAFAFA, sizeof( MsgPacketheader ) + sizeof( MsgRequestFragment ), MSG_GET_TEMPLATE_FRAGMENT, 0x00 },
  .oPayload.FragmentRequest = { 0x0, 0xF5F5F5F5 }
  };

  const MsgPacket sSaveImage = {
  .Msgheader = { 0xFAFAFAFA, sizeof( MsgPacketheader ) + sizeof( MsgStoreScan ), MSG_STORE_IMAGE, 0x00 },
  .oPayload.StoreScan = { 14, "this-is-a-guid", 0, 0xF5F5F5F5 }
  };

  const MsgPacket sCrashLog = {
  .Msgheader = { 0xFAFAFAFA, sizeof( MsgPacketheader ) + sizeof( MsgCrashLogRequest ), MSG_GET_CRASH_LOG, 0x00 },
  .oPayload.CrashLogRequest = { 0xF5F5F5F5 }
  };

  MsgPacket sHwConfig = {
  .Msgheader = { 0xFAFAFAFA, sizeof( MsgPacketheader ) + sizeof( MsgHardwareConfigRequest ), MSG_SET_HW_CONFIG, 0x00 },
  .oPayload.HardwareConfig = { 0x00, 0xF5F5F5F5 }
  };


