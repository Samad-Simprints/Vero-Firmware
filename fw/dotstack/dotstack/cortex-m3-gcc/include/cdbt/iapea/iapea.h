/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __IAPEA_H
#define __IAPEA_H

#include "cdbt/iap/iap.h"
#include "cdbt/iap2/iap2.h"

/**
* \defgroup iapea IAP External Accessory
*/

#ifdef __cplusplus
extern "C" {
#endif

#define IAP_EA_IDENTIFY_STATUS_SUCCESS       0
#define IAP_EA_IDENTIFY_STATUS_FAILED        5

#define IAP_EA_AUTHENTICATION_STATUS_SUCCESS 0

typedef enum _iapea_accessory_state_e 
{
	IAP_EA_SESSION_STATE_FREE,            /**< Used internally to mark a session structure
										       as available for allocation. The application
										       should never encounter a session in this state. */
	IAP_EA_SESSION_STATE_DISCONNECTED,    /**< Session is not connected. */
	IAP_EA_SESSION_STATE_DISCONNECTING,   /**< Disconnecting from remote device. */
	IAP_EA_SESSION_STATE_CONNECTING,      /**< Connecting to a remote device. */
	IAP_EA_SESSION_STATE_CONNECTED        /**< Session is connected to a remote device. */
} iapea_accessory_state_e;

typedef enum _iapea_accessory_event_e
{
	IAP_EA_SESSION_EVENT_CONNECTED = 1,
	IAP_EA_SESSION_EVENT_DISCONNECTED,
	IAP_EA_SESSION_EVENT_CONNECTION_FAILED,
	IAP_EA_SESSION_EVENT_IDENTIFY_REQUESTED,
	IAP_EA_SESSION_EVENT_IDENTIFY_COMPLETED,
	IAP_EA_SESSION_EVENT_AUTHENTICATION_COMPLETED,
	IAP_EA_SESSION_EVENT_START_EA_SESSION,
	IAP_EA_SESSION_EVENT_STOP_EA_SESSION,
	IAP_EA_SESSION_EVENT_EA_SESSION_DATAGRAM,
} iapea_accessory_event_e;

/** Send operation status. */
typedef enum _iapea_send_status_e
{
	IAP_EA_SEND_STATUS_SUCCESS = 0,          /**< The operation completed successfuly. */
	IAP_EA_SEND_STATUS_TIMEOUT,              /**< The operation timed out. */
	IAP_EA_SEND_STATUS_NOT_ENOUGH_RESOURCES, /**< There was not enough resources to complete the operation. */
	IAP_EA_SEND_STATUS_CANCELED,             /**< There operation was canceled. */
	IAP_EA_SEND_STATUS_FAILED                /**< The iPod acknowledged data transfer with error code. */

} iapea_send_status_e;

typedef struct _iapea_evt_identify_completed_t
{
	bt_byte status;
	bt_byte ipod_status;
} iapea_evt_identify_completed_t;


typedef struct _iapea_evt_authentication_completed_s
{
	bt_byte status;
	bt_byte ipod_status;
} iapea_evt_authentication_completed_t;

typedef struct _iapea_evt_start_ea_session_t
{
	bt_byte status;
	bt_uint session_id;
	bt_byte protocol_id;
} iapea_evt_start_ea_session_t;

typedef struct _iapea_evt_stop_ea_session_t
{
	bt_uint session_id;
} iapea_evt_stop_ea_session_t;

typedef struct _iapea_evt_ea_session_datagram_t
{
	bt_uint session_id;
	bt_byte* data;
	bt_uint data_len;
} iapea_evt_ea_session_datagram_t;

typedef union _iapea_accessory_event_s
{
	iapea_evt_authentication_completed_t auth_completed;
	iapea_evt_identify_completed_t       identify_completed;
	iapea_evt_start_ea_session_t         start_ea_session;
	iapea_evt_stop_ea_session_t          stop_ea_session;
	iapea_evt_ea_session_datagram_t      ea_datagram;

} iapea_accessory_event_t;

typedef struct _iapea_accessory_s iapea_accessory_t;

typedef void (*iapea_session_callback_fp)(iapea_accessory_t* session, iapea_accessory_event_e evt, iapea_accessory_event_t* evt_param, void* callback_param);

typedef void (*iapea_send_callback_fp)(iapea_accessory_t* session, bt_uint data_session_id, bt_ulong bytes_sent, iapea_send_status_e status, void* param);

typedef struct _iapea_id_info_s
{
	const bt_char*                   name;
	const bt_char*                   model;
	const bt_char*                   manufacturer;
	const bt_char*                   serial_number;
	const bt_char*                   firmware_version;
	const bt_char*                   hardware_version;

	iap2_ext_accessory_protocol_t*   ext_accessory_protocols;
	bt_byte                          ext_accessory_protocols_count;

	const bt_char*                   preferred_app_bundle_seed_id;
	const bt_char*                   current_language;
	const bt_char**                  supported_language;
	bt_byte                          supported_language_count;

	bt_ulong                         in_max_packet_size;

} iapea_id_info_t;

struct _iapea_accessory_s
{
	iapea_accessory_state_e   state;
	bt_byte                   protocol_version;

	iap_session_t*            iap1_session;
	iap_session_callback_fp   iap1_session_callback;

	iap2_session_t*           iap2_session;
	iap2_session_callback_fp  iap2_session_callback;

	iapea_session_callback_fp session_callback;
	void*                     session_callback_param;

	iapea_send_callback_fp    send_callback;
};

bt_bool iapea_init(void);

iapea_accessory_t* iapea_allocate_accessory(
	iap_transport_t* transport, 
	iapea_session_callback_fp session_callback, void* callback_param);

void iapea_free_session(iapea_accessory_t* session);

bt_bool iapea_identify_accessory(iapea_accessory_t* session, const iapea_id_info_t* id_info);

bt_bool iapea_identify_accessory2(iapea_accessory_t* session, const iapea_id_info_t* id_info);

bt_bool iapea_listen(iapea_accessory_t* session, const void* transport_listen_params);

bt_bool iapea_connect(iapea_accessory_t* session, const void* transport_connect_params);

void iapea_disconnect(iapea_accessory_t* session);

bt_bool iapea_send(iapea_accessory_t* session, bt_uint session_id, bt_byte* data, bt_uint data_len, iapea_send_callback_fp callback);

#ifdef __cplusplus
}
#endif

#include "cdbt/iapea/iapea_private.h"

#endif // __IAPEA_H
