/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __ATT_CLIENT_PACKET_H
#define __ATT_CLIENT_PACKET_H

#include "cdbt/utils/packet.h"

#ifdef __cplusplus
extern "C" {
#endif

	struct _bt_att_client_session_s;
	typedef struct _bt_att_client_packet_s
	{
		bt_packet_t header;
		bt_byte pdu_opcode;
		struct _bt_att_client_session_s* session;

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
				bt_uint start_handle;
				bt_uint end_handle;
				bt_att_find_info_response_t* result;
				bt_uint max_results;
			} find;

			struct  
			{
				bt_uint start_handle;
				bt_uint end_handle;
				bt_uint type;
				const bt_byte* value;
				bt_uint len;
				bt_uint pos;
				bt_att_find_by_type_value_response_t* result;
				bt_uint max_results;
			} find_by_type_value;

			struct  
			{
				bt_uint handle;
			} read;

			struct  
			{
				bt_uint start_handle;
				bt_uint end_handle;
				union
				{
					bt_uint uuid16;
					bt_uuid_t* uuid128;
				} type;
				bt_uint type_len;
				bt_att_read_by_type_response_t* result;
				bt_uint max_results;
			} read_by_type;

			struct  
			{
				bt_uint handle;
				bt_uint offset;
			} read_blob;

			struct  
			{
				const bt_uint* handles;
				bt_uint count;
				bt_uint index;
			} read_multiple;

			struct  
			{
				bt_uint max_results;
				bt_att_read_by_group_type_response_t* result;
				bt_uint start_handle;
				bt_uint end_handle;
				union
				{
					bt_uint uuid16;
					bt_uuid_t* uuid128;
				} group_type;
				bt_byte group_type_len;
			} read_by_group_type;

			struct 
			{
				bt_uint handle;
				const bt_byte* value;
				bt_uint len;
				bt_uint pos;
				const bt_byte* signature;
				bt_uint value_offset;
			} write;

			struct
			{
				bt_byte flags;
			} execute_write;

			struct
			{
				bt_uint handle;
				const bt_byte* value;
				bt_uint len;
				bt_uint value_pos;
				bt_uint bytecount;
			} indicate_notify;

		} data;

	} bt_att_client_packet_t;

	typedef bt_int (*bt_att_client_packet_assembler_fp)(bt_att_client_packet_t* packet, bt_byte* buffer, bt_int buffer_len);

	bt_int _bt_att_client_packet_assembler(bt_packet_t* packet, bt_byte* buffer, bt_int buffer_len);
	bt_int _bt_att_assemble_exchange_mtu_request(bt_att_client_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);
	bt_int _bt_att_assemble_info_request(bt_att_client_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);
	bt_int _bt_att_assemble_find_by_value_type_request(bt_att_client_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);
	bt_int _bt_att_assemble_read_by_type_request(bt_att_client_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);
	bt_int _bt_att_assemble_read_by_group_type_request(bt_att_client_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);
	bt_int _bt_att_assemble_read_request(bt_att_client_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);
	bt_int _bt_att_assemble_read_multi_request(bt_att_client_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);
	bt_int _bt_att_assemble_blob_request(bt_att_client_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);
	bt_int _bt_att_assemble_prepare_write_request(bt_att_client_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);
	bt_int _bt_att_assemble_exchange_mtu_request(bt_att_client_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);
	bt_int _bt_att_assemble_write_request(bt_att_client_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);
	bt_int _bt_att_assemble_write_command(bt_att_client_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);
	bt_int _bt_att_assemble_signed_write_command(bt_att_client_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);
	bt_int _bt_att_assemble_execute_write_request(bt_att_client_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);
	bt_int _bt_att_assemble_handle_value_confirmation(bt_att_client_packet_t* att_packet, bt_byte* buffer, bt_int buffer_len);

#ifdef __cplusplus
}
#endif

#endif // __ATT_CLIENT_PACKET_H
