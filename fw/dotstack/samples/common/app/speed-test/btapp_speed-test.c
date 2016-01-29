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

#define TEST_TX        1
#define TEST_RX        2
#define TEST_TXRX      3

static btapp_Status    mAppStatus = BTAPP_STATUS_IDLE;
static bt_spp_port_t*  mPort = NULL;
static char            mConnected = 0;
static char            mSending = 0;
static char            mReceiving = 0;
static bt_int          mBytesReceived;
static char            mTest = 0;

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
static void processReceivedData(void);
static void sendTestData(void);
static void setConnected(char connected);


void btapp_init(void)
{
	mAppStatus = BTAPP_STATUS_INITIALIZED;
}


void btapp_start(void)
{
	btdisplay_setApplication(BTAPP_APP_SPEED_TEST);
	btdisplay_setConnectedState(0);
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

		mPort = bt_spp_allocate(bt_sys_get_l2cap_manager(), &sppStateCallback, NULL);
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
			mSending = 0;
			mReceiving = 1;
			bt_spp_receive(mPort, mRxBuffer, sizeof(mRxBuffer), &sppReceiveCallback);
			break;

		case SPP_PORT_EVENT_DISCONNECTED:
			setConnected(0);
			mSending = 0;
			mReceiving = 0;
			mTest = 0;
			break;

		case SPP_PORT_EVENT_SEND_PROGRESS:
			break;

		default:
			break;
	}
}


static void sppReceiveCallback(bt_spp_port_t* port, bt_int bytesReceived, void* param)
{
	mReceiving = 0;
	mBytesReceived = bytesReceived;
	if (!mSending)
	{
		processReceivedData();
	}
	
	sendTestData();
}


static void sppSendCallback(bt_spp_port_t* port, bt_ulong bytesSent, bt_spp_send_status_e result, void* param)
{
	mSending = 0;
	
	if (mBytesReceived)
	{
		processReceivedData();
	}
	
	sendTestData();
}


static void sendTestData(void)
{
	if (!mSending && (mTest == TEST_RX || mTest == TEST_TXRX))
	{
		memset(mTxBuffer, 0x30, sizeof(mTxBuffer));
		
		mSending = 1;
		bt_spp_send(mPort, mTxBuffer, bt_spp_get_frame_length(mPort), &sppSendCallback);
	}
}

static void processReceivedData(void)
{
	bt_int i;
	bt_int bytesReceived = mBytesReceived;
	char* buffer = mRxBuffer;
	int bufferLen = bytesReceived;
	char cmd = 0;

	for (i = 0; i < bufferLen; i++)
	{
		if (buffer[i] == '\r')
		{
			buffer[i] = 0;
			if (!strcmp(buffer, "txrx"))
			{
				mTest = TEST_TXRX;
				cmd = 1;
			}
			else if (!strcmp(buffer, "tx"))
			{
				mTest = TEST_TX;
				cmd = 1;
			}
			else if (!strcmp(buffer, "rx"))
			{
				mTest = TEST_RX;
				cmd = 1;
			}
			else if (!strcmp(buffer, "stop"))
			{
				mTest = 0;
				cmd = 1;
			}
			
			if (cmd)
			{
				bytesReceived = i + 1;
				memcpy(mTxBuffer, buffer, i);
				mTxBuffer[i] = '\r';
				break;
			}
			else
			{
				buffer += i + 1;
				bufferLen -= i + 1;
				i = -1;
			}
		}
	}
	
	if (cmd)
	{
		mSending = 1;
		bt_spp_send(mPort, mTxBuffer, bytesReceived, &sppSendCallback);
		mBytesReceived = 0;
	}
	

	if (!mReceiving)
	{
		mReceiving = 1;
		bt_spp_receive(mPort, mRxBuffer, sizeof(mRxBuffer), &sppReceiveCallback);
	}
}

static void setConnected(char connected)
{
	mConnected = connected;
	btdisplay_setConnectedState((bt_bool)connected);
	btdisplay_update();
}


void btapp_onButtonDown(bt_uint button, bt_uint repeatCount)
{
	if (button == BTAPP_BUTTON_S1 && mConnected && !mSending && mTest)
	{
		strcpy(mTxBuffer, "\r");
		mSending = 1;
		bt_spp_send(mPort, mTxBuffer, strlen(mTxBuffer), &sppSendCallback);
	}
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
	return "dotstack Speed Test(Index 1800)";
}


bt_long bt_oem_get_device_class(void)
{
	return COS_INFORMATION |
		COD_MAJOR_COMPUTER | COD_MINOR_COMPUTER_HANDHELD;
}
