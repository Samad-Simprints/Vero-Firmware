/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __GATT_CLIENT_H
#define __GATT_CLIENT_H

/**
 * \defgroup gatt_client GATT Client
 * \ingroup gatt
 *
 *  This module describes functions and data structures used to implement a GATT client (central).
 */

#include "cdbt/att/att_client.h"
#include "cdbt/gatt/gatt_common.h"
#include "cdbt/gatt/gatt_service.h"
#include "cdbt/gatt/gatt_characteristic.h"

#define GATT_CLIENT_SESSION_STATE_FREE                 0x00
#define GATT_CLIENT_SESSION_STATE_DISCONNECTED         0x01
#define GATT_CLIENT_SESSION_STATE_CONNECTED            0x02
#define GATT_CLIENT_SESSION_STATE_TRAN_STARTED         0x04
#define GATT_CLIENT_SESSION_STATE_CONNECTING           0x08
#define GATT_CLIENT_SESSION_STATE_DISCONNECTING        0x10

#define GATT_CLIENT_SESSION_FLAG_DISCOVERING           0x01

#define GATT_CLIENT_EVT_NOTHING                                0
/** \addtogroup gatt_client 
* @{
*/
/** @name Events
*
*/
/**@{
*/
#define GATT_CLIENT_EVT_SESSION_CONNECTED                      1     ///< This event is generated when the client connected to the server.
#define GATT_CLIENT_EVT_SESSION_DISCONNECTED                   2     ///< This event is generated when the client disconnected from the server.
#define GATT_CLIENT_EVT_CONNECTION_FAILED                      3     ///< This event is generated when the client failed to connect to the server.
#define GATT_CLIENT_EVT_TRAN_TIMEOUT                           7     ///< This event is generated if operation (discover, read, write) timed out.

#define GATT_CLIENT_EVT_DISCOVER_ALL_SERVICES_COMPLETED        10    ///< This event is generated when the "discover all services" operation has completed. 
#define GATT_CLIENT_EVT_DISCOVER_SERVICE_BY_UUID_COMPLETED     11    ///< This event is generated when the "discover service by UUID" operation has completed.
#define GATT_CLIENT_EVT_DISCOVER_ALL_CHARS_COMPLETED           12    ///< This event is generated when the "discover all cgaracteristics" operation has completed.
#define GATT_CLIENT_EVT_READ_CHAR_VALUE_COMPLETED              13    ///< This event is generated when the "read characteristic value" operation has completed.
#define GATT_CLIENT_EVT_READ_CHAR_LONG_VALUE_COMPLETED         14    ///< This event is generated when the "read long characteristic value" operation has completed.
#define GATT_CLIENT_EVT_WRITE_CHAR_VALUE_COMPLETED             15    ///< This event is generated when the "write characteristic value" operation has completed.
#define GATT_CLIENT_EVT_WRITE_CHAR_LONG_VALUE_COMPLETED        16    ///< This event is generated when the "write long characteristic value" operation has completed.
#define GATT_CLIENT_EVT_VALUE_NOTIFICATION                     17    ///< This event is generated when the client received value notification or indication.
#define GATT_CLIENT_EVT_DISCOVER_CHAR_DESCRIPTORS_COMPLETED    18    ///< This event is generated when the "discover characteristic descriptiors" operation has completed.
#define GATT_CLIENT_EVT_WRITE_CHAR_DESCRIPTOR_COMPLETED        19    ///< This event is generated when the "write characteristic descriptor" operation has completed.
#define GATT_CLIENT_EVT_WRITE_CHAR_LONG_DESCRIPTOR_COMPLETED   20    ///< This event is generated when the "write characteristic long descriptor" operation has completed.
#define GATT_CLIENT_EVT_READ_CHAR_DESCRIPTOR_COMPLETED         21    ///< This event is generated when the "read characteristic descriptor" operation has completed.
#define GATT_CLIENT_EVT_READ_CHAR_LONG_DESCRIPTOR_COMPLETED    22    ///< This event is generated when the "read characteristic long descriptor" operation has completed.
#define GATT_CLIENT_EVT_EXCHANGE_MTU_COMPLETED                 23    ///< This event is generated when the "exchange MTU" operation has completed.
#define GATT_CLIENT_EVT_FIND_INCLUDED_SERVICES_COMPLETED       24    ///< This event is generated when the "find included services" operation has completed.
#define GATT_CLIENT_EVT_DISCOVER_CHAR_BY_UUID_COMPLETED        25    ///< This event is generated when the "discover characteristic by UUID" operation has completed.
#define GATT_CLIENT_EVT_READ_USING_CHAR_UUID_COMPLETED         26    ///< This event is generated when the "read characteristic value using characteristic UUID" operation has completed.
#define GATT_CLIENT_EVT_READ_MULTIPLE_CHAR_VALUES_COMPLETED    27    ///< This event is generated when the "read multiple characteristic values" operation has completed.
#define GATT_CLIENT_EVT_CONN_PARAM_UPDATE_REQUEST              28    ///< This event is generated when the client received "connection parameter update" request.
#define GATT_CLIENT_EVT_CONN_PARAM_UPDATE_COMPLETED            29    ///< This event is generated after the new connection parameters have been set.

#define GATT_CLIENT_EVT_PROFILE_FINDER_STARTED                 100   ///< 
#define GATT_CLIENT_EVT_PROFILE_FINDER_COMPLETED               101   ///< 
#define GATT_CLIENT_EVT_PROFILE_FOUND                          102   ///< 
/**@}*/ 
/**@}*/

#define GATT_CLIENT_MAX_ATT_RESULTS    1

#define GATT_CLIENT_PROFILE_INSTANCE_STATE_FREE          0
#define GATT_CLIENT_PROFILE_INSTANCE_STATE_INCOMPLETE    1
#define GATT_CLIENT_PROFILE_INSTANCE_STATE_COMPLETE      2

#ifdef __cplusplus
extern "C" {
#endif

/**
* \brief UUID
* \ingroup gatt_client
*
* \details This structure is used to hold a UUID - 16 or 128 bit.
*/
typedef struct _bt_att_client_uuid_s
{
	bt_byte len;              ///< UUID length. This can be one of the following values:
                              ///<   @arg ATT_CLIENT_UUID_LEN_16
                              ///<   @arg ATT_CLIENT_UUID_LEN_128
	union
	{
		bt_uint uuid16;       ///< 16-bit UUID.
		bt_uuid_t uuid128;    ///< 128-bit UUID.
	} val;
} bt_att_client_uuid_t;

/**
* \brief Service Definition
* \ingroup gatt_client
*
* \details This structure is used to hold a service definition.
*/
typedef struct _bt_gatt_client_service_definition_s
{
	bt_att_client_uuid_t uuid;    ///< Service UUID.
	bt_uint start_handle;         ///< First attribute handle.
	bt_uint end_handle;           ///< Last attribute handle.

} bt_gatt_client_service_definition_t;

/**
* \brief Included Service Declaration
* \ingroup gatt_client
*
* \details This structure is used to hold an include service declaration.
*/
typedef struct _bt_gatt_client_included_service_definition_s
{
	bt_uint uuid;                 ///< Service UUID
	bt_uint decl_handle;          ///< Included service declaration attribute handle.
	bt_uint start_handle;         ///< First service attribute handle.
	bt_uint end_handle;           ///< Last service attribute handle.

} bt_gatt_client_inc_service_declaration_t;

/**
* \brief Characteristic Declaration
* \ingroup gatt_client
*
* \details This structure is used to hold an characteristic declaration.
*/
typedef struct _bt_gatt_client_char_declaration_s
{
	bt_byte props;                ///< Characteristic properties. 
	bt_uint start_handle;         ///< First characteristic attribute handle.
	bt_uint end_handle;           ///< Last characteristic attribute handle.
	bt_uint value_handle;         ///< Characteristic value attribute handle.
	bt_att_client_uuid_t uuid;    ///< Characteristic UUID.
} bt_gatt_client_char_declaration_t;

/**
* \brief Characteristic Value
* \ingroup gatt_client
*
* \details This structure is used to hold an characteristic value.
*/
typedef struct _bt_gatt_client_char_value_s
{
	bt_uint handle;               ///< Characteristic value attribute handle.
	bt_byte* value;               ///< Characteristic value.
	bt_uint len;                  ///< Characteristic value length.
} bt_gatt_client_char_value_t;

/**
* \brief Characteristic Descriptor Declaration
* \ingroup gatt_client
*
* \details This structure is used to hold an characteristic descriptor declaration.
*/
typedef struct _bt_gatt_client_char_descriptor_s
{
	bt_uint handle;              ///< Characteristic descriptor attribute handle.
	bt_att_client_uuid_t uuid;   ///< Characteristic descriptor UUID.
} bt_gatt_client_char_descriptor_t;

/**
* \brief  Parameter to GATT_CLIENT_EVT_EXCHANGE_MTU_COMPLETED event
* \ingroup gatt_client
*
* \details A pointer to this structure is passed to the GATT client application callback 
*          when the client received a response (either positive or negative) to a "exchange MTU" request.
*/
typedef struct _bt_gatt_client_evt_exchange_mtu_completed_s
{
	bt_byte status;     ///< Operation status
	bt_uint mtu;        ///< Server's MTU

} bt_gatt_client_evt_exchange_mtu_completed_t;

/**
* \brief  Parameter to GATT_CLIENT_EVT_DISCOVER_ALL_SERVICES_COMPLETED event
* \ingroup gatt_client
*
* \details A pointer to this structure is passed to the GATT client application callback 
*          when "discover all services" operation has completed.
*/
typedef struct _bt_gatt_client_evt_discover_all_services_completed_s
{
	bt_uint count;                                     ///< The number of found services
	bt_gatt_client_service_definition_t* services;     ///< An array of service definitions. 
                                                       ///< This is the same array that is passed to bt_gatt_client_discover_all_services_ex().

} bt_gatt_client_evt_discover_all_services_completed_t;

/**
* \brief  Parameter to GATT_CLIENT_EVT_DISCOVER_SERVICE_BY_UUID_COMPLETED event
* \ingroup gatt_client
*
* \details A pointer to this structure is passed to the GATT client application callback 
*          when "discover services by UUID" operation has completed.
*/
typedef struct _bt_gatt_client_evt_discover_service_by_uuid_completed_s
{
	bt_uint count;                                     ///< The number of found services
	bt_gatt_client_service_definition_t* services;     ///< An array of service definitions. 
                                                       ///< This is the same array that is passed to bt_gatt_client_discover_by_service_uuid().

} bt_gatt_client_evt_discover_service_by_uuid_completed_t;

/**
* \brief  Parameter to GATT_CLIENT_EVT_FIND_INCLUDED_SERVICES_COMPLETED event
* \ingroup gatt_client
*
* \details A pointer to this structure is passed to the GATT client application callback 
*          when "find included services" operation has completed.
*/
typedef struct _bt_gatt_client_evt_find_included_completed_s
{
	bt_uint count;                                          ///< The number of included service declarations found.
	bt_gatt_client_inc_service_declaration_t* services;     ///< An array of included service declarations. 
                                                            ///< This is the same array that is passed to bt_gatt_client_find_included_services_ex().

} bt_gatt_client_evt_find_included_completed_t;

/**
* \brief  Parameter to GATT_CLIENT_EVT_DISCOVER_ALL_CHARS_COMPLETED event
* \ingroup gatt_client
*
* \details A pointer to this structure is passed to the GATT client application callback 
*          when "discover all characteristics of a service" operation has completed.
*/
typedef struct _bt_gatt_client_evt_discover_all_chars_completed_s
{
	bt_uint count;                                     ///< The number of characteristic declarations found.
	bt_gatt_client_char_declaration_t* chars;          ///< An array of characteristic declarations. 
                                                       ///< This is the same array that is passed to bt_gatt_client_discover_all_chars_ex().

} bt_gatt_client_evt_discover_all_chars_completed_t;

/**
* \brief  Parameter to GATT_CLIENT_EVT_DISCOVER_CHAR_BY_UUID_COMPLETED event
* \ingroup gatt_client
*
* \details A pointer to this structure is passed to the GATT client application callback 
*          when "discover characteristics by UUID" operation has completed.
*/
typedef struct _bt_gatt_client_evt_discover_char_by_uuid_completed_s
{
	bt_uint count;                                     ///< The number of characteristic declarations found.
	bt_gatt_client_char_declaration_t* chars;          ///< An array of characteristic declarations. 
                                                       ///< This is the same array that is passed to bt_gatt_client_discover_char_by_uuid() or bt_gatt_client_discover_char_by_uuid_80().

} bt_gatt_client_evt_discover_char_by_uuid_completed_t;

/**
* \brief  Parameter to GATT_CLIENT_EVT_READ_CHAR_VALUE_COMPLETED event
* \ingroup gatt_client
*
* \details A pointer to this structure is passed to the GATT client application callback 
*          when "read characteristic value" operation has completed.
*/
typedef struct _bt_gatt_client_evt_read_char_value_completed_s
{
	bt_byte status;               ///< Operation status.
	const bt_byte* value;         ///< Characteristic value. This is the same buffer that is passed to bt_gatt_client_read_char_value().
	bt_uint len;                  ///< The length of the characteristic value.
	bt_att_client_uuid_t* uuid;   ///< Characteristic UUID;
} bt_gatt_client_evt_read_char_value_completed_t;

/**
* \brief  Parameter to GATT_CLIENT_EVT_READ_USING_CHAR_UUID_COMPLETED event
* \ingroup gatt_client
*
* \details A pointer to this structure is passed to the GATT client application callback 
*          when "read using characteristic UUID" operation has completed.
*/
typedef struct _bt_gatt_client_evt_read_by_char_uuid_completed_s
{
	bt_att_client_uuid_t* uuid;                        ///< Characteristic UUID.
	bt_uint count;                                     ///< The number of characteristic values read.
	bt_gatt_client_char_value_t* values;               ///< An array of characteristic values. 
                                                       ///< This is the same array that is passed to bt_gatt_client_read_by_char_uuid() or bt_gatt_client_read_by_char_uuid_80().
} bt_gatt_client_evt_read_by_char_uuid_completed_t;

/**
* \brief  Parameter to GATT_CLIENT_EVT_READ_MULTIPLE_CHAR_VALUES_COMPLETED event
* \ingroup gatt_client
*
* \details A pointer to this structure is passed to the GATT client application callback 
*          when "read multiple characteristic values" operation has completed.
*/
typedef struct _bt_gatt_client_evt_read_multiple_char_values_completed_s
{
	bt_byte status;          ///< Operation status.
	const bt_byte* value;    ///< Characteristic values. This is the same buffer that is passed to bt_gatt_client_read_multiple_char_values().
	bt_uint len;             ///< The length of the characteristic values.
} bt_gatt_client_evt_read_multiple_char_values_completed_t;

/**
* \brief  Parameter to GATT_CLIENT_EVT_READ_CHAR_LONG_VALUE_COMPLETED event
* \ingroup gatt_client
*
* \details A pointer to this structure is passed to the GATT client application callback 
*          when "read long characteristic value" operation has completed.
*/
typedef struct _bt_gatt_client_evt_read_char_long_value_completed_s
{
	bt_byte status;               ///< Operation status.
	bt_uint offset;               ///< The offset of the first octet read.
	const bt_byte* value;         ///< Characteristic value. This is the same buffer that is passed to bt_gatt_client_read_char_long_value().
	bt_uint len;                  ///< The length of the characteristic value.
	bt_att_client_uuid_t* uuid;   ///< Characteristic UUID;
} bt_gatt_client_evt_read_char_long_value_completed_t;

/**
* \brief  Parameter to GATT_CLIENT_EVT_WRITE_CHAR_VALUE_COMPLETED event
* \ingroup gatt_client
*
* \details A pointer to this structure is passed to the GATT client application callback 
*          when "write characteristic value" or "write without response" operation has completed.
*/
typedef struct _bt_gatt_client_evt_write_char_value_completed_s
{
	bt_byte status;          ///< Operation status.
	bt_uint handle;          ///< Characteristic value attribute handle.
} bt_gatt_client_evt_write_char_value_completed_t;

/**
* \brief  Parameter to GATT_CLIENT_EVT_WRITE_CHAR_LONG_VALUE_COMPLETED event
* \ingroup gatt_client
*
* \details A pointer to this structure is passed to the GATT client application callback 
*          when "write long characteristic value" operation has completed.
*/
typedef struct _bt_gatt_client_evt_write_char_long_value_completed_s
{
	bt_byte status;          ///< Operation status.
} bt_gatt_client_evt_write_char_long_value_completed_t;

/**
* \brief  Parameter to GATT_CLIENT_EVT_VALUE_NOTIFICATION event
* \ingroup gatt_client
*
* \details A pointer to this structure is passed to the GATT client application callback 
*          when the client received a value notification or indication.
*/
typedef struct _bt_gatt_client_evt_value_notification_s
{
	bt_uint handle;          ///< Characteristic value attribute handle.
	const bt_byte* value;    ///< Characteristic value.
	bt_uint len;             ///< Characteristic value length.
	bt_bool indication;      ///< If non-0 a value indication has been received.
} bt_gatt_client_evt_value_notification_t;

/**
* \brief  Parameter to GATT_CLIENT_EVT_DISCOVER_CHAR_DESCRIPTORS_COMPLETED event
* \ingroup gatt_client
*
* \details A pointer to this structure is passed to the GATT client application callback 
*          when "discover all characteristic descriptors" operation has completed.
*/
typedef struct _bt_gatt_client_evt_discover_descriptors_completed_s
{
	bt_uint count;                                     ///< The number of characteristic descriptors found.
	bt_gatt_client_char_descriptor_t* descriptors;     ///< An array of characteristic descriptors. 
                                                       ///< This is the same array that is passed to bt_gatt_client_discover_char_descriptors().
} bt_gatt_client_evt_discover_descriptors_completed_t;

/**
* \brief  Parameter to GATT_CLIENT_EVT_READ_CHAR_DESCRIPTOR_COMPLETED event
* \ingroup gatt_client
*
* \details A pointer to this structure is passed to the GATT client application callback 
*          when "read characteristic descriptor" operation has completed.
*/
typedef struct _bt_gatt_client_evt_read_char_descriptor_completed_s
{
	bt_byte status;          ///< Operation status.
	const bt_byte* value;    ///< Characteristic descriptor. This is the same buffer that is passed to bt_gatt_client_read_char_descriptor().
	bt_uint len;             ///< The length of the characteristic descriptor.
} bt_gatt_client_evt_read_char_descriptor_completed_t;

/**
* \brief  Parameter to GATT_CLIENT_EVT_READ_CHAR_LONG_DESCRIPTOR_COMPLETED event
* \ingroup gatt_client
*
* \details A pointer to this structure is passed to the GATT client application callback 
*          when "read long characteristic descriptor" operation has completed.
*/
typedef struct _bt_gatt_client_evt_read_char_long_descriptor_completed_s
{
	bt_byte status;          ///< Operation status.
	bt_uint offset;          ///< The offset of the first octet read.
	const bt_byte* value;    ///< Characteristic descriptor. This is the same buffer that is passed to bt_gatt_client_read_char_long_descriptor().
	bt_uint len;             ///< The length of the characteristic descriptor.
} bt_gatt_client_evt_read_char_long_descriptor_completed_t;

/**
* \brief  Parameter to GATT_CLIENT_EVT_WRITE_CHAR_DESCRIPTOR_COMPLETED event
* \ingroup gatt_client
*
* \details A pointer to this structure is passed to the GATT client application callback 
*          when "write characteristic descriptor" operation has completed.
*/
typedef struct _bt_gatt_client_evt_write_char_descriptor_completed_s
{
	bt_byte status;          ///< Operation status.
	bt_uint handle;          ///< Characteristic descriptor attribute handle.
} bt_gatt_client_evt_write_char_descriptor_completed_t;

/**
* \brief  Parameter to GATT_CLIENT_EVT_WRITE_CHAR_LONG_DESCRIPTOR_COMPLETED event
* \ingroup gatt_client
*
* \details A pointer to this structure is passed to the GATT client application callback 
*          when "write long characteristic descriptor" operation has completed.
*/
typedef struct _bt_gatt_client_evt_write_char_long_descriptor_completed_s
{
	bt_byte status;          ///< Operation status.
} bt_gatt_client_evt_write_char_long_descriptor_completed_t;

/**
* \brief  Parameter to GATT_CLIENT_EVT_CONN_PARAM_UPDATE_REQUEST event
* \ingroup gatt_client
*
* \details A pointer to this structure is passed to the GATT client application callback 
*          when client received update connection parameters request.
*/
typedef struct _bt_gatt_client_evt_conn_param_update_s
{
	bt_uint min_interval;           ///< Minimum connection interval expressed in 1.25ms units
	bt_uint max_interval;           ///< Maximum connection interval expressed in 1.25ms units
	bt_uint slave_latency;          ///< Slave latency expressed as number of connection events
	bt_uint supervision_timeout;    ///< Link supervision timeout expressed in 10ms units.
} bt_gatt_client_evt_conn_param_update_t;

/**
* \brief  Parameter to GATT_CLIENT_EVT_CONN_PARAM_UPDATE_COMPLETED event
* \ingroup gatt_client
*
* \details A pointer to this structure is passed to the GATT client application callback 
*          after the new connection parameters have been set.
*/
typedef struct _bt_gatt_client_evt_conn_param_update_completed_s
{
	bt_byte status;          ///< Operation status
	bt_int hci_status;       ///< HCI command status
} bt_gatt_client_evt_conn_param_update_completed_t;

typedef struct _bt_gatt_client_profile_header_s bt_gatt_client_profile_header_t;

typedef struct _bt_gatt_client_evt_profile_found_s
{
	bt_gatt_client_profile_header_t* profile_instance;
} bt_gatt_client_evt_profile_found_t;

/**
* \brief  Parameter to GATT client application callback.
* \ingroup gatt_client
*
* \details This union is used to pass event specific data to the GATT client consumer. Which member of the union 
*          points to a valid structure depends on the event reported to the consumer. In general, each event
*          has a corresponding member in the union.
*/
typedef union _bt_gatt_client_evt_u
{
	bt_gatt_client_evt_exchange_mtu_completed_t                exchange_mtu;                ///< Valid if event is GATT_CLIENT_EVT_EXCHANGE_MTU_COMPLETED
	bt_gatt_client_evt_discover_all_services_completed_t       discover_all_services;       ///< Valid if event is GATT_CLIENT_EVT_DISCOVER_ALL_SERVICES_COMPLETED
	bt_gatt_client_evt_discover_service_by_uuid_completed_t    discover_service_by_uuid;    ///< Valid if event is GATT_CLIENT_EVT_DISCOVER_SERVICE_BY_UUID_COMPLETED
	bt_gatt_client_evt_discover_all_chars_completed_t          discover_all_chars;          ///< Valid if event is GATT_CLIENT_EVT_DISCOVER_ALL_CHARS_COMPLETED
	bt_gatt_client_evt_discover_char_by_uuid_completed_t       discover_char_by_uuid;       ///< Valid if event is GATT_CLIENT_EVT_DISCOVER_CHAR_BY_UUID_COMPLETED
	bt_gatt_client_evt_read_char_value_completed_t             read_char_value;             ///< Valid if event is GATT_CLIENT_EVT_READ_CHAR_VALUE_COMPLETED
	bt_gatt_client_evt_read_char_long_value_completed_t        read_char_long_value;        ///< Valid if event is GATT_CLIENT_EVT_READ_CHAR_LONG_VALUE_COMPLETED
	bt_gatt_client_evt_write_char_value_completed_t            write_char_value;            ///< Valid if event is GATT_CLIENT_EVT_WRITE_CHAR_VALUE_COMPLETED
	bt_gatt_client_evt_write_char_long_value_completed_t       write_char_long_value;       ///< Valid if event is GATT_CLIENT_EVT_WRITE_CHAR_LONG_VALUE_COMPLETED
	bt_gatt_client_evt_value_notification_t                    value_notification;          ///< Valid if event is GATT_CLIENT_EVT_VALUE_NOTIFICATION
	bt_gatt_client_evt_discover_descriptors_completed_t        discover_descriptors;        ///< Valid if event is GATT_CLIENT_EVT_DISCOVER_CHAR_DESCRIPTORS_COMPLETED
	bt_gatt_client_evt_read_char_descriptor_completed_t        read_descriptor;             ///< Valid if event is GATT_CLIENT_EVT_READ_CHAR_DESCRIPTOR_COMPLETED
	bt_gatt_client_evt_read_char_long_descriptor_completed_t   read_long_descriptor;        ///< Valid if event is GATT_CLIENT_EVT_READ_CHAR_LONG_DESCRIPTOR_COMPLETED
	bt_gatt_client_evt_write_char_descriptor_completed_t       write_descriptor;            ///< Valid if event is GATT_CLIENT_EVT_WRITE_CHAR_DESCRIPTOR_COMPLETED
	bt_gatt_client_evt_write_char_long_descriptor_completed_t  write_long_descriptor;       ///< Valid if event is GATT_CLIENT_EVT_WRITE_CHAR_LONG_DESCRIPTOR_COMPLETED
	bt_gatt_client_evt_find_included_completed_t               find_included;               ///< Valid if event is GATT_CLIENT_EVT_FIND_INCLUDED_SERVICES_COMPLETED
	bt_gatt_client_evt_read_by_char_uuid_completed_t           read_by_char_uuid;           ///< Valid if event is GATT_CLIENT_EVT_READ_USING_CHAR_UUID_COMPLETED
	bt_gatt_client_evt_read_multiple_char_values_completed_t   read_multiple_char_values;   ///< Valid if event is GATT_CLIENT_EVT_READ_MULTIPLE_CHAR_VALUES_COMPLETED
	bt_gatt_client_evt_conn_param_update_t                     conn_param_update;           ///< Valid if event is GATT_CLIENT_EVT_CONN_PARAM_UPDATE_REQUEST
	bt_gatt_client_evt_conn_param_update_completed_t           conn_param_update_completed; ///< Valid if event is GATT_CLIENT_EVT_CONN_PARAM_UPDATE_COMPLETED

	bt_gatt_client_evt_profile_found_t                         profile_found;               ///< Valid if event is GATT_CLIENT_EVT_PROFILE_FOUND
} bt_gatt_client_evt_t;

typedef struct _bt_gatt_client_session_s bt_gatt_client_session_t;
typedef void (*bt_gatt_client_session_callback_fp)(bt_gatt_client_session_t* session, bt_int evt, bt_gatt_client_evt_t* evt_param, void* param);

typedef struct _bt_gatt_client_listener_t
{
	struct _bt_gatt_client_listener_t* next_listener;

	bt_gatt_client_session_callback_fp callback;
	void* callback_param;
} bt_gatt_client_listener_t;

typedef struct _bt_gatt_client_profile_finder_state_s
{
	bt_byte phase;
	bt_byte sub_phase;
	bt_uint current_service;
	bt_uint service_count;
	bt_uint current_char;
	bt_uint char_count;
	bt_uint current_descriptor;
	bt_uint descriptor_count;
} bt_gatt_client_profile_finder_state_t;

struct _bt_gatt_client_session_s
{
	bt_byte state;
	bt_byte flags;
	bt_att_client_session_t* att_session;

	bt_byte cur_cmd;
	union
	{
		struct
		{
			bt_gatt_client_service_definition_t* result;
			bt_uint max_results;
			bt_uint count;
			bt_att_read_by_group_type_response_t att_result[GATT_CLIENT_MAX_ATT_RESULTS];
		} discover_all;

		struct
		{
			bt_att_client_uuid_t uuid;
			bt_gatt_client_service_definition_t* result;
			bt_uint max_results;
			bt_uint count;
			bt_att_find_by_type_value_response_t att_result[GATT_CLIENT_MAX_ATT_RESULTS];
		} discover_service_by_uuid;

		struct
		{
			bt_uint end_handle;
			bt_gatt_client_inc_service_declaration_t* result;
			bt_uint max_results;
			bt_uint count;
			bt_att_read_by_type_response_t att_result[GATT_CLIENT_MAX_ATT_RESULTS];
		} find_included;

		struct
		{
			bt_uint end_handle;
			bt_gatt_client_char_declaration_t* result;
			bt_uint max_results;
			bt_uint count;
			bt_att_read_by_type_response_t att_result[GATT_CLIENT_MAX_ATT_RESULTS];
		} discover_all_chars;

		struct
		{
			bt_uint end_handle;
			bt_byte uuid_len;
			bt_att_client_uuid_t uuid;
			bt_gatt_client_char_declaration_t* result;
			bt_uint max_results;
			bt_uint count;
			bt_att_read_by_type_response_t att_result[GATT_CLIENT_MAX_ATT_RESULTS];
		} discover_char_by_uuid;

		struct
		{
			bt_byte* buffer;
			bt_uint  buffer_size;
			bt_att_client_uuid_t uuid;
		} read_char_val;

		struct
		{
			bt_att_client_uuid_t uuid;
			bt_gatt_client_char_value_t* result;
			bt_uint max_results;
			bt_uint count;
			bt_uint end_handle;
			bt_att_read_by_type_response_t att_result[GATT_CLIENT_MAX_ATT_RESULTS];
		} read_by_uuid;

		struct
		{
			bt_att_client_uuid_t uuid;
			bt_uint offset;
			bt_byte* buffer;
			bt_uint  buffer_size;
			bt_uint  buffer_len;
		} read_char_long_val;

		struct
		{
			bt_uint handles[ATT_DEFAULT_MTU / 2];
			bt_byte* buffer;
			bt_uint  buffer_size;
		} read_multi_char_val;

		struct
		{
			bt_uint handle;
			bt_uint offset;
			const bt_byte* value;
			bt_uint  value_len;
			bt_uint  value_pos;
		} write_char_long_val;

		struct
		{
			bt_uint end_handle;
			bt_gatt_client_char_descriptor_t* result;
			bt_uint max_results;
			bt_uint count;
			bt_att_find_info_response_t att_result[GATT_CLIENT_MAX_ATT_RESULTS];
		} discover_descriptors;

		struct
		{
			bt_byte* buffer;
			bt_uint  buffer_size;
		} read_char_descriptor;

		struct
		{
			bt_uint offset;
			bt_byte* buffer;
			bt_uint  buffer_size;
			bt_uint  buffer_len;
		} read_char_long_descriptor;

		struct
		{
			bt_uint handle;
			bt_uint offset;
			const bt_byte* value;
			bt_uint  value_len;
			bt_uint  value_pos;
		} write_char_long_descriptor;

	} cmd_params;

	bt_gatt_client_listener_t  main_listener;
	bt_gatt_client_listener_t* listeners;

	bt_gatt_client_profile_finder_state_t profile_finder_state;
	bt_gatt_client_profile_header_t* profile_instances;
	bt_gatt_client_listener_t  profile_finder_listener;

	bt_long tran_start_time;
};

typedef struct _bt_gatt_client_profile_finder_s bt_gatt_client_profile_finder_t;
typedef bt_bool (*bt_gatt_client_profile_service_finder_fp)(bt_gatt_client_session_t* session, const bt_gatt_client_service_definition_t* service);
typedef bt_bool (*bt_gatt_client_profile_char_finder_fp)(bt_gatt_client_session_t* session, const bt_gatt_client_char_declaration_t* characteristic, bt_bool* find_descriptors);
typedef bt_bool (*bt_gatt_client_profile_descriptor_finder_fp)(bt_gatt_client_session_t* session, const bt_gatt_client_char_descriptor_t* characteristic);
typedef bt_bool (*bt_gatt_client_profile_initializer_fp)(bt_gatt_client_session_t* session, bt_bool start, const bt_att_client_uuid_t* client_uuid, const bt_byte* value, bt_uint len);
typedef bt_bool (*bt_gatt_client_profile_value_reader_fp)(bt_gatt_client_session_t* session, const bt_att_client_uuid_t* client_uuid, const bt_byte* value, bt_uint len);
typedef bt_bool (*bt_gatt_client_profile_value_notifier)(bt_gatt_client_session_t* session, bt_uint value_handle, const bt_byte* value, bt_uint len);
typedef bt_gatt_client_profile_header_t* (*bt_gatt_client_profile_get_instance_fp)(bt_gatt_client_session_t* session);
typedef void (*bt_gatt_client_profile_free_instance_fp)(bt_gatt_client_profile_header_t* instance);
typedef bt_bool (*bt_gatt_client_profile_value_writer_fp)(bt_gatt_client_session_t* session, bt_byte status, bt_uint value_handle);
typedef bt_bool (*bt_gatt_client_profile_descriptor_writer_fp)(bt_gatt_client_session_t* session, bt_byte status, bt_uint value_handle);
typedef bt_bool (*bt_gatt_client_profile_queue_processor_fp)(bt_gatt_client_session_t* session);

struct _bt_gatt_client_profile_header_s
{
	bt_gatt_client_profile_header_t* next_profile;

	bt_uint size;
	bt_byte state;
	bt_uuid_t service_id;
	bt_gatt_client_session_t* session;
	bt_gatt_client_profile_finder_t* finder;
};

struct _bt_gatt_client_profile_finder_s
{
	bt_gatt_client_profile_finder_t* next_finder;
	
	bt_gatt_client_profile_service_finder_fp service_finder;
	bt_gatt_client_profile_char_finder_fp char_finder;
	bt_gatt_client_profile_descriptor_finder_fp descriptor_finder;
	bt_gatt_client_profile_initializer_fp initializer;
	bt_gatt_client_profile_value_reader_fp value_reader;
	bt_gatt_client_profile_value_notifier value_notifier;
	bt_gatt_client_profile_get_instance_fp get_instance;
	bt_gatt_client_profile_free_instance_fp free_instance;
	bt_gatt_client_profile_value_writer_fp value_writer;
	bt_gatt_client_profile_descriptor_writer_fp descriptor_writer;
	bt_gatt_client_profile_queue_processor_fp queue_processor;
};

typedef struct _bt_gatt_client_mgr_t
{
	bt_gatt_client_session_t* sessions;

} bt_gatt_client_mgr_t;

/**
 * \brief Initialize the GATT client.
 * \ingroup gatt_client
 *
 * \details This function initializes the GATT client of the stack. It must be called prior to any other
 *          GATT client function can be called.
 *
 * \return  Always \c TRUE
 */
bt_bool bt_gatt_client_init(void);

/**
 * \brief Return a pointer to an instance of the GATT client manager.
 * \ingroup gatt_client
 *
 * \details This function returns a pointer to an instance of the GATT client manager. There is only one instance of the manager allocated by the stack. 
 *
 * \return
 *         \li A pointer to the GATT client manager.
 */
bt_gatt_client_mgr_t* bt_gatt_get_client_mgr(void);

/**
 * \brief Allocate GATT client session
 * \ingroup gatt_client
 *
 * \details This function allocates a new GATT client session.
 *
 * \param callback The callback function that will be called when the GATT client generates an event.
 * \param callback_param A pointer to arbitrary data to be passed to the \c callback callback.
 *
 * \return
 *         \li A pointer to the new GATT client session structure if the function succeeds.
 *         \li \c NULL otherwise.
 */
bt_gatt_client_session_t* bt_gatt_client_allocate_session(bt_gatt_client_session_callback_fp callback, void* callback_param);

/**
 * \brief Destroy GATT client session.
 * \ingroup gatt_client
 *
 * \details This function frees memory used by the session. The session has to exist and be in the "idle" state for this function to succeed. 
 *          I.e. the session has to be disconnected before this function can be called.
 *
 * \param session GATT session.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
*/
void bt_gatt_client_free_sessions(bt_gatt_client_session_t* session);

/**
 * \brief Connect to a remote device (peripheral).
 * \ingroup gatt_client
 *
 * \details This function establishes a connection to a remote device specified by the \p addr. If connection
 *          cannot be initiated for some reason, for example, there is not enough resources, it returns FALSE and no events are generated. Otherwise
 *          the result of an attempt to connect to the remote device is reported via the GATT client callback. 
 *          The events generated will either be GATT_CLIENT_SESSION_EVT_SESSION_CONNECTED or GATT_CLIENT_SESSION_EVT_CONNECTION_FAILED.
 *
 * \param session GATT session.
 * \param addr The address of a remote device.
 * 
 * \return
 *        \li \c TRUE if connection establishment has been started.
 *        \li \c FALSE otherwise.
*/
bt_bool bt_gatt_client_connect(bt_gatt_client_session_t* session, bt_bdaddr_t* addr);

/**
 * \brief Disconnect from a remote device.
 * \ingroup gatt_client
 *
 * \details This function closes a connection to a remote device. After the connection has been terminated 
 *          the GATT client callback is called with GATT_CLIENT_SESSION_EVT_SESSION_DISCONNECTED event.
 *
 * \param session GATT session.
 * 
 * \return
 *        \li \c TRUE if disconnection has been started.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_gatt_client_disconnect(bt_gatt_client_session_t* session);

bt_bool bt_gatt_client_attach_connection(bt_gatt_client_session_t* session, struct _bt_att_session_s* server_session);

bt_bool bt_gatt_client_detach_connection(bt_gatt_client_session_t* session);

/**
 * \brief Exchange MTU.
 * \ingroup gatt_client
 *
 * \details This function informs the server about the client's MTU. In response to the "exchange MTU" request 
 *         the server sends its MTU to the client.
 *
 * \param session GATT session.
 * \param mtu Client's MTU.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_gatt_exchange_mtu(bt_gatt_client_session_t* session, bt_uint mtu);

#define bt_gatt_client_discover_all_services(session, result, max_results)	\
	bt_gatt_client_discover_all_services_ex(session, 1, result, max_results)

/**
 * \brief Discover All Services.
 * \ingroup gatt_client
 *
 * \details This function discovers all the primary services on the server.
 *
 * \param session GATT session.
 * \param start_handle First attribute handle from which the server has to start searching for services.
 * \param result Pointer to a buffer where the client will store definitions of the found services.
 * \param max_result The maximum number of service definitions that can be stored in \p result.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_gatt_client_discover_all_services_ex(
	bt_gatt_client_session_t* session,
	bt_uint start_handle,
	bt_gatt_client_service_definition_t* result, bt_uint max_results);

/**
 * \brief Discover Services by UUID (16-bit).
 * \ingroup gatt_client
 *
 * \details This function discovers a specific primary service on the server when only the Service UUID is known.
 *
 * \param session GATT session.
 * \param service_uuid 16-bit Service UUID.
 * \param result Pointer to a buffer where the client will store definitions of the found services.
 * \param max_result The maximum number of service definitions that can be stored in \p result.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_gatt_client_discover_by_service_uuid(
	bt_gatt_client_session_t* session, bt_uint service_uuid,
	bt_gatt_client_service_definition_t* result, bt_uint max_results);

#define bt_gatt_client_find_included_services(session, service, result, max_results)	\
	bt_gatt_client_find_included_services_ex(session, service, (service)->start_handle, result, max_results)

/**
 * \brief Find Included Services.
 * \ingroup gatt_client
 *
 * \details This function finds include service declarations within a service definition on a server.
 *
 * \param session GATT session.
 * \param service Service definition.
 * \param start_handle First attribute handle from which the server has to start searching for include service declarations.
 * \param result Pointer to a buffer where the client will store found include service declarations.
 * \param max_result The maximum number of include service declarations that can be stored in \p result.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_gatt_client_find_included_services_ex(
	bt_gatt_client_session_t* session,
	const bt_gatt_client_service_definition_t* service,
	bt_uint start_handle,
	bt_gatt_client_inc_service_declaration_t* result, bt_uint max_results);

/**
 * \brief Discover Services by UUID (128-bit).
 * \ingroup gatt_client
 *
 * \details This function discovers a specific primary service on the server when only the Service UUID is known.
 *
 * \param session GATT session.
 * \param service_uuid 128-bit Service UUID.
 * \param result Pointer to a buffer where the client will store definitions of the found services.
 * \param max_result The maximum number of service definitions that can be stored in \p result.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_gatt_client_discover_by_service_uuid_80(
	bt_gatt_client_session_t* session, bt_uuid_t* service_uuid,
	bt_gatt_client_service_definition_t* result, bt_uint max_results);

#define bt_gatt_client_discover_all_chars(session, service, result, max_results)	\
	bt_gatt_client_discover_all_chars_ex(session, service, (service)->start_handle, result, max_results)

/**
 * \brief Discover All Characteristics of a Service.
 * \ingroup gatt_client
 *
 * \details This function finds all the characteristic declarations within a service definition on a server.
 *
 * \param session GATT session.
 * \param service Service definition.
 * \param start_handle First attribute handle from which the server has to start searching for characteristic declarations.
 * \param result Pointer to a buffer where the client will store found characteristic declarations.
 * \param max_result The maximum number of characteristic declarations that can be stored in \p result.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_gatt_client_discover_all_chars_ex(
	bt_gatt_client_session_t* session,
	const bt_gatt_client_service_definition_t* service,
	bt_uint start_handle,
	bt_gatt_client_char_declaration_t* result, bt_uint max_results);

/**
 * \brief Discover Characteristics by UUID (16-bit).
 * \ingroup gatt_client
 *
 * \details This function finds characteristic declarations within a service definition on a server by the characteristic UUID.
 *
 * \param session GATT session.
 * \param service Service definition.
 * \param char_uuid 16-bit characteristic UUID.
 * \param result Pointer to a buffer where the client will store found characteristic declarations.
 * \param max_result The maximum number of characteristic declarations that can be stored in \p result.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_gatt_client_discover_char_by_uuid(
	bt_gatt_client_session_t* session,
	const bt_gatt_client_service_definition_t* service, bt_uint char_uuid,
	bt_gatt_client_char_declaration_t* result, bt_uint max_results);

/**
 * \brief Discover Characteristics by UUID (128-bit).
 * \ingroup gatt_client
 *
 * \details This function finds characteristic declarations within a service definition on a server by the characteristic UUID.
 *
 * \param session GATT session.
 * \param service Service definition.
 * \param char_uuid 128-bit characteristic UUID.
 * \param result Pointer to a buffer where the client will store found characteristic declarations.
 * \param max_result The maximum number of characteristic declarations that can be stored in \p result.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_gatt_client_discover_char_by_uuid_80(
	bt_gatt_client_session_t* session,
	const bt_gatt_client_service_definition_t* service, bt_uuid_t* char_uuid,
	bt_gatt_client_char_declaration_t* result, bt_uint max_results);

/**
 * \brief Read Characteristic Value.
 * \ingroup gatt_client
 *
 * \details This function reads characteristic value.
 *
 * \param session GATT session.
 * \param characteristic Characteristic declaration.
 * \param buffer Pointer to a buffer where the client will store the characteristic's value.
 * \param len The length of the \c buffer.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_gatt_client_read_char_value(
	bt_gatt_client_session_t* session,
	const bt_gatt_client_char_declaration_t* characteristic,
	bt_byte* buffer, bt_uint len);

/**
 * \brief Read Using Characteristic UUID (16-bit).
 * \ingroup gatt_client
 *
 * \details This function reads characteristic values from a server when only the characteristic UUID is known.
 *
 * \param session GATT session.
 * \param service Service definition.
 * \param char_uuid 16-bit characteristic UUID.
 * \param result Pointer to a buffer where the client will store characteristic values.
 * \param max_result The maximum number of characteristic values that can be stored in \p result.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_gatt_client_read_by_char_uuid(
	bt_gatt_client_session_t* session,
	const bt_gatt_client_service_definition_t* service,
	bt_uint char_uuid,
	bt_gatt_client_char_value_t* result, bt_uint max_results);

/**
 * \brief Read Using Characteristic UUID (128-bit).
 * \ingroup gatt_client
 *
 * \details This function reads characteristic values from a server when only the characteristic UUID is known.
 *
 * \param session GATT session.
 * \param service Service definition.
 * \param char_uuid 128-bit characteristic UUID.
 * \param result Pointer to a buffer where the client will store characteristic values.
 * \param max_result The maximum number of characteristic values that can be stored in \p result.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_gatt_client_read_by_char_uuid_80(
	bt_gatt_client_session_t* session,
	const bt_gatt_client_service_definition_t* service,
	bt_uuid_t* char_uuid,
	bt_gatt_client_char_value_t* result, bt_uint max_results);

/**
 * \brief Read Long Characteristic Value.
 * \ingroup gatt_client
 *
 * \details This function reads characteristic value.
 *
 * \param session GATT session.
 * \param characteristic Characteristic declaration.
 * \param offset The offset of the first octet to read.
 * \param buffer Pointer to a buffer where the client will store the characteristic's value.
 * \param len The length of the \c buffer.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_gatt_client_read_char_long_value(
	bt_gatt_client_session_t* session,
	const bt_gatt_client_char_declaration_t* characteristic,
	bt_uint offset, bt_byte* buffer, bt_uint len);

/**
 * \brief Read Multiple Characteristic Values.
 * \ingroup gatt_client
 *
 * \details This function reads multiple characteristic values from a server when the characteristic value handles are known.
 *
 * \param session GATT session.
 * \param handles An array of characteristic value handles.
 * \param count The number of handles.
 * \param buffer Pointer to a buffer where the client will store the characteristic values.
 * \param len The length of the \c buffer.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_gatt_client_read_multiple_char_values(
	bt_gatt_client_session_t* session,
	const bt_uint* handles, bt_uint count,
	bt_byte* buffer, bt_uint len);

/**
 * \brief Write Characteristic Value.
 * \ingroup gatt_client
 *
 * \details This function writes characteristic value to a server using ATT "write" request, i.e. the server will send a response after writing the value.
 *
 * \param session GATT session.
 * \param characteristic Characteristic declaration.
 * \param value Characteristic value.
 * \param len The length of the \c value.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_gatt_client_write_char_value(
	bt_gatt_client_session_t* session,
	const bt_gatt_client_char_declaration_t* characteristic,
	const bt_byte* value, bt_uint len);

/**
 * \brief Write Without Response.
 * \ingroup gatt_client
 *
 * \details This function writes characteristic value to a server using ATT "write" command, i.e. the server will not send a response after writing the value.
 *
 * \param session GATT session.
 * \param characteristic Characteristic declaration.
 * \param value Characteristic value.
 * \param len The length of the \c value.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_gatt_client_set_char_value(
	bt_gatt_client_session_t* session,
	const bt_gatt_client_char_declaration_t* characteristic,
	const bt_byte* value, bt_uint len);

/**
 * \brief Write Long Characteristic Value.
 * \ingroup gatt_client
 *
 * \details This function writes characteristic value to a server using ATT "prepare write" and "execute write" requests.
 *
 * \param session GATT session.
 * \param characteristic Characteristic declaration.
 * \param value_offset The offset of the first octet to be written.
 * \param value Characteristic value.
 * \param len The length of the \c value.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_gatt_client_write_char_long_value(
	bt_gatt_client_session_t* session,
	const bt_gatt_client_char_declaration_t* characteristic,
	bt_uint value_offset, const bt_byte* value, bt_uint len);

/**
 * \brief Discover All Characteristic Descriptors.
 * \ingroup gatt_client
 *
 * \details This function finds all the characteristic descriptors.
 *
 * \param session GATT session.
 * \param characteristic Characteristic declaration.
 * \param start_handle First attribute handle from which the server has to start searching for characteristic declarations.
 * \param result Pointer to a buffer where the client will store found characteristic descriptors.
 * \param max_result The maximum number of characteristic descriptors that can be stored in \p result.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_gatt_client_discover_char_descriptors(
	bt_gatt_client_session_t* session,
	const bt_gatt_client_char_declaration_t* characteristic,
	bt_gatt_client_char_descriptor_t* result, bt_uint max_results);

/**
 * \brief Read Characteristic Descriptor.
 * \ingroup gatt_client
 *
 * \details This function reads characteristic descriptor.
 *
 * \param session GATT session.
 * \param descriptor Characteristic descriptor.
 * \param buffer Pointer to a buffer where the client will store the characteristic's descriptor.
 * \param len The length of the \c buffer.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_gatt_client_read_char_descriptor(
   bt_gatt_client_session_t* session,
   const bt_gatt_client_char_descriptor_t* descriptor,
   bt_byte* buffer, bt_uint len);

/**
 * \brief Read Long Characteristic Descriptor.
 * \ingroup gatt_client
 *
 * \details This function reads long characteristic descriptor.
 *
 * \param session GATT session.
 * \param descriptor Characteristic descriptor.
 * \param offset The offset of the first octet to read.
 * \param buffer Pointer to a buffer where the client will store the characteristic's descriptor.
 * \param len The length of the \c buffer.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_gatt_client_read_char_long_descriptor(
	bt_gatt_client_session_t* session,
	const bt_gatt_client_char_descriptor_t* descriptor,
	bt_uint offset, bt_byte* buffer, bt_uint len);

/**
 * \brief Write Characteristic Descriptor.
 * \ingroup gatt_client
 *
 * \details This function writes characteristic descriptor.
 *
 * \param session GATT session.
 * \param descriptor Characteristic descriptor.
 * \param value Characteristic descriptor value.
 * \param len The length of the \c value.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_gatt_client_write_char_descriptor(
	bt_gatt_client_session_t* session,
	const bt_gatt_client_char_descriptor_t* descriptor,
	const bt_byte* value, bt_uint len);

/**
 * \brief Write Long Characteristic Descriptor.
 * \ingroup gatt_client
 *
 * \details This function writes characteristic descriptor.
 *
 * \param session GATT session.
 * \param descriptor Characteristic descriptor.
 * \param value_offset The offset of the first octet to be written.
 * \param value Characteristic descriptor value.
 * \param len The length of the \c buffer.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_gatt_client_write_char_long_descriptor(
	bt_gatt_client_session_t* session,
	const bt_gatt_client_char_descriptor_t* descriptor,
	bt_uint value_offset, const bt_byte* value, bt_uint len);

/**
 * \brief Get HCI connection for a session
 * \ingroup gatt_client
 *
 * \details This function returns a pointer to a structure that describes an HCI connection a session is open on. 
 *          The return value can be used to call various function from the HCI layer. For example, if an app
 *          wants to force disconnection from a remote device it can call ::bt_hci_disconnect.
 *
 * \param session GATT session.
 * 
 * \return
 *        \li \c Pointer to a structure that describes an HCI connection if the function succeeds.
 *        \li \c NULL otherwise. The function fails only if a session specified by the \p session parameter 
 *        \li does not exist or there is no HCI connection between local and remote devices associated with the session.
 */
bt_hci_conn_state_t* bt_gatt_client_get_hci_connection(const bt_gatt_client_session_t* session);

/**
 * \brief Accept connection parameters update.
 * \ingroup gatt_client
 *
 * \details When a server sends "connection parameters update" request a GATT_CLIENT_EVT_CONN_PARAM_UPDATE_REQUEST event is generated.
 *          Client has to either accept the request or deny it. \c bt_gatt_client_accept_conn_param_update is used to accept the request.
 *
 * \param session GATT session.
 * \param min_interval Minimum connection interval expressed in 1.25ms units.
 * \param max_interval Maximum connection interval expressed in 1.25ms units.
 * \param slave_latency Slave latency expressed as number of connection events.
 * \param supervision_timeout Link supervision timeout expressed in 10ms units.
 * \param min_ce_length Information parameter about the minimum length of connection needed for this LE connection expressed in 0.625ms units.
 * \param max_ce_length Information parameter about the maximum length of connection needed for this LE connection expressed in 0.625ms units.
 *
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
#define bt_gatt_client_accept_conn_param_update(session, min_interval, max_interval, slave_latency,	supervision_timeout, min_ce_length, max_ce_length)	\
	bt_att_client_accept_conn_param_update(session->att_session, min_interval, max_interval, slave_latency, supervision_timeout, min_ce_length, max_ce_length)

/**
 * \brief Reject connection parameters update.
 * \ingroup gatt_client
 *
 * \details When a server sends "connection parameters update" request a GATT_CLIENT_EVT_CONN_PARAM_UPDATE_REQUEST event is generated.
 *          Client has to either accept the request or deny it. \c bt_gatt_client_reject_conn_param_update is used to deny the request.
 *
 * \param session GATT session.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
#define bt_gatt_client_reject_conn_param_update(session)    bt_att_client_reject_conn_param_update(session->att_session)

bt_bool bt_gatt_client_register_listener(bt_gatt_client_session_t* session, bt_gatt_client_listener_t* listener);

void bt_gatt_client_unregister_listener(bt_gatt_client_session_t* session, bt_gatt_client_listener_t* listener);

void bt_gatt_client_init_profile_finder(
	bt_gatt_client_service_definition_t* services,
	bt_uint max_services,
	bt_gatt_client_char_declaration_t* characteristics,
	bt_uint max_chars,
	bt_gatt_client_char_descriptor_t* descriptors,
	bt_uint max_descriptors);

bt_bool bt_gatt_client_find_profiles(bt_gatt_client_session_t* session);

void bt_gatt_client_register_profile_finder(bt_gatt_client_profile_finder_t* finder);

#include "cdbt/gatt/gatt_client_private.h"

#ifdef __cplusplus
}
#endif

#endif // __GATT_CLIENT_H
