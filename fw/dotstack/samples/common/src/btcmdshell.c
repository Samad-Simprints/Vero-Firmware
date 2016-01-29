/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#include "btapp.h"
#include "btcmdshell.h"

#include "cdbt/bt/bt_std.h"
#include "cdbt/bt/bt_system.h"
#include "cdbt/spp/spp.h"

#include <stdarg.h>

#define RFCOMM_CHANNEL_CMDSHELL       30

#define TX_BUFFER_SIZE                (4096)
#define RX_BUFFER_SIZE                30
#define CMD_BUFFER_SIZE               128
#define MAX_CMD_PARAMS                5
#define MAX_MESSAGE_LEN               128

#define TX_BUFFER_SIZE_MASK           (TX_BUFFER_SIZE - 1)

static btcmdshell_CommandHandler mHandler;
static bt_spp_port_t*            mPort = NULL;
static char                      mConnected = 0;
static char                      mSending = 0;
static char                      mReceiving = 0;
static bt_int                    mBytesReceived;
static bt_int                    mCmdLen = 0;

static char                      mTxBuffer[TX_BUFFER_SIZE];
static volatile int              mTxHead;
static int                       mTxTail;

static char                      mRxBuffer[RX_BUFFER_SIZE];
static char                      mCmdBuffer[CMD_BUFFER_SIZE + 1];
static const char*               mCmdParams[MAX_CMD_PARAMS];
static char                      mNumBuffer[37];

// Buffer for composing output messages.
static char                      mBuffer[MAX_MESSAGE_LEN];

// Tx buffer overflow indicator
static char                      mOverflow;

static void sppStateCallback(bt_spp_port_t* port, bt_spp_port_event_e evt, void* param);
static void sppReceiveCallback(bt_spp_port_t* port, bt_int bytesReceived, void* param);
static void sppSendCallback(bt_spp_port_t* port, bt_ulong bytesSent, bt_spp_send_status_e result, void* param);
static void processReceivedData(void);
static void processCommand(void);
static void writeToTxBuffer(const char* msg, int len);
static void sendTxBuffer(void);

void btcmdshell_init(btcmdshell_CommandHandler handler)
{
	mTxHead = 0;
	mTxTail = 0;
	mOverflow = 0;
	
	if (handler)
	{
		mHandler = handler;

		bt_spp_init();
		mPort = bt_spp_allocate(bt_sys_get_l2cap_manager(), &sppStateCallback, NULL);
		if (mPort)
			bt_spp_listen(mPort, RFCOMM_CHANNEL_CMDSHELL);
	}
}

void btcmdshell_execCommand(const char* cmd, const char* params)
{
	int len = bt_min(strlen(cmd), CMD_BUFFER_SIZE);
	
	memcpy(mCmdBuffer, cmd, len);
	
	if (len < CMD_BUFFER_SIZE && params)
	{
		int paramsLen = bt_min(strlen(params), CMD_BUFFER_SIZE - len - 1);
		
		mCmdBuffer[len++] = ' ';
		memcpy(mCmdBuffer + len, params, paramsLen);
		len += paramsLen;
	}

	mCmdBuffer[len] = 0;
	processCommand();
}

void btcmdshell_dumpMemory(const char* p, unsigned int len)
{
	unsigned int i, index = 0;

	btcmdshell_write(NULL, "\r\n");
	
	for (i = 0; i < len; i++)
	{
		const char* byte_str = _ulong2str(p[i]);

		if (i % 16 == 0)
		{
			index = 0;
			mBuffer[index++] = 0;
		}

		mBuffer[index++] = '0';
		mBuffer[index++] = 'x';

		if (*(byte_str + 1) == 0)
		{
			mBuffer[index++] = '0';
			mBuffer[index++] = *byte_str;
		}
		else
		{
			mBuffer[index++] = *byte_str;
			mBuffer[index++] = *(byte_str + 1);
		}
		if ((i + 1) % 16 == 0)
		{
			mBuffer[index++] = '\r';
			mBuffer[index++] = '\n';
			writeToTxBuffer(mBuffer, index);
		}
		else
		{
			mBuffer[index++] = ' ';
		}
	}

	if (i % 16)
	{
		mBuffer[index++] = '\r';
		mBuffer[index++] = '\n';
		writeToTxBuffer(mBuffer, index);
	}
}

void btcmdshell_write(const char* tag, const char* format, ...)
{
	int index = 0;
	int size = MAX_MESSAGE_LEN;
	char c;
	va_list ap;
	
	va_start(ap, format);
	
	if (tag != NULL)
	{
		index = 0;
		while (*tag && index < 10) // limit tag to 10 characters
		{
			mBuffer[index++] = *tag++;
		}
		mBuffer[index++] = ':';
		size -= index;
	}
	
	size -= 1;//3; // reserve space for end of line
	while (*format && index < size)
	{
		c = *format++;
		if (c == '\\')
		{
			c = *format;
			if (c != 0)
			{
				format++;
				mBuffer[index++] = c;
			}
		}
		else if (c == '%')
		{
			c = *format;
			if (c != 0)
			{
				int i = 0;
				
				format++;
				if (c == 'd')
				{
					int val, r;
					val =  va_arg(ap, int);
					do
					{
						r = val % 10;
						val /= 10;
						mNumBuffer[i++] = r + '0';
					}
					while (val);
				}
				else if (c == 'x')
				{
					unsigned int val, r;
					val =  va_arg(ap, unsigned int);
					do
					{
						r = val % 16;
						val /= 16;
						if (r < 10)
							mNumBuffer[i++] = r + '0';
						else
							mNumBuffer[i++] = r + 'A' - 10;
					}
					while (val);
				}
				else if (c == 'b')
				{
					bt_bdaddr_t* bdaddr;
					
					bdaddr = va_arg(ap, bt_bdaddr_t*);

					mNumBuffer[i++] = *_ulong2str(bdaddr->bd_addr_l & 0x0000000F);
					mNumBuffer[i++] = *_ulong2str((bdaddr->bd_addr_l & 0x000000F0) >> 4);
					mNumBuffer[i++] = ':';
					mNumBuffer[i++] = *_ulong2str((bdaddr->bd_addr_l & 0x00000F00) >> 8);
					mNumBuffer[i++] = *_ulong2str((bdaddr->bd_addr_l & 0x0000F000) >> 12);
					mNumBuffer[i++] = ':';
					mNumBuffer[i++] = *_ulong2str((bdaddr->bd_addr_l & 0x000F0000) >> 16);
					mNumBuffer[i++] = *_ulong2str((bdaddr->bd_addr_l & 0x00F00000) >> 20);
					mNumBuffer[i++] = ':';
					mNumBuffer[i++] = *_ulong2str((bdaddr->bd_addr_l & 0x0F000000) >> 24);
					mNumBuffer[i++] = *_ulong2str((bdaddr->bd_addr_l & 0xF0000000) >> 28);
					mNumBuffer[i++] = ':';
					mNumBuffer[i++] = *_ulong2str(bdaddr->bd_addr_m & 0x000F);
					mNumBuffer[i++] = *_ulong2str((bdaddr->bd_addr_m & 0x00F0) >> 4);
					mNumBuffer[i++] = ':';
					mNumBuffer[i++] = *_ulong2str((bdaddr->bd_addr_m & 0x0F00) >> 8);
					mNumBuffer[i++] = *_ulong2str((bdaddr->bd_addr_m & 0xF000) >> 12);
				}
				else if (c == 'u')
				{
					bt_uuid_t* uuid;
					
					uuid = va_arg(ap, bt_uuid_t*);
					
					mNumBuffer[i++] = *_ulong2str((uuid->uuid0 & 0x0000000F));
					mNumBuffer[i++] = *_ulong2str((uuid->uuid0 & 0x000000F0) >> 4);
					mNumBuffer[i++] = *_ulong2str((uuid->uuid0 & 0x00000F00) >> 8);
					mNumBuffer[i++] = *_ulong2str((uuid->uuid0 & 0x0000F000) >> 12);
					mNumBuffer[i++] = *_ulong2str((uuid->uuid0 & 0x000F0000) >> 16);
					mNumBuffer[i++] = *_ulong2str((uuid->uuid0 & 0x00F00000) >> 20);
					mNumBuffer[i++] = *_ulong2str((uuid->uuid0 & 0x0F000000) >> 24);
					mNumBuffer[i++] = *_ulong2str((uuid->uuid0 & 0xF0000000) >> 28);
					mNumBuffer[i++] = *_ulong2str((uuid->uuid1 & 0x0000000F));
					mNumBuffer[i++] = *_ulong2str((uuid->uuid1 & 0x000000F0) >> 4);
					mNumBuffer[i++] = *_ulong2str((uuid->uuid1 & 0x00000F00) >> 8);
					mNumBuffer[i++] = *_ulong2str((uuid->uuid1 & 0x0000F000) >> 12);
					mNumBuffer[i++] = '-';
					mNumBuffer[i++] = *_ulong2str((uuid->uuid1 & 0x000F0000) >> 16);
					mNumBuffer[i++] = *_ulong2str((uuid->uuid1 & 0x00F00000) >> 20);
					mNumBuffer[i++] = *_ulong2str((uuid->uuid1 & 0x0F000000) >> 24);
					mNumBuffer[i++] = *_ulong2str((uuid->uuid1 & 0xF0000000) >> 28);
					mNumBuffer[i++] = '-';
					mNumBuffer[i++] = *_ulong2str((uuid->uuid2 & 0x0000000F));
					mNumBuffer[i++] = *_ulong2str((uuid->uuid2 & 0x000000F0) >> 4);
					mNumBuffer[i++] = *_ulong2str((uuid->uuid2 & 0x00000F00) >> 8);
					mNumBuffer[i++] = *_ulong2str((uuid->uuid2 & 0x0000F000) >> 12);
					mNumBuffer[i++] = '-';
					mNumBuffer[i++] = *_ulong2str((uuid->uuid2 & 0x000F0000) >> 16);
					mNumBuffer[i++] = *_ulong2str((uuid->uuid2 & 0x00F00000) >> 20);
					mNumBuffer[i++] = *_ulong2str((uuid->uuid2 & 0x0F000000) >> 24);
					mNumBuffer[i++] = *_ulong2str((uuid->uuid2 & 0xF0000000) >> 28);
					mNumBuffer[i++] = '-';
					mNumBuffer[i++] = *_ulong2str((uuid->uuid3 & 0x0000000F));
					mNumBuffer[i++] = *_ulong2str((uuid->uuid3 & 0x000000F0) >> 4);
					mNumBuffer[i++] = *_ulong2str((uuid->uuid3 & 0x00000F00) >> 8);
					mNumBuffer[i++] = *_ulong2str((uuid->uuid3 & 0x0000F000) >> 12);
					mNumBuffer[i++] = *_ulong2str((uuid->uuid3 & 0x000F0000) >> 16);
					mNumBuffer[i++] = *_ulong2str((uuid->uuid3 & 0x00F00000) >> 20);
					mNumBuffer[i++] = *_ulong2str((uuid->uuid3 & 0x0F000000) >> 24);
					mNumBuffer[i++] = *_ulong2str((uuid->uuid3 & 0xF0000000) >> 28);
					
				}
				else if (c == 's')
				{
					char* s;
					
					s = va_arg(ap, char*);
					
					while (index < size && *s)
					{
						mBuffer[index++] = *s++;
					}
				}
				
				while (index < size && i > 0)
				{
					mBuffer[index++] = mNumBuffer[--i];
				}
			}
		}
		else
		{
			mBuffer[index++] = c;
		}
	}
//	mBuffer[index++] = '\r';
//	mBuffer[index++] = '\n';
	mBuffer[index++] = 0;

	writeToTxBuffer(mBuffer, index);
				
	va_end(ap);
}

static void sppStateCallback(bt_spp_port_t* port, bt_spp_port_event_e evt, void* param)
{
	switch (evt)
	{
		case SPP_PORT_EVENT_CONNECTION_FAILED:
			break;

		case SPP_PORT_EVENT_CONNECTED:
			mConnected = 1;
			mSending = 0;
			mReceiving = 1;
			bt_spp_receive(mPort, mRxBuffer, sizeof(mRxBuffer), &sppReceiveCallback);
			break;

		case SPP_PORT_EVENT_DISCONNECTED:
			mConnected = 0;
			mSending = 0;
			mReceiving = 0;
			mTxHead = 0;
			mTxTail = 0;
			mOverflow = 0;
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
}


static void sppSendCallback(bt_spp_port_t* port, bt_ulong bytesSent, bt_spp_send_status_e result, void* param)
{
	mSending = 0;

	mTxHead = (mTxHead + bytesSent) & TX_BUFFER_SIZE_MASK;
	sendTxBuffer();
	
	if (mBytesReceived)
	{
		processReceivedData();
	}
}

static void processReceivedData(void)
{
	bt_int i;
	bt_int bytesReceived = mBytesReceived;
	bt_byte cmdReceived = 0;

//	memcpy(mBuffer, mRxBuffer, bytesReceived);
	writeToTxBuffer(mRxBuffer, bytesReceived);

	mBytesReceived = 0;
	
	for (i = 0; i < bytesReceived; i++)
	{
		if (mCmdLen < CMD_BUFFER_SIZE)
		{
			if (mRxBuffer[i] == '\r' || mRxBuffer[i] == '\n')
			{
				cmdReceived = 1;
				mCmdBuffer[mCmdLen] = 0;
			}
			else
			{
				mCmdBuffer[mCmdLen++] = mRxBuffer[i];
			}
		}
		else
		{
			cmdReceived = 1;
			mCmdBuffer[mCmdLen] = 0;
		}

		if (cmdReceived)
		{
			processCommand();
			mCmdLen = 0;
		}
	}

	if (!mReceiving)
	{
		mReceiving = 1;
		bt_spp_receive(mPort, mRxBuffer, sizeof(mRxBuffer), &sppReceiveCallback);
	}
}

static void processCommand(void)
{
	char* p = strtok(mCmdBuffer, " ");
	char i = 0;
	const char* cmdId = NULL;
	bt_byte paramsCount = 0;

	while (p != 0)
	{
		if (!i++)
			cmdId = p;
		else
		{
			if (paramsCount < MAX_CMD_PARAMS)
				mCmdParams[paramsCount++] = p;
		}
		
		p = strtok(NULL, " ");
	}

	if (cmdId)
	{
		mHandler(cmdId, mCmdParams, paramsCount);
		writeToTxBuffer("\r\n> ", 4);
	}
}

static void writeToTxBuffer(const char* msg, int len)
{
	int avail;
	
	// Get available space in the buffer
	avail = TX_BUFFER_SIZE - 1 - ((mTxTail - mTxHead) & TX_BUFFER_SIZE_MASK);
	
	avail -= 3; // reserve space for the overflow indicator
	
	if (len > avail)
	{
		if (mOverflow)
			return;
		mOverflow = 1;
		msg = "*\r\n";  // overflow indicator
		len = 3;
	}
	else
	{
		mOverflow = 0;
	}

	if (len > 0)
	{
		if ((TX_BUFFER_SIZE - mTxTail) >= len)
		{
			// Whole message can be written at once.
			memcpy(mTxBuffer + mTxTail, msg, len);
		}
		else
		{
			// Part of the message is written to the end of the buffer
			// and next part to the beginning.
			
			// Free bytes available from the tail pointer to the end of the buffer
			int avail2 = TX_BUFFER_SIZE - mTxTail;
			memcpy(mTxBuffer + mTxTail, msg, avail2);
			memcpy(mTxBuffer, msg + avail2, len - avail2);
		}
		
		mTxTail = (mTxTail + len) & TX_BUFFER_SIZE_MASK;
		
		sendTxBuffer();
	}
}

static void sendTxBuffer(void)
{
	if (mConnected && !mSending)
	{
		// Calculate how many bytes can be transmitted
		int len = mTxTail - mTxHead;
		if (len < 0)
			len = TX_BUFFER_SIZE - mTxHead;
		
		if (len)
		{
			bt_spp_send(mPort, mTxBuffer + mTxHead, len, &sppSendCallback);
			mSending = 1;
		}
	}
}
