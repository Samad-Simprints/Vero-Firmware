/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*
* SEARAN LLC is the exclusive licensee and developer of dotstack with
* all its modifications and enhancements.
*
* Contains proprietary and confidential information of CandleDragon and
* may not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2009, 2010, 2011 CandleDragon. All Rights Reserved.
*******************************************************************************/

#ifndef __MAP_H
#define __MAP_H

#include "cdbt/obex/obex.h"
#include "cdbt/sdp/sdp.h"

#ifdef __cplusplus
extern "C" {
#endif


// The handle shall be represented by a null - terminated Unicode text string with 16 hexadecimal digits.
#define MAP_MSG_HANDLE_SIZE         16

#define MAP_MSG_STATUS_TYPE_READ    0
#define MAP_MSG_STATUS_TYPE_DELETE  1

typedef enum _bt_map_session_state_enum
{
	MAP_SESSION_STATE_FREE = 0,
	MAP_SESSION_STATE_USED,
} bt_map_session_state_enum;


struct _bt_map_session_s;

typedef enum _bt_map_session_event_enum
{
    MAP_SESSION_NOTHING,
    MAP_SESSION_CONNECT_FAILED,
    MAP_SESSION_OBEX_CONNECTION_STATE_CHANGED,
    MAP_SESSION_GET_FOLDER_FRAGMENT_RECEIVED,
    MAP_SESSION_GET_MSG_LISTING_FRAGMENT_RECEIVED,
    MAP_SESSION_GET_FOLDER_COMPLETED,
    MAP_SESSION_GET_MSG_LISTING_COMPLETED,
    MAP_SESSION_GET_MSG_FRAGMENT_RECEIVED,
    MAP_SESSION_GET_MSG_COMPLETED,
    MAP_SESSION_SETPATH_COMPLETED,
    MAP_SESSION_GET_MSG_ABORTED,
    MAP_SESSION_PUT_MSG_ABORTED,
    MAP_SESSION_GET_MSG_FRAGMENT,
    MAP_SESSION_PUT_MSG_COMPLETED,
    MAP_SESSION_PUT_FOLDER_COMPLETED,
    MAP_SESSION_PUT_MSG_LISTING_COMPLETED,
    MAP_SESSION_UPDATE_INBOX_COMPLETED,
    MAP_SESSION_REGISTER_COMPLETED,
    MAP_SESSION_DELETE_OBJECT_COMPLETED,
#if 0
    MAP_SESSION_SERVER_GET_FOLDER,
    MAP_SESSION_SERVER_GET_MSG_LISTING,
    MAP_SESSION_SERVER_GET_MSG,
    MAP_SESSION_SERVER_GET_FOLDER_FRAGMENT,
    MAP_SESSION_SERVER_GET_MSG_LISTING_FRAGMENT,
    MAP_SESSION_SERVER_GET_MSG_FRAGMENT,
    MAP_SESSION_SERVER_GET_FOLDER_COMPLETED,
    MAP_SESSION_SERVER_GET_MSG_LISTING_COMPLETED,
    MAP_SESSION_SERVER_GET_MSG_COMPLETED,
#endif
    MAP_SESSION_SERVER_PUT_SEND_EVENT,

    MAP_SESSION_SERVER_DELETE_MSG,
    MAP_SESSION_SERVER_DELETE_FOLDER = MAP_SESSION_SERVER_DELETE_MSG,
    MAP_SESSION_SERVER_DELETE_MSG_LISTING = MAP_SESSION_SERVER_DELETE_MSG,

    MAP_SESSION_SERVER_SETPATH,

    MAP_SESSION_SERVER_GET_FOLDER_ABORTED,
    MAP_SESSION_SERVER_GET_MSG_LISTING_ABORTED,
    MAP_SESSION_SERVER_GET_MSG_ABORTED,
    MAP_SESSION_SERVER_PUT_MSG_ABORTED,
} bt_map_session_event_enum;

#define MAP_SESSION_OBJECT_TYPE					0x1F
#define MAP_SESSION_OBJECT_TYPE_REGISTER	    0x01
#define MAP_SESSION_OBJECT_TYPE_UPDATE_INBOX    0x02
#define MAP_SESSION_OBJECT_TYPE_FOLDER			0x03
#define MAP_SESSION_OBJECT_TYPE_MSG_LISTING		0x04
#define MAP_SESSION_OBJECT_TYPE_MSG			    0x05
#define MAP_SESSION_OBJECT_TYPE_DELETE_OBJECT	0x06
#define MAP_SESSION_OBJECT_TYPE_SET_MSG_STATUS  0x07

#define MAP_SESSION_OPERATION					0xE0
#define MAP_SESSION_OPERATION_GET				0x20
#define MAP_SESSION_OPERATION_PUT				0x40
#define MAP_SESSION_OPERATION_SET_PATH			0x80

typedef void (*bt_map_find_server_callback_pf)(struct _bt_map_session_s* session, bt_byte server_channel, bt_bool found);

typedef void (*bt_map_session_callback_pf)(struct _bt_map_session_s* session, bt_map_session_event_enum event, void* what_param, void* what_param2, void* callback_param);

typedef struct _bt_map_session_s
{
	bt_map_session_state_enum state;
	bt_obex_session* obex_session;
	bt_ulong connection_id;
    bt_byte instance;
	bt_byte flags;
	const bt_byte* folder_name;
	bt_uint folder_name_len;
	bt_uint sub_folder_name_start;
	bt_uint sub_folder_name_len;

	bt_map_session_callback_pf session_callback;
	void* session_callback_param;

	bt_l2cap_channel_t* find_server_sdp_channel;
	bt_sdp_data_element_t sdp_pattern;
	bt_sr_handle_t find_server_service_record_handle;
	bt_bool find_server_next_attr_is_server_channel;
	bt_map_find_server_callback_pf find_server_callback;

} bt_map_session_t;

typedef struct _bt_map_server_put_object_params
{
	bt_byte response_code;
	bt_byte* object;
	bt_uint object_len;

} bt_map_server_put_object_params;

bt_bool bt_map_init(void);

bt_map_session_t* bt_map_allocate_session(void);

void bt_map_free_session(bt_map_session_t* psess);

bt_bool bt_map_connect(
	bt_map_session_t* session, bt_byte instance, bt_bdaddr_t* pbdaddr_remote,
	bt_map_session_callback_pf callback, void* param);

bt_bool bt_map_mns_listen(bt_map_session_t* session, bt_byte server_channel, bt_map_session_callback_pf callback, void* param);

bt_bool bt_map_disconnect(bt_map_session_t* session);

bt_bool bt_map_read_folder(bt_map_session_t* session, bt_byte* folder_name, bt_uint folder_name_len);

bt_bool bt_map_register_notifications(bt_map_session_t* session, bt_bool on);

bt_bool bt_map_update_inbox(bt_map_session_t* session);

bt_bool bt_map_read_msg_listing(bt_map_session_t* session, bt_byte* folder_name, bt_uint folder_name_len);

bt_bool bt_map_setpath(bt_map_session_t* session, const bt_byte* folder_name, bt_uint folder_name_len);

bt_bool bt_map_read_msg(bt_map_session_t* session, bt_byte* handle, bt_byte transcode_utf8);

bt_bool bt_map_set_msg_status(bt_map_session_t* mMapSession, bt_byte* handle, bt_byte status_read, bt_byte status_deleted);

bt_bool bt_map_push_message(bt_map_session_t* session, bt_byte* folder_name, bt_uint folder_name_len, bt_byte transparent, bt_byte retry, bt_byte charset);

bt_bool bt_map_abort(bt_map_session_t* session);

#include "cdbt/map/map_private.h"

#ifdef __cplusplus
}
#endif

#endif // __MAP_H
