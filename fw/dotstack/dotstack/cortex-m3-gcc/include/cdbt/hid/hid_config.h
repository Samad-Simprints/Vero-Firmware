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

#ifndef __HID_CONFIG_H
#define __HID_CONFIG_H

#ifndef HID_MAX_SESSIONS
#error "HID_MAX_SESSIONS is not defined"
#endif

#ifndef HID_MAX_REPORTS
#error "HID_MAX_REPORTS is not defined"
#endif

#ifndef HID_MAX_PDU_BUFFERS
#error "HID_MAX_PDU_BUFFERS is not defined"
#endif

#ifdef _DEBUG

#define HID_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\
	const bt_uint _ram_size_hid_buffers =	\
		sizeof(bt_buffer_mgr_t) +	\
		sizeof(_hid_sessions) +	\
		sizeof(_hid_max_sessions) +	\
		sizeof(_hid_reports) +	\
		sizeof(_hid_max_reports) +	\
		sizeof(_hid_pdu_buffer_headers) +	\
		sizeof(_hid_pdu_buffers) +	\
		sizeof(_hid_max_pdu_buffers);

#else
	#define HID_ALLOCATE_BUFFERS_RAM_SIZE_VAR
#endif

#define HID_INIT_FRAME_LEN		(HID_HEADER_LEN + HID_PAYLOAD_LEN)
#define HID_INIT_LARGE_FRAME_LEN	(HCI_L2CAP_BUFFER_LEN - L2CAP_HEADER_LEN)
#define HID_INIT_LARGE_PAYLOAD_LEN	(HID_INIT_LARGE_FRAME_LEN - HID_HEADER_LEN)

#define HID_DATA_BUFFER_SIZE        (HID_INIT_FRAME_LEN)
#define HID_LARGE_DATA_BUFFER_SIZE  (HID_INIT_LARGE_FRAME_LEN)

#define HID_ALLOCATE_BUFFERS_VARS()	\
	bt_hid_session_t     _hid_sessions[HID_MAX_SESSIONS];	\
	const bt_byte        _hid_max_sessions = HID_MAX_SESSIONS;	\
	bt_hid_report_t      _hid_reports[HID_MAX_REPORTS * HID_MAX_SESSIONS];	\
	const bt_byte        _hid_max_reports = HID_MAX_REPORTS;	\
	bt_buffer_header_t   _hid_pdu_buffer_headers[HID_MAX_PDU_BUFFERS];	\
	bt_hid_pdu_t         _hid_pdu_buffers[HID_MAX_PDU_BUFFERS];	\
	const bt_byte        _hid_max_pdu_buffers = HID_MAX_PDU_BUFFERS;	\
	HID_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\


#define HID_ALLOCATE_BUFFERS_FUNCTION()	\
	void _hid_allocate_buffers()	\
	{	\
	}	\

#define HID_ALLOCATE_BUFFERS()	\
	HID_ALLOCATE_BUFFERS_VARS()	\
	HID_ALLOCATE_BUFFERS_FUNCTION()	\
	typedef int HID_BUFFERS_ALLOCATED \

#endif // __HID_CONFIG_H
