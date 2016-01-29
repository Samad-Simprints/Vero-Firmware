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

#ifndef __HSP_AG_PRIVATE_H
#define __HSP_AG_PRIVATE_H


#ifdef __cplusplus
extern "C" {
#endif

extern bt_hsp_gateway_t    _hsp_gateways[];
extern bt_byte             _hsp_max_gateways;
extern bt_byte             _hsp_max_headsets;
extern bt_buffer_header_t    _hsp_ag_cmd_buffer_headers[];
extern bt_hsp_ag_command_t _hsp_ag_cmd_buffers[];
extern bt_byte             _hsp_ag_max_cmd_buffers;
extern bt_byte             _hsp_ag_max_search_headset_results;
extern bt_hsp_headset_t    _hsp_headsets_buffer[];
extern bt_byte             _hsp_ag_max_headset_name_len;
extern bt_byte             _hsp_ag_headset_name_buffers[];

#ifdef _DEBUG
extern const bt_uint _ram_size_hsp_ag_buffers;
#endif

extern const bt_uuid_t HSP_HS_SERVICE_CLSID;

void _bt_hsp_init_gateways(void);

void _bt_hsp_ag_spp_state_callback(bt_spp_port_t* port, bt_spp_port_event_e evt, void* param);
void _bt_hsp_ag_init_timer(void);
void _bt_hsp_ag_spp_receive_callback(bt_spp_port_t* port, bt_int bytes_received, void* param);
void _bt_hsp_ag_execute_action(bt_hsp_gateway_t* gw);
bt_bool _bt_hsp_ag_connect_audio(bt_hsp_gateway_t* gw);
bt_bool _bt_hsp_ag_disconnect_audio(bt_hsp_gateway_t* gw);
bt_bool _bt_hsp_ag_control(bt_hsp_gateway_t* gw, bt_byte action);
void _bt_hsp_ag_send_response(bt_hsp_gateway_t* gw, const bt_byte* response, bt_byte len);
void _bt_hsp_ag_sco_read_data_callback(bt_hci_conn_state_t* pconn, bt_byte_p data, bt_byte len);

void _bt_hsp_ag_init_cmd_buffers(void);
bt_hsp_ag_command_t* _bt_hsp_ag_allocate_cmd(void);
void _bt_hsp_ag_free_cmd(bt_hsp_ag_command_t* command);

void _bt_hsp_ag_send_cmd(bt_hsp_gateway_t* gw, bt_hsp_ag_command_t* command);
void _bt_hsp_ag_send_command_from_queue(bt_hsp_gateway_t* gw);

void _bt_hsp_ag_init_signal(void);
void _bt_hsp_ag_set_signal(void);

bt_ulong _bt_hsp_ag_get_tick_count(void);


#ifdef __cplusplus
}
#endif

#endif // __HSP_AG_PRIVATE_H
