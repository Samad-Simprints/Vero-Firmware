/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __ATT_PRIVATE_H
#define __ATT_PRIVATE_H

#define ATT_DEFAULT_MTU    23

#define ATT_TRANSACTION_TIMEOUT    30 // seconds

#define ATT_OPCODE_FLAG_SIG        0x80
#define ATT_OPCODE_FLAG_COMMAND    0x40
#define ATT_OPCODE_FLAG_METHOD     0x3F

// Attribute flags

#define ATT_ACCESS_READ                0x0001
#define ATT_ACCESS_WRITE               0x0002

#define ATT_AUTHORIZATION_READ         0x0004
#define ATT_AUTHORIZATION_WRITE        0x0200
#define ATT_AUTHORIZATION_READWRITE    0x0204

#define ATT_AUTHENTICATION_READ        0x0008
#define ATT_AUTHENTICATION_WRITE       0x0400
#define ATT_AUTHENTICATION_READWRITE   0x0408

#define ATT_GROUP_START                0x0010
#define ATT_GROUP_END                  0x0020

#define ATT_DB_TYPE_VAR                0
#define ATT_DB_TYPE_CONST              0x0040

#define ATT_TYPE_FORMAT_UUID16         0
#define ATT_TYPE_FORMAT_UUID128        0x0080

#define ATT_VALUE_LENGTH_FIXED         0
#define ATT_VALUE_LENGTH_VARIABLE      0x0100

// End attribute flags

#define ATT_OPCODE_ERROR_RESPONSE               0x1
#define ATT_OPCODE_EXCHANGE_MTU_REQUEST         0x2
#define ATT_OPCODE_EXCHANGE_MTU_RESPONSE        0x3
#define ATT_OPCODE_FIND_INFORMATION_REQUEST     0x4
#define ATT_OPCODE_FIND_INFORMATION_RESPONSE    0x5
#define ATT_OPCODE_FIND_BY_TYPE_VALUE_REQUEST   0x6
#define ATT_OPCODE_FIND_BY_TYPE_VALUE_RESPONSE  0x7
#define ATT_OPCODE_READ_BY_TYPE_REQUEST         0x8
#define ATT_OPCODE_READ_BY_TYPE_RESPONSE        0x9
#define ATT_OPCODE_READ_REQUEST                 0xA
#define ATT_OPCODE_READ_RESPONSE                0xB
#define ATT_OPCODE_READ_BLOB_REQUEST            0xC
#define ATT_OPCODE_READ_BLOB_RESPONSE           0xD
#define ATT_OPCODE_READ_MULTIPLE_REQUEST        0xE
#define ATT_OPCODE_READ_MULTIPLE_RESPONSE       0xF
#define ATT_OPCODE_READ_BY_GROUP_TYPE_REQUEST   0x10
#define ATT_OPCODE_READ_BY_GROUP_TYPE_RESPONSE  0x11
#define ATT_OPCODE_WRITE_REQUEST                0x12
#define ATT_OPCODE_WRITE_RESPONSE               0x13
#define ATT_OPCODE_WRITE_COMMAND                0x52
#define ATT_OPCODE_SIGNED_WRITE_COMMAND         0xD2
#define ATT_OPCODE_PREPARE_WRITE_REQUEST        0x16
#define ATT_OPCODE_PREPARE_WRITE_RESPONSE       0x17
#define ATT_OPCODE_EXECUTE_WRITE_REQUEST        0x18
#define ATT_OPCODE_EXECUTE_WRITE_RESPONSE       0x19
#define ATT_OPCODE_HANDLE_VALUE_NOTIFICATION    0x1B
#define ATT_OPCODE_HANDLE_VALUE_INDICATION      0x1D
#define ATT_OPCODE_HANDLE_VALUE_CONFIRMATION    0x1E
#define ATT_OPCODE_LAST                         ATT_OPCODE_HANDLE_VALUE_CONFIRMATION

#ifdef __cplusplus
extern "C" {
#endif

extern bt_att_session_t            _att_sessions[];
extern const bt_byte               _att_max_sessions;
extern const bt_byte               _att_max_found_attributes;
extern bt_att_found_attribyte_t    _att_found_attributes[];
extern const bt_uint               _att_max_max_queued_write_buffer_size;
extern bt_byte*                    _att_queued_write_buffers;

extern bt_byte*                    _att_db_authorization_write_buffer;
extern bt_uint                     _att_db_authorization_write_value_len;
extern const bt_uint               _att_db_authorization_write_buffer_len;

extern const bt_att_packet_assembler_fp _att_packet_assemblers[];

extern bt_l2cap_read_data_callback_fp     _att_l2cap_read_data_cb;
extern bt_l2cap_state_changed_callback_fp _att_l2cap_state_changed_cb;
extern void (*_att_set_signal_fp)(void);
extern void (*_att_client_set_signal_fp)(void);

#ifdef _DEBUG
extern const bt_uint _ram_size_att_buffers;
#endif


void _bt_att_init_mgr(void);
void _bt_att_mgr_notify_listeners(bt_int evt, void* evt_param);

void _bt_att_init_timer(void);
bt_long _bt_att_get_tick_count(void);

void _bt_att_init_signal(void);

bt_att_session_t* _bt_att_allocate_session(void);
bt_att_session_t* _bt_att_find_session(bt_bdaddr_t* bdaddr);
bt_att_session_t* _bt_att_find_session_by_channel(bt_l2cap_channel_t* channel);
void _bt_att_free_session(bt_att_session_t* session);

bt_bool _bt_att_mgr_listen(bt_byte* att_var_db, bt_uint att_var_db_len,
						   const bt_byte* att_const_db, bt_uint att_const_db_len,
						   bt_bool listen_on_dynamic_channel);

void _bt_att_server_command_handler(bt_att_session_t* session, const bt_byte* data, bt_int len);
void _bt_att_server_request_handler(bt_att_session_t* session, const bt_byte* data, bt_int len);
void _bt_att_server_response_handler(bt_att_session_t* session, const bt_byte* data, bt_int len);

bt_bool _bt_att_server_exchange_mtu_handler(bt_att_session_t* session, const bt_byte* data, bt_int len);
bt_bool _bt_att_server_find_info_handler(bt_att_session_t* session, const bt_byte* data, bt_int len);
bt_bool _bt_att_server_find_by_type_value_handler(bt_att_session_t* session, const bt_byte* data, bt_int len);
bt_bool _bt_att_server_read_by_type_handler(bt_att_session_t* session, const bt_byte* data, bt_int len);
bt_bool _bt_att_server_read_handler(bt_att_session_t* session, const bt_byte* data, bt_int len);
bt_bool _bt_att_server_read_blob_handler(bt_att_session_t* session, const bt_byte* data, bt_int len);
bt_bool _bt_att_server_read_multiple_handler(bt_att_session_t* session, const bt_byte* data, bt_int len);
bt_bool _bt_att_server_read_by_group_type_handler(bt_att_session_t* session, const bt_byte* data, bt_int len);
bt_bool _bt_att_server_write_handler(bt_att_session_t* session, const bt_byte* data, bt_int len);
bt_bool _bt_att_server_prepare_write_handler(bt_att_session_t* session, const bt_byte* data, bt_int len);
bt_bool _bt_att_server_execute_write_request_handler(bt_att_session_t* session, const bt_byte* data, bt_int len);
void _bt_att_server_write_command_handler(bt_att_session_t* session, const bt_byte* data, bt_int len);
void _bt_att_server_signed_write_command_handler(bt_att_session_t* session, const bt_byte* data, bt_int len);

bt_att_session_t* _bt_att_server_get_sessions(void);

void _bt_att_server_notify_invalid_handle(bt_att_session_t* session, bt_uint handle);
void _bt_att_server_indicate_invalid_handle(bt_att_session_t* session, bt_uint handle);

bt_bool _bt_att_send_error_response(bt_att_session_t* session, bt_byte opcode, bt_uint attr_handle, bt_byte error_code);
bt_bool _bt_att_send_exchange_mtu_response(bt_att_session_t* session, bt_uint mtu);
bt_bool _bt_att_send_find_response(bt_att_session_t* session, bt_byte opcode, bt_byte count);
bt_bool _bt_att_send_read_response(bt_att_session_t* session, bt_byte opcode, bt_byte count);
bt_bool _bt_att_send_write_response(bt_att_session_t* session, bt_byte opcode);
void    _bt_att_send_value_indication(bt_att_session_t* session);
void    _bt_att_send_value_notification(bt_att_session_t* session);
bt_bool _bt_att_send_prepare_write_response(
	bt_att_session_t* session, bt_uint handle, 
	bt_uint value_offset, bt_uint value_len, bt_int queue_offset);

bt_bool _bt_att_get_first_attribute(bt_att_attribute_t* attr, bt_att_enum_context_t* enum_context);
bt_bool _bt_att_get_next_attribute(bt_att_attribute_t* attr, bt_att_enum_context_t* enum_context);
void _bt_att_get_attribute(bt_att_attribute_t* attr, bt_byte db_type, bt_int* offset);
void _bt_att_read_attribute(bt_att_attribute_t* attr, const bt_byte* db, bt_uint db_len, bt_int* offset);
bt_uint _bt_att_find_group_end_handle(bt_uint group_start_handle, bt_att_session_t* session);
bt_byte _bt_att_write_attribute_value(
	bt_att_attribute_t* attr, bt_uint offset, 
	const bt_byte* value, bt_uint value_len, bt_int notify);

void _bt_att_get_attr_type(bt_att_attribute_t* attr, bt_uuid_t* attr_type);
void _bt_att_uuid16_to_uuid128(bt_uuid16 uuid16, bt_uuid_p puuid);
bt_bool _bt_att_type_equals_uuid16(bt_att_attribute_t* attr, bt_uuid16 uuid);
bt_bool _bt_att_type_equals_uuid128(bt_att_attribute_t* attr, bt_uuid_t* uuid);
bt_bool _bt_att_compare_value(bt_att_attribute_t* attr, const bt_byte* value, bt_uint len);

bt_bool _bt_att_get_attr_value_uuid(bt_att_attribute_t* attr, bt_uuid_t* attr_value);

bt_byte _bt_att_find_attributes(
	bt_att_session_t* session, 
	bt_uint start_handle, bt_uint end_handle);

bt_byte _bt_att_find_attributes_by_type_value(
	bt_att_session_t* session, 
	bt_uint start_handle, bt_uint end_handle, 
	bt_uuid16 type, const bt_byte* value, bt_uint len);

bt_byte _bt_att_find_attributes_by_type(
	bt_att_session_t* session, 
	bt_uint start_handle, bt_uint end_handle, 
	bt_uuid_t* type);

bt_bool _bt_att_find_attribute(bt_uint handle, bt_att_attribute_t* attr, bt_att_enum_context_t* enum_context);

bt_bool _bt_att_find_and_save_attribute(
	bt_uint handle, bt_att_attribute_t* attr, bt_att_session_t* session,
	bt_byte save_index);

bt_bool _bt_att_is_read_authorization_required(bt_att_session_t* session, bt_att_attribute_t* attr, bt_byte opcode);
void _bt_att_server_authorize_attr(bt_att_session_t* session);

void _bt_att_set_signal(void);
void _bt_att_send_pending_packets(bt_att_session_t* session);

#ifdef __cplusplus
}
#endif

#endif // __ATT_PRIVATE_H
