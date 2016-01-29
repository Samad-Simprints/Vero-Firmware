/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __SMP_H
#define __SMP_H

#ifdef __cplusplus
extern "C" {
#endif

#define SMP_KEY_LEN    16
#define SMP_RAND_LEN   8
#define SMP_PAIRING_REQUEST_LEN    7
#define SMP_PAIRING_RESPONSE_LEN   7

#include "cdbt/l2cap/l2cap.h"
#include "cdbt/sm/smp_packet.h"

#define SMP_CMD_PAIRING_REQUEST                 1
#define SMP_CMD_PAIRING_RESPONSE                2
#define SMP_CMD_PAIRING_CONFIRM                 3
#define SMP_CMD_PAIRING_RANDOM                  4
#define SMP_CMD_PAIRING_FAILED                  5
#define SMP_CMD_ENCRYPTION_INFORMATION          6
#define SMP_CMD_MASTER_IDENTIFICATION           7
#define SMP_CMD_IDENTITY_INFORMATION            8
#define SMP_CMD_IDENTITY_ADDRESS_INFORMATION    9
#define SMP_CMD_SIGNING_INFORMATION             10
#define SMP_CMD_SECURITY_REQUEST                11

#define SMP_SESSION_STATE_FREE                 0x00
#define SMP_SESSION_STATE_DISCONNECTED         0x01
#define SMP_SESSION_STATE_CONNECTED            0x02
#define SMP_SESSION_STATE_PAIRING_STARTED      0x04
#define SMP_SESSION_STATE_SENDING              0x08
#define SMP_SESSION_STATE_PAIRING_FAILED       0x10

#define SMP_EVT_NOTHING                                0
#define SMP_EVT_SESSION_CONNECTED                      1
#define SMP_EVT_SESSION_DISCONNECTED                   2
#define SMP_EVT_PAIRING_TIMEOUT                        3

#define SMP_SESSION_EVT_NOTHING                        0
#define SMP_SESSION_EVT_SESSION_CONNECTED              1
#define SMP_SESSION_EVT_SESSION_DISCONNECTED           2
#define SMP_SESSION_EVT_CONNECTION_FAILED              3
#define SMP_SESSION_EVT_DATA_RECEIVED                  4
#define SMP_SESSION_EVT_PAIRING_REQUEST                5
#define SMP_SESSION_EVT_PAIRING_RESPONSE               6
#define SMP_SESSION_EVT_PAIRING_CONFIRM                7
#define SMP_SESSION_EVT_PAIRING_RANDOM                 8
#define SMP_SESSION_EVT_PAIRING_FAILED                 9
#define SMP_SESSION_EVT_ENCRYPTION_INFORMATION         10
#define SMP_SESSION_EVT_MASTER_IDENTIFICATION          11
#define SMP_SESSION_EVT_IDENTITY_INFORMATION           12
#define SMP_SESSION_EVT_IDENTITY_ADDRESS_INFORMATION   13
#define SMP_SESSION_EVT_SIGNING_INFORMATION            14
#define SMP_SESSION_EVT_SECURITY_REQUEST               15
#define SMP_SESSION_EVT_PAIRING_TIMEOUT                16
#define SMP_SESSION_EVT_PACKET_SENT                    17

#define SMP_PAIRING_ERROR_SUCCESS                        0
#define SMP_PAIRING_ERROR_PASSKEY_ENTRY_FAILED           1
#define SMP_PAIRING_ERROR_OOB_NOT_AVAILABLE              2
#define SMP_PAIRING_ERROR_AUTHENTICATION_REQUIREMENTS    3
#define SMP_PAIRING_ERROR_CONFIRM_VALUE_FAILED           4
#define SMP_PAIRING_ERROR_PAIRING_NOT_SUPPORTED          5
#define SMP_PAIRING_ERROR_ENCRYPTION_KEY_SIZE            6
#define SMP_PAIRING_ERROR_COMMAND_NOT_SUPPORTED          7
#define SMP_PAIRING_ERROR_UNSPECIFIED_REASON             8
#define SMP_PAIRING_ERROR_REPEATED_ATTEMPTS              9
#define SMP_PAIRING_ERROR_INVALID_PARAMETERS             10
#define SMP_PAIRING_ERROR_TIMEOUT                        128
#define SMP_PAIRING_ERROR_DISCONNECTED                   129

#define SMP_IO_CAPABILITY_DISPLAY_ONLY        0
#define SMP_IO_CAPABILITY_DISPLAY_YESNO       1
#define SMP_IO_CAPABILITY_KEYBOARD_ONLY       2
#define SMP_IO_CAPABILITY_NOINPUT_NOOUTPUT    3
#define SMP_IO_CAPABILITY_KEYBOARD_DISPLAY    4

#define SMP_OOB_DATA_NOT_PRESENT    0
#define SMP_OOB_DATA_PRESENT        1

#define SMP_AUTHENTICATION_REQUIREMENTS_NO_BONDING       0
#define SMP_AUTHENTICATION_REQUIREMENTS_BONDING          1
#define SMP_AUTHENTICATION_REQUIREMENTS_MITM_REQUIRED    4

#define SMP_KEY_DISTRIBUTION_ENC_KEY     1
#define SMP_KEY_DISTRIBUTION_ID_KEY      2
#define SMP_KEY_DISTRIBUTION_SIGN_KEY    4

typedef void (*bt_smp_mgr_callback_fp)(bt_int evt, void* evt_param, void* param);

struct _bt_smp_session_t;
typedef void (*bt_smp_session_callback_fp)(struct _bt_smp_session_t* session, bt_int evt, void* evt_param, void* param);

typedef struct _bt_smp_session_t
{
	struct _bt_smp_session_t* next_session;

	bt_byte state;
	bt_l2cap_channel_t* channel;
	bt_smp_packet_t packet;

	bt_smp_session_callback_fp callback;
	void* callback_param;

	bt_ulong pairing_tick_count;

	bt_byte preq[SMP_PAIRING_REQUEST_LEN];
	bt_byte pres[SMP_PAIRING_RESPONSE_LEN];

} bt_smp_session_t;

typedef struct _bt_smp_listener_t
{
	struct _bt_smp_listener_t* next_listener;

	bt_smp_mgr_callback_fp callback;
	void* callback_param;
} bt_smp_listener_t;

// server events

typedef struct _bt_smp_evt_session_connected_t
{
	bt_smp_session_t* session;
} bt_smp_evt_session_connected_t;

typedef struct _bt_smp_evt_session_disconnected_t
{
	bt_smp_session_t* session;
} bt_smp_evt_session_disconnected_t;

typedef struct _bt_smp_evt_tran_timeout_s
{
	bt_smp_session_t* session;
} bt_smp_evt_tran_timeout_t;

// ----------------------------------------------------------------

// session events
typedef struct _bt_smp_evt_data_received_t
{
	const bt_byte* pdata;
	bt_int len;
} bt_smp_evt_data_received_t;

typedef struct _bt_smp_evt_pairing_request_t
{
	bt_byte io_capability;
	bt_byte oob_data_flag;
	bt_byte auth_req;
	bt_byte max_key_size;
	bt_byte initiator_key_distribution;
	bt_byte responder_key_distribution;
} bt_smp_evt_pairing_request_t;

typedef struct _bt_smp_evt_pairing_response_t
{
	bt_byte io_capability;
	bt_byte oob_data_flag;
	bt_byte auth_req;
	bt_byte max_key_size;
	bt_byte initiator_key_distribution;
	bt_byte responder_key_distribution;
} bt_smp_evt_pairing_response_t;

typedef struct _bt_smp_evt_pairing_confirm_t
{
	bt_byte confirm_value[SMP_KEY_LEN];
} bt_smp_evt_pairing_confirm_t;

typedef struct _bt_smp_evt_pairing_random_t
{
	bt_byte random_value[SMP_KEY_LEN];
} bt_smp_evt_pairing_random_t;

typedef struct _bt_smp_evt_pairing_failed_t
{
	bt_byte reason;
} bt_smp_evt_pairing_failed_t;

typedef struct _bt_smp_evt_encryption_info_t
{
	bt_byte ltk[SMP_KEY_LEN]; // Long Term Key
} bt_smp_evt_encryption_info_t;

typedef struct _bt_smp_evt_master_ident_t
{
	bt_uint ediv;
	bt_byte rand[SMP_RAND_LEN];
} bt_smp_evt_master_ident_t;

typedef struct _bt_smp_evt_identity_info_t
{
	bt_byte irk[SMP_KEY_LEN]; // Identity Resolving Key
} bt_smp_evt_identity_info_t;

typedef struct _bt_smp_evt_identity_address_info_t
{
	bt_byte addr_type;
	bt_bdaddr_t bdaddr;
} bt_smp_evt_identity_address_info_t;

typedef struct _bt_smp_evt_signing_info_t
{
	bt_byte signature_key[SMP_KEY_LEN];
} bt_smp_evt_signing_info_t;

typedef struct _bt_smp_evt_sequrity_request_t
{
	bt_byte auth_req;
} bt_smp_evt_sequrity_request_t;

// ----------------------------------------------------------------

typedef struct _bt_smp_mgr_t
{
	bt_byte state;

	bt_smp_listener_t* listeners;
	bt_smp_session_t* sessions;

} bt_smp_mgr_t;



bt_bool bt_smp_init(void);

bt_smp_mgr_t* bt_smp_get_mgr(void);

bt_bool bt_smp_register_listener(bt_smp_listener_t* listener);
void bt_smp_unregister_listener(bt_smp_listener_t* listener);

bt_bool bt_smp_server_start(void);

bt_bool bt_smp_send_pairing_request(
	bt_smp_session_t* session,
	bt_byte io_capability, bt_byte oob_data_flag, 
	bt_byte auth_req, bt_byte max_enc_key_size, 
	bt_byte initiator_key_distribution, 
	bt_byte responder_key_distribution);

bt_bool bt_smp_send_pairing_response(
	bt_smp_session_t* session,
	bt_byte io_capability, bt_byte oob_data_flag, 
	bt_byte auth_req, bt_byte max_enc_key_size, 
	bt_byte initiator_key_distribution, 
	bt_byte responder_key_distribution);

bt_bool bt_smp_send_pairing_confirm(
	bt_smp_session_t* session,
	const bt_byte* confirm_value);

bt_bool bt_smp_send_pairing_random(bt_smp_session_t* session, const bt_byte* random_value);

bt_bool bt_smp_end_pairing(bt_smp_session_t* session, bt_byte reason);

bt_bool bt_smp_send_encryption_info(bt_smp_session_t* session, const bt_byte* ltk);

bt_bool bt_smp_send_master_ident(bt_smp_session_t* session, bt_uint ediv, const bt_byte* rand);

bt_bool bt_smp_send_identity_info(bt_smp_session_t* session, const bt_byte* irk);

bt_bool bt_smp_send_identity_address_info(
	bt_smp_session_t* session, bt_byte addr_type, const bt_bdaddr_t* bdaddr);

bt_bool bt_smp_send_signing_info(bt_smp_session_t* session, const bt_byte* signing_key);

bt_bool bt_smp_send_security_request(bt_smp_session_t* session, bt_byte auth_req);

#include "cdbt/sm/smp_private.h"

#ifdef __cplusplus
}
#endif

#endif // __SMP_H
