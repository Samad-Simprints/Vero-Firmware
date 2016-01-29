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

#ifndef __PBAP_H
#define __PBAP_H

#include "cdbt/l2cap/l2cap.h"
#include "cdbt/rfcomm/rfcomm.h"
#include "cdbt/obex/obex.h"
#include "cdbt/sdp/sdp.h"
#include "cdbt/utils/vcard_parser.h"
#include "cdbt/utils/xml_parser.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PBAP_SESSION_STATE_FREE                         0x00
#define PBAP_SESSION_STATE_USED                         0x01
#define PBAP_SESSION_STATE_SEARCHING                    0x02

#define PBAP_SESSION_FLAG_PULL_LISTING                  0x01
#define PBAP_SESSION_FLAG_PULL_VCARD                    0x02

#define PBAP_EVT_SESSION_NOTHING						0
#define PBAP_EVT_SESSION_CONNECT_FAILED					1
#define PBAP_EVT_SESSION_CONNECTED						2
#define PBAP_EVT_SESSION_DISCONNECTED					3
#define PBAP_EVT_SESSION_PULL_PHONEBOOK_COMPLETED		4
#define PBAP_EVT_SESSION_PULL_VCARD_LISTING_COMPLETED	5
#define PBAP_EVT_SESSION_PULL_VCARD_COMPLETED			6
#define PBAP_EVT_SESSION_PULL_PHONEBOOK_ABORTED			7
#define PBAP_EVT_SESSION_PULL_VCARD_LISTING_ABORTED		8
#define PBAP_EVT_SESSION_PULL_VCARD_ABORTED				9

#define PBAP_EVT_SESSION_VCARD_STARTED					10
#define PBAP_EVT_SESSION_VCARD_ENDED					11
#define PBAP_EVT_SESSION_VCARD_TYPE_STARTED				12
#define PBAP_EVT_SESSION_VCARD_TYPE_VALUE				13
#define PBAP_EVT_SESSION_VCARD_TYPE_PARAM				14

#define PBAP_EVT_SESSION_PHONEBOOK_ENTRY				15
#define PBAP_EVT_SESSION_VCARD_LIST						16
#define PBAP_EVT_SEARCH_COMPLETED                       17
#define PBAP_EVT_SESSION_AUTH_REQUESTED               18

#define PBAP_SESSION_OBJECT_TYPE				0x0F
#define PBAP_SESSION_OBJECT_TYPE_FOLDER			0x01
#define PBAP_SESSION_OBJECT_TYPE_FILE			0x02
#define PBAP_SESSION_OBJECT_TYPE_DELETE_OBJECT	0x04

#define PBAP_VCARD_ATTR_FILTER_VERSION					0x0000000000000001
#define PBAP_VCARD_ATTR_FILTER_FN						0x0000000000000002
#define PBAP_VCARD_ATTR_FILTER_N						0x0000000000000004
#define PBAP_VCARD_ATTR_FILTER_PHOTO					0x0000000000000008
#define PBAP_VCARD_ATTR_FILTER_BDAY						0x0000000000000010
#define PBAP_VCARD_ATTR_FILTER_ADR						0x0000000000000020
#define PBAP_VCARD_ATTR_FILTER_LABEL					0x0000000000000040
#define PBAP_VCARD_ATTR_FILTER_TEL						0x0000000000000080
#define PBAP_VCARD_ATTR_FILTER_EMAIL					0x0000000000000100
#define PBAP_VCARD_ATTR_FILTER_MAILER					0x0000000000000200
#define PBAP_VCARD_ATTR_FILTER_TZ						0x0000000000000400
#define PBAP_VCARD_ATTR_FILTER_GEO						0x0000000000000800
#define PBAP_VCARD_ATTR_FILTER_TITLE					0x0000000000001000
#define PBAP_VCARD_ATTR_FILTER_ROLE						0x0000000000002000
#define PBAP_VCARD_ATTR_FILTER_LOGO						0x0000000000004000
#define PBAP_VCARD_ATTR_FILTER_AGENT					0x0000000000008000
#define PBAP_VCARD_ATTR_FILTER_ORG						0x0000000000010000
#define PBAP_VCARD_ATTR_FILTER_NOTE						0x0000000000020000
#define PBAP_VCARD_ATTR_FILTER_REV						0x0000000000040000
#define PBAP_VCARD_ATTR_FILTER_SOUND					0x0000000000080000
#define PBAP_VCARD_ATTR_FILTER_URL						0x0000000000100000
#define PBAP_VCARD_ATTR_FILTER_UID						0x0000000000200000
#define PBAP_VCARD_ATTR_FILTER_KEY						0x0000000000400000
#define PBAP_VCARD_ATTR_FILTER_NICKNAME					0x0000000000800000
#define PBAP_VCARD_ATTR_FILTER_CATEGORIES				0x0000000001000000
#define PBAP_VCARD_ATTR_FILTER_PROID					0x0000000002000000
#define PBAP_VCARD_ATTR_FILTER_CLASS					0x0000000004000000
#define PBAP_VCARD_ATTR_FILTER_SORT_STRING				0x0000000008000000
#define PBAP_VCARD_ATTR_FILTER_X_IRMC_CALL_DATETIME		0x0000000010000000

#define PBAP_SEARCH_ATTR_NAME           0x0
#define PBAP_SEARCH_ATTR_NUMBER         0x1
#define PBAP_SEARCH_ATTR_SOUND          0x2

#define PBAP_PHONEBOOK_LOCATION_LOCAL	0x000
#define PBAP_PHONEBOOK_LOCATION_TELECOM	0x010
#define PBAP_PHONEBOOK_LOCATION_SIM1	0x100

#define PBAP_PHONEBOOK_TYPE_LOCAL_ROOT		0x010
#define PBAP_PHONEBOOK_TYPE_LOCAL_MAIN		0x011
#define PBAP_PHONEBOOK_TYPE_LOCAL_INCOMING	0x012
#define PBAP_PHONEBOOK_TYPE_LOCAL_OUTGOING	0x013
#define PBAP_PHONEBOOK_TYPE_LOCAL_MISSED	0x014
#define PBAP_PHONEBOOK_TYPE_LOCAL_COMBINED	0x015

#define PBAP_PHONEBOOK_TYPE_SIM1_ROOT		0x110
#define PBAP_PHONEBOOK_TYPE_SIM1_MAIN		0x111
#define PBAP_PHONEBOOK_TYPE_SIM1_INCOMING	0x112
#define PBAP_PHONEBOOK_TYPE_SIM1_OUTGOING	0x113
#define PBAP_PHONEBOOK_TYPE_SIM1_MISSED		0x114
#define PBAP_PHONEBOOK_TYPE_SIM1_COMBINED	0x115

#define PBAP_PHONEBOOK_NAME_MAX_LEN     40
#define PBAP_APP_PARAMS_MAX_LEN         50

#define PBAP_SESSION_OPERATION                  0xF0
#define PBAP_SESSION_OPERATION_PULL_PHONEBOOK   0x10
#define PBAP_SESSION_OPERATION_PULL_LISTING     0x20
#define PBAP_SESSION_OPERATION_PULL_VCARD       0x40
#define PBAP_SESSION_OPERATION_SET_PHONEBOOK    0x80

#define PBAP_VCARD_FORMAT_21    0
#define PBAP_VCARD_FORMAT_30    1

#define PBAP_OPERATION_STATUS_SUCCESS       0
#define PBAP_OPERATION_STATUS_ERROR         1
#define PBAP_OPERATION_STATUS_OBEX_ERROR    2

#define PBAP_MAX_HANDLE_LEN		XML_BUFFER_LEN
#define PBAP_MAX_NAME_LEN		XML_BUFFER_LEN

#define PBAP_WAIT_ENTRY_START   0
#define PBAP_WAIT_ATTR_START    1
#define PBAP_WAIT_HANDLE_VALUE  2
#define PBAP_WAIT_NAME_VALUE    3

typedef struct _bt_pbap_session_s bt_pbap_session_t;

typedef void (*bt_pbap_find_server_callback_pf)(bt_pbap_session_t* session, bt_byte server_channel, bt_bool found);

typedef void (*bt_pbap_session_callback_pf)(bt_pbap_session_t* session, bt_byte evt, void* evt_param, void* callback_param);

typedef struct _bt_vcard_property
{
	bt_byte* value;
	bt_byte value_len;
} bt_vcard_property;

typedef struct _bt_vcard
{
	bt_vcard_property version;              // vCard Version
	bt_vcard_property fn;                   // Formatted Name
	bt_vcard_property n;                    // Structured Presentation of Name
	bt_vcard_property photo;                // Associated Image or Photo
	bt_vcard_property bday;                 // Birthday
	bt_vcard_property adr;                  // Delivery Address
	bt_vcard_property label;                // Delivery
	bt_vcard_property tel;                  // Telephone Number
	bt_vcard_property email;                // Electronic Mail Address
	bt_vcard_property mailer;               // Electronic Mail
	bt_vcard_property tz;                   // Time Zone
	bt_vcard_property geo;                  // Geographic Position
	bt_vcard_property title;                // Job
	bt_vcard_property role;                 // Role within the Organization
	bt_vcard_property logo;                 // Organization Logo
	bt_vcard_property agent;                // vCard of Person Representing
	bt_vcard_property org;                  // Name of Organization
	bt_vcard_property note;                 // Comments
	bt_vcard_property rev;                  // Revision
	bt_vcard_property sound;                // Pronunciation of Name
	bt_vcard_property url;                  // Uniform Resource Locator
	bt_vcard_property uid;                  // Unique ID
	bt_vcard_property key;                  // Public Encryption Key
	bt_vcard_property nickname;             // Nickname
	bt_vcard_property categories;           // Categories
	bt_vcard_property proid;                // Product ID
	bt_vcard_property vclass;               // Class information
	bt_vcard_property sort_string;          // String used for sorting operations
	bt_vcard_property x_irmc_call_datetime; // Time stamp
	bt_vcard_property callType;
} bt_vcard;

typedef struct _bt_pbap_evt_pull_phonebook_completed_t
{
	bt_byte status;
	bt_byte obex_packet_type;
	bt_uint pb_size;
	bt_byte new_missed_calls_count;
	bt_uint pb_type;
} bt_pbap_evt_pull_phonebook_completed_t;

typedef struct _bt_pbap_evt_set_phonebook_completed_s
{
	bt_byte status;
	bt_byte obex_packet_type;
} bt_pbap_evt_set_phonebook_completed_t;

typedef struct _bt_pbap_evt_pull_vcard_listing_completed_t
{
	bt_byte status;
	bt_byte obex_packet_type;
	bt_uint vcard_listing_size;
	bt_byte new_missed_calls_count;
	bt_uint pb_type;
} bt_pbap_evt_pull_vcard_listing_completed_t;

typedef struct _bt_pbap_evt_pull_vcard_completed_t
{
	bt_byte status;
	bt_byte obex_packet_type;
} bt_pbap_evt_pull_vcard_completed_t;

typedef union _bt_pbap_evt_operation_completed_t
{
	bt_pbap_evt_pull_phonebook_completed_t ppb;
	bt_pbap_evt_set_phonebook_completed_t spb;
	bt_pbap_evt_pull_vcard_listing_completed_t pvcl;
	bt_pbap_evt_pull_vcard_completed_t pvc;

} bt_pbap_evt_operation_completed_t;

typedef struct _bt_pbap_evt_vcard_t
{
	bt_bool begin;
	bt_uint pb_type;
} bt_pbap_evt_vcard_t;

typedef struct _bt_pbap_evt_vcard_type_t
{
	bt_byte type_id;
	bt_byte* type_value;
	bt_byte value_len;
	bt_bool final;
	bt_uint pb_type;
} bt_pbap_evt_vcard_type_t;

typedef struct _bt_pbap_evt_vcard_type_param_t
{
	bt_byte param_id;
	bt_byte* param_value;
	bt_byte param_len;
	bt_uint pb_type;
} bt_pbap_evt_vcard_type_param_t;

typedef struct _bt_pbap_evt_vcard_list_t
{
	bt_vcard* vcard_list;
	bt_uint vcard_list_size;
	bt_uint pb_type;
} bt_pbap_evt_vcard_list_t;

typedef union _bt_pbap_evt_vcard_events_t
{
	bt_pbap_evt_vcard_t vcard;
	bt_pbap_evt_vcard_type_t type;
	bt_pbap_evt_vcard_type_param_t param;
} bt_pbap_evt_vcard_events_t;

typedef struct _bt_pbap_evt_pb_entry_t
{
	bt_byte handle[PBAP_MAX_HANDLE_LEN];
	bt_byte handle_len;
	bt_byte name[PBAP_MAX_NAME_LEN];
	bt_byte name_len;
} bt_pbap_evt_pb_entry_t;

typedef struct _bt_pbap_server_s bt_pbap_server_t;

typedef struct _bt_pbap_evt_search_completed_s
{
	bt_pbap_server_t* servers; 
	bt_byte count;
} bt_pbap_evt_search_completed_t;

typedef struct _bt_pbap_evt_auth_requested_s
{
	obex_auth_challenge_t* challenge;

} bt_pbap_evt_auth_requested_t;


struct _bt_pbap_server_s
{
	bt_bdaddr_t address;
	bt_byte* name;
	bt_byte server_channel;
	bt_byte pg_scan_rpt_mode;
	bt_byte pg_scan_period_mode;
	bt_long cod;
	bt_int clock_offset;
	bt_byte rssi;
};

struct _bt_pbap_session_s
{
	bt_byte state;
	bt_byte flags;
	bt_obex_session* obex_session;
	bt_ulong connection_id;
	bt_byte cur_operation;

	bt_pbap_session_callback_pf session_callback;
	void* session_callback_param;

	bt_l2cap_channel_t* find_server_sdp_channel;
	bt_sdp_data_element_t sdp_pattern;
	bt_sr_handle_t find_server_service_record_handle;
	bt_bool find_server_next_attr_is_server_channel;
	bt_pbap_find_server_callback_pf find_server_callback;

	bt_obex_header headers[4];
	bt_byte pb_name_buffer[PBAP_PHONEBOOK_NAME_MAX_LEN];
	bt_byte pb_name_len;
	bt_byte app_params[PBAP_APP_PARAMS_MAX_LEN];
	bt_byte app_params_len;
	bt_uint listing_pb_type;
	bt_uint current_pb_type;
	bt_byte new_missed_calls_count;
	bt_uint pb_size;
	bt_pbap_evt_pb_entry_t pb_entry;
	bt_byte read_pb_entry_state;

	bt_vcard_parser_t vcard_parser;
	bt_xml_parser_t xml_parser;

	bt_vcard* vcard_list;
	bt_uint vcard_list_size;
	bt_uint cur_vcard_index;
	bt_bool collect_vcards;
	bt_vcard_property* cur_vcard_property;
	bt_byte cur_vcard_property_len;

	bt_byte cur_server;
	bt_byte server_count;
	bt_pbap_server_t* found_servers;

};

bt_bool bt_pbap_init(void);

bt_pbap_session_t* bt_pbap_allocate_session(void);

void bt_pbap_free_session(bt_pbap_session_t* psess);

bt_bool bt_pbap_connect(
	bt_pbap_session_t* session, bt_bdaddr_t* pbdaddr_remote,
	bt_uint header_count, bt_obex_header* headers,
	bt_pbap_session_callback_pf callback, void* param);

//bt_bool bt_ftp_listen(bt_ftp_session* session, bt_byte server_channel, bt_ftp_session_callback_pf callback, void* param);

bt_bool bt_pbap_disconnect(bt_pbap_session_t* session);

bt_bool bt_pbap_disconnect_ex(bt_pbap_session_t* session, bt_uint header_count, bt_obex_header* headers);

void bt_pbap_set_vcard_buffer(bt_pbap_session_t* session, bt_vcard* buffer, bt_uint buffer_size);

bt_uint bt_pbap_init_vcard_buffer(
	bt_vcard* buffer, 
	bt_uint buffer_size, 
	bt_byte* property_buffer, 
	bt_uint property_buffer_size, 
	bt_byte property_size,
	bt_uint filter);

bt_bool bt_pbap_pull_phonebook(
	bt_pbap_session_t* session, 
	bt_byte pb_type,
	bt_ulong filter, bt_byte format,
	bt_uint max_list_count, bt_uint list_start_offset);

#define bt_pbap_get_phonebook_size(session, pb_type)	\
	bt_pbap_pull_phonebook(session, pb_type, 0, 0, 0, 0)

bt_bool bt_pbap_set_phonebook(bt_pbap_session_t* session, bt_byte pb_type);

bt_bool bt_pbap_pull_vcard_listing(
   bt_pbap_session_t* session, 
   bt_byte pb_type,
   bt_byte order,
   bt_byte search_attr, bt_byte* search_value, bt_byte search_value_len,
   bt_uint max_list_count, bt_uint list_start_offset);

#define bt_pbap_pull_vcard_listing_all(session, pb_type, order)	\
	bt_pbap_pull_vcard_listing(session, pb_type, order, 0, NULL, 0, 0xFFFF, 0)

#define bt_pbap_get_vcard_listing_size(session, pb_type)	\
	bt_pbap_pull_vcard_listing(session, pb_type, 0, 0, NULL, 0, 0, 0)

bt_bool bt_pbap_pull_vcard(
   bt_pbap_session_t* session, 
   bt_byte pb_type,
   bt_byte* vcard_name, bt_byte vcard_name_len,
   bt_ulong filter, bt_byte format);

bt_bool bt_pbap_login(bt_pbap_session_t* session, const obex_auth_challenge_t* challenge, const bt_char* password);

bt_bool bt_pbap_abort(bt_pbap_session_t* session);

bt_hci_conn_state_t* bt_pbap_get_hci_connection(const bt_pbap_session_t* session);

#include "cdbt/pbap/pbap_private.h"

#ifdef __cplusplus
}
#endif

#endif // __PBAP_H
