/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __BTPCMPORT_H_INCLUDED__
#define __BTPCMPORT_H_INCLUDED__

#define BTSCOPORT_EVENT_INITIALIZED            1
#define BTSCOPORT_EVENT_SENDING_STARTED        2
#define BTSCOPORT_EVENT_SENDING_STOPPED        3
#define BTSCOPORT_EVENT_PACKET_SENT            4
#define BTSCOPORT_EVENT_SENDING_PAUSED         5
#define BTSCOPORT_EVENT_PACKET_RECEIVED        6
#define BTSCOPORT_EVENT_CONNECTED              7
#define BTSCOPORT_EVENT_DISCONNECTED           8

typedef void (*btpcmport_Callback)(bt_int evt, void* param);

void btpcmport_init(btpcmport_Callback callback);
bt_bool btpcmport_configure(btaudio_CodecConfig* config);
void btpcmport_start(void);
void btpcmport_stop(void);
void btpcmport_receivePacket(btaudio_MediaPacket* packet);
void btpcmport_sendPacket(btaudio_MediaPacket* packet);
void btpcmport_connect(bt_uint hconn, bt_id cid);
void btpcmport_disconnect(void);

void btpcmport_pal_init(btpcmport_Callback callback);
void btpcmport_pal_configure(bt_ulong samplingRate, bt_byte codecType, void* codecConfig, bt_byte btAudioInterface, bt_byte audioProcessor);
void btpcmport_pal_start(void);
void btpcmport_pal_stop(void);
void btpcmport_pal_receivePacket(btaudio_MediaPacket* packet);
void btpcmport_pal_sendPacket(btaudio_MediaPacket* packet);
void btpcmport_pal_connect(bt_uint hconn, bt_id cid);
void btpcmport_pal_disconnect(void);

#endif // __BTPCMPORT_H_INCLUDED__
