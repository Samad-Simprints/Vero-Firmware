/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __IAPEA_CONFIG_H
#define __IAPEA_CONFIG_H

#ifdef _DEBUG

#define IAPEA_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\
	const bt_uint _ram_size_iapea_buffers =	\
		sizeof(_iapea_max_protocols) +	\
		sizeof(_iapea_protocols) + \
		sizeof(_iapea_iap1_tokens);

#else
#define IAPEA_ALLOCATE_BUFFERS_RAM_SIZE_VAR
#endif

#ifndef IAPEA_MAX_SESSIONS
	#error "IAPEA_MAX_SESSIONS is not defined"
#endif

#ifndef IAPEA_MAX_PACKET_SIZE
	#error "IAPEA_MAX_PACKET_SIZE is not defined"
#endif

#ifndef IAPEA_MAX_PROTOCOLS
	#define IAPEA_MAX_PROTOCOLS       1
#endif

#define IAPEA_ALLOCATE_BUFFERS_VARS()	\
	iapea_accessory_t             _iapea_sessions[IAPEA_MAX_SESSIONS];	\
	const bt_byte                 _iapea_max_sessions = IAPEA_MAX_SESSIONS;	\
	const bt_byte                 _iapea_max_protocols = IAPEA_MAX_PROTOCOLS;	\
	iap_fid_token_sdk_protocol_t  _iapea_protocols[IAPEA_MAX_PROTOCOLS];	\
	const iap_fid_token_header_t* _iapea_iap1_tokens[10 +  IAPEA_MAX_PROTOCOLS];	\
	const bt_uint                 _iapea_max_packet_size = IAPEA_MAX_PACKET_SIZE;	\
	IAPEA_ALLOCATE_BUFFERS_RAM_SIZE_VAR

#define IAPEA_ALLOCATE_BUFFERS()	\
	IAPEA_ALLOCATE_BUFFERS_VARS()	\
	typedef int IAPEA_BUFFERS_ALLOCATED

#endif // __IAPEA_CONFIG_H
