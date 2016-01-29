/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __SM_PRIVATE_H
#define __SM_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*bt_sm_tk_generator_fp)(bt_sm_session_t* session);

extern bt_sm_session_t     _sm_sessions[];

#ifdef _DEBUG
	extern const bt_uint _ram_size_smp_buffers;
#endif

void _bt_sm_init_mgr(void);

void _bt_smp_allocate_buffers(void);

bt_sm_session_t* _bt_sm_allocate_session(bt_smp_session_t* smp_session) ;
bt_sm_session_t* _bt_sm_find_session_by_smp_session(bt_smp_session_t* smp_session);
bt_sm_session_t* _bt_sm_find_session_by_hci_connection(bt_hci_conn_state_t* conn);
void _bt_sm_free_session(bt_sm_session_t* session);

bt_hci_conn_state_t* _bt_sm_get_hci_connection(const bt_sm_session_t* session);

void _bt_sm_long_term_key_request_handler(bt_hci_event_t* evt);

void _bt_sm_pairing_request_handler(bt_sm_session_t* session, const bt_smp_evt_pairing_request_t* params);
void _bt_sm_pairing_response_handler(bt_sm_session_t* session, const bt_smp_evt_pairing_response_t* params);
void _bt_sm_pairing_confirm_handler(bt_sm_session_t* session, const bt_smp_evt_pairing_confirm_t* params);
void _bt_sm_pairing_random_handler(bt_sm_session_t* session, const bt_smp_evt_pairing_random_t* params);
void _bt_sm_pairing_failed_handler(bt_sm_session_t* session, const bt_smp_evt_pairing_failed_t* params);
void _bt_sm_encryption_information_handler(bt_sm_session_t* session, const bt_smp_evt_encryption_info_t* params);
void _bt_sm_master_identification_handler(bt_sm_session_t* session, const bt_smp_evt_master_ident_t* params);
void _bt_sm_identity_information_handler(bt_sm_session_t* session, const bt_smp_evt_identity_info_t* params);
void _bt_sm_identity_address_information_handler(bt_sm_session_t* session, const bt_smp_evt_identity_address_info_t* params);
void _bt_sm_signing_information_handler(bt_sm_session_t* session, const bt_smp_evt_signing_info_t* params);
void _bt_sm_security_request_handler(bt_sm_session_t* session, const bt_smp_evt_sequrity_request_t* params);
void _bt_sm_spacket_sent_handler(bt_sm_session_t* session);

bt_sm_tk_generator_fp _bt_sm_select_tk_generation_method(bt_sm_session_t* session);
void _bt_sm_session_event_handler(bt_sm_session_t* session, bt_int evt, void* evt_param);

bt_bool _bt_sm_pair(bt_sm_session_t* session);

#define _bt_sm_is_initiator(session)    (session->smp_session->channel->hci_conn->role == HCI_CONN_ROLE_MASTER)

void _bt_sm_distribute_keys(bt_sm_session_t* session);

void _bt_sm_notify_listeners(bt_int evt, bt_sm_event_t* evt_param);

#ifdef __cplusplus
}
#endif

#endif // __SM_PRIVATE_H
