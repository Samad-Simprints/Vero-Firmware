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

#ifndef __MAP_CONFIG_H
#define __MAP_CONFIG_H

#ifndef MAP_MAX_SESSIONS
#error "MAP_MAX_SESSIONS is not defined"
#endif

#if MAP_MAX_SESSION > OBEX_MAX_SESSIONS
#error "MAP_MAX_SESSION cannot exceed OBEX_MAX_SESSIONS"
#endif

#ifdef _DEBUG

#define MAP_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\
	const bt_uint _ram_size_map_buffers =	\
		sizeof(_map_sessions) +	\
		sizeof(_map_max_sessions);

#else
	#define MAP_ALLOCATE_BUFFERS_RAM_SIZE_VAR
#endif

#define MAP_ALLOCATE_BUFFERS_VARS()	\
	bt_map_session_t _map_sessions[MAP_MAX_SESSIONS];	\
	bt_byte          _map_max_sessions = MAP_MAX_SESSIONS;	\
	MAP_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\

#define MAP_ALLOCATE_BUFFERS_FUNCTION()	\
	void _map_allocate_buffers(void)	\
	{	\
	}	\

#define MAP_ALLOCATE_BUFFERS()	\
	MAP_ALLOCATE_BUFFERS_VARS()	\
	MAP_ALLOCATE_BUFFERS_FUNCTION()	\
	typedef int MAP_BUFFERS_ALLOCATED \

#endif // __MAP_CONFIG_H
