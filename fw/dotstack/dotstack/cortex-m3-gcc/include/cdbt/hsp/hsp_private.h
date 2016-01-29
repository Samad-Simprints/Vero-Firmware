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

#ifndef __HSP_PRIVATE_H
#define __HSP_PRIVATE_H


#ifdef __cplusplus
extern "C" {
#endif

extern bt_hsp_session_t _hsp_sessions[];
extern bt_byte          _hsp_max_sessions;

#ifdef _DEBUG
extern const bt_uint _ram_size_hsp_buffers;
#endif

void bt_hsp_init_sessions(void);
bt_hsp_session_t* bt_hsp_find_session(bt_rfcomm_dlc_t* rfcomm_dlc);
bt_hsp_session_t* bt_hsp_find_session_by_hconn(bt_hci_conn_state_t* hci_conn);

void _bt_hsp_init_signal(void);
void _bt_hsp_set_signal(void);
bt_bool _bt_hsp_send_audio_packet(bt_hsp_session_t* session);

#ifdef __cplusplus
}
#endif

#endif // __HSP_PRIVATE_H
