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

#ifndef __OBEX_CONFIG_H
#define __OBEX_CONFIG_H

#ifndef OBEX_MAX_SESSIONS
#error "OBEX_MAX_SESSIONS is not defined"
#endif

#ifndef OBEX_MAX_PACKET_LENGTH
#error "OBEX_MAX_PACKET_LENGTH is not defined"
#endif

#if OBEX_MAX_PACKET_LENGTH < 255
//#error "OBEX_MAX_PACKET_LENGTH must be at least 255"
#endif

#if RFCOMM_INFO_LEN < OBEX_MIN_PACKET_LENGTH
//#error "RFCOMM_INFO_LEN must be at least 256"
#endif

#if OBEX_MAX_PACKET_LENGTH + 1 > RFCOMM_INFO_LEN
#error "OBEX_MAX_PACKET_LENGTH + 1 cannot exceed RFCOMM_INFO_LEN"
#endif

#ifdef _DEBUG

#define OBEX_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\
	const bt_uint _ram_size_obex_buffers =	\
		sizeof(_obex_sessions) +	\
		sizeof(_obex_max_sessions) +	\
		sizeof(_obex_rx_packet_buffers) +	\
		sizeof(_obex_max_packet_length) +	\
		sizeof(_obex_rx_tmp_buffers) +	\
		sizeof(_obex_tx_packet_buffers);

#else
	#define OBEX_ALLOCATE_BUFFERS_RAM_SIZE_VAR
#endif

#define OBEX_ALLOCATE_BUFFERS_VARS()	\
	bt_obex_session   _obex_sessions[OBEX_MAX_SESSIONS];	\
	bt_byte           _obex_max_sessions = OBEX_MAX_SESSIONS;	\
	bt_byte           _obex_rx_packet_buffers[OBEX_MAX_PACKET_LENGTH * OBEX_MAX_SESSIONS];	\
	bt_uint           _obex_max_packet_length = OBEX_MAX_PACKET_LENGTH;	\
	bt_byte           _obex_rx_tmp_buffers[(OBEX_MAX_PACKET_LENGTH + OBEX_PACKET_HEADER_LEN) * OBEX_MAX_SESSIONS];	\
	bt_byte           _obex_tx_packet_buffers[OBEX_MAX_PACKET_LENGTH * OBEX_MAX_SESSIONS];	\
	OBEX_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\

#define OBEX_ALLOCATE_BUFFERS()	\
	OBEX_ALLOCATE_BUFFERS_VARS()	\
	typedef int OBEX_BUFFERS_ALLOCATED \

#endif // __OBEX_CONFIG_H
