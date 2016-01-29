/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/


#ifndef __SMP_CONFIG_H
#define __SMP_CONFIG_H

#include "cdbt/sm/sm.h"

#ifdef _DEBUG

#define SMP_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\
	const bt_uint _ram_size_smp_buffers =	\
	sizeof(_smp_sessions) +	\
	sizeof(_smp_max_sessions);

#else
#define SMP_ALLOCATE_BUFFERS_RAM_SIZE_VAR
#endif

#ifndef SMP_MAX_CLIENTS
	#error "SMP_MAX_CLIENTS is not defined"
#endif

#if L2CAP_MAX_CHANNELS < 2
	#error "L2CAP_MAX_CHANNELS must be >= 2 if Security Manager is used"
#endif

#if L2CAP_MAX_FIXED_CHANNELS < 2
	#error "L2CAP_MAX_FIXED_CHANNELS must be >= 2 if Security Manager is used"
#endif

#define SMP_ALLOCATE_BUFFERS_VARS()	\
	bt_smp_session_t                   _smp_sessions[SMP_MAX_CLIENTS];	\
	bt_sm_session_t                    _sm_sessions[SMP_MAX_CLIENTS];	\
	bt_byte                            _smp_max_sessions = SMP_MAX_CLIENTS; \
	\
	SMP_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\

#define SMP_ALLOCATE_BUFFERS_FUNCTION()	\
	void _bt_smp_allocate_buffers(void)	\
	{	\
		_phci_ctrl->le_ctrl_state->sm = bt_sm_get_mgr();	\
		_phci_ctrl->le_ctrl_state->sm_long_term_key_request_handler = &_bt_sm_long_term_key_request_handler;	\
		_phci_ctrl->le_ctrl_state->sm_get_session_state = &bt_sm_get_session_state;	\
		_phci_ctrl->le_ctrl_state->sm_get_session_key_size = &bt_sm_get_session_key_size;	\
		_phci_ctrl->le_ctrl_state->sm_authenticate = &bt_sm_authenticate;	\
	} \

#define SMP_ALLOCATE_BUFFERS()	\
	SMP_ALLOCATE_BUFFERS_VARS()	\
	SMP_ALLOCATE_BUFFERS_FUNCTION()	\
	typedef int SMP_BUFFERS_ALLOCATED

#endif // __SMP_CONFIG_H
