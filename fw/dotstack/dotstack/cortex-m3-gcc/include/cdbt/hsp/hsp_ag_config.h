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

#ifndef __HSP_AG_CONFIG_H
#define __HSP_AG_CONFIG_H

#ifndef SPP_MAX_PORTS
#error "SPP_MAX_PORTS is not defined"
#endif

#ifndef HSP_MAX_GATEWAYS
#error "HSP_MAX_GATEWAYS is not defined"
#endif

#if HSP_MAX_GATEWAYS < 1
#error "HSP_MAX_GATEWAYS >= 1 required"
#endif

#ifndef HSP_AG_MAX_SEARCH_HEADSET_RESULTS
#error "HSP_AG_MAX_SEARCH_HEADSET_RESULTS is not defined"
#endif

#ifndef HSP_AG_MAX_HEADSET_NAME_LEN
#error "HSP_AG_MAX_HEADSET_NAME_LEN is not defined"
#endif

#ifndef HSP_AG_MAX_CMD_BUFFERS
#define HSP_AG_MAX_CMD_BUFFERS	(HSP_MAX_GATEWAYS * 2)
#endif

#if HSP_AG_MAX_CMD_BUFFERS < HSP_MAX_GATEWAYS * 2
#error "HSP_AG_MAX_CMD_BUFFERS >= HSP_MAX_GATEWAYS * 2 required"
#endif

#ifdef _DEBUG

#define HSP_AG_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\
	const bt_uint _ram_size_hsp_ag_buffers =	\
		sizeof(_hsp_gateways) +	\
		sizeof(_hsp_max_gateways) +	\
		sizeof(_hsp_ag_cmd_buffer_headers) +	\
		sizeof(_hsp_ag_cmd_buffers) +	\
		sizeof(_hsp_ag_max_cmd_buffers) +	\
		sizeof(_hsp_headsets_buffer) +	\
		sizeof(_hsp_ag_max_search_headset_results) +	\
		sizeof(_hsp_ag_max_headset_name_len) +	\
		sizeof(_hsp_ag_headset_name_buffers);

#else
	#define HSP_AG_ALLOCATE_BUFFERS_RAM_SIZE_VAR
#endif

#define HSP_AG_ALLOCATE_BUFFERS_VARS()	\
	bt_hsp_gateway_t     _hsp_gateways[HSP_MAX_GATEWAYS];	\
	bt_byte              _hsp_max_gateways = HSP_MAX_GATEWAYS;	\
	bt_buffer_header_t   _hsp_ag_cmd_buffer_headers[(HSP_AG_MAX_CMD_BUFFERS)];	\
	bt_hsp_ag_command_t  _hsp_ag_cmd_buffers[(HSP_AG_MAX_CMD_BUFFERS)];	\
	bt_byte              _hsp_ag_max_cmd_buffers = HSP_AG_MAX_CMD_BUFFERS; \
	bt_byte              _hsp_ag_max_search_headset_results = HSP_AG_MAX_SEARCH_HEADSET_RESULTS;	\
	bt_hsp_headset_t     _hsp_headsets_buffer[HSP_AG_MAX_SEARCH_HEADSET_RESULTS * HSP_MAX_GATEWAYS];	\
	bt_byte              _hsp_ag_max_headset_name_len = HSP_AG_MAX_HEADSET_NAME_LEN;	\
	bt_byte              _hsp_ag_headset_name_buffers[HSP_AG_MAX_SEARCH_HEADSET_RESULTS * HSP_MAX_GATEWAYS * (HSP_AG_MAX_HEADSET_NAME_LEN + 1)];	\
	HSP_AG_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\

#define HSP_AG_ALLOCATE_BUFFERS()	\
	HSP_AG_ALLOCATE_BUFFERS_VARS()	\
	typedef int HSP_AG_BUFFERS_ALLOCATED \

#endif // __HSP_AG_CONFIG_H
