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

#ifndef __PBAP_PRIVATE_H
#define __PBAP_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

#define PBAP_PARAM_ORDER				1
#define PBAP_PARAM_SEARCH_VALUE			2
#define PBAP_PARAM_SEARCH_ATTR			3
#define PBAP_PARAM_MAX_LIST_COUNT		4
#define PBAP_PARAM_LIST_START_OFFSET	5
#define PBAP_PARAM_FILTER				6
#define PBAP_PARAM_FORMAT				7
#define PBAP_PARAM_PB_SIZE				8
#define PBAP_PARAM_NEW_MISSED_CALLS		9


extern bt_pbap_session_t     _pbap_sessions[];
extern const bt_byte       _pbap_max_sessions;
extern const bt_byte       _pbap_max_search_server_results;
extern bt_pbap_server_t    _pbap_server_buffers[];
extern const bt_byte       _pbap_max_server_name_len;
extern bt_byte             _pbap_server_name_buffers[];

#ifdef _DEBUG
extern const bt_uint _ram_size_pbap_buffers;
#endif

void bt_pbap_init_sessions(void);
bt_pbap_session_t* bt_pbap_find_session(bt_obex_session* obex_session);
bt_bool bt_pbap_find_server(bt_pbap_session_t* session, bt_bdaddr_t* deviceAddress, bt_pbap_find_server_callback_pf callback);

#ifdef __cplusplus
}
#endif

#endif // __PBAP_PRIVATE_H
