/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#include "btapp.h"
#include "btmgr.h"
#include "btapp_speed-test_sdpdb.h"
#include "btdisplay.h"

#include "cdbt/bt/bt_std.h"
#include "cdbt/bt/bt_system.h"
#include "cdbt/spp/spp.h"
#include "cdbt/hci/baseband.h"

#include "hal.h"

static btapp_Status    mAppStatus = BTAPP_STATUS_IDLE;
static bt_spp_port_t*  mPort = NULL;
static char            mConnected = 0;

typedef struct {
  int mBytesToSend;
  uint8_t *mDataToSend;
  void (*pfCompletionCallback)(int result);
} tSPPCallback;

static tSPPCallback sSPPCallbackData;

#ifdef FAST_CONFIG
#if defined(__ICC430__) || defined(STM32L15XXB_128K) || defined(__PIC24FJ256GB110__) || defined(EFM32G890F128)
#define BUFFER_SIZE    650
#else
#define BUFFER_SIZE    1024
#endif
#else
#define BUFFER_SIZE        30
#endif

static char            mTxBuffer[BUFFER_SIZE];
static char            mRxBuffer[BUFFER_SIZE];


static void sysStartCallback(bt_bool success, void* param);
static void sppStateCallback(bt_spp_port_t* port, bt_spp_port_event_e evt, void* param);
static void sppReceiveCallback(bt_spp_port_t* port, bt_int bytesReceived, void* param);
static void sppSendCallback(bt_spp_port_t* port, bt_ulong bytesSent, bt_spp_send_status_e result, void* param);
static void setConnected(char connected);

void btapp_init(void)
{
  mAppStatus = BTAPP_STATUS_INITIALIZED;
}

void btapp_start(void)
{
  btdisplay_setApplication(BTAPP_APP_SPEED_TEST);
  setConnected(0);
  btdisplay_update();

  // Initialize with Role switch allowed 
  // and all low power modes - SNIFF, PARK, HOLD - disallowed
  bt_sys_init_ex(HCI_LINK_POLICY_ENABLE_ROLE_SWITCH);
  bt_spp_init();

  bt_sys_start(BT_TRUE, BT_TRUE,
                  sdp_db_speed_test, sdp_db_speed_test_len,
                  &sysStartCallback, NULL);
}

btapp_Status btapp_getStatus(void)
{
  return mAppStatus;
}

static void sysStartCallback(bt_bool success, void* param)
{
  if (success)
  {
    bt_bdaddr_t bdaddr;
    bt_bool paired;

    mAppStatus = BTAPP_STATUS_STARTED;
    setConnected(0);

    paired = btmgr_getLastConnectedDevice(&bdaddr);
    btdisplay_setPairedState(paired);
    btdisplay_update();

    mPort = bt_spp_allocate(bt_sys_get_l2cap_manager(), &sppStateCallback, &sSPPCallbackData);
    bt_spp_listen(mPort, RFCOMM_SERIAL_PORT_CH_1);
  }
  else
  {
    btapp_onFatalError();
  }
}


static void sppStateCallback(bt_spp_port_t* port, bt_spp_port_event_e evt, void* param)
{
  switch (evt)
  {
    case SPP_PORT_EVENT_CONNECTION_FAILED:
      setConnected(0);
      break;

    case SPP_PORT_EVENT_CONNECTED:
      setConnected(1);
      bt_spp_receive(mPort, mRxBuffer, sizeof(mRxBuffer), &sppReceiveCallback);
      break;

    case SPP_PORT_EVENT_DISCONNECTED:
      setConnected(0);
      break;

    case SPP_PORT_EVENT_SEND_PROGRESS:
      break;

    default:
      break;
  }
}

// cancel any pending transfers and disconnect
int sppCancelAndDisconnect()
{
  if ( mPort )
  {
    bt_spp_cancel_receive( mPort );
    bt_spp_cancel_send( mPort );
    bt_spp_disconnect(mPort);
  }
}

static void sppReceiveCallback(bt_spp_port_t* port, bt_int bytesReceived, void* param)
{
  tSPPCallback *psCallbackParam = (tSPPCallback *)param;
  tEventRxData oRxData;

  // deliver the data to the HAL
  oRxData.iLength = bytesReceived;
  oRxData.pcData = mRxBuffer;
  vBtCallbackFunction( INTERFACE_EVENT_RX_DATA, &oRxData);

  // queue another read
  bt_spp_receive(mPort, mRxBuffer, sizeof(mRxBuffer), &sppReceiveCallback);
}

// kick off an outbound transfer (TRUE if started, false if not)
int sppStartSendData(void *data, int count, void (*callback)(int result))
{
  tSPPCallback *psCallbackParam = &sSPPCallbackData;

  psCallbackParam->mBytesToSend = count;
  psCallbackParam->mDataToSend = (uint8_t*)data;
  psCallbackParam->pfCompletionCallback = callback;

  return bt_spp_send(mPort, psCallbackParam->mDataToSend, min(psCallbackParam->mBytesToSend, bt_spp_get_frame_length(mPort)), &sppSendCallback);
}

// send the next chunk if there is more to send
static void sppSendCallback(bt_spp_port_t* port, bt_ulong bytesSent, bt_spp_send_status_e result, void* param)
{
  tSPPCallback *psCallbackParam = (tSPPCallback *)param;
  bt_bool boDone;
  bt_bool boOk = TRUE;

  psCallbackParam->mBytesToSend -= bytesSent;
  psCallbackParam->mDataToSend += bytesSent;

  boDone = (psCallbackParam->mBytesToSend <= 0);

  if ( !boDone )
  {
    boOk = bt_spp_send(mPort, psCallbackParam->mDataToSend, min(psCallbackParam->mBytesToSend, bt_spp_get_frame_length(mPort)), &sppSendCallback);
  }

  if ( boDone || !boOk )
  {
    psCallbackParam->pfCompletionCallback( boOk );
  }
}

static void setConnected(char connected)
{
  // Notify the Bluetooth callback
  tEventConnDisconn oInterface;
  oInterface.eInterface = BT_HOST;
  vBtCallbackFunction( (connected ? INTERFACE_EVENT_CONNECTED : INTERFACE_EVENT_DISCONNECTED), &oInterface);
  
  mConnected = connected;
  btdisplay_setConnectedState((bt_bool)connected);
  btdisplay_update();
}


void btapp_onButtonDown(bt_uint button, bt_uint repeatCount)
{
}


void btapp_onButtonUp(bt_uint button, bt_uint repeatCount)
{
}


//
// Bluetooth device name and class
// -------------------------------------------------------------------
//
const char* bt_oem_get_device_name(void)
{
  return "Index Scanner";
}


bt_long bt_oem_get_device_class(void)
{
  return COS_INFORMATION | COD_MAJOR_COMPUTER | COD_MINOR_COMPUTER_HANDHELD;
}
