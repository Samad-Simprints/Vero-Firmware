/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __ATT_PACKET_H
#define __ATT_PACKET_H

#include "cdbt/utils/packet.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _bt_att_session_s;
typedef struct _bt_att_packet_t
{
	bt_packet_t header;
	bt_byte pdu_opcode;
	struct _bt_att_session_s* session;

	union
	{
		struct  
		{
			bt_byte request_opcode;
			bt_byte error_code;
			bt_uint attr_handle;
		} error;

		bt_uint mtu;

		struct  
		{
			bt_byte count;
			bt_byte cur_attr;
			bt_byte entry_len;
		} find;

		struct  
		{
			bt_byte count;
			bt_byte cur_attr;
			bt_byte entry_len;
			bt_uint value_pos;
			bt_uint bytecount;
		} read;

		struct
		{
			bt_uint attr_handle;
			bt_uint value_offset;
			bt_uint value_len;
			bt_uint queue_offset;
			bt_uint bytecount;
			bt_uint value_pos;

		} prepare_write;

		struct
		{
			bt_uint handle;
			const bt_byte* value;
			bt_uint len;
			bt_uint value_pos;
			bt_uint bytecount;
		} indicate_notify;

	} data;

} bt_att_packet_t;

typedef bt_int (*bt_att_packet_assembler_fp)(bt_att_packet_t* packet, bt_byte* buffer, bt_int buffer_len);

bt_int _bt_att_packet_assembler(bt_packet_t* packet, bt_byte* buffer, bt_int buffer_len);

bt_int _bt_att_assemble_info_response(bt_att_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);

bt_int _bt_att_assemble_find_by_value_type_response(bt_att_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);

bt_int _bt_att_assemble_read_by_type_response(bt_att_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);

bt_int _bt_att_assemble_read_by_type_or_group_type_response(bt_att_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);

bt_int _bt_att_assemble_read_response(bt_att_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);

bt_int _bt_att_assemble_read_by_group_type_response(bt_att_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);

bt_int _bt_att_assemble_read_multi_response(bt_att_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);

bt_int _bt_att_assemble_blob_response(bt_att_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);

bt_int _bt_att_assemble_value_indication_notification(bt_att_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);

bt_int _bt_att_assemble_prepare_write_response(bt_att_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);

bt_int _bt_att_assemble_error(bt_att_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);

bt_int _bt_att_assemble_exchange_mtu(bt_att_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);

bt_int _bt_att_assemble_write_response(bt_att_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);

bt_int _bt_att_assemble_execute_write_response(bt_att_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);

#ifdef __cplusplus
}
#endif

#endif // __ATT_PACKET_H
