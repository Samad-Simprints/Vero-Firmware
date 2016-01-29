/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __GATT_PASP_H
#define __GATT_PASP_H

#define GATT_PASS_ALERT_STATUS_RINGER_ACTIVE           1
#define GATT_PASS_ALERT_STATUS_VIBRATE_ACTIVE          2
#define GATT_PASS_ALERT_STATUS_DISPLAY_ACTIVE          4

#define GATT_PASS_RINGER_STATE_SILENT                  0
#define GATT_PASS_RINGER_STATE_NORMAL                  1

#define GATT_PASS_RINGER_CTRL_SILENT_MODE              1
#define GATT_PASS_RINGER_CTRL_MUTE_ONCE                2
#define GATT_PASS_RINGER_CTRL_CANCEL_SILENT_MODE       3

#define GATT_PASS_CLIENT_EVT_ALERT_STATUS_RECEIVED     0
#define GATT_PASS_CLIENT_EVT_RINGER_SETTING_RECEIVED   1

typedef struct _bt_gatt_client_pasp_evt_alert_status_received_s
{
	bt_byte alert_status;
} bt_gatt_client_pasp_evt_alert_status_received_t;

typedef struct _bt_gatt_client_pasp_evt_ringer_setting_received_s
{
	bt_byte ringer_setting;
} bt_gatt_client_pasp_evt_ringer_setting_received_t;

typedef union _bt_gatt_client_pasp_evt_u
{
	bt_gatt_client_pasp_evt_alert_status_received_t      alert_status;    ///< Valid if event is GATT_PASS_CLIENT_EVT_ALERT_STATUS_RECEIVED
	bt_gatt_client_pasp_evt_ringer_setting_received_t    ringer_setting;  ///< Valid if event is GATT_PASS_CLIENT_EVT_RINGER_SETTING_RECEIVED

} bt_gatt_client_pasp_evt_t;

typedef struct _bt_gatt_client_pasp_s bt_gatt_client_pasp_t;
typedef void (*bt_gatt_client_pasp_callback_fp)(bt_gatt_client_pasp_t* instance, bt_int evt, bt_gatt_client_pasp_evt_t* evt_param, void* param);

struct _bt_gatt_client_pasp_s
{
	bt_gatt_client_profile_header_t header;

	// PASS characteristics
	bt_gatt_client_char_declaration_t alert_status_char;
	bt_gatt_client_char_declaration_t ringer_setting_char;
	bt_gatt_client_char_declaration_t ringer_control_point_char;

	// PASS characteristic descriptors
	bt_gatt_client_char_descriptor_t  alert_status_client_config;
	bt_gatt_client_char_descriptor_t  ringer_setting_client_config;

	bt_byte phase;

	union
	{
		struct
		{
			unsigned int alert_status_present:1;
			unsigned int ringer_setting_present:1;
			unsigned int ringer_setting_control_point_present:1;
			unsigned int alert_status_client_config_present:1;
			unsigned int ringer_setting_client_config_present:1;
		} bits;

		unsigned int all;
	} flags;

	bt_byte alert_status;
	bt_byte ringer_setting;

	bt_gatt_client_pasp_callback_fp callback;
	void* callback_param;
};

void bt_gatt_client_pasp_add_instance(bt_gatt_client_session_t* session, bt_gatt_client_pasp_t* instance, bt_gatt_client_pasp_callback_fp callback, void* callback_param);

void bt_gatt_client_register_pasp_finder(void);

bt_byte bt_gatt_client_pasp_get_alert_status(bt_gatt_client_pasp_t* instance);

bt_byte bt_gatt_client_pasp_get_ringer_setting(bt_gatt_client_pasp_t* instance);

bt_bool bt_gatt_client_pasp_read_alert_status(bt_gatt_client_pasp_t* instance);

bt_bool bt_gatt_client_pasp_read_ringer_setting(bt_gatt_client_pasp_t* instance);

bt_bool bt_gatt_client_pasp_enable_alert_status_notifiaction(bt_gatt_client_pasp_t* instance, bt_bool enable);

bt_bool bt_gatt_client_pasp_enable_unread_alert_status_notifiaction(bt_gatt_client_pasp_t* instance, bt_bool enable);

bt_bool bt_gatt_client_pasp_control(bt_gatt_client_pasp_t* instance, bt_byte command);

#endif // __GATT_PASP_H


