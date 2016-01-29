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

#ifndef __HID_PRIVATE_H
#define __HID_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

extern bt_hid_session_t     _hid_sessions[];
extern const bt_byte        _hid_max_sessions;
extern bt_hid_report_t      _hid_reports[];
extern const bt_byte        _hid_max_reports;
extern bt_buffer_header_t   _hid_pdu_buffer_headers[];
extern bt_hid_pdu_t         _hid_pdu_buffers[];
extern const bt_byte        _hid_max_pdu_buffers;

#ifdef _DEBUG
extern const bt_uint _ram_size_hid_buffers;
#endif

// Defined by OEM through library configuration
void _hid_allocate_buffers(void);

void hid_init_sessions(void);
bt_hid_session_p hid_find_session(bt_l2cap_channel_t* pch);

bt_hid_report_t* hid_find_report(bt_hid_session_p psess, bt_byte type, bt_byte id);

bt_bool hid_send_handshake(bt_hid_session_p psess, bt_byte code);

bt_hid_pdu_p _hid_alloc_pdu_buffer(void);
void _hid_free_pdu_buffer(bt_hid_pdu_p pdu);
void _hid_send_pdu(bt_hid_session_p psess, bt_hid_pdu_p pdu);

void _hid_handle_handshake(bt_hid_session_p psess, bt_byte res);
void _hid_handle_control(bt_hid_session_p psess, bt_byte op);
void _hid_handle_get_report(bt_hid_session_p psess, bt_byte param, bt_byte_p pdata, bt_int len, bt_int_p poffset);
void _hid_handle_set_report(bt_hid_session_p psess, bt_byte param,  bt_byte_p pdata, bt_int len, bt_int_p poffset);
void _hid_handle_get_protocol(bt_hid_session_p psess);
void _hid_handle_set_protocol(bt_hid_session_p psess, bt_byte protocol);
void _hid_handle_get_idle(bt_hid_session_p psess);
void _hid_handle_set_idle(bt_hid_session_p psess, bt_byte_p pdata, bt_int len, bt_int_p poffset);
void _hid_handle_data(bt_hid_session_p psess, bt_byte type, bt_byte_p pdata, bt_int len, bt_int_p poffset);
void _hid_handle_datc(bt_hid_session_p psess, bt_byte type, bt_byte_p pdata, bt_int len, bt_int_p poffset);

#ifdef __cplusplus
}
#endif

#endif // __HID_PRIVATE_H
