/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __PCMPORT_H_INCLUDED__
#define __PCMPORT_H_INCLUDED__

#include <stdint.h>

#ifdef BTAPP_PCMPORT_RECEIVE_BUFFER_SIZE
    #define PCMPORT_RECEIVE_BUFFER_SIZE  BTAPP_PCMPORT_RECEIVE_BUFFER_SIZE
#else
    #define PCMPORT_RECEIVE_BUFFER_SIZE  (120)
#endif

#ifdef BTAPP_PCMPORT_SEND_BUFFER_SIZE
    #define PCMPORT_SEND_BUFFER_SIZE  BTAPP_PCMPORT_SEND_BUFFER_SIZE
#else
    #define PCMPORT_SEND_BUFFER_SIZE  (1152 * 1)//1440//120
#endif

#define MSG_START_SENDING         1
#define MSG_STOP_SENDING          2
#define MSG_SEND_PACKET           3
#define MSG_FILL_SEND_BUFFER      4

#define MSG_START_RECEIVING       5
#define MSG_STOP_RECEIVING        6
#define MSG_BUFFER_RECEIVED       7
#define MSG_FILL_RECEIVE_PACKET   8
#define MSG_RECEIVE_PACKET        9

#define MSG_NOTIFY_CONNECTED      10
#define MSG_NOTIFY_DISCONNECTED   11
#define MSG_NOTIFY_INITIALIZED    12

typedef struct _pcmport_SendBuffer
{
    uint32_t dataLen;
    int16_t  data[PCMPORT_SEND_BUFFER_SIZE];
	
} pcmport_SendBuffer;

typedef struct _pcmport_ReceiveBuffer
{
    uint32_t dataLen;
    uint32_t maxDataLen;
    int16_t  data[PCMPORT_RECEIVE_BUFFER_SIZE];

} pcmport_ReceiveBuffer;

typedef struct
{
    uint8_t msg;
    union
    {
        struct
        {
            btaudio_MediaPacket* packet;
        } sendPacket;

        struct
        {
            btaudio_MediaPacket* packet;
        } receivePacket;

        struct
        {
            pcmport_ReceiveBuffer* buffer;
        } bufferReceived;

    } param;
} ScoPortTaskMessage;

void pcmport_pal_init(void);
void pcmport_pal_configure(bt_ulong samplingRate, bt_byte codecType, void* codecConfig);
void pcmport_pal_connect(bt_uint hconn, bt_id cid);
void pcmport_pal_disconnect(void);
void pcmport_pal_startSending(void);
void pcmport_pal_stopSending(void);
void pcmport_pal_startReceiving(void);
void pcmport_pal_stopReceiving(void);
void btpcmport_receivePacket(btaudio_MediaPacket* packet);
void btpcmport_sendPacket(btaudio_MediaPacket* packet);

pcmport_SendBuffer* pcmport_getNextSendBuffer(void);
pcmport_SendBuffer* pcmport_getNextSendBufferFromISR(portBASE_TYPE *higherPriorityTaskWoken);

pcmport_ReceiveBuffer* pcmport_getReceiveBufferFromISR(portBASE_TYPE *higherPriorityTaskWoken);

void pcmport_sendPcmPortMessage(ScoPortTaskMessage* msg);
void pcmport_sendPcmPortMessageFromISR(ScoPortTaskMessage* msg, signed portBASE_TYPE* higherPriorityTaskWoken);

#endif // __PCMPORT_H_INCLUDED__
