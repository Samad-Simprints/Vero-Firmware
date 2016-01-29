/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __GATT_ANCS_H
#define __GATT_ANCS_H

#define GATT_ANCS_CATEGORY_OTHER                 0
#define GATT_ANCS_CATEGORY_INCOMINGCALL          1
#define GATT_ANCS_CATEGORY_MISSEDCALL            2
#define GATT_ANCS_CATEGORY_VOICEMAIL             3
#define GATT_ANCS_CATEGORY_SOCIAL                4
#define GATT_ANCS_CATEGORY_SCHEDULE              5
#define GATT_ANCS_CATEGORY_EMAIL                 6
#define GATT_ANCS_CATEGORY_NEWS                  7
#define GATT_ANCS_CATEGORY_HEALTHANDFITNESS      8
#define GATT_ANCS_CATEGORY_BUSINESSANDFINANCE    9
#define GATT_ANCS_CATEGORY_LOCATION              10
#define GATT_ANCS_CATEGORY_ENTERTAINMENT         11


#define GATT_ANCS_EVENT_NOTIFICATION_ADDED       0
#define GATT_ANCS_EVENT_NOTIFICATION_MODIFIED    1
#define GATT_ANCS_EVENT_NOTIFICATION_REMOVED     2

#define GATT_ANCS_EVENT_FLAG_SILENT              (1 << 0)
#define GATT_ANCS_EVENT_FLAG_IMPORTANT           (1 << 1)
#define GATT_ANCS_EVENT_FLAG_PREEXISTING         (1 << 2)
#define GATT_ANCS_EVENT_FLAG_POSITIVEACTION      (1 << 3)
#define GATT_ANCS_EVENT_FLAG_NEGATIVEACTION      (1 << 4)

#define GATT_ANCS_COMMAND_GET_NOTIFICATION_ATTRIBUTES        0
#define GATT_ANCS_COMMAND_GET_APP_ATTRIBUTES                 1
#define GATT_ANCS_COMMAND_PERFORM_NOTIFICATION_ACTION        2

#define GATT_ANCS_NOTIFICATION_ATTR_APPIDENTIFIER            0
#define GATT_ANCS_NOTIFICATION_ATTR_TITLE                    1 // (Needs to be followed by a 2-bytes max length parameter)
#define GATT_ANCS_NOTIFICATION_ATTR_SUBTITLE                 2 // (Needs to be followed by a 2-bytes max length parameter)
#define GATT_ANCS_NOTIFICATION_ATTR_MESSAGE                  3 // (Needs to be followed by a 2-bytes max length parameter)
#define GATT_ANCS_NOTIFICATION_ATTR_MESSAGE_SIZE             4
#define GATT_ANCS_NOTIFICATION_ATTR_DATE                     5
#define GATT_ANCS_NOTIFICATION_ATTR_POSITIVE_ACTION_LABEL    6
#define GATT_ANCS_NOTIFICATION_ATTR_NEGATIVE_ACTION_LABEL    7

#define GATT_ANCS_ACTION_POSITIVE                            0
#define GATT_ANCS_ACTION_NEGATIVE                            1

#define GATT_ANCS_APP_ATTR_DISPLAY_NAME                      0

#define GATT_ANCS_CLIENT_EVT_NOTIFICATION_RECEIVED           0
#define GATT_ANCS_CLIENT_EVT_NOTIFICATION_ATTRS_RECEIVED     1
#define GATT_ANCS_CLIENT_EVT_APP_ATTRS_RECEIVED              2
#define GATT_ANCS_CLIENT_EVT_NOTIFICATION_ACTION_PERFORMED   3

#define GATT_ANCS_CLIENT_OP_ENABLE_NOTIFICATION_SOURCE_NOTIFIACTION    0
#define GATT_ANCS_CLIENT_OP_ENABLE_DATA_SOURCE_NOTIFIACTION            1
#define GATT_ANCS_CLIENT_OP_GET_NOTIFICATION_ATTRIBUTES                2
#define GATT_ANCS_CLIENT_OP_GET_APP_ATTRIBUTES                         3
#define GATT_ANCS_CLIENT_OP_PERFORM_NOTIFICATION_ACTION                4

extern const bt_uuid_t GATT_ANCS_SERVICE_ID;

typedef struct _bt_gatt_client_ancs_attribute_s
{
	bt_byte id;
	bt_uint len;
	bt_byte* value;
	bt_uint max_len;
} bt_gatt_client_ancs_attribute_t;

typedef struct _bt_gatt_client_anp_evt_notification_received_s
{
	bt_byte event_id;
	bt_byte event_flags;
	bt_byte category_id;
	bt_byte category_count;
	bt_ulong notification_uid;
} bt_gatt_client_anp_evt_notification_received_t;

typedef struct _bt_gatt_client_anp_evt_notification_attrs_received_s
{
	bt_byte status;
	bt_ulong notification_uid;
	bt_gatt_client_ancs_attribute_t* attrs;
	bt_ulong attr_count;
} bt_gatt_client_anp_evt_notification_attrs_received_t;

typedef struct _bt_gatt_client_anp_evt_app_attrs_received_s
{
	bt_byte status;
	const char* app_id;
	bt_gatt_client_ancs_attribute_t* attrs;
	bt_ulong attr_count;
} bt_gatt_client_anp_evt_app_attrs_received_t;

typedef struct _bt_gatt_client_anp_evt_notification_action_performed_s
{
	bt_byte status;
	bt_byte action_id;
} bt_gatt_client_anp_evt_notification_action_performed_t;

typedef union _bt_gatt_client_ancs_evt_u
{
	bt_gatt_client_anp_evt_notification_received_t              notification;                ///< Valid if event is GATT_ANCS_CLIENT_EVT_NOTIFICATION_RECEIVED
	bt_gatt_client_anp_evt_notification_attrs_received_t        notification_attributes;     ///< Valid if event is GATT_ANCS_CLIENT_EVT_NOTIFICATION_ATTRS_RECEIVED
	bt_gatt_client_anp_evt_app_attrs_received_t                 app_attributes;              ///< Valid if event is GATT_ANCS_CLIENT_EVT_APP_ATTRS_RECEIVED
	bt_gatt_client_anp_evt_notification_action_performed_t      notification_action;         ///< Valid if event is GATT_ANCS_CLIENT_EVT_NOTIFICATION_ACTION_PERFORMED

} bt_gatt_client_ancs_evt_t;


typedef struct _bt_gatt_client_ancs_operation_s
{
	bt_byte op_id;
	bt_byte* object_id;

	union
	{
		struct
		{
			bt_bool enable;

		} enable_notification_source_notifiaction;

		struct
		{
			bt_bool enable;

		} enable_data_source_notifiaction;

		struct
		{
			bt_gatt_client_ancs_attribute_t* attrs;
			bt_uint count;

		} get_attributes;

		struct
		{
			bt_byte action_id;

		} perform_notification_action;

	} op_params;

} bt_gatt_client_ancs_operation_t;


typedef struct _bt_gatt_client_ancs_s bt_gatt_client_ancs_t;
typedef void (*bt_gatt_client_ancs_callback_fp)(bt_gatt_client_ancs_t* instance, bt_int evt, bt_gatt_client_ancs_evt_t* evt_param, void* param);

struct _bt_gatt_client_ancs_s
{
	bt_gatt_client_profile_header_t header;

	// ANCS characteristics
	bt_gatt_client_char_declaration_t notification_source;
	bt_gatt_client_char_declaration_t control_point;
	bt_gatt_client_char_declaration_t data_source;

	// ANS characteristic descriptors
	bt_gatt_client_char_descriptor_t  notification_source_client_config;
	bt_gatt_client_char_descriptor_t  data_source_client_config;

	bt_byte phase;

	union
	{
		struct
		{
			unsigned int notification_source_present:1;
			unsigned int control_point_present:1;
			unsigned int data_source_present:1;
			unsigned int notification_source_client_config_present:1;
			unsigned int data_source_client_config_present:1;
			unsigned int executing_op:1;
		} bits;

		unsigned int all;
	} flags;

	bt_gatt_client_ancs_attribute_t* attrs;
	bt_uint attr_count;
	bt_uint current_attr;
	bt_uint current_attr_len;
	bt_uint attr_value_bytes_read;
	bt_ulong notification_id;
	bt_int current_cmd_id_header_len;
	bt_byte tmp1;
	bt_byte tmp2;

	bt_gatt_client_ancs_operation_t* op_queue;
	bt_uint op_queue_len;
	bt_uint op_queue_head;
	bt_uint op_queue_tail;

	bt_gatt_client_ancs_callback_fp callback;
	void* callback_param;
};

void bt_gatt_client_register_ancs_finder(void);

bt_bool bt_gatt_client_ancs_add_instance(
	bt_gatt_client_session_t* session, bt_gatt_client_ancs_t* instance, 
	bt_gatt_client_ancs_callback_fp callback, void* callback_param);

bt_bool bt_gatt_client_ancs_enable_notification_source_notifiaction(bt_gatt_client_ancs_t* instance, bt_bool enable);

bt_bool bt_gatt_client_anp_enable_data_source_notifiaction(bt_gatt_client_ancs_t* instance, bt_bool enable);

bt_bool bt_gatt_client_ancs_get_notification_attributes(bt_gatt_client_ancs_t* instance, bt_ulong notification_id, bt_gatt_client_ancs_attribute_t* attrs, bt_uint count);

bt_bool bt_gatt_client_ancs_get_app_attributes(bt_gatt_client_ancs_t* instance, const char* app_id, bt_gatt_client_ancs_attribute_t* attrs, bt_uint count);

bt_bool bt_gatt_client_ancs_perform_notification_action(bt_gatt_client_ancs_t* instance, bt_ulong notification_id, bt_byte action_id);

#endif // __GATT_ANSC_H


