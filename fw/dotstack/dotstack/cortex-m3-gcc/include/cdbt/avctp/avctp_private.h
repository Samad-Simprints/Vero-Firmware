/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __AVCTP_PRIVATE_H
#define __AVCTP_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

extern const bt_byte                      _avctp_max_channels;
extern bt_avctp_channel_t                 _avctp_channels[];
extern const bt_uint                      _avctp_max_rx_message_len;
extern bt_byte                            _avctp_rx_buffers[];
extern bt_buffer_header_t                 _avctp_message_buffer_headers[];
extern bt_avctp_message_t                 _avctp_message_buffers[];
extern const bt_byte                      _avctp_max_message_buffers;
extern bt_avctp_transport_t               _avctp_transports[];
extern const bt_byte                      _avctp_max_transports;

#ifdef _DEBUG
extern const bt_uint _ram_size_avctp_buffers;
#endif

void _bt_avctp_init_message_buffers(void);
void _bt_avctp_init_signal(void);
void _bt_avctp_set_signal(void);
void _bt_avrcp_init_cmd_buffers(void);

void _bt_avctp_l2cap_read_data_callback(struct _bt_l2cap_channel_t *pch, bt_byte* pdata, bt_int len);
bt_avctp_channel_t* _bt_avctp_find_channel(
	bt_avctp_mgr_t* mgr, bt_bdaddr_t* remote_addr, bt_uint profile_id, bt_int psm);

bt_avctp_channel_t* _bt_avctp_allocate_channel(bt_avctp_mgr_t* mgr);
void _bt_avctp_free_channel(bt_avctp_channel_t* channel);

bt_avctp_message_t* _bt_avctp_allocate_message(bt_byte msg_type);
void _bt_avctp_free_message(bt_avctp_message_t* message);

bt_bool _bt_avctp_send_ipid(bt_avctp_transport_t* transport, bt_byte tran_id);

bt_avctp_transport_t* _bt_avctp_allocate_transport(bt_avctp_mgr_t* mgr);
void _bt_avctp_free_transport(bt_avctp_transport_t* transport);
bt_avctp_transport_t* _bt_avctp_find_transport(bt_avctp_mgr_t* mgr, bt_l2cap_channel_t* l2cap_channel);

#ifdef __cplusplus
}
#endif

#endif // __AVCTP_PRIVATE_H
