/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __AVDTP_PRIVATE_H
#define __AVDTP_PRIVATE_H

#define AVDTP_TC_OPCODE_CONNECT       0
#define AVDTP_TC_OPCODE_DISCONNECT    1

#ifdef __cplusplus
extern "C" {
#endif
	
#define AVDTP_TC_STATUS_SUCCESS           0x00
#define AVDTP_TC_STATUS_ERROR             0xFF

extern bt_avdtp_sep_t                     _avdtp_seps[];
extern const bt_byte                      _avdtp_max_seps;
extern bt_avdtp_stream_t                  _avdtp_streams[];
extern const bt_byte                      _avdtp_max_streams;
extern bt_avdtp_control_channel_t         _avdtp_control_channels[];
extern const bt_byte                      _avdtp_max_control_channels;
extern bt_buffer_header_t                 _avdtp_cmd_buffer_headers[];
extern bt_avdtp_control_cmd_t             _avdtp_cmd_buffers[];
extern const bt_byte                      _avdtp_max_cmd_buffers;
extern bt_byte                            _avdtp_cmd_param_buffers[];
extern const bt_uint                      _avdtp_max_cmd_param_len;
extern bt_avdtp_transport_channel_t       _avdtp_transport_channels[];
extern const bt_byte                      _avdtp_max_transport_channels;
extern const bt_byte                      _avdtp_max_tx_buffer_len;
extern bt_byte                            _avdtp_tx_buffers[];
extern bt_byte                            _avdtp_codec_cfg_buffers[];
extern const bt_byte                      _avdtp_max_codec_config_buffer_len;
extern bt_buffer_header_t                 _avdtp_sep_cfg_buffer_headers[];
extern bt_avdtp_sep_capabilities_t        _avdtp_sep_cfg_buffers[];
extern bt_byte                            _avdtp_listen_sep_buffers[];
extern bt_avdtp_sep_capabilities_t        _avdtp_rcv_sep_caps;
extern bt_byte                            _avdtp_rcv_sep_codec_cfg_buffer[];
extern bt_byte                            _avdtp_l2cap_media_packet_buffer[];

#ifdef _DEBUG
extern const bt_uint _ram_size_avdtp_buffers;
#endif

typedef void (*bt_avdtp_transport_op_callback_fp)(bt_byte status, bt_byte opcode, void* param);

void _bt_avdtp_allocate_buffers(void);
bt_avdtp_sep_t* _bt_avdtp_allocate_sep(void);
bt_bool _bt_avdtp_is_sep_inuse(bt_avdtp_mgr_t* mgr, bt_avdtp_sep_t* sep, bt_bdaddr_t* remote_addr);
bt_avdtp_stream_t* _bt_avdtp_allocate_stream(bt_avdtp_mgr_t* mgr);
void _bt_avdtp_free_stream(bt_avdtp_stream_t* strm);
bt_avdtp_stream_t* _bt_avdtp_find_stream(bt_avdtp_mgr_t* mgr, bt_byte strm_handle);
bt_avdtp_stream_t* _bt_avdtp_find_stream_by_remote_sep_id(bt_avdtp_mgr_t* mgr, bt_bdaddr_t* remote_addr, bt_byte remote_seid);
bt_avdtp_stream_t* _bt_avdtp_find_stream_by_sep_id(bt_avdtp_mgr_t* mgr, bt_bdaddr_t* remote_addr, bt_byte sep_id);
bt_avdtp_stream_t* _bt_avdtp_find_listening_stream(bt_avdtp_mgr_t* mgr, bt_byte sep_id);

bt_byte _bt_avdtp_allocate_transport_session_id(bt_avdtp_mgr_t* mgr, bt_byte ts_id);
bt_avdtp_transport_channel_t* _bt_avdtp_allocate_transport_channel(
	bt_avdtp_mgr_t* mgr, bt_byte strm_handle, bt_byte tc_id, bt_byte tc_type);
void _bt_avdtp_free_transport_channel(bt_avdtp_transport_channel_t* channel);

void _bt_avdtp_init_cmd_buffers(void);
bt_avdtp_control_cmd_t* _bt_avdtp_allocate_cmd(bt_byte msg_type);
void _bt_avdtp_free_cmd(bt_avdtp_control_cmd_t* command);
void _bt_avdtp_send_command(bt_avdtp_control_channel_t* channel, bt_avdtp_control_cmd_t* cmd);

void _bt_avdtp_control_channel_cmd_handler(
	bt_avdtp_control_channel_t* channel, 
	bt_byte cmd_code, bt_byte trans_id, bt_byte* data, bt_int len, bt_int* offset);
void _bt_avdtp_control_channel_reject_handler(
	bt_avdtp_control_channel_t* channel, 
	bt_byte cmd_code, bt_byte trans_id, bt_byte* data, bt_int len, bt_int* offset);
void _bt_avdtp_control_channel_accept_handler(
	bt_avdtp_control_channel_t* channel, 
	bt_byte cmd_code, bt_byte trans_id, bt_byte* data, bt_int len, bt_int* offset);

bt_bool _bt_avdtp_open_control_channel_ex(bt_avdtp_mgr_t* mgr, bt_bdaddr_t* remote_addr, bt_uint acl_config);

void _bt_avdtp_init_signal(void);
void _bt_avdtp_set_signal(void);

bt_bool _bt_avdtp_write_caps(
	bt_avdtp_mgr_t* mgr,
	const bt_avdtp_sep_capabilities_t* caps,
	bt_avdtp_stream_t* strm,
	bt_avdtp_control_cmd_t* command);

bt_byte _bt_avdtp_read_caps(
	bt_avdtp_mgr_t* mgr,
	bt_avdtp_sep_capabilities_t* caps,
	bt_byte* data, bt_int len, bt_int* offset,
	bt_byte* err_category);

void _bt_avdtp_init_sep_config_buffers(void);
bt_avdtp_sep_capabilities_t* _bt_avdtp_allocate_sep_config(void);
void _bt_avdtp_free_sep_config(bt_avdtp_sep_capabilities_t* sep_config);

void _bt_avdtp_transport_l2cap_read_data_callback(struct _bt_l2cap_channel_t *pch, bt_byte_p pdata, bt_int len);
void _bt_avdtp_transport_l2cap_state_changed_callback(bt_l2cap_channel_t* pch, bt_int new_state, void* param);

void _bt_avdtp_begin_tc_channel_operation(bt_byte opcode, bt_avdtp_transport_op_callback_fp callback, void* callback_param);
void _bt_avdtp_commit_tc_channel_operation(void);
void _bt_avdtp_register_transport_channel_for_operation(bt_avdtp_transport_channel_t* channel);
void _bt_avdtp_execute_tc_channel_operation(void);

bt_bool _bt_avdtp_send_media_packet(bt_avdtp_stream_t* strm);

#ifdef __cplusplus
}
#endif

#endif // __AVDTP_PRIVATE_H
