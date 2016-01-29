/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __IAPEA_PRIVATE_H
#define __IAPEA_PRIVATE_H


#ifdef __cplusplus
extern "C" {
#endif


extern iapea_accessory_t             _iapea_sessions[];
extern const bt_byte                 _iapea_max_sessions;
extern const bt_byte                 _iapea_max_protocols;
extern iap_fid_token_sdk_protocol_t  _iapea_protocols[];
extern const iap_fid_token_header_t* _iapea_iap1_tokens[];
extern const bt_uint                 _iapea_max_packet_size;

#ifdef _DEBUG
extern const bt_uint _ram_size_iapea_buffers;
#endif

void _iapea_init_sessions(void);

void _iapea_iap2_session_callback(iap2_session_t* session, iap2_session_event_e evt, void* evt_param, void* callback_param);

void _iapea_iap_session_callback(iap_session_t* session, iap_session_event_e evt, void* evt_param, void* callback_param);

#ifdef __cplusplus
}
#endif

#endif // __IAPEA_PRIVATE_H
