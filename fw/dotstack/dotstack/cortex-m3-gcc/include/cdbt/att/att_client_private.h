/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __ATT_CLIENT_PRIVATE_H
#define __ATT_CLIENT_PRIVATE_H

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

#define ATT_ERROR_SUCCESS                            0x00
#define ATT_ERROR_INVALID_HANDLE                     0x01
#define ATT_ERROR_READ_NOT_PERMITTED                 0x02
#define ATT_ERROR_WRITE_NOT_PERMITTED                0x03
#define ATT_ERROR_INVALID_PDU                        0x04
#define ATT_ERROR_INSUFFICIENT_AUTHENTICATION        0x05
#define ATT_ERROR_REQUEST_NOT_SUPPORTED              0x06
#define ATT_ERROR_INVALID_OFFSET                     0x07
#define ATT_ERROR_INSUFFICIENT_AUTHORIZATION         0x08
#define ATT_ERROR_PREPARE_QUEUE_FULL                 0x09
#define ATT_ERROR_ATTRIBUTE_NOT_FOUND                0x0A
#define ATT_ERROR_ATTRIBUTE_NOT_LONG                 0x0B
#define ATT_ERROR_INSUFFICIENT_ENCRYPTION_KEY_SIZE   0x0C
#define ATT_ERROR_INVALID_ATTRIBUTE_VALUE_LENGTH     0x0D
#define ATT_ERROR_UNLIKELY_ERROR                     0x0E
#define ATT_ERROR_INSUFFICIENT_ENCRYPTION            0x0F
#define ATT_ERROR_UNSUPPORTED_GROUP_TYPE             0x10
#define ATT_ERROR_INSUFFICIENT_RESOURCES             0x11

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

#define ATT_OPCODE_CONN_PARAM_UPDATE_REQUEST    0x70

#ifdef __cplusplus
extern "C" {
#endif

	extern bt_att_client_session_t     _att_client_sessions[];
	extern const bt_byte               _att_max_client_sessions;
	extern bt_att_bridge_t*				_att_bridge;
	extern void (*_bt_gatt_client_register_sm_listener_fp)(bt_att_client_session_t* session, bt_att_client_err_handler_fp error_handler, bt_uint handle, bt_byte errcode);
	extern void (*_bt_gatt_client_unregister_sm_listener_fp)(bt_att_client_session_t* session);
	extern void (*_att_client_set_signal_fp)(void);

#ifdef _DEBUG
	extern const bt_uint _ram_size_att_client_buffers;
#endif


	void _bt_att_client_init_mgr(void);
	void _bt_att_mgr_notify_listeners(bt_int evt, void* evt_param);

	void _bt_att_client_init_timer(void);
	bt_long _bt_att_client_get_tick_count(void);

	bt_bool _bt_att_client_send_request(bt_att_client_session_t* session);

	bt_att_client_session_t* _bt_att_find_client_session_by_channel(bt_l2cap_channel_t* channel);

	void _bt_att_client_exchange_mtu_handler(bt_att_client_session_t* session, const bt_byte* data, bt_uint len);
	void _bt_att_client_find_info_handler(bt_att_client_session_t* session, const bt_byte* data, bt_uint len);
	void _bt_att_client_find_by_type_value_handler(bt_att_client_session_t* session, const bt_byte* data, bt_uint len);
	void _bt_att_client_read_by_type_handler(bt_att_client_session_t* session, const bt_byte* data, bt_uint len);
	void _bt_att_client_read_handler(bt_att_client_session_t* session, const bt_byte* data, bt_uint len);
	void _bt_att_client_read_blob_handler(bt_att_client_session_t* session, const bt_byte* data, bt_uint len);
	void _bt_att_client_read_multiple_handler(bt_att_client_session_t* session, const bt_byte* data, bt_uint len);
	void _bt_att_client_read_by_group_type_handler(bt_att_client_session_t* session, const bt_byte* data, bt_uint len);
	void _bt_att_client_write_handler(bt_att_client_session_t* session, const bt_byte* data, bt_uint len);
	void _bt_att_client_prepare_write_handler(bt_att_client_session_t* session, const bt_byte* data, bt_uint len);
	void _bt_att_client_execute_write_request_handler(bt_att_client_session_t* session, const bt_byte* data, bt_uint len);
	void _bt_att_client_value_notification_handler(bt_att_client_session_t* session, const bt_byte* data, bt_uint len);
	void _bt_att_client_value_indication_handler(bt_att_client_session_t* session, const bt_byte* data, bt_uint len);

	void _bt_att_client_exchange_mtu_err_handler(bt_att_client_session_t* session, bt_uint handle, bt_byte errcode);
	void _bt_att_client_find_info_err_handler(bt_att_client_session_t* session, bt_uint handle, bt_byte errcode);
	void _bt_att_client_find_by_type_value_err_handler(bt_att_client_session_t* session, bt_uint handle, bt_byte errcode);
	void _bt_att_client_read_by_type_err_handler(bt_att_client_session_t* session, bt_uint handle, bt_byte errcode);
	void _bt_att_client_read_err_handler(bt_att_client_session_t* session, bt_uint handle, bt_byte errcode);
	void _bt_att_client_read_blob_err_handler(bt_att_client_session_t* session, bt_uint handle, bt_byte errcode);
	void _bt_att_client_read_multiple_err_handler(bt_att_client_session_t* session, bt_uint handle, bt_byte errcode);
	void _bt_att_client_read_by_group_type_err_handler(bt_att_client_session_t* session, bt_uint handle, bt_byte errcode);
	void _bt_att_client_write_err_handler(bt_att_client_session_t* session, bt_uint handle, bt_byte errcode);
	void _bt_att_client_prepare_write_err_handler(bt_att_client_session_t* session, bt_uint handle, bt_byte errcode);
	void _bt_att_client_execute_write_request_err_handler(bt_att_client_session_t* session, bt_uint handle, bt_byte errcode);

	void _bt_att_client_init_signal(void);
	void _bt_att_client_set_signal(void);

	void _bt_gatt_client_register_sm_listener(
		bt_att_client_session_t* session, 
		bt_att_client_err_handler_fp error_handler, 
		bt_uint handle, bt_byte errcode);
	void _bt_gatt_client_unregister_sm_listener(bt_att_client_session_t* session);

#ifdef __cplusplus
}
#endif

#endif // __ATT_CLIENT_PRIVATE_H
