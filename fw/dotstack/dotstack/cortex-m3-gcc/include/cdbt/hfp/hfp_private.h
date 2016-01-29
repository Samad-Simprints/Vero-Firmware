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

#ifndef __HFP_PRIVATE_H
#define __HFP_PRIVATE_H

/**
* \defgroup hsp HSP
*/

#include "cdbt/sdp/sdp.h"

#ifdef __cplusplus
extern "C" {
#endif

extern bt_hfp_session    _hfp_sessions[];
extern bt_byte           _hfp_max_sessions;
extern bt_byte           _hfp_ag_max_search_hf_results;
extern bt_hfp_hf_t       _hfp_hf_buffers[];
extern bt_byte           _hfp_ag_max_hf_name_len;
extern bt_byte           _hfp_ag_hf_name_buffers[];
extern bt_uint           _hfp_codecs[];
extern bt_uint           _hfp_max_codecs;

#define HFP_RSP_NONE	0
#define HFP_RSP_OK		1
#define HFP_RSP_ERROR	2
#define HFP_RSP_RING	3

extern const bt_uuid_t HFP_AG_SERVICE_CLSID;
extern const bt_uuid_t HFP_HF_SERVICE_CLSID;

#ifdef _DEBUG
extern const bt_uint _ram_size_hfp_buffers;
#endif

void bt_hfp_init_sessions(void);
bt_hfp_session* bt_hfp_find_session(bt_rfcomm_dlc_t* rfcomm_dlc);
bt_hfp_session* bt_hfp_find_session_by_hconn(bt_hci_conn_state_t* hci_conn);

bt_bool _bt_hfp_listent_sco(bt_hfp_session* session);

void _bt_hfp_ag_init_timer(void);

void bt_hfp_hf_slc_setup(bt_hfp_session* session);
void bt_hfp_hf_handle_slc_setup(bt_hfp_session* session, bt_byte cmd_id, bt_byte res_type, bt_char* param);
void _bt_hfp_hf_update_indicators_mask(bt_hfp_session* session);

bt_bool bt_hfp_send_cmd(bt_hfp_session* session, bt_hfp_cmd_id_enum cmd_id, bt_byte cmd_type, const char* param, bt_int param_len);
bt_bool bt_hfp_send_cmd_param_ulong(bt_hfp_session* session, bt_hfp_cmd_id_enum cmd_id, bt_ulong param);
bt_hfp_cmd_id_enum bt_hfp_cmd_code_to_id(const bt_char* code, bt_byte len);

bt_bool bt_hfp_send_status(bt_hfp_session* session, bt_byte status_id);
bt_bool bt_hfp_send_rsp(bt_hfp_session* session, bt_hfp_cmd_id_enum cmd_id, bt_bool add_ok, const char* param, bt_int param_len);
bt_bool bt_hfp_send_rsp_param_ulong(bt_hfp_session* session, bt_hfp_cmd_id_enum cmd_id, bt_bool add_ok, bt_ulong param);

void bt_hfp_hf_handle_response(bt_hfp_session* session, bt_byte* code, bt_uint len);
void bt_hfp_ag_handle_command(bt_hfp_session* session, bt_byte* code, bt_uint len);

void bt_hfp_hf_handle_indicators(bt_hfp_session* session, bt_char* param);
void bt_hfp_hf_handle_query_operator(bt_hfp_session* session, bt_char* param);
void bt_hfp_hf_handle_clip(bt_hfp_session* session, bt_char* param);
void bt_hfp_hf_handle_vgm(bt_hfp_session* session, bt_char* param);
void bt_hfp_hf_handle_vgs(bt_hfp_session* session, bt_char* param);
void bt_hfp_hf_handle_ccwa(bt_hfp_session* session, bt_char* param);
void bt_hfp_hf_handle_clcc(bt_hfp_session* session, bt_char* param);
void bt_hfp_hf_handle_bvra(bt_hfp_session* session, bt_char* param);
void bt_hfp_hf_handle_bsir(bt_hfp_session* session, bt_char* param);
void bt_hfp_hf_handle_get_subscriber_number(bt_hfp_session* session, bt_char* param);
void bt_hfp_hf_handle_bcs(bt_hfp_session* session, bt_char* param);

bt_bool _bt_hfp_hf_send_codec_list(bt_hfp_session* session);

bt_byte bt_hfp_get_response_parameters(bt_char* params, bt_char** params_array, bt_byte params_array_len);
bt_char* bt_hfp_unquote_str(bt_char* str);

void _bt_hfp_ag_slc_setup(bt_hfp_session* session);
void _bt_hfp_ag_handle_slc_connected(bt_hfp_session* session);
void _bt_hfp_ag_handle_cind_read(bt_hfp_session* session);
void _bt_hfp_ag_handle_cind_test(bt_hfp_session* session);
void _bt_hfp_ag_handle_cmer_param(bt_hfp_session* session, bt_char* param);
void _bt_hfp_ag_handle_bia_param(bt_hfp_session* session, bt_char* param);
void _bt_hfp_ag_handle_cmer(bt_hfp_session* session);
void _bt_hfp_ag_handle_atd_param(bt_hfp_session* session, bt_char* param);
void _bt_hfp_ag_handle_atd(bt_hfp_session *session);
void _bt_hfp_ag_handle_bldn(bt_hfp_session *session);
bt_hfp_call_t* _bt_hfp_ag_allocate_call(bt_hfp_session* session);
void _bt_hfp_ag_deallocate_call(bt_hfp_session* session, bt_hfp_call_t*call);
void bt_hfp_ag_incoming_call_send_status(bt_hfp_session* session);
void _bt_hfp_ag_waiting_call_send_status(bt_hfp_session* session);
void _bt_hfp_ag_set_indicator_value(bt_hfp_session* session, bt_byte indicator_id, bt_byte indicator_value);
void _bt_hfp_ag_update_indicator_value(bt_hfp_session* session, bt_byte ind_inx);
void _bt_hfp_ag_handle_clip(bt_hfp_session* session, bt_char* param);
void _bt_hfp_ag_handle_ccwa(bt_hfp_session* session, bt_char* param);
void _bt_hfp_ag_handle_cmee(bt_hfp_session* session, bt_char* param);
void _bt_hfp_ag_handle_chup(bt_hfp_session* session);
void _bt_hfp_ag_handle_ata(bt_hfp_session* session);
void _bt_hfp_ag_handle_chld_test(bt_hfp_session* session);
void _bt_hfp_ag_handle_chld_param(bt_hfp_session* session, bt_char* param);
void _bt_hfp_ag_handle_chld(bt_hfp_session* session);
void _bt_hfp_ag_handle_bvra(bt_hfp_session* session, bt_char* param);
void _bt_hfp_ag_handle_vts(bt_hfp_session* session, bt_char* param);
void _bt_hfp_init_signal(void);
void _bt_hfp_set_signal(void);
bt_bool _bt_hfp_send_audio_packet(bt_hfp_session* session);
void _bt_hfp_ag_handle_cops_read(bt_hfp_session* session);
void _bt_hfp_ag_handle_clcc(bt_hfp_session* session);
void _bt_hfp_ag_clcc_send_next_call(bt_hfp_session* session);
void _bt_hfp_ag_handle_cnum(bt_hfp_session* session);


#ifdef __cplusplus
}
#endif

#endif // __HFP_PRIVATE_H
