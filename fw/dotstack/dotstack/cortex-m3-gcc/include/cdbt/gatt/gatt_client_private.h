/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __GATT_CLIENT_PRIVATE_H
#define __GATT_CLIENT_PRIVATE_H

#define GATT_CMD_DISCOVER_ALL_SERVICES                       1
#define GATT_CMD_DISCOVER_PRIMARY_SERVICE_BY_SERVICE_UUID    2
#define GATT_CMD_FIND_INCLUDED_SERVICES                      3
#define GATT_CMD_DISCOVER_ALL_CHARACTERISTICS                4
#define GATT_CMD_DISCOVER_CHARACTERISTICS_BY_UUID            5
#define GATT_CMD_DISCOVER_ALL_CHARACTERISTIC_DESCRIPTORS     6
#define GATT_CMD_READ_CHARACTERISTIC_VALUE                   7
#define GATT_CMD_READ_USING_CHARACTERISTIC_UUID              8
#define GATT_CMD_READ_LONG_CHARACTERISTIC_VALUE              9
#define GATT_CMD_READ_MULTIPLE_CHARACTERISTIC_VALUES         10
#define GATT_CMD_WRITE_WITHOUT_RESPONSE                      11
#define GATT_CMD_SIGNED_WRITE_WITHOUT_RESPONSE               12
#define GATT_CMD_WRITE_CHARACTERISTIC_VALUE                  13
#define GATT_CMD_WRITE_LONG_CHARACTERISTIC_VALUE             14
#define GATT_CMD_RELIABLE_WRITES                             15
#define GATT_CMD_READ_CHARACTERISTIC_DESCRIPTOR              16
#define GATT_CMD_READ_LONG_CHARACTERISTIC_DESCRIPTOR         17
#define GATT_CMD_WRITE_CHARACTERISTIC_DESCRIPTOR             18
#define GATT_CMD_WRITE_LONG_CHARACTERISTIC_DESCRIPTOR        19
#define GATT_CMD_EXCHANGE_MTU                                20

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*bt_gatt_client_response_handler_fp)(bt_gatt_client_session_t* session, bt_att_client_evt_t* result);

extern bt_gatt_client_session_t     _gatt_client_sessions[];

extern const bt_gatt_client_response_handler_fp _bt_gatt_client_rbgt_response_handler_fp;
extern const bt_gatt_client_response_handler_fp _bt_gatt_client_fbtv_response_handler_fp;
extern const bt_gatt_client_response_handler_fp _bt_gatt_client_fi_rbt_response_handler_fp;
extern const bt_gatt_client_response_handler_fp _bt_gatt_client_dc_rbt_response_handler_fp;
extern const bt_gatt_client_response_handler_fp _bt_gatt_client_rcv_r_response_handler_fp;
extern const bt_gatt_client_response_handler_fp _bt_gatt_client_rcd_r_response_handler_fp;
extern const bt_gatt_client_response_handler_fp _bt_gatt_client_rcv_rb_response_handler_fp;
extern const bt_gatt_client_response_handler_fp _bt_gatt_client_rcd_rb_response_handler_fp;
extern const bt_gatt_client_response_handler_fp _bt_gatt_client_w_response_handler_fp;
extern const bt_gatt_client_response_handler_fp _bt_gatt_client_wcv_pw_response_handler_fp;
extern const bt_gatt_client_response_handler_fp _bt_gatt_client_wcd_pw_response_handler_fp;
extern const bt_gatt_client_response_handler_fp _bt_gatt_client_wcv_ew_response_handler_fp;
extern const bt_gatt_client_response_handler_fp _bt_gatt_client_wcd_ew_response_handler_fp;
extern const bt_gatt_client_response_handler_fp _bt_gatt_client_fi_response_handler_fp;
extern const bt_gatt_client_response_handler_fp _bt_gatt_client_rucu_rbt_response_handler_fp;
extern const bt_gatt_client_response_handler_fp _bt_gatt_client_rm_response_handler_fp;

extern bt_byte       _gatt_client_ancs_cmd_buffer[];
extern const bt_uint _gatt_client_ancs_cmd_buffer_len;

void _bt_gatt_client_init_mgr(void);

void _bt_gatt_client_init_profile_finder(void);

void _bt_gatt_client_profile_finder_process_queues(bt_gatt_client_session_t* session);

bt_gatt_client_profile_header_t* _bt_gatt_client_get_free_profile_instance(bt_gatt_client_session_t* session, bt_gatt_client_profile_finder_t* finder);

bt_gatt_client_profile_header_t* _bt_gatt_client_get_profile_instance(bt_gatt_client_session_t* session, bt_gatt_client_profile_finder_t* finder);

bt_bool _bt_gatt_client_add_profile_instance(bt_gatt_client_session_t* session, bt_gatt_client_profile_header_t* instance);
void _bt_gatt_client_free_profile_instance(bt_gatt_client_session_t* session, bt_gatt_client_profile_header_t* instance);

void _bt_gatt_client_init_handlers(void);

void _bt_gatt_client_notify_listeners(bt_gatt_client_session_t* session, bt_int evt, void* evt_param);

void _bt_gatt_client_uuid_to_uuid128(const bt_att_client_uuid_t* client_uuid, bt_uuid_t* uuid);

void _bt_gatt_client_default_response_handler(
	bt_gatt_client_session_t* session,
	bt_att_client_evt_t* result);

void _bt_gatt_client_read_by_group_type_response_handler(
	bt_gatt_client_session_t* session,
	bt_att_client_evt_t* result);

void _bt_gatt_client_find_by_type_value_response_handler(
	bt_gatt_client_session_t* session,
	bt_att_client_evt_t* result);

void _bt_gatt_client_read_by_type_response_handler(
	bt_gatt_client_session_t* session,
	bt_att_client_evt_t* result);

void _bt_gatt_client_find_included_read_by_type_response_handler(
	bt_gatt_client_session_t* session,
	bt_att_client_evt_t* result);

void _bt_gatt_client_discover_chars_read_by_type_response_handler(
	bt_gatt_client_session_t* session,
	bt_att_client_evt_t* result);

void _bt_gatt_client_read_response_handler(
	bt_gatt_client_session_t* session,
	bt_att_client_evt_t* result);

void _bt_gatt_client_read_char_value_read_response_handler(
	bt_gatt_client_session_t* session,
	bt_att_client_evt_t* result);

void _bt_gatt_client_read_by_char_uuid_read_by_type_response_handler(
	bt_gatt_client_session_t* session,
	bt_att_client_evt_t* result);

void _bt_gatt_client_read_char_descriptor_read_response_handler(
	bt_gatt_client_session_t* session,
	bt_att_client_evt_t* result);

void _bt_gatt_client_read_multiple_response_handler(
	bt_gatt_client_session_t* session,
	bt_att_client_evt_t* result);

void _bt_gatt_client_read_blob_response_handler(
	bt_gatt_client_session_t* session,
	bt_att_client_evt_t* result);

void _bt_gatt_client_read_char_value_read_blob_response_handler(
	bt_gatt_client_session_t* session,
	bt_att_client_evt_t* result);

void _bt_gatt_client_read_char_descriptor_read_blob_response_handler(
	bt_gatt_client_session_t* session,
	bt_att_client_evt_t* result);

void _bt_gatt_client_write_response_handler(
	bt_gatt_client_session_t* session,
	bt_att_client_evt_t* result);

void _bt_gatt_client_prepare_write_response_handler(
	bt_gatt_client_session_t* session,
	bt_att_client_evt_t* result);

void _bt_gatt_client_write_char_value_prepare_write_response_handler(
	bt_gatt_client_session_t* session,
	bt_att_client_evt_t* result);

void _bt_gatt_client_write_char_descriptor_prepare_write_response_handler(
	bt_gatt_client_session_t* session,
	bt_att_client_evt_t* result);

void _bt_gatt_client_execute_write_response_handler(
	bt_gatt_client_session_t* session,
	bt_att_client_evt_t* result);

void _bt_gatt_client_write_char_value_execute_write_response_handler(
	bt_gatt_client_session_t* session,
	bt_att_client_evt_t* result);

void _bt_gatt_client_write_char_descriptor_execute_write_response_handler(
	bt_gatt_client_session_t* session,
	bt_att_client_evt_t* result);

void _bt_gatt_client_find_info_response_handler(
	bt_gatt_client_session_t* session,
	bt_att_client_evt_t* result);

#ifdef __cplusplus
}
#endif

#endif // __GATT_CLIENT_PRIVATE_H
