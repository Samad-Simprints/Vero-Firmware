/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __AVRCP_PRIVATE_H
#define __AVRCP_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

#define AVRCP_MAX_ELEMENT_ATTRIBUTES    7

extern const bt_byte       _avrcp_max_channels;
extern bt_avrcp_channel_t  _avrcp_channels[];
extern bt_buffer_header_t  _avrcp_cmd_buffer_headers[];
extern bt_av_command_t     _avrcp_cmd_buffers[];
extern const bt_byte       _avrcp_max_cmd_buffers;
extern bt_byte             _avrcp_cmd_param_buffers[];
extern const bt_int        _avrcp_max_cmd_param_len;
extern const bt_byte       _avrcp_max_search_results;
extern bt_avrcp_device_t   _avrcp_devices_buffer[];
extern const bt_byte       _avrcp_max_device_name_len;
extern bt_byte             _avrcp_device_name_buffers[];
extern const bt_byte       _avrcp_cmd_timeout;

extern void (*_bt_avrcp_response_sent_handler)(bt_avrcp_mgr_t* mgr, bt_avctp_evt_response_sent_t* evt_param);
extern void (*_bt_avrcp_command_handler)(bt_avrcp_mgr_t* mgr, bt_avctp_evt_command_received_t* evt_param);
extern void (*_bt_avrcp_command_sent_handler)(bt_avrcp_mgr_t* mgr, bt_avctp_evt_command_sent_t* evt_param);
extern void (*_bt_avrcp_response_handler)(bt_avrcp_mgr_t* mgr, bt_avctp_evt_response_received_t* evt_param);

#ifdef _DEBUG
extern const bt_uint _ram_size_avrcp_buffers;
#endif

//void _bt_avctp_init_message_buffers();
void _bt_avrcp_init_signal(void);
void _bt_avrcp_set_signal(void);

/*
void _bt_avctp_l2cap_read_data_callback(struct _channel *pch, bt_byte* pdata, bt_int len);
bt_avctp_channel_t* _bt_avctp_find_channel(bt_avctp_mgr_t* mgr, bt_bdaddr_t* remote_addr, bt_uint profile_id);
*/

bt_avrcp_channel_t* _bt_avrcp_allocate_channel(bt_avrcp_mgr_t* mgr);
void _bt_avrcp_free_channel(bt_avrcp_channel_t* channel);
bt_avrcp_channel_t* _bt_avrcp_find_channel(bt_avrcp_mgr_t* mgr, bt_avctp_channel_t* avctp_channel);

bt_av_command_t* _bt_avrcp_allocate_cmd(void);
bt_av_command_t* _bt_avrcp_allocate_simple_panel_cmd(bt_byte ctype, bt_byte opid, bt_byte button_state);
bt_av_command_t* _bt_avrcp_allocate_bt_specific_cmd(bt_byte ctype, bt_byte pduid);
bt_av_command_t* _bt_avrcp_allocate_browsing_cmd(bt_byte pduid);
void _bt_avrcp_free_cmd(bt_av_command_t* command);
bt_int _bt_avrcp_write_command_header(bt_av_command_t* command);
bt_av_command_t* _bt_avrcp_allocate_response(bt_byte tran_id);
bt_av_command_t* _bt_avrcp_allocate_simple_panel_response(bt_byte ctype, bt_byte tran_id, bt_byte opid, bt_byte button_state);
bt_av_command_t* _bt_avrcp_allocate_bt_specific_response(bt_byte ctype, bt_byte tran_id, bt_byte pduid);
bt_av_command_t* _bt_avrcp_allocate_browsing_response(bt_byte tran_id, bt_byte pduid);

void _bt_avrcp_init_timer(void);
void _bt_avrcp_start_timer(void);
bt_long _bt_avrcp_get_tick_count(void);

void _bt_avrcp_register_next_notification(bt_avrcp_channel_t* channel);
void _bt_avrcp_register_pending_notification(bt_avrcp_channel_t* channel);

void _bt_avrcp_handle_get_capabilities(bt_avrcp_channel_t* channel, bt_byte ctype, bt_byte* data, bt_uint len);
void _bt_avrcp_handle_list_player_application_setting_attributes(bt_avrcp_channel_t* channel, bt_byte ctype, bt_byte* data, bt_uint len, void* response_buffer);
void _bt_avrcp_handle_list_player_application_setting_values(bt_avrcp_channel_t* channel, bt_byte ctype, bt_byte* data, bt_uint len, void* response_buffer);
void _bt_avrcp_handle_get_current_player_application_setting_value(bt_avrcp_channel_t* channel, bt_byte ctype, bt_byte* data, bt_uint len, void* response_buffer);
void _bt_avrcp_handle_set_player_application_setting_value(bt_avrcp_channel_t* channel, bt_byte ctype, bt_byte* data, bt_uint len, void* response_buffer);
void _bt_avrcp_handle_get_player_application_setting_attribute_text(bt_avrcp_channel_t* channel, bt_byte ctype, bt_byte* data, bt_uint len, void* response_buffer);
void _bt_avrcp_handle_get_player_application_setting_value_text(bt_avrcp_channel_t* channel, bt_byte ctype, bt_byte* data, bt_uint len, void* response_buffer);
void _bt_avrcp_handle_inform_displayable_character_set(bt_avrcp_channel_t* channel, bt_byte ctype, bt_byte* data, bt_uint len);
void _bt_avrcp_handle_inform_battery_status_of_ct(bt_avrcp_channel_t* channel, bt_byte ctype, bt_byte* data, bt_uint len);
void _bt_avrcp_handle_get_element_attributes(bt_avrcp_channel_t* channel, bt_byte ctype, bt_byte* data, bt_uint len);
void _bt_avrcp_handle_get_play_status(bt_avrcp_channel_t* channel, bt_byte ctype, bt_byte* data, bt_uint len);
void _bt_avrcp_handle_register_notification(bt_avrcp_channel_t* channel, bt_byte ctype, bt_byte event_id, bt_byte* data, bt_uint len);
void _bt_avrcp_handle_set_absolute_volume(bt_avrcp_channel_t* channel, bt_byte ctype, bt_byte* data, bt_uint len);
void _bt_avrcp_handle_set_addressed_player(bt_avrcp_channel_t* channel, bt_byte ctype, bt_byte* data, bt_uint len);
void _bt_avrcp_handle_play_item(bt_avrcp_channel_t* channel, bt_byte ctype, bt_byte* data, bt_uint len);
void _bt_avrcp_handle_add_to_now_playing(bt_avrcp_channel_t* channel, bt_byte ctype, bt_byte* data, bt_uint len);
void _bt_avrcp_handle_request_continuing_response(bt_avrcp_channel_t* channel, bt_byte ctype, bt_byte* data, bt_uint len);

void _bt_avrcp_tg_handle_get_capabilities(bt_avrcp_channel_t* channel, bt_byte ctype, bt_byte tran_id, bt_byte* data, bt_uint len);
void _bt_avrcp_tg_handle_get_play_status(bt_avrcp_channel_t* channel, bt_byte ctype, bt_byte tran_id, bt_byte* data, bt_uint len);
void _bt_avrcp_tg_handle_set_absolute_volume(bt_avrcp_channel_t* channel, bt_byte ctype, bt_byte tran_id, bt_byte* data, bt_uint len);
void _bt_avrcp_tg_handle_inform_battery_status_of_ct(bt_avrcp_channel_t* channel, bt_byte ctype, bt_byte tran_id, bt_byte* data, bt_uint len);
void _bt_avrcp_tg_handle_inform_displayable_character_set(bt_avrcp_channel_t* channel, bt_byte ctype, bt_byte tran_id, bt_byte* data, bt_uint len);
void _bt_avrcp_tg_handle_get_element_attributes(bt_avrcp_channel_t* channel, bt_byte ctype, bt_byte tran_id, bt_byte* data, bt_uint len);
void _bt_avrcp_tg_handle_register_notification(bt_avrcp_channel_t* channel, bt_byte ctype, bt_byte tran_id, bt_byte* data, bt_uint len);
void _bt_avrcp_send_notifications(bt_avrcp_channel_t* channel);

void _bt_avrcp_handle_command_sent(bt_avrcp_mgr_t* mgr, bt_avctp_evt_command_sent_t* evt_param);
void _bt_avrcp_handle_response_sent(bt_avrcp_mgr_t* mgr, bt_avctp_evt_response_sent_t* evt_param);
void _bt_avrcp_handle_response(bt_avrcp_mgr_t* mgr, bt_avctp_evt_response_received_t* evt_param);
void _bt_avrcp_handle_command(bt_avrcp_mgr_t* mgr, bt_avctp_evt_command_received_t* evt_param);

bt_bool bt_avrcp_avrcpt_request_continuing_response(bt_avrcp_channel_t* channel, bt_byte pduid);
bt_bool bt_avrcp_abort_continuing_response(bt_avrcp_channel_t* channel, bt_byte pduid);

void _bt_avrcp_send_rejected_response(bt_avrcp_channel_t* channel, bt_byte tran_id, bt_byte pdu_id);

bt_bool _bt_avrcp_general_reject(bt_avrcp_channel_t* channel, bt_byte reason, bt_byte tran_id);

/*bt_avctp_message_t* _bt_avctp_allocate_message(bt_byte msg_type);
void _bt_avctp_free_message(bt_avctp_message_t* message);

bt_bool _bt_avctp_send_ipid(bt_avctp_transport_t* transport, bt_byte tran_id);

bt_avctp_transport_t* _bt_avctp_allocate_transport(bt_avctp_mgr_t* mgr);
void _bt_avctp_free_transport(bt_avctp_transport_t* transport);
bt_avctp_transport_t* _bt_avctp_find_transport(bt_avctp_mgr_t* mgr, bt_l2cap_channel* l2cap_channel);
*/

#ifdef __cplusplus
}
#endif

#endif // __AVRCP_PRIVATE_H
