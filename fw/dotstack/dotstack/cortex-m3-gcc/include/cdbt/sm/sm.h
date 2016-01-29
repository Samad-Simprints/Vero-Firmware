/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __SM_H
#define __SM_H

/**
* \defgroup sm Security Manager
*
*  This module describes functions and data structures used to implement Security Manager.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "cdbt/sm/smp.h"

#define SM_SESSION_STATE_FREE                 0x0000
#define SM_SESSION_STATE_DISCONNECTED         0x0001
#define SM_SESSION_STATE_CONNECTED            0x0002
#define SM_SESSION_STATE_PAIRING_STARTED      0x0004
#define SM_SESSION_STATE_SENDING              0x0008
#define SM_SESSION_STATE_PAIRING_FAILED       0x0010
#define SM_SESSION_STATE_CONFIRM_GENERATED    0x0020
#define SM_SESSION_STATE_WAIT_STK_ENCRYPTION  0x0040
#define SM_SESSION_STATE_CONFIRM_RECEIVED     0x0080
#define SM_SESSION_STATE_AUTHENTICATED        0x0100

#define SM_EVENT_PAIRING_REQUEST              1
#define SM_EVENT_PASSKEY_REQUEST              2
#define SM_EVENT_PASSKEY_NOTIFICATION         3
#define SM_EVENT_OOB_DATA_REQUEST             4
#define SM_EVENT_PAIRING_COMPLETE             5
#define SM_EVENT_PEER_LTK_REQUEST             6
#define SM_EVENT_CONNECTED                    7
#define SM_EVENT_DISCONNECTED                 8
#define SM_EVENT_GENERATE_LTK                 9
#define SM_EVENT_LTK_GENERATED                10
#define SM_EVENT_LOCAL_LTK_REQUEST            11
#define SM_EVENT_PEER_KEYS_NOTIFICATION       12

#define SM_SESSION_EVENT_TK_GENERATED         1
#define SM_SESSION_EVENT_RAND_GENERATED       2
#define SM_SESSION_EVENT_RAND_RECEIVED        3
#define SM_SESSION_EVENT_CONFIRM_RECEIVED     4
#define SM_SESSION_EVENT_LTK_GENERATED        5
#define SM_SESSION_EVENT_LTK_FAILED           6
#define SM_SESSION_EVENT_LTK_SENT             7
#define SM_SESSION_EVENT_MASTER_IDENT_SENT    8
#define SM_SESSION_EVENT_IRK_SENT             9
#define SM_SESSION_EVENT_IDENT_ADDR_INFO_SENT 10
#define SM_SESSION_EVENT_CSRK_SENT            11
#define SM_SESSION_EVENT_RAND_SENT            12

#define SM_PEER_KEYS_LTK_VALID                1
#define SM_PEER_KEYS_IRK_VALID                2
#define SM_PEER_KEYS_CSRK_VALID               4


typedef struct _bt_sm_pairing_features_t
{
	bt_byte io_capability;
	bt_byte oob_data_flag;
	bt_byte auth_req;
	bt_byte min_key_size;
	bt_byte max_key_size;
	bt_byte ikd;
	bt_byte rkd;
} bt_sm_pairing_features_t;

typedef struct _bt_sm_ltk_t
{
	bt_byte* ltk;
	bt_byte  ltk_len;
	bt_uint  div;
	bt_byte* rand;
} bt_sm_ltk_t;

typedef struct _bt_sm_session_t
{
	struct _bt_sm_session_t* next_session;

	bt_uint state;
	bt_byte phase;
	bt_smp_session_t* smp_session;
	bt_sm_pairing_features_t local_pairing_features;
	bt_byte initiator_key_distribution;
	bt_byte responder_key_distribution;
	bt_sm_pairing_features_t remote_pairing_features;
	union
	{
		bt_byte stk[SMP_KEY_LEN];
		bt_byte ltk[SMP_KEY_LEN];
		bt_byte irk[SMP_KEY_LEN];
	} key;
	bt_byte key_size;
	bt_byte random[SMP_KEY_LEN];
	bt_byte peer_random[SMP_KEY_LEN];
	bt_byte mconfirm[SMP_KEY_LEN];
	bt_byte sconfirm[SMP_KEY_LEN];
	bt_uint mask;
	bt_uint div;

	struct
	{
		bt_byte flags;

		bt_uint div;
		bt_byte random[SMP_RAND_LEN];
		bt_byte ltk[SMP_KEY_LEN];

		bt_bdaddr_t addr;
		bt_byte addr_type;
		bt_byte irk[SMP_KEY_LEN];

		bt_byte csrk[SMP_KEY_LEN];
	} peer_keys;

	bt_hci_ctrl_listener_t encryption_change_listener;

} bt_sm_session_t;

typedef struct _bt_sm_evt_connected_t
{
	bt_sm_session_t* session;
} bt_sm_evt_connected_t;

typedef struct _bt_sm_evt_disconnected_t
{
	bt_sm_session_t* session;
} bt_sm_evt_disconnected_t;

typedef struct _bt_sm_evt_pairing_request_t
{
	bt_sm_session_t* session;
	const bt_sm_pairing_features_t* remote_features;
} bt_sm_evt_pairing_request_t;

typedef struct _bt_sm_evt_passkey_request_t
{
	bt_sm_session_t* session;
} bt_sm_evt_passkey_request_t;

typedef struct _bt_sm_evt_oob_data_request_t
{
	bt_sm_session_t* session;
} bt_sm_evt_oob_data_request_t;

typedef struct _bt_sm_evt_passkey_notification_t
{
	bt_sm_session_t* session;
	bt_ulong passkey;
} bt_sm_evt_passkey_notification_t;

typedef struct _bt_sm_evt_pairing_complete_t
{
	bt_sm_session_t* session;
	bt_byte status;
} bt_sm_evt_pairing_complete_t;

typedef struct _bt_sm_evt_generate_ltk_s
{
	bt_sm_session_t* session;
	bt_byte key_size;
} bt_sm_evt_generate_lkt_t;

typedef struct _bt_sm_evt_ltk_generated_s
{
	bt_sm_session_t* session;
	bt_sm_ltk_t ltk;
} bt_sm_evt_ltk_generated_t;

typedef struct _bt_sm_evt_master_ltk_request_t
{
	bt_sm_session_t* session;
	bt_bdaddr_t* bdaddr_remote;
	bt_byte addr_type;
} bt_sm_evt_master_ltk_request_t;

typedef struct _bt_sm_evt_slave_ltk_request_s
{
	bt_sm_session_t* session;
	bt_uint div;
	const bt_byte* random;
} bt_sm_evt_slave_ltk_request_t;

typedef struct _bt_sm_evt_master_ltk_notif_s
{
	bt_sm_session_t* session;
	const bt_bdaddr_t* bdaddr_remote;
	bt_byte addr_type;
	bt_sm_ltk_t ltk;
} bt_sm_evt_master_ltk_notif_t;

typedef struct _bt_sm_evt_master_irk_notif_s
{
	bt_sm_session_t* session;
	const bt_bdaddr_t* bdaddr_remote;
	bt_byte addr_type;
	const bt_byte* irk;
} bt_sm_evt_master_irk_notif_t;

typedef struct _bt_sm_evt_master_csrk_notif_s
{
	bt_sm_session_t* session;
	const bt_bdaddr_t* bdaddr_remote;
	bt_byte addr_type;
	const bt_byte* csrk;
} bt_sm_evt_master_csrk_notif_t;

typedef struct _bt_sm_evt_peer_keys_notif_s
{
	bt_sm_session_t* session;

	bt_byte flags;

	bt_uint div;
	bt_byte* random;
	bt_byte* ltk;

	bt_bdaddr_t* addr;
	bt_byte addr_type;
	bt_byte* irk;

	bt_byte* csrk;
} bt_sm_evt_peer_keys_notif_t;

typedef union _bt_sm_event_t
{
	bt_sm_evt_connected_t            connected;
	bt_sm_evt_disconnected_t         disconnected;
	bt_sm_evt_pairing_request_t      pairing_request;
	bt_sm_evt_passkey_request_t      passkey_request;
	bt_sm_evt_oob_data_request_t     oob_data_request;
	bt_sm_evt_passkey_notification_t passkey_notification;
	bt_sm_evt_pairing_complete_t     pairing_complete;
	bt_sm_evt_master_ltk_request_t   master_ltk_request;
	bt_sm_evt_generate_lkt_t         generate_ltk;
	bt_sm_evt_ltk_generated_t        ltk_generated;
	bt_sm_evt_slave_ltk_request_t    slave_ltk_request;
	bt_sm_evt_peer_keys_notif_t      peer_keys;
} bt_sm_event_t;

typedef void (*bt_sm_mgr_callback_fp)(bt_int evt, bt_sm_event_t* evt_param, void* param);

typedef struct _bt_cm_listener_t
{
	struct _bt_cm_listener_t* next_listener;

	bt_sm_mgr_callback_fp callback;
	void* callback_param;
} bt_sm_listener_t;

typedef struct _bt_security_mgr_t
{
	bt_byte state;

	bt_sm_session_t* sessions;

	const bt_byte* irk;
	const bt_byte* csrk;
	const bt_byte* dhk;
	const bt_byte* er;

	bt_sm_listener_t* listeners;

} bt_security_mgr_t;


bt_security_mgr_t* bt_sm_get_mgr(void);

bt_bool bt_sm_init(void);

void bt_sm_set_er(const void* key);

void bt_sm_set_dhk(const void* key);

void bt_sm_set_irk(const void* key);

const void* bt_sm_get_irk(void);

void bt_sm_set_csrk(const void* key);

void bt_sm_start(void);

bt_bool bt_sm_authenticate(bt_hci_conn_state_t* conn, bt_byte auth_req);

bt_uint bt_sm_get_session_state(bt_hci_conn_state_t* conn);

bt_byte bt_sm_get_session_key_size(bt_hci_conn_state_t* conn);

bt_bool bt_sm_distribute_ltk(bt_sm_session_t* session, bt_sm_ltk_t* ltk);

bt_bool bt_sm_set_pairing_features(bt_sm_session_t* session, bt_byte status, const bt_sm_pairing_features_t* features);

bt_bool bt_sm_set_passkey(bt_sm_session_t* session, bt_byte status, bt_ulong passkey);

bt_bool bt_sm_set_oob_data(bt_sm_session_t* session, bt_byte status, const bt_byte* oob_data);

bt_bool bt_sm_generate_ltk(bt_sm_session_t* session, bt_byte key_size);

bt_bool bt_sm_send_ltk(bt_sm_session_t* session, const bt_sm_ltk_t* ltk);

bt_bool bt_sm_register_listener(bt_sm_listener_t* listener);

void bt_sm_unregister_listener(bt_sm_listener_t* listener);

#include "cdbt/sm/sm_private.h"

#ifdef __cplusplus
}
#endif

#endif // __SM_H
