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

#ifndef __HFP_CONFIG_H
#define __HFP_CONFIG_H

#ifndef HFP_MAX_SESSIONS
#error "HFP_MAX_SESSIONS is not defined"
#endif

#ifndef HFP_AG_MAX_SEARCH_HF_RESULTS
	#define HFP_AG_MAX_SEARCH_HF_RESULTS    5
#endif

#ifndef HFP_AG_MAX_HF_NAME_LEN
	#define HFP_AG_MAX_HF_NAME_LEN          32
#endif

#ifndef HFP_MAX_CODECS
	#define HFP_MAX_CODECS                  2
#endif

#ifdef _DEBUG

#define HFP_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\
	const bt_uint _ram_size_hfp_buffers =	\
		sizeof(_hfp_sessions) +	\
		sizeof(_hfp_max_sessions) +	\
		sizeof(_hfp_ag_max_search_hf_results) +	\
		sizeof(_hfp_hf_buffers) + \
		sizeof(_hfp_ag_max_hf_name_len) +	\
		sizeof(_hfp_ag_hf_name_buffers) +	\
		sizeof(_hfp_codecs) +	\
		sizeof(_hfp_max_codecs);

#else
	#define HFP_ALLOCATE_BUFFERS_RAM_SIZE_VAR
#endif

#define HFP_ALLOCATE_BUFFERS_VARS()	\
	bt_hfp_session    _hfp_sessions[HFP_MAX_SESSIONS];	\
	bt_byte           _hfp_max_sessions = HFP_MAX_SESSIONS;	\
	bt_byte           _hfp_ag_max_search_hf_results = HFP_AG_MAX_SEARCH_HF_RESULTS;	\
	bt_hfp_hf_t       _hfp_hf_buffers[(HFP_AG_MAX_SEARCH_HF_RESULTS) * (HFP_MAX_SESSIONS)];	\
	bt_byte           _hfp_ag_max_hf_name_len = HFP_AG_MAX_HF_NAME_LEN;	\
	bt_byte           _hfp_ag_hf_name_buffers[(HFP_AG_MAX_SEARCH_HF_RESULTS) * (HFP_MAX_SESSIONS) * (HFP_AG_MAX_HF_NAME_LEN + 1)];	\
	bt_uint           _hfp_codecs[HFP_MAX_CODECS * HFP_MAX_SESSIONS];	\
	bt_uint           _hfp_max_codecs = HFP_MAX_CODECS;	\
	HFP_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\

#define HFP_ALLOCATE_BUFFERS()	\
	HFP_ALLOCATE_BUFFERS_VARS()	\
	typedef int HFP_BUFFERS_ALLOCATED \

#endif // __HFP_CONFIG_H
