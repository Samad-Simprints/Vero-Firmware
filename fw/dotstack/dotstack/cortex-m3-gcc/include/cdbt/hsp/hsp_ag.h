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

#ifndef __HSP_AG_H
#define __HSP_AG_H

/**
* \defgroup hsp HSP
*/

#include "cdbt/sdp/sdp.h"
#include "cdbt/spp/spp.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HSP_GATEWAY_PHASE_IDLE                      0
#define HSP_GATEWAY_PHASE_WAITING_RFCOMM_CONNECT    1
#define HSP_GATEWAY_PHASE_WAITING_SCO_CONNECT       2
#define HSP_GATEWAY_PHASE_CONNECTING                3
#define HSP_GATEWAY_PHASE_WAITING_CMD_RESPONSE      4

#define HSP_GATEWAY_STATE_FREE                 0xffff
#define HSP_GATEWAY_STATE_DISCONNECTED         0x0000
#define HSP_GATEWAY_STATE_RFCOMM_CONNECTED     0x0001
#define HSP_GATEWAY_STATE_SCO_CONNECTED        0x0002
#define HSP_GATEWAY_STATE_CONNECTING           0x0004
#define HSP_GATEWAY_STATE_DISCONNECTING        0x0008
#define HSP_GATEWAY_STATE_ALERTING             0x0010
#define HSP_GATEWAY_STATE_ALERTING_INBAND      0x0020
#define HSP_GATEWAY_STATE_CONNECTING_AUDIO     0x0040
#define HSP_GATEWAY_STATE_DISCONNECTING_AUDIO  0x0080
#define HSP_GATEWAY_STATE_SEARCHING_HEADSETS   0x0100
#define HSP_GATEWAY_STATE_CALL_IN_PROGRESS     0x0200

#define HSP_GATEWAY_EVT_NOTHING                               0
#define HSP_GATEWAY_EVT_HEADSET_CONNECTED                     1
#define HSP_GATEWAY_EVT_HEADSET_DISCONNECTED                  2
#define HSP_GATEWAY_EVT_HEADSET_AUDIO_CONNECTED               3
#define HSP_GATEWAY_EVT_HEADSET_AUDIO_DISCONNECTED            4
#define HSP_GATEWAY_EVT_AUDIO_DATA_RECEIVED                   6
#define HSP_GATEWAY_EVT_HEADSET_CONNECT_ATTEMPT_FAILED        7
#define HSP_GATEWAY_EVT_MIC_GAIN_CHANGED                      8
#define HSP_GATEWAY_EVT_SPK_VOLUME_CHANGED                    9
#define HSP_GATEWAY_EVT_HEADSET_AUDIO_CONNECT_ATTEMPT_FAILED  10
#define HSP_GATEWAY_EVT_AUDIO_ALERT_DATA_REQUEST              11
#define HSP_GATEWAY_EVT_HEADSET_ANSWERED                      12
#define HSP_GATEWAY_EVT_SET_MIC_GAIN_COMPLETED                13
#define HSP_GATEWAY_EVT_SET_SPK_VOLUME_COMPLETED              14
#define HSP_GATEWAY_EVT_BUTTON_PRESSED                        15
#define HSP_GATEWAY_EVT_SEARCH_COMPLETED                      16
#define HSP_GATEWAY_EVT_HANGUP                                17
#define HSP_GATEWAY_EVT_AUDIO_DATA_SENT                       18

#define HSP_GATEWAY_ACTION_NOTHING            0
#define HSP_GATEWAY_ACTION_CONNECT            1
#define HSP_GATEWAY_ACTION_ALERT_INBAND       2
#define HSP_GATEWAY_ACTION_ALERT_RING         3
#define HSP_GATEWAY_ACTION_CONNECT_AUDIO      4
#define HSP_GATEWAY_ACTION_DISCONNECT_AUDIO   5

#define HSP_GATEWAY_CMD_ID_RING              0
#define HSP_GATEWAY_CMD_ID_SET_SPK_VOLUME    1
#define HSP_GATEWAY_CMD_ID_SET_MIC_GAIN      2

#define HSP_GATEWAY_CONNECT_ACTION_DEFAULT             0
#define HSP_GATEWAY_CONNECT_ACTION_IGNORE              1
#define HSP_GATEWAY_CONNECT_ACTION_CONNECT_AUDIO       2

#define HSP_GATEWAY_BUTTON_ACTION_DEFAULT             0
#define HSP_GATEWAY_BUTTON_ACTION_IGNORE              1
#define HSP_GATEWAY_BUTTON_ACTION_CONNECT_AUDIO       2
#define HSP_GATEWAY_BUTTON_ACTION_DISCONNECT_AUDIO    3
#define HSP_GATEWAY_BUTTON_ACTION_DISCONNECT          4


#define HSP_AG_BUFFER_LENGTH	12

#define HSP_AG_RING_INTERVAL	2

#define HSP_HS_CMD_BUTTON_PRESS      "AT+CKPD=200\r"
#define HSP_HS_CMD_SET_MIC_GAIN      "AT+VGM="
#define HSP_HS_CMD_SET_SPK_VOLUME    "AT+VGS="

#define HSP_AG_CMD_SET_MIC_VOLUME		(const bt_byte*)"\r\n+VGM="
#define HSP_AG_CMD_SET_SPK_VOLUME		(const bt_byte*)"\r\n+VGS="
#define HSP_AG_CMD_END                  "\r\n"

#define HSP_AG_RES_OK      (const bt_byte*)"\r\nOK\r\n"
#define HSP_AG_RES_ERROR   (const bt_byte*)"\r\nERROR\r\n"
#define HSP_AG_RES_RING    (const bt_byte*)"\r\nRING\r\n"

#define HSP_AG_MAX_VOLUME	15

struct _bt_hsp_gateway_t;
struct _bt_hsp_headset_t;

typedef void (*bt_hsp_gateway_callback_fp)(struct _bt_hsp_gateway_t* gw, bt_byte evt, void* evt_param, void* callback_param);

typedef void (*bt_hsp_find_hs_callback_fp)(struct _bt_hsp_gateway_t* gw, bt_byte server_channel, bt_bool found);

typedef void (*bt_hsp_ag_send_audio_callback_fp)(struct _bt_hsp_gateway_t* gw, void* callback_param);

typedef struct _bt_hsp_headset_t
{
	bt_bdaddr_t address;
	bt_byte* name;
	bt_byte server_channel;
	bt_byte pg_scan_rpt_mode;
	bt_byte pg_scan_period_mode;
	bt_long cod;
	bt_int clock_offset;
	bt_byte rssi;
} bt_hsp_headset_t;

typedef struct _bt_hsp_ag_evt_connected_t
{
	bt_byte action;
} bt_hsp_ag_evt_connected_t;

typedef struct _bt_hsp_ag_evt_mic_gain_changed_t
{
	bt_byte gain;             ///< The value of the microphone gain
} bt_hsp_ag_evt_mic_gain_changed_t;

typedef struct _bt_hsp_ag_evt_spk_volume_changed_t
{
	bt_byte volume;             ///< The value of the speaker volume
} bt_hsp_ag_evt_spk_volume_changed_t;

typedef struct _bt_hsp_ag_evt_set_mic_gain_completed_t
{
	bt_bool succeeded;
	bt_byte gain;             ///< The value of the microphone gain
	bt_spp_send_status_e spp_status;
} bt_hsp_ag_evt_set_mic_gain_completed_t;

typedef struct _bt_hsp_ag_evt_set_spk_volume_completed_t
{
	bt_bool succeeded;
	bt_byte volume;             ///< The value of the speaker volume
	bt_spp_send_status_e spp_status;
} bt_hsp_ag_evt_set_spk_volume_completed_t;

typedef struct _bt_hsp_ag_evt_button_pressed_t
{
	bt_byte action;
	bt_ulong timestamp;
} bt_hsp_ag_evt_button_pressed_t;

typedef struct _bt_hsp_ag_evt_search_completed_t
{
	bt_hsp_headset_t* headsets; 
	bt_byte count;
} bt_hsp_ag_evt_search_completed_t;

typedef struct _bt_hsp_ag_evt_audio_data_t
{
	bt_byte* data;          ///< a pointer to the audio data received from the remote device
	bt_byte len;            ///< the length of the audio data
} bt_hsp_ag_evt_audio_data_t;

struct _bt_hsp_ag_command_t;
typedef struct _bt_hsp_ag_command_t bt_hsp_ag_command_t;

struct _bt_hsp_ag_command_t
{
	bt_hsp_ag_command_t* next_cmd;

	bt_byte cmd_id;
	bt_byte command[HSP_AG_BUFFER_LENGTH];
	bt_byte len;
};

typedef struct _bt_hsp_gateway_t
{
	bt_uint state;
	bt_byte phase;
	bt_rfcomm_dlc_t* rfcomm_dlc;
	bt_spp_port_t* port;
	bt_hci_conn_state_t* audio_hci_conn;
	bt_byte action;
	bt_byte ring_interval;
	bt_byte spk_volume;
	bt_byte mic_gain;

	bt_hsp_gateway_callback_fp gateway_callback;
	void* gateway_callback_param;

	bt_bdaddr_t connect_hs_bdaddr;

	bt_hsp_ag_send_audio_callback_fp audio_send_callback;

	bt_byte cur_headset;
	bt_byte headsets_count;
	bt_hsp_headset_t* found_headsets;

	bt_byte rx_buffer[HSP_AG_BUFFER_LENGTH];

	bt_queue_element_t*    send_cq_head;
	bt_hsp_ag_command_t* tx_cmd;

	bt_hci_listener_t hci_disconnect_listener;

} bt_hsp_gateway_t;

bt_bool bt_hsp_ag_init(void);

bt_hsp_gateway_t* bt_hsp_allocate_gateway(bt_l2cap_mgr_t* l2cap_mgr);

void bt_hsp_free_gateway(bt_hsp_gateway_t* gw);

bt_bool bt_hsp_ag_start(bt_hsp_gateway_t* gw, bt_byte server_channel, bt_hsp_gateway_callback_fp callback, void* param);

bt_bool bt_hsp_ag_connect(bt_hsp_gateway_t* gw, bt_bdaddr_p headset_addr);

bt_bool bt_hsp_ag_disconnect(bt_hsp_gateway_t* gw);

bt_bool bt_hsp_ag_alert(bt_hsp_gateway_t* gw);

bt_bool bt_hsp_ag_alert_inband(bt_hsp_gateway_t* gw);

void bt_hsp_ag_cancel_alert(bt_hsp_gateway_t* gw);

bt_bool bt_hsp_ag_end_call(bt_hsp_gateway_t* gw);

bt_bool bt_hsp_ag_connect_audio(bt_hsp_gateway_t* gw);

bt_bool bt_hsp_ag_disconnect_audio(bt_hsp_gateway_t* gw);

bt_bool bt_hsp_ag_set_spk_volume(bt_hsp_gateway_t* gw, bt_byte volume);

bt_bool bt_hsp_ag_set_mic_gain(bt_hsp_gateway_t* gw, bt_byte gain);

bt_bool bt_hsp_ag_find_headsets(bt_hsp_gateway_t* gw, bt_byte search_length);

bt_bool bt_hsp_ag_cancel_find_headsets(bt_hsp_gateway_t* gw);

bt_bool bt_hsp_ag_send_audio(bt_hsp_gateway_t* session, bt_byte* data, bt_uint len);

#ifdef __cplusplus
}
#endif

#include "cdbt/hsp/hsp_ag_private.h"

#endif // __HSP_AG_H
