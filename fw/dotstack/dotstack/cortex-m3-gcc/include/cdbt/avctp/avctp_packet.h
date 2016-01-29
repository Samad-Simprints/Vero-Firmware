/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __AVCTP_PACKET_H
#define __AVCTP_PACKET_H

#include "cdbt/utils/packet.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _bt_avctp_packet_t
{
	bt_packet_t header;
	bt_byte tran_id;
	bt_byte packet_type;
	bt_byte num_packets;
	bt_byte message_type;
	bt_byte ipid;
	bt_uint profile_id;
	bt_byte* data;
	bt_uint data_len;
	bt_int data_pos;

} bt_avctp_packet_t;

bt_int _bt_avctp_packet_assembler(bt_packet_t* packet, bt_byte* buffer, bt_int buffer_len);

#ifdef __cplusplus
}
#endif

#endif // __AVCTP_PACKET_H
