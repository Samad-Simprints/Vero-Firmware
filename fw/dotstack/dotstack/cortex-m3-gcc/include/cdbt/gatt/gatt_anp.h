/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __GATT_ANP_H
#define __GATT_ANP_H

#define GATT_ANS_CATEGORY_SIMPLE_ALERT                 0
#define GATT_ANS_CATEGORY_EMAIL                        1
#define GATT_ANS_CATEGORY_NEWS                         2
#define GATT_ANS_CATEGORY_CALL                         3
#define GATT_ANS_CATEGORY_MISSED_CALL                  4
#define GATT_ANS_CATEGORY_SMS                          5
#define GATT_ANS_CATEGORY_VOICE_MAIL                   6
#define GATT_ANS_CATEGORY_SCHEDULE                     7
#define GATT_ANS_CATEGORY_HIGH_PRIORITIZES_ALERT       8
#define GATT_ANS_CATEGORY_INSTANT_MESSAGE              9

#define GATT_ANS_CATEGORY_MASK1_SIMPLE_ALERT           (1 << 0)
#define GATT_ANS_CATEGORY_MASK1_EMAIL                  (1 << 1)
#define GATT_ANS_CATEGORY_MASK1_NEWS                   (1 << 2)
#define GATT_ANS_CATEGORY_MASK1_CALL                   (1 << 3)
#define GATT_ANS_CATEGORY_MASK1_MISSED_CALL            (1 << 4)
#define GATT_ANS_CATEGORY_MASK1_SMS                    (1 << 5)
#define GATT_ANS_CATEGORY_MASK1_VOICE_MAIL             (1 << 6)
#define GATT_ANS_CATEGORY_MASK1_SCHEDULE               (1 << 7)
#define GATT_ANS_CATEGORY_MASK2_HIGH_PRIORITIZES_ALERT (1 << 8)
#define GATT_ANS_CATEGORY_MASK2_INSTANT_MESSAGE        (1 << 9)

#define GATT_ANS_CTRL_ENABLE_NEW_INCOMING_ALERT_NOTIFICATION         0
#define GATT_ANS_CTRL_ENABLE_UNREAD_CATEGORY_STATUS_NOTIFICATION     1
#define GATT_ANS_CTRL_DISABLE_NEW_INCOMING_ALERT_NOTIFICATION        2
#define GATT_ANS_CTRL_DISABLE_UNREAD_CATEGORY_STATUS_NOTIFICATION    3
#define GATT_ANS_CTRL_NOTIFY_NEW_INCOMING_ALERT_IMMEDIATELY          4
#define GATT_ANS_CTRL_NOTIFY_UNREAD_CATEGORY_STATUS_IMMEDIATELY      5

#define GATT_ANS_CLIENT_EVT_NEW_ALERT_RECEIVED                       0
#define GATT_ANS_CLIENT_EVT_UNREAD_ALERT_STATUS_RECEIVED             1

typedef struct _bt_gatt_client_anp_evt_new_alert_received_s
{
	bt_byte category;
	bt_uint count;
	const bt_byte* description;
	bt_uint description_len;
} bt_gatt_client_anp_evt_new_alert_received_t;

typedef struct _bt_gatt_client_anp_evt_unread_alert_status_received_s
{
	bt_byte category;
	bt_uint count;
} bt_gatt_client_anp_evt_unread_alert_status_received_t;

typedef union _bt_gatt_client_anp_evt_u
{
	bt_gatt_client_anp_evt_new_alert_received_t              new_alert;                ///< Valid if event is GATT_ANS_CLIENT_EVT_NEW_ALERT_RECEIVED
	bt_gatt_client_anp_evt_unread_alert_status_received_t    unread_alert_status;      ///< Valid if event is GATT_ANS_CLIENT_EVT_UNREAD_ALERT_STATUS_RECEIVED

} bt_gatt_client_anp_evt_t;

typedef struct _bt_gatt_client_anp_s bt_gatt_client_anp_t;
typedef void (*bt_gatt_client_anp_callback_fp)(bt_gatt_client_anp_t* instance, bt_int evt, bt_gatt_client_anp_evt_t* evt_param, void* param);

struct _bt_gatt_client_anp_s
{
	bt_gatt_client_profile_header_t header;

	// ANS characteristics
	bt_gatt_client_char_declaration_t supported_new_alert_category_char;
	bt_gatt_client_char_declaration_t new_alert_char;
	bt_gatt_client_char_declaration_t supported_unread_alert_category_char;
	bt_gatt_client_char_declaration_t unread_alert_status_char;
	bt_gatt_client_char_declaration_t alert_notification_control_point;

	// ANS characteristic descriptors
	bt_gatt_client_char_descriptor_t  new_alert_client_config;
	bt_gatt_client_char_descriptor_t  unread_alert_status_client_config;

	bt_byte phase;

	union
	{
		struct
		{
			unsigned int new_alert_category_present:1;
			unsigned int new_alert_present:1;
			unsigned int unread_alert_category_present:1;
			unsigned int unread_alert_status_present:1;
			unsigned int alert_notification_control_point_present:1;
			unsigned int new_alert_client_config_present:1;
			unsigned int unread_alert_status_client_config_present:1;
		} bits;

		unsigned int all;
	} flags;

	bt_uint supported_new_alert_category;
	bt_uint supported_unread_alert_category;

	bt_gatt_client_anp_callback_fp callback;
	void* callback_param;
};

void bt_gatt_client_register_anp_finder(void);

void bt_gatt_client_anp_add_instance(bt_gatt_client_session_t* session, bt_gatt_client_anp_t* instance, bt_gatt_client_anp_callback_fp callback, void* callback_param);

bt_uint bt_gatt_client_anp_get_supported_new_alert_category(bt_gatt_client_anp_t* instance);

bt_uint bt_gatt_client_anp_get_supported_unread_alert_category(bt_gatt_client_anp_t* instance);

bt_bool bt_gatt_client_anp_enable_new_alert_notifiaction(bt_gatt_client_anp_t* instance, bt_bool enable);

bt_bool bt_gatt_client_anp_enable_unread_alert_status_notifiaction(bt_gatt_client_anp_t* instance, bt_bool enable);

bt_bool bt_gatt_client_anp_control(bt_gatt_client_anp_t* instance, bt_byte command);

#endif // __GATT_ANP_H


