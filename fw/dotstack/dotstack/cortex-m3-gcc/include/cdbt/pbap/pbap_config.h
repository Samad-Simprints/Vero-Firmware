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

#ifndef __PBAP_CONFIG_H
#define __PBAP_CONFIG_H

#ifndef PBAP_MAX_SESSIONS
#error "PBAP_MAX_SESSIONS is not defined"
#endif

#if PBAP_MAX_SESSION > OBEX_MAX_SESSIONS
#error "PBAP_MAX_SESSION cannot exceed OBEX_MAX_SESSIONS"
#endif

#ifndef PBAP_MAX_SEARCH_SERVER_RESULTS
#define PBAP_MAX_SEARCH_SERVER_RESULTS    5
#endif

#ifndef PBAP_MAX_SERVER_NAME_LEN
#define PBAP_MAX_SERVER_NAME_LEN          32
#endif

#ifdef _DEBUG

#define PBAP_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\
	const bt_uint _ram_size_pbap_buffers =	\
		sizeof(_pbap_sessions) +	\
		sizeof(_pbap_max_sessions);

#else
	#define PBAP_ALLOCATE_BUFFERS_RAM_SIZE_VAR
#endif

#define PBAP_ALLOCATE_BUFFERS_VARS()	\
	bt_pbap_session_t     _pbap_sessions[PBAP_MAX_SESSIONS];	\
	const bt_byte       _pbap_max_sessions = PBAP_MAX_SESSIONS;	\
	const bt_byte       _pbap_max_search_server_results = PBAP_MAX_SEARCH_SERVER_RESULTS;	\
	bt_pbap_server_t    _pbap_server_buffers[(PBAP_MAX_SEARCH_SERVER_RESULTS) * (PBAP_MAX_SESSIONS)];	\
	const bt_byte       _pbap_max_server_name_len = PBAP_MAX_SERVER_NAME_LEN;	\
	bt_byte             _pbap_server_name_buffers[(PBAP_MAX_SEARCH_SERVER_RESULTS) * (PBAP_MAX_SESSIONS) * (PBAP_MAX_SERVER_NAME_LEN + 1)];	\
	PBAP_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\

#define PBAP_ALLOCATE_BUFFERS()	\
	PBAP_ALLOCATE_BUFFERS_VARS()	\
	typedef int PBAP_BUFFERS_ALLOCATED \

#endif // __PBAP_CONFIG_H
