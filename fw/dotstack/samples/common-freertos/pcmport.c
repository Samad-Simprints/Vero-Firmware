/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#include "board.h"
#include "error.h"
#include "btaudio.h"
#include "bttask.h"
#include "btapp.h"
#include "btpcmport.h"
#include "pcmport.h"
#include "cdbt/utils/queue.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#ifndef BTAPP_ROUTE_VOICE_VIA_HOST

#define BTLOG_TAG "PCMPORT"
#include "btlog_define.h"

#define MESSAGE_QUEUE_SIZE        20
#define NOTIFICATION_QUEUE_SIZE   20

#define NOTIF_SENDING_STARTED     1
#define NOTIF_SENDING_STOPPED     2
#define NOTIF_PACKET_SENT         3
#define NOTIF_SENDING_PAUSED      4
#define NOTIF_PACKET_RECEIVED     5
#define NOTIF_RECEIVING_STARTED   6
#define NOTIF_RECEIVING_STOPPED   7
#define NOTIF_RECEIVING_PAUSED    8
#define NOTIF_CONNECTED           9
#define NOTIF_DISCONNECTED        10
#define NOTIF_INITIALIZED         11
#define NOTIF_START_SENDING       12
#define NOTIF_STOP_SENDING        13

#define SEND_PACKET_QUEUE_SIZE    8
#define NUM_SEND_BUFFERS          2

#ifdef BTAPP_PCMPORT_NUM_RECEIVE_BUFFERS
	#define NUM_RECEIVE_BUFFERS      BTAPP_PCMPORT_NUM_RECEIVE_BUFFERS
#else
	#define NUM_RECEIVE_BUFFERS      16
#endif


#define STATE_IDLE                0
#define STATE_SENDING             1
#define STATE_PAUSED              2
#define STATE_SEND_REQUESTED      3
#define STATE_SEND_STARTING       4

typedef struct
{
	uint8_t notif;
	union
	{
		struct
		{
			btaudio_MediaPacket* packet;
		} packetSent;

		struct
		{
			btaudio_MediaPacket* packet;
		} packetReceived;
	} param;
} PcmPortTaskNotification;

static xQueueHandle              mMessageQueue;
static xQueueHandle              mNotifQueue;
static xQueueHandle              mSendPacketQueue;

static btpcmport_Callback        mBtPcmPortCallback;

static uint8_t                   mFirstSendBuffer;
static uint8_t                   mSendingState;

static pcmport_SendBuffer        mSendBuffers[NUM_SEND_BUFFERS];
static uint8_t                   mSendQueueHead;
static uint8_t                   mSendQueueTail;
static volatile uint8_t          mSendQueueLen;
static pcmport_SendBuffer*       mCurrentSendBuffer;

static pcmport_ReceiveBuffer     mReceiveBuffers[NUM_RECEIVE_BUFFERS];
static uint8_t                   mReceiveFreeQueueHead;
static uint8_t                   mReceiveFreeQueueTail;
static volatile uint8_t          mReceiveFreeQueueLen;
static uint8_t                   mReceiveFilledQueueHead;
static uint8_t                   mReceiveFilledQueueTail;
static volatile uint8_t          mReceiveFilledQueueLen;
static pcmport_ReceiveBuffer*    mCurrentReceiveBuffer;
static size_t                    mReceiveBufferDataPos;

static btaudio_MediaPacket*      mCurrentSendPacket;
static const uint8_t*            mSendPacketData;
static size_t                    mSendPacketDataPos;
static size_t                    mSendPacketDataLen;

static bt_queue_element_t*       mMediaReceiveQueue;
static btaudio_MediaPacket*      mCurrentReceivePacket;

static uint8_t                   mBtAudioInterface;
static uint8_t                   mAudioProcessor;

static void pcmPortTaskProc(void* params);
static void pcmPortSignalHandler(void);
static void sendNotification(PcmPortTaskNotification* notif);
static void fillSendBuffer(void);
static void fillReceivePacket(void);

void btpcmport_pal_init(btpcmport_Callback callback)
{
	int i;

	mSendPacketData = NULL;
	mSendPacketDataPos = 0;
	mSendPacketDataLen = 0;
	mCurrentSendPacket = NULL;
	mFirstSendBuffer = BT_TRUE;
	mSendingState = STATE_IDLE;
	mCurrentSendBuffer = NULL;
	mSendQueueHead = 0;
	mSendQueueTail = 0;
	mSendQueueLen = 0;
	mMediaReceiveQueue = NULL;
	mReceiveFreeQueueHead = 0;
	mReceiveFreeQueueTail = 0;
	mReceiveFreeQueueLen = NUM_RECEIVE_BUFFERS;
	mReceiveFilledQueueHead = 0;
	mReceiveFilledQueueTail = 0;
	mReceiveFilledQueueLen = 0;
	mCurrentReceiveBuffer = NULL;
	mReceiveBufferDataPos = 0;

	for (i = 0; i < NUM_RECEIVE_BUFFERS; i++)
		mReceiveBuffers[i].maxDataLen = PCMPORT_RECEIVE_BUFFER_SIZE;

	
	mBtPcmPortCallback = callback;
	
	bttask_setScoPortSignalHandler(&pcmPortSignalHandler);

	mMessageQueue = xQueueCreate(MESSAGE_QUEUE_SIZE, sizeof(ScoPortTaskMessage));
	mNotifQueue = xQueueCreate(NOTIFICATION_QUEUE_SIZE, sizeof(PcmPortTaskNotification));
	mSendPacketQueue = xQueueCreate(SEND_PACKET_QUEUE_SIZE, sizeof(btaudio_MediaPacket*));
	
	pcmport_pal_init();
	
	xTaskCreate(
			&pcmPortTaskProc,               // task proc
			(signed portCHAR*)"SCOPORT",    // task name
			configSCOPORT_TASK_STACK_SIZE,  // stack depth
			NULL,                           // task parameters
			configSCOPORT_TASK_PRIORITY,    // priority
			NULL);                          // task handle (not used)
}

void btpcmport_pal_configure(bt_ulong samplingRate, bt_byte codecType, void* codecConfig, bt_byte btAudioInterface, bt_byte audioProcessor)
{
	mBtAudioInterface = btAudioInterface;
	mAudioProcessor = audioProcessor;
	
	pcmport_pal_configure(samplingRate, codecType, codecConfig);
}

void btpcmport_pal_start(void)
{
	ScoPortTaskMessage msg;

	msg.msg = MSG_START_SENDING;
	xQueueSendToBack(mMessageQueue, &msg, portMAX_DELAY);

	msg.msg = MSG_START_RECEIVING;
	xQueueSendToBack(mMessageQueue, &msg, portMAX_DELAY);
}

void btpcmport_pal_stop(void)
{
	ScoPortTaskMessage msg;

	msg.msg = MSG_STOP_SENDING;
	xQueueSendToBack(mMessageQueue, &msg, portMAX_DELAY);

	msg.msg = MSG_STOP_RECEIVING;
	xQueueSendToBack(mMessageQueue, &msg, portMAX_DELAY);
}

void btpcmport_pal_receivePacket(btaudio_MediaPacket* packet)
{
	ScoPortTaskMessage msg;

	msg.msg = MSG_RECEIVE_PACKET;
	msg.param.receivePacket.packet = packet;
	xQueueSendToBack(mMessageQueue, &msg, portMAX_DELAY);
}

void btpcmport_pal_sendPacket(btaudio_MediaPacket* packet)
{
	if (mSendingState == STATE_IDLE)
	{
		PcmPortTaskNotification notif;

		notif.notif = NOTIF_PACKET_SENT;
		notif.param.packetSent.packet = packet;
		sendNotification(&notif);
		BTLOG("Packet skipped");
	}
	else
	{
		ScoPortTaskMessage msg;

		xQueueSendToBack(mSendPacketQueue, &packet, portMAX_DELAY);

		msg.msg = MSG_SEND_PACKET;
		msg.param.sendPacket.packet = packet;
		xQueueSendToBack(mMessageQueue, &msg, portMAX_DELAY);
	}
}

void btpcmport_pal_connect(bt_uint hconn, bt_id cid)
{
	pcmport_pal_connect(hconn, cid);
}

void btpcmport_pal_disconnect(void)
{
	pcmport_pal_disconnect();
}

static void pcmPortTaskProc(void* params)
{
	ScoPortTaskMessage msg;
	PcmPortTaskNotification notif;
	
	for (;;)
	{
		xQueueReceive(mMessageQueue, &msg, portMAX_DELAY);
		
		switch (msg.msg)
		{
			case MSG_START_SENDING:
				if (mAudioProcessor & BTAUDIO_SNK_PROCESSOR_HOST)
				{
					mSendingState = STATE_SEND_REQUESTED;
				}
				else if (mAudioProcessor & BTAUDIO_SNK_PROCESSOR_EXT)
				{
					notif.notif = NOTIF_START_SENDING;
					sendNotification(&notif);
				}
				
				break;
				
			case MSG_STOP_SENDING:
			{
				notif.notif = NOTIF_STOP_SENDING;
				sendNotification(&notif);
				
				break;
			}
				
			case MSG_SEND_PACKET:
				fillSendBuffer();
				if (mSendingState == STATE_SEND_REQUESTED && mSendQueueLen >= NUM_SEND_BUFFERS)
				{
					mSendingState = STATE_SEND_STARTING;
					
					notif.notif = NOTIF_START_SENDING;
					sendNotification(&notif);
				}
				break;
				
			case MSG_FILL_SEND_BUFFER:
				if (mSendingState != STATE_IDLE)
				{
					fillSendBuffer();
				}
				break;

			case MSG_START_RECEIVING:
				pcmport_pal_startReceiving();

				notif.notif = NOTIF_RECEIVING_STARTED;
				sendNotification(&notif);

				break;

			case MSG_STOP_RECEIVING:
				pcmport_pal_stopReceiving();
				
				taskENTER_CRITICAL();
				{
					mMediaReceiveQueue = NULL;
					mReceiveFreeQueueHead = 0;
					mReceiveFreeQueueTail = 0;
					mReceiveFreeQueueLen = NUM_RECEIVE_BUFFERS;

					mReceiveFilledQueueHead = 0;
					mReceiveFilledQueueTail = 0;
					mReceiveFilledQueueLen = 0;

					mCurrentReceivePacket = NULL;
					mCurrentReceiveBuffer = NULL;
				}
				taskEXIT_CRITICAL();

				notif.notif = NOTIF_RECEIVING_STOPPED;
				sendNotification(&notif);

				break;

			case MSG_RECEIVE_PACKET:
				msg.param.receivePacket.packet->dataLen = 0;
				bt_q_add(&mMediaReceiveQueue, msg.param.receivePacket.packet);

				fillReceivePacket();

//				msg.msg = MSG_FILL_RECEIVE_PACKET;
//				xQueueSendToBack(mMessageQueue, &msg, portMAX_DELAY);

				break;
				
			case MSG_FILL_RECEIVE_PACKET:
				fillReceivePacket();
				break;

			case MSG_BUFFER_RECEIVED:

				if (++mReceiveFilledQueueTail == NUM_RECEIVE_BUFFERS)
					mReceiveFilledQueueTail = 0;
				mReceiveFilledQueueLen++;

				fillReceivePacket();

				break;

			case  MSG_NOTIFY_CONNECTED:
				notif.notif = NOTIF_CONNECTED;
				sendNotification(&notif);

				break;

			case MSG_NOTIFY_DISCONNECTED:
				notif.notif = NOTIF_DISCONNECTED;
				sendNotification(&notif);

				break;

			case MSG_NOTIFY_INITIALIZED:
				notif.notif = NOTIF_INITIALIZED;
				sendNotification(&notif);

				break;
		}
	}
}

pcmport_SendBuffer* pcmport_getNextSendBuffer(void)
{
	ScoPortTaskMessage msg;
	
	if (mSendQueueLen == 0)
	{
		return NULL;
	}
	
	if (mFirstSendBuffer)
	{
		mFirstSendBuffer = BT_FALSE;
	}
	else
	{
		if (mSendQueueLen == 1)
		{
			// TODO: fill the current buffer with silence
		}
		else
		{
			if (++mSendQueueHead == NUM_SEND_BUFFERS)
				mSendQueueHead = 0;
			mSendQueueLen--;
			
			msg.msg = MSG_FILL_SEND_BUFFER;
			xQueueSendToBack(mMessageQueue, &msg, portMAX_DELAY);
		}
	}
	
	return &mSendBuffers[mSendQueueHead];
}


pcmport_SendBuffer* pcmport_getNextSendBufferFromISR(portBASE_TYPE *higherPriorityTaskWoken)
{
	ScoPortTaskMessage msg;
	
	if (mSendQueueLen == 0)
	{
		return NULL;
	}
	
	if (mFirstSendBuffer)
	{
		mFirstSendBuffer = BT_FALSE;
	}
	else
	{
		if (mSendQueueLen == 1)
		{
			pcmport_SendBuffer* buffer = &mSendBuffers[mSendQueueHead];
			memset(buffer->data, 0, PCMPORT_SEND_BUFFER_SIZE * 2);
			buffer->dataLen = PCMPORT_SEND_BUFFER_SIZE;
		}
		else
		{
			if (++mSendQueueHead == NUM_SEND_BUFFERS)
				mSendQueueHead = 0;
			mSendQueueLen--;
			
			msg.msg = MSG_FILL_SEND_BUFFER;
			if (!xQueueSendToBackFromISR(mMessageQueue, &msg, higherPriorityTaskWoken))
			{
				btapp_onFatalError();
			}
		}
	}
	
	return &mSendBuffers[mSendQueueHead];
}

static void sendNotification(PcmPortTaskNotification* notif)
{
	xQueueSendToBack(mNotifQueue, notif, portMAX_DELAY);
	bttask_setSignal(BTTASK_SIG_SCOPORT);
}

static void fillSendBuffer(void)
{
	size_t copyLen;
	uint8_t sendQueueLen;
	PcmPortTaskNotification notif;

	for (;;)
	{
		if (mSendPacketData == NULL)
		{
			if (xQueuePeek(mSendPacketQueue, &mCurrentSendPacket, 0))
			{
				mSendPacketData = mCurrentSendPacket->data;
				mSendPacketDataLen = mCurrentSendPacket->dataLen;
				mSendPacketDataPos = 0;
			}
			else
			{
				return;
			}
		}
		
		if (!mCurrentSendBuffer)
		{
			taskENTER_CRITICAL();
			{
				sendQueueLen = mSendQueueLen;
			}
			taskEXIT_CRITICAL();
			if (sendQueueLen < NUM_SEND_BUFFERS)
			{
				mCurrentSendBuffer = &mSendBuffers[mSendQueueTail];
				mCurrentSendBuffer->dataLen = 0;
			}
			else
			{
				return;
			}
		}
		
		copyLen = (mSendPacketDataLen - mSendPacketDataPos) / 2;
		copyLen = bt_min(copyLen, PCMPORT_SEND_BUFFER_SIZE - mCurrentSendBuffer->dataLen);
		memcpy(mCurrentSendBuffer->data + mCurrentSendBuffer->dataLen, mSendPacketData + mSendPacketDataPos, copyLen * 2);
		mCurrentSendBuffer->dataLen += copyLen;
		mSendPacketDataPos += copyLen * 2;

		if (mCurrentSendBuffer->dataLen == PCMPORT_SEND_BUFFER_SIZE)
		{
			mCurrentSendBuffer = NULL;
			if (++mSendQueueTail == NUM_SEND_BUFFERS)
				mSendQueueTail = 0;
			taskENTER_CRITICAL();
			{
				mSendQueueLen++;
			}
			taskEXIT_CRITICAL();
		}
		
		if (mSendPacketDataPos == mSendPacketDataLen)
		{
			xQueueReceive(mSendPacketQueue, &mCurrentSendPacket, portMAX_DELAY);
			
			mSendPacketData = NULL;
			notif.notif = NOTIF_PACKET_SENT;
			notif.param.packetSent.packet = mCurrentSendPacket;
			mCurrentSendPacket = NULL;
			sendNotification(&notif);
		}
	}
}

static void fillReceivePacket(void)
{
	size_t copyLen;
	PcmPortTaskNotification notif;

	for (;;)
	{
		if (!mCurrentReceivePacket)
		{
			mCurrentReceivePacket = (btaudio_MediaPacket*)bt_q_get_head(&mMediaReceiveQueue, BT_FALSE);

			if (!mCurrentReceivePacket)
				return;
		}

		if (!mCurrentReceiveBuffer)
		{
			if (mReceiveFilledQueueLen)
			{
				mCurrentReceiveBuffer = &mReceiveBuffers[mReceiveFilledQueueHead];
				mReceiveBufferDataPos = 0;
			}
			else
			{
				return;
			}
		}

		copyLen = (mCurrentReceivePacket->maxDataLen - mCurrentReceivePacket->dataLen) / 2;
		copyLen = bt_min(copyLen, mCurrentReceiveBuffer->dataLen - mReceiveBufferDataPos);
		memcpy(mCurrentReceivePacket->data + mCurrentReceivePacket->dataLen,
			mCurrentReceiveBuffer->data + mReceiveBufferDataPos, copyLen * 2);
		mReceiveBufferDataPos += copyLen;
		mCurrentReceivePacket->dataLen += copyLen * 2;

		if (mReceiveBufferDataPos == mCurrentReceiveBuffer->dataLen)
		{
			mCurrentReceiveBuffer = NULL;
			if (++mReceiveFilledQueueHead == NUM_RECEIVE_BUFFERS)
				mReceiveFilledQueueHead = 0;
			if (++mReceiveFreeQueueTail == NUM_RECEIVE_BUFFERS)
				mReceiveFreeQueueTail = 0;
			taskENTER_CRITICAL();
			{
				mReceiveFilledQueueLen--;
				mReceiveFreeQueueLen++;
			}
			taskEXIT_CRITICAL();
		}

		if ((mCurrentReceivePacket->maxDataLen - mCurrentReceivePacket->dataLen) / 2  == 0)
		{
			bt_q_get_head(&mMediaReceiveQueue, BT_TRUE);

			notif.notif = NOTIF_PACKET_RECEIVED;
			notif.param.packetReceived.packet = mCurrentReceivePacket;
			mCurrentReceivePacket = NULL;
			sendNotification(&notif);
		}
	}
}

pcmport_ReceiveBuffer* pcmport_getReceiveBufferFromISR(portBASE_TYPE *higherPriorityTaskWoken)
{
	pcmport_ReceiveBuffer* buffer;

	if (!mReceiveFreeQueueLen)
		return NULL;

	buffer = &mReceiveBuffers[mReceiveFreeQueueHead++];
	buffer->dataLen = 0;
	if (mReceiveFreeQueueHead == NUM_RECEIVE_BUFFERS)
		mReceiveFreeQueueHead = 0;
	mReceiveFreeQueueLen--;

	return buffer;
}

void pcmport_sendPcmPortMessage(ScoPortTaskMessage* msg)
{
	xQueueSendToBack(mMessageQueue, msg, portMAX_DELAY);
}

void pcmport_sendPcmPortMessageFromISR(ScoPortTaskMessage* msg, signed portBASE_TYPE* higherPriorityTaskWoken)
{
	if (!xQueueSendToBackFromISR(mMessageQueue, msg, higherPriorityTaskWoken))
	{
		btapp_onFatalError();
	}
}

static void pcmPortSignalHandler(void)
{
	PcmPortTaskNotification notif;
	
	while (xQueueReceive(mNotifQueue, &notif, 0))
	{
		switch (notif.notif)
		{
			case NOTIF_SENDING_STARTED:
				mBtPcmPortCallback(BTSCOPORT_EVENT_SENDING_STARTED, NULL);
				break;
			case NOTIF_SENDING_PAUSED:
				mBtPcmPortCallback(BTSCOPORT_EVENT_SENDING_PAUSED, NULL);
				break;
			case NOTIF_SENDING_STOPPED:
				BTLOG("NOTIF_SENDING_STOPPED");
				mBtPcmPortCallback(BTSCOPORT_EVENT_SENDING_STOPPED, NULL);
				break;
			case NOTIF_PACKET_SENT:
//				BTLOG("NOTIF_PACKET_SENT");
				mBtPcmPortCallback(BTSCOPORT_EVENT_PACKET_SENT, notif.param.packetSent.packet);
				break;

			case NOTIF_RECEIVING_STARTED:
				break;

			case NOTIF_RECEIVING_STOPPED:
				break;

			case NOTIF_RECEIVING_PAUSED:
				break;

			case NOTIF_PACKET_RECEIVED:
//				BTLOG("NOTIF_PACKET_RECEIVED");
				mBtPcmPortCallback(BTSCOPORT_EVENT_PACKET_RECEIVED, notif.param.packetSent.packet);
				break;

			case NOTIF_CONNECTED:
				mBtPcmPortCallback(BTSCOPORT_EVENT_CONNECTED, NULL);
				break;

			case NOTIF_DISCONNECTED:
				mBtPcmPortCallback(BTSCOPORT_EVENT_DISCONNECTED, NULL);
				break;

			case NOTIF_INITIALIZED:
				mBtPcmPortCallback(BTSCOPORT_EVENT_INITIALIZED, NULL);
				break;
				
			case NOTIF_START_SENDING:
				mFirstSendBuffer = BT_TRUE;
				mSendingState = STATE_SENDING;
				pcmport_pal_startSending();

				notif.notif = NOTIF_SENDING_STARTED;
				sendNotification(&notif);
				
				break;
				
			case NOTIF_STOP_SENDING:
			{
				btaudio_MediaPacket*  packet;

				mSendingState = STATE_IDLE;

				pcmport_pal_stopSending();

				while (xQueuePeek(mSendPacketQueue, &packet, 0))
				{
					xQueueReceive(mSendPacketQueue, &packet, portMAX_DELAY);

					notif.notif = NOTIF_PACKET_SENT;
					notif.param.packetSent.packet = packet;
					sendNotification(&notif);
				}

				taskENTER_CRITICAL();
				{
					mCurrentSendPacket = NULL;
					mSendQueueHead = 0;
					mSendQueueTail = 0;
					mSendQueueLen = 0;
				}
				taskEXIT_CRITICAL();

				notif.notif = NOTIF_SENDING_STOPPED;
				sendNotification(&notif);
				break;
			}
		}
	}
}

#endif
