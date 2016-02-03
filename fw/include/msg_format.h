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
// MODULE: msg_format.h
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:  INDEX header file
//
//******************************************************************************

#if !defined(_MSG_FORMAT_H_)
#define _MSG_FORMAT_H_

//******************************************************************************
// Includes
//******************************************************************************

#include "global.h"
#include "hal.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constants
//******************************************************************************
#define MAX_STORED_IMAGES     5
#define MAX_STORED_TEMPLATES  5

#define MAX_SMILE_STAGES      4 // how many "stages" the smile LEDs are grouped into

#define MSG_PACKET_HEADER_SYNC_WORD 0xFAFAFAFA
#define MSG_PACKET_FOOTER_SYNC_WORD 0xF5F5F5F5

// Maximum size (inc. syncwords, headers, footers etc.) of a protocol message.
#define MSG_PACKET_MAX_SIZE   1024

//******************************************************************************
// Definitions
//******************************************************************************
//
// Note: Multibyte data items are communicated in network byte order
//
typedef signed char     int8;
typedef signed short    int16;
typedef signed int      int32;
typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint32;

typedef unsigned char BT_ADDR[6];

enum
{
  MSG_STATUS_GOOD = 0,          // No error
  MSG_STATUS_ERROR,             // Non-specific error
  MSG_STATUS_UN20_STATE_ERROR   // UN20 is powered down
};

// message-ids as held in the message bMsgId field.
enum
{
  // Messages sent from phone to LPC scanner app.
  MSG_GET_SENSOR_INFO = 0,  // Processed by LPC App: Request has no payload, Response carries a MsgSensorInfo payload
  MSG_SET_SENSOR_CONFIG,    // Processed by LPC App: Request has a MsgSensorConfig payload, Response carries no payload
  MSG_SET_UI,               // Processed by LPC App: Request has a MsgUIControl payload, Response carries no payload
  MSG_PAIR,                 // Processed by LPC App: Request has a MsgPairRequest payload, Response carries no payload

  // Messages sent from LPC scanner app to phone app.
  MSG_REPORT_UI,            // Originated by LPC App: Request has a MsgUINotification payload, Response carries no payload

  // Messages sent between phone and UN20 application.
  MSG_CAPTURE_IMAGE,        // Capture an image (processed by UN20 App)
  MSG_CAPTURE_PROGRESS,     // Capture progress (sent by UN20 App)
  MSG_CAPTURE_ABORT,        // Processed by UN20 App
  MSG_RECOVER_IMAGE,        // Processed by UN20 App
  MSG_IMAGE_FRAGMENT,       // Processed by UN20 App
  MSG_STORE_IMAGE,          // Processed by UN20 App
  MSG_IMAGE_QUALITY,        // Processed by UN20 App
  MSG_GENERATE_TEMPLATE,    // Processed by UN20 App
  MSG_RECOVER_TEMPLATE,     // Processed by UN20 App
  MSG_COMPARE_TEMPLATE,     // Processed by UN20 App

  MSG_UN20_SHUTDOWN,        // Tell UN20 to shutdown
  MSG_UN20_WAKEUP,          // Wakeup the UN20 (noted by LPC App)
  MSG_UN20_WAKINGUP,        // UN20 is waking up (sent by LPC App if UN20 asleep)
  MSG_UN20_READY,           // UN20 is ready (sent by UN20 app)
  MSG_UN20_ISSHUTDOWN,      // UN20 is shutting/shut down (sent by LPC app if UN20 idle times out)
  MSG_UN20_GET_INFO,        // Requested and processed by LPC App: Request has no payload, Response carries a MsgUN20Info payload

  MSG_NUM_MSGS,             // Number of valid message types

  MSG_REPLY = 0x80          // Top bit set indicates a reply
};

typedef enum
{
  // Possible message sources.
  MSG_SOURCE_UN20_USB = 0,  // Message came from the scanner via USB
  MSG_SOURCE_PHONE_USB,     // Message came from the phone via USB
  MSG_SOURCE_PHONE_BT,      // Message came from the phone via Bluetooth
  MSG_SOURCE_NUM_SOURCES    // Number of message sources
}
tMsgSource;

//
// Scanner unit status information
//
PACK( typedef struct tagSensorInfo
{
  BT_ADDR btAddr;	// Scanner's BT addresss
  int16	iUCVersion;	// ARM controller firmware version
  int16	iUN20Version;	// UN20 client firmware version
  int16 iBatteryLevel1;	// Analog input 1
  int16 iBatteryLevel2;	// Analog Input 2
  int16 iStoreCount;    // Number of stored images and templates
  bool boPowerOn;       // true if UN20 is powered on, false if not
  uint32 uMsgFooterSyncWord;  // Message footer sync word - used to detect unsynchronisation
})
MsgSensorInfo;

//
// Scanner unit configuration information
//
PACK( typedef struct tagSensorConfig
{
  int16 iPowerOffTimeoutSecs; // idle seconds before total power off
  int16 iUn20IdleTimeoutSecs; // idle seconds before UN20 power off
  int16 iGoodImageThreshold;  // good image quality threshold for LED
  int16 iSmileThresholds[MAX_SMILE_STAGES]; // thesholds used to drive the smile
  // these may be unnecessary as Capture message contains them
  int16 iScanTimeout;         // default scan timeout
  int16 iScanBrightness;      // default scan brightness
  int16 iRetryLimit;          // default scan retry limit
  uint32 uMsgFooterSyncWord;  // Message footer sync word - used to detect unsynchronisation
})
MsgSensorConfig;

//
// UN20 status information
//
typedef struct tagUN20Info
{
  int16 iVersion;	// UN20 client firmware version
  int16 iStoreCount;    // Number of stored images and templates
  uint32 uMsgFooterSyncWord;  // Message footer sync word - used to detect unsynchronisation
}
MsgUN20Info;

//
// UI Control - configuration and update
//
PACK( typedef struct tagUIControl
{
  bool boEnableTrigger;               // Enable the trigger button
  bool boSetLeds;                     // Set LED's according to bLedState
  bool boTriggerVibrate;              // Set vibrate according to boVibrateState
  uint8 bLedState[LED_MAX_LED_COUNT]; // Off, red, green, orange, on
  int16 iVibrateMs;                   // 0 = off, > 0 trigger vibrate for Ms (then stop)
  uint32 uMsgFooterSyncWord;  // Message footer sync word - used to detect unsynchronisation
})
MsgUIControl;

//
// UI Notification - async UI updated from LPC app to phone
//
PACK( typedef struct tagUINotification
{
  bool boTriggerPressed;               // The trigger (capture) button has been pushed
  uint32 uMsgFooterSyncWord;  // Message footer sync word - used to detect unsynchronisation
})
MsgUINotification;

//
// Scanning and image manipulation
//
PACK( typedef struct tagScanRequest
{
  bool  boWaitForButton;// true if button is the trigger (immediate if not)
  int16 iTimeout;	// how long to wait for a good image
  int16 iRetryCount;    // maximum number of retries to get a good image
  int16 iQuality;	// how good that image needs to be
  int16 iBrightness;	// brightness to set before scan
  uint32 uMsgFooterSyncWord;  // Message footer sync word - used to detect unsynchronisation
})
MsgScanRequest;

//
// Report progress information to allow UI to be updated
//
PACK( typedef struct tagScanProgress
{
  int16 iNotification;	// how many notifications have been sent
  int16 Quality;	// quality of the last collected image
  uint32 uMsgFooterSyncWord;  // Message footer sync word - used to detect unsynchronisation
})
MsgScanProgress;

//
// Recover a previous scan image
//
PACK( typedef struct tagScanRecover
{
  int iFileId;		// handle of image to be recovered
  uint32 uMsgFooterSyncWord;  // Message footer sync word - used to detect unsynchronisation
})
MsgScanRecover;

//
// Scanned image data are preceded by a header...
//
PACK( typedef struct tagScanResponse
{
  int16 iQualityMin;	// quality requested
  int16 iQualityActual; // quality actually achieved
  int16 iBrightness;	// brightness selected
  int16 iWidth;		// width of the image in pixels
  int16 iHeight;	// height of the image in pixels
  int32 iDataLength;	// total length of the data
  int16 iFileTag;	// -1 means realtime scan data
  bool  boCompressed;	// true of image is compressed
  uint32 uMsgFooterSyncWord;  // Message footer sync word - used to detect unsynchronisation
})
MsgScanResponse;

//
// Followed by multiple fragments
//
PACK( typedef struct tagScanFragment
{
  int16 iFragmentSize;		// how much data in this fragment
  int16 iFragmentNo;		// Identifier for this fragment
  int16 iTotalFragments;	// total number of fragments
  uint8 bFragmentData[100];	// the fragment data
  uint32 uMsgFooterSyncWord;  // Message footer sync word - used to detect unsynchronisation
})
MsgScanFragment;

//
// Store the last scan for later recovery
//
PACK( typedef struct tagStoreScan
{
  int32 iFileTag;	// user supplied file tag
  bool  boCompress;     // compress image when storing
  uint32 uMsgFooterSyncWord;  // Message footer sync word - used to detect unsynchronisation
})
MsgStoreScan;

//
// Response includes a handle used to recover the image
//
PACK( typedef struct tagStoreScanResponse
{
  int32 iFileTag;	// file tag
  int32 iFileId;	// handle to locate image on UN20
  uint32 uMsgFooterSyncWord;  // Message footer sync word - used to detect unsynchronisation
})
MsgStoreScanReponse;

//
// Template generation...
//
PACK( typedef struct tagTemplateRequest
{
  int32 iFileId;	// what to template (-1 is realtime)
  int8  iFormat;	// template format to generate
  uint32 uMsgFooterSyncWord;  // Message footer sync word - used to detect unsynchronisation
})
MsgTemplateRequest;

//
// Template response
//
PACK( typedef struct tagTemplateResponse
{
  int32 iFileId;	// what this template represents (-1 is realtime)
  int8  iFormat;	// template format
  int16 iSize;		// size of template data
  uint8 bData[1];	// the template data
  uint32 uMsgFooterSyncWord;  // Message footer sync word - used to detect unsynchronisation
})
MsgTemplateResponse;

//
// Template comparison
//
PACK( typedef struct tagTemplateCompare
{
  int iFileId1;         // First template id
  int iFileId2;         // Second template id
  uint32 uMsgFooterSyncWord;  // Message footer sync word - used to detect unsynchronisation
})
MsgTemplateCompare;

PACK( typedef struct tagTemplateCompareResponse
{
  bool boMatched;       // True is: templates match
  uint32 uMsgFooterSyncWord;  // Message footer sync word - used to detect unsynchronisation
})
MsgTemplateCompareResponse;

PACK( typedef struct tagDummyPayload
{
  bool boDummy;
  uint32 uMsgFooterSyncWord;  // Message footer sync word - used to detect unsynchronisation
})
MsgDummyPayload;

//
//
//
PACK( typedef struct tagPairRequest
{
  BT_ADDR	btAddr;		// Host's BT address
  uint32 uMsgFooterSyncWord;  // Message footer sync word - used to detect unsynchronisation
})
MsgPairRequest;

PACK( typedef struct tagMsgPacketHeader
{
  // The first element is 32 bits wide so that the whole struct is 32-bit aligned when instantiated.
  uint32 uMsgHeaderSyncWord;   // Message header sync word - used to detect unsynchronisation
  uint16 iLength;              // Message length including all header and footer bytes
  uint8  bMsgId;               // The message type
  uint8  bStatus;              // Status for responses
})
MsgPacketheader;

PACK( typedef struct tagMsgPacket
{
  MsgPacketheader Msgheader;

  PACK( union
  {
    MsgSensorInfo       SensorInfo;         // payload of MSG_GET_SENSOR_INFO message response
    MsgSensorConfig     SensorConfig;       // payload of MSG_SET_SENSOR_CONFIG message
    MsgUN20Info         UN20Info;           // payload of MSG_UN20_GET_INFO message response
    MsgUIControl        UIControl;          // payload of MSG_SET_UI message
    MsgPairRequest      PairRequest;        // payload of MSG_PAIR message
    MsgUINotification   UINotification;     // payload of MSG_REPORT_UI message
    MsgScanRequest      ScanRequest;        // payload of MSG_CAPTURE_IMAGE message
    MsgScanRecover      ScanRecover;        // payload of MSG_RECOVER_IMAGE message
    MsgScanResponse     ScanResponse;       // payload of ? message
    MsgScanFragment     ScanFragment;       // payload of MSG_IMAGE_FRAGMENT message
    MsgScanProgress     ScanProgress;       // payload of MSG_CAPTURE_PROGRESS message
    MsgStoreScan        StoreScan;          // payload of MSG_STORE_IMAGE message
    MsgStoreScanReponse StoreScanResponse;  // payload of ? message
    MsgTemplateRequest  TemplateRequest;    // payload of ? message
    MsgTemplateResponse TemplateResponse;   // payload of ? message
    MsgTemplateCompare  TemplateCompare;    // payload of MSG_COMPARE_TEMPLATE message
    MsgDummyPayload     DummyPayload;       // Used for creating no-payload messages
  })
  oPayload;

  // We don't declare the footer syncword here, as we don't want it placed after "sizeof(oPayload)"
  // which is the max size of any union element above.
})
MsgPacket;

PACK( typedef struct tagInternalMsg
{
  // The first element is 32 bits wide so that the whole struct is 32-bit aligned when instantiated.
  uint32 eSource;	// How this message arrived (so we can send it back)
  MsgPacket oMsg;	// The message
})
MsgInternalPacket;

#ifdef __cplusplus
}
#endif

#endif  // _MSG_FORMAT_H_
