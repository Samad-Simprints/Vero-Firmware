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

#ifndef __HSP_CONFIG_H
#define __HSP_CONFIG_H

#ifdef _DEBUG

#define HSP_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\
	const bt_uint _ram_size_hsp_buffers =	\
		sizeof(_hsp_sessions);

#else
	#define HSP_ALLOCATE_BUFFERS_RAM_SIZE_VAR
#endif

#define HSP_ALLOCATE_BUFFERS_VARS()	\
	bt_hsp_session_t _hsp_sessions[HSP_MAX_SESSIONS];	\
	bt_byte          _hsp_max_sessions = HSP_MAX_SESSIONS;	\
	HSP_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\

#define HSP_ALLOCATE_BUFFERS()	\
	HSP_ALLOCATE_BUFFERS_VARS()	\
	typedef int HSP_BUFFERS_ALLOCATED \

#endif // __HSP_CONFIG_H
