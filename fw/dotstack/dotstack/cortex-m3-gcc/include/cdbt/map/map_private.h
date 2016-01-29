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

#ifndef __MAP_PRIVATE_H
#define __MAP_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

extern bt_map_session_t _map_sessions[];
extern bt_byte        _map_max_sessions;

#ifdef _DEBUG
extern const bt_uint  _ram_size_map_buffers;
#endif

// Defined by OEM through library configuration
void _map_allocate_buffers(void);

void bt_map_init_sessions(void);
bt_map_session_t* bt_map_find_session(bt_obex_session* obex_session);
bt_bool bt_map_find_server(bt_map_session_t* session, bt_bdaddr_t* deviceAddress, bt_map_find_server_callback_pf callback);

#ifdef __cplusplus
}
#endif

#endif // __MAP_PRIVATE_H
