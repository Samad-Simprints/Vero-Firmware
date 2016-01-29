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

#ifndef __OPP_H
#define __OPP_H

#include "cdbt/obex/obex.h"
#include "cdbt/sdp/sdp.h"

#ifdef __cplusplus
extern "C" {
#endif


#define OPP_MAX_SESSIONS	2

#define OPP_SESSION_OPERATION					0xF0
#define OPP_SESSION_OPERATION_PULL				0x10
#define OPP_SESSION_OPERATION_PUSH				0x20

typedef enum _bt_opp_session_state_enum
{
	OPP_SESSION_STATE_FREE = 0,
	OPP_SESSION_STATE_USED,
} bt_opp_session_state_enum;


struct _bt_opp_session;

typedef enum _bt_opp_session_event_enum
{
	OPP_EVT_SESSION_NOTHING,
	OPP_EVT_SESSION_CONNECT_FAILED,
	OPP_EVT_SESSION_CONNECTED,
	OPP_EVT_SESSION_DISCONNECTED,
	OPP_EVT_SESSION_GET_OBJECT_FRAGMENT,
	OPP_EVT_SESSION_PUSH_OBJECT_COMPLETED,
	OPP_EVT_SESSION_PUSH_OBJECT_ABORTED,
	OPP_EVT_SESSION_PULL_OBJECT_COMPLETED,
	OPP_EVT_SESSION_PULL_OBJECT_ABORTED,
} bt_opp_session_event_enum;

typedef enum _bt_opp_object_format_enum
{
	OPP_OBJECT_FORMAT_VCARD_21 = 1,
	OPP_OBJECT_FORMAT_VCARD_30,
	OPP_OBJECT_FORMAT_VCAL_10,
	OPP_OBJECT_FORMAT_ICAL_20,
	OPP_OBJECT_FORMAT_VNOTE,
	OPP_OBJECT_FORMAT_VMESSAGE,
	OPP_OBJECT_FORMAT_ANY = 0xFF,
} bt_opp_object_format_enum;


typedef struct _bt_opp_evt_connected_t
{
	bt_byte server_channel;
} bt_opp_evt_connected_t;

typedef struct _bt_opp_evt_get_object_fragment_t
{
	bt_byte* buffer;
	bt_uint buffer_len;
	bt_uint offset;
} bt_opp_evt_get_object_fragment_t;

typedef struct _bt_opp_evt_push_object_completed_t
{
	bt_byte obex_response;
} bt_opp_evt_push_object_completed_t;

typedef struct _bt_opp_evt_push_object_aborted_t
{
	bt_byte obex_response;
} bt_opp_evt_push_object_aborted_t;

typedef struct _bt_opp_evt_pull_object_aborted_t
{
	bt_byte obex_response;
} bt_opp_evt_pull_object_aborted_t;

typedef union _bt_opp_event_u
{
	bt_opp_evt_connected_t             connected;
	bt_opp_evt_get_object_fragment_t   get_object_fragment;
	bt_opp_evt_push_object_completed_t push_object_completed;
	bt_opp_evt_push_object_aborted_t   push_object_aborted;
	bt_opp_evt_pull_object_aborted_t   pull_object_aborted;
} bt_opp_event_t;

typedef void (*bt_opp_find_server_callback_pf)(struct _bt_opp_session* session, bt_byte server_channel, bt_bool found);

typedef void (*bt_opp_session_callback_pf)(struct _bt_opp_session* session, bt_opp_session_event_enum event, bt_opp_event_t* evt_param, void* callback_param);

typedef struct _bt_opp_session
{
	bt_opp_session_state_enum state;
	bt_byte flags;
	bt_obex_session* obex_session;
	bt_ulong connection_id;
	bt_uint cur_object_offset;
	bt_byte server_channel;

	bt_byte* push_type;
	bt_byte* push_name;
	bt_uint push_name_len;
	const bt_byte* push_data;
	bt_uint push_data_len;
	bt_uint push_data_index;

	bt_opp_session_callback_pf session_callback;
	void* session_callback_param;

	bt_l2cap_channel_t* find_server_sdp_channel;
	bt_sdp_data_element_t sdp_pattern;
	bt_sr_handle_t find_server_service_record_handle;
	bt_bool find_server_next_attr_is_server_channel;
	bt_opp_find_server_callback_pf find_server_callback;

} bt_opp_session;

bt_bool bt_opp_init(void);

bt_opp_session* bt_opp_allocate_session(bt_opp_session_callback_pf callback, void* param);

void bt_opp_free_session(bt_opp_session* psess);

bt_bool bt_opp_listen(bt_opp_session* session, bt_byte server_channel, bt_opp_session_callback_pf callback, void* param);

bt_bool bt_opp_disconnect(bt_opp_session* session);

//bt_bool bt_opp_pull_object(bt_opp_session* session, bt_byte* file_name, bt_uint file_name_len);

bt_bool bt_opp_connect(bt_opp_session* session, bt_bdaddr_t* pbdaddr_remote, bt_byte server_channel);

bt_bool bt_opp_push_object(
	bt_opp_session* session, bt_bdaddr_t* pbdaddr_remote, bt_byte server_channel,
	bt_byte* type, bt_byte* name, bt_uint name_len,
	const bt_byte* data, bt_uint data_len);

bt_bool bt_opp_abort(bt_opp_session* session);

#include "cdbt/opp/opp_private.h"

#ifdef __cplusplus
}
#endif

#endif // __OPP_H
