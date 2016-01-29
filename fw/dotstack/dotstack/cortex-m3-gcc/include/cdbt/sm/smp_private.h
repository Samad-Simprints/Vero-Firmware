/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __SMP_PRIVATE_H
#define __SMP_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

extern bt_smp_session_t    _smp_sessions[];
extern bt_byte             _smp_max_sessions;

#ifdef _DEBUG
extern const bt_uint _ram_size_smp_buffers;
#endif

void _bt_smp_init_mgr(void);
void _bt_smp_init_timer(void);
bt_smp_session_t* _bt_smp_get_active_sessions(void);

bt_smp_session_t* _bt_smp_allocate_session(void) ;
bt_smp_session_t* _bt_smp_find_session(bt_bdaddr_t* bdaddr);
bt_smp_session_t* _bt_smp_find_session_by_channel(bt_l2cap_channel_t* channel);
void _bt_smp_free_session(bt_smp_session_t* session);

void _bt_smp_mgr_notify_listeners(bt_int evt, void* evt_param);

void _bt_smp_start_timer(bt_smp_session_t* session);
void _bt_smp_stop_timer(bt_smp_session_t* session);
void _bt_smp_reset_timer(bt_smp_session_t* session);

#define _bt_smp_is_initiator(session)    (session->channel->hci_conn->role == HCI_CONN_ROLE_MASTER)

#ifdef __cplusplus
}
#endif

#endif // __SMP_PRIVATE_H
