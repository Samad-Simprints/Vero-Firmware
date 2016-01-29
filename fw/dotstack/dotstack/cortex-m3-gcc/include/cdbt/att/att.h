/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __ATT_H
#define __ATT_H

/**
 * \defgroup att ATT
 * The attribute protocol allows a device referred to as the server to expose a set
 * of attributes and their associated values to a peer device referred to as the client.
 * These attributes exposed by the server can be discovered, read, and written
 * by a client, and can be indicated and notified by the server.
 *
 * \defgroup att_server ATT Server
 * \ingroup att
 */

#include "cdbt/l2cap/l2cap.h"
#include "cdbt/att/att_packet.h"
#include "cdbt/att/att_bridge.h"

#define ATT_SESSION_STATE_FREE                 0x00
#define ATT_SESSION_STATE_DISCONNECTED         0x01
#define ATT_SESSION_STATE_CONNECTED            0x02
#define ATT_SESSION_STATE_TRAN_STARTED         0x04
#define ATT_SESSION_STATE_NOTIFYING            0x08
#define ATT_SESSION_STATE_RESPONSE_PENDING     0x10
#define ATT_SESSION_STATE_PROCESSING_REQUEST   0x20

#define ATT_MGR_EVT_NOTHING                            0
#define ATT_MGR_EVT_SESSION_CONNECTED                  1
#define ATT_MGR_EVT_SESSION_DISCONNECTED               2
#define ATT_MGR_EVT_ATTR_VALUE_CHANGED_BY_SERVER       3
#define ATT_MGR_EVT_ATTR_VALUE_CHANGED_BY_CLIENT       4
#define ATT_MGR_EVT_ATTR_VALUE_NOTIFIED		           5
#define ATT_MGR_EVT_ATTR_VALUE_INDICATED               6
#define ATT_MGR_EVT_TRAN_TIMEOUT                       7
#define ATT_MGR_EVT_AUTHORIZATION_REQUESTED            8
#define ATT_MGR_EVT_ATTR_VALUE_READ_BY_CLIENT          9

#define ATT_SESSION_EVT_NOTHING                        0
#define ATT_SESSION_EVT_SESSION_CONNECTED              1
#define ATT_SESSION_EVT_SESSION_DISCONNECTED           2
#define ATT_SESSION_EVT_CONNECTION_FAILED              3
#define ATT_SESSION_EVT_DATA_RECEIVED                  4
#define ATT_SESSION_EVT_VALUE_CONFIRMATION_RECEIVED    5
#define ATT_SESSION_EVT_PACKET_SENT                    6
#define ATT_SESSION_EVT_TRAN_TIMEOUT                   7
#define ATT_SESSION_EVT_AUTHORIZATION_REQUESTED        8

#define ATT_NOTIFY_QUEUE_LEN                           2

/** \addtogroup att
* @{
*/
/** @name Error Codes
*
*/
/**@{
*/
#define ATT_ERROR_SUCCESS                            0x00    ///< The operation completed with no errors.
#define ATT_ERROR_INVALID_HANDLE                     0x01    ///< The attribute handle given was not valid on this server.
#define ATT_ERROR_READ_NOT_PERMITTED                 0x02    ///< The attribute cannot be read.
#define ATT_ERROR_WRITE_NOT_PERMITTED                0x03    ///< The attribute cannot be written.
#define ATT_ERROR_INVALID_PDU                        0x04    ///< The attribute PDU was invalid.
#define ATT_ERROR_INSUFFICIENT_AUTHENTICATION        0x05    ///< The attribute requires authentication before it can be read or written.
#define ATT_ERROR_REQUEST_NOT_SUPPORTED              0x06    ///< Attribute server does not support the request received from the client.
#define ATT_ERROR_INVALID_OFFSET                     0x07    ///< Offset specified was past the end of the attribute.
#define ATT_ERROR_INSUFFICIENT_AUTHORIZATION         0x08    ///< The attribute requires authorization before it can be read or written.
#define ATT_ERROR_PREPARE_QUEUE_FULL                 0x09    ///< Too many prepare writes have been queued.
#define ATT_ERROR_ATTRIBUTE_NOT_FOUND                0x0A    ///< No attribute found within the given attribute handle range.
#define ATT_ERROR_ATTRIBUTE_NOT_LONG                 0x0B    ///< The attribute cannot be read or written using the Read Blob Request.
#define ATT_ERROR_INSUFFICIENT_ENCRYPTION_KEY_SIZE   0x0C    ///< The Encryption Key Size used for encrypting this link is insufficient.
#define ATT_ERROR_INVALID_ATTRIBUTE_VALUE_LENGTH     0x0D    ///< The attribute value length is invalid for the operation.
#define ATT_ERROR_UNLIKELY_ERROR                     0x0E    ///< The attribute request that was requested has encountered an error
                                                             ///< that was unlikely, and therefore could
                                                             ///< not be completed as requested.
#define ATT_ERROR_INSUFFICIENT_ENCRYPTION            0x0F    ///< The attribute requires encryption before it can be read or written.
#define ATT_ERROR_UNSUPPORTED_GROUP_TYPE             0x10    ///< The attribute type is not a supported
                                                             ///< grouping attribute as defined by a
                                                             ///< higher layer specification.
#define ATT_ERROR_INSUFFICIENT_RESOURCES             0x11    ///< Insufficient Resources to complete the request.
/**@}*/ 
/**@}*/

#ifdef __cplusplus
extern "C" {
#endif

struct _bt_att_session_s;
struct _bt_att_mgr_t;

typedef void (*bt_att_session_callback_fp)(struct _bt_att_session_s* session, bt_int evt, void* evt_param, void* param);

/**
 * \brief ATT application callback.
 * \ingroup att_server
 *
 * \details In order to be notified of various events a consumer of the ATT layer has to register a callback function. 
 *          The stack will call this function whenever a new event has been generated passing the code of the event as the first parameter.
 *
 * \param evt ATT event
 * \param evt_param Event parameter. 
 *                  This can be one of the following values:
 *                    @arg ATT_SERVER_EVT_SESSION_CONNECTED                 A client connected to the server
 *                    @arg ATT_SERVER_EVT_SESSION_DISCONNECTED              A client disconnected from the server
 *                    @arg ATT_SERVER_EVT_ATTR_VALUE_CHANGED_BY_SERVER      Attribute's value has been changed locally on the server
 *                    @arg ATT_SERVER_EVT_ATTR_VALUE_CHANGED_BY_CLIENT      A client has changed the attribute's value
 *                    @arg ATT_SERVER_EVT_ATTR_VALUE_NOTIFIED               A value notification has bee sent to the client
 *                    @arg ATT_SERVER_EVT_ATTR_VALUE_INDICATED              A value indication has been sent to the client
 *                    @arg ATT_SERVER_EVT_TRAN_TIMEOUT                      Operation timed out
 *                    @arg ATT_SERVER_EVT_AUTHORIZATION_REQUESTED           Authorization is required in order to access the attribute's value
 *                    @arg ATT_SERVER_EVT_STOPPED                           Server stooped
 * \param callback_param A pointer to an arbitrary data set by a call to bt_att_server_register_callback.
 * 
 */
typedef void (*bt_att_mgr_callback_fp)(bt_int evt, void* evt_param, void* param);

typedef struct _bt_att_enum_context_t
{
	bt_int var_db_attr_offset;
	bt_int const_db_attr_offset;

} bt_att_enum_context_t;

typedef struct _bt_att_found_attribyte_t
{
	bt_byte db_type;
	bt_uint offset;

} bt_att_found_attribyte_t;

typedef struct _bt_att_attribute_t
{
	union
	{
		bt_uuid_t uuid128;
		bt_uuid16 uuid16;
	} type;
	bt_uint handle;
	bt_uint flags;
	bt_byte min_key_size;
	bt_byte* value;
	bt_uint len;
	bt_uint max_len;
	bt_int offset;
} bt_att_attribute_t;

struct _bt_att_client_session_s;
typedef struct _bt_att_session_s bt_att_session_t;

struct _bt_att_session_s 
{
	struct _bt_att_session_s* next_session;

	bt_byte state;
	bt_l2cap_channel_t* channel;
	bt_uint mtu;
	bt_att_packet_t response_packet;
	bt_att_packet_t notif_packet;

	bt_att_session_callback_fp callback;
	void* callback_param;

	bt_att_enum_context_t enum_context;
	bt_att_found_attribyte_t* found_attributes;

	bt_uint notify_queue[ATT_NOTIFY_QUEUE_LEN];
	bt_byte notify_queue_len;
	bt_uint indicate_queue[ATT_NOTIFY_QUEUE_LEN];
	bt_byte indicate_queue_len;

	bt_byte* queued_write_buffer;
	bt_uint queued_write_buffer_len;

	bt_signal_t signal;

	bt_long tran_start_time;

	bt_att_bridge_t* bridge;
};


typedef struct _bt_att_listener_t
{
	struct _bt_att_listener_t* next_listener;

	bt_att_mgr_callback_fp callback;
	void* callback_param;
} bt_att_listener_t;

// manager events

/**
* \brief  Parameter to ATT_SERVER_EVT_SESSION_CONNECTED event
* \ingroup att_server
*
* \details A pointer to this structure is passed to the ATT application callback 
*          when a client connected to the server. 
*/
typedef struct _bt_att_evt_session_connected_t
{
	bt_att_session_t* session;    ///< ATT session
} bt_att_evt_session_connected_t;

/**
* \brief  Parameter to ATT_SERVER_EVT_SESSION_DISCONNECTED event
* \ingroup att_server
*
* \details A pointer to this structure is passed to the ATT application callback 
*          when a client disconnected from the server. 
*/
typedef struct _bt_att_evt_session_disconnected_t
{
	bt_att_session_t* session;    ///< ATT session
} bt_att_evt_session_disconnected_t;

/**
* \brief  Parameter to ATT_SERVER_EVT_ATTR_VALUE_CHANGED_BY_SERVER and ATT_SERVER_EVT_ATTR_VALUE_CHANGED_BY_CLIENT events
* \ingroup att_server
*
* \details A pointer to this structure is passed to the ATT application callback
*          when the value of an attribute has been changed locally on the server or by a client. 
*/
typedef struct _bt_att_evt_attr_value_changed_s
{
	bt_att_attribute_t* attr;    ///< ATT attribute
} bt_att_evt_attr_value_changed_t;

/**
* \brief  Parameter to ATT_SERVER_EVT_ATTR_VALUE_READ event
* \ingroup att_server
*
* \details A pointer to this structure is passed to the ATT application callback
*          when the value of an attribute has been read by a client. 
*/
typedef struct _bt_att_evt_attr_value_read_s
{
	bt_att_attribute_t* attr;    ///< ATT attribute
	bt_uint value_offset;        ///< Offset from which value of the attribute has been read
} bt_att_evt_attr_value_read_t;

/**
* \brief  Parameter to ATT_SERVER_EVT_ATTR_VALUE_NOTIFIED event
* \ingroup att_server
*
* \details A pointer to this structure is passed to the ATT application callback
*          when a value notification has bee sent to the client.
*/
typedef struct _bt_att_evt_attr_notification_sent_s
{
	bt_byte status;               ///< Error code
	bt_att_session_t* session;    ///< ATT session
	bt_uint handle;               ///< Attribute handle
} bt_att_evt_attr_notification_sent_t;

/**
* \brief  Parameter to ATT_SERVER_EVT_ATTR_VALUE_INDICATED event
* \ingroup att_server
*
* \details A pointer to this structure is passed to the ATT application callback
*          when a value indication has bee sent to the client.
*/
typedef struct _bt_att_evt_attr_indication_sent_s
{
	bt_byte status;               ///< Error code
	bt_att_session_t* session;    ///< ATT session
	bt_uint handle;               ///< Attribute handle
} bt_att_evt_attr_indication_sent_t;

/**
* \brief  Parameter to ATT_SERVER_EVT_TRAN_TIMEOUT event
* \ingroup att_server
*
* \details A pointer to this structure is passed to the ATT application callback
*          if operation has timed out.
*/
typedef struct _bt_att_evt_tran_timeout_s
{
	bt_att_session_t* session;    ///< ATT session
} bt_att_evt_tran_timeout_t;

/**
* \brief  Parameter to ATT_SERVER_EVT_AUTHORIZATION_REQUESTED event
* \ingroup att_server
*
* \details A pointer to this structure is passed to the ATT application callback
*          if authorization is required in order to access the attribute's value.
*/
typedef struct _bt_att_evt_authorization_requested_t
{
	bt_att_session_t* session;    ///< ATT session
	bt_byte opcode;               ///< Operation code
	bt_att_attribute_t* attr;     ///< ATT attribute
} bt_att_evt_authorization_requested_t;

// ----------------------------------------------------------------

// session events
typedef struct _bt_att_evt_data_received_t
{
	const bt_byte* pdata;
	bt_int len;
} bt_att_evt_data_received_t;

typedef struct _bt_att_session_evt_authorization_requested_t
{
	bt_byte opcode;
	bt_att_attribute_t* attr;
} bt_att_session_evt_authorization_requested_t;

typedef struct _bt_att_session_evt_packet_sent_t
{
	bt_att_packet_t* packet;
} bt_att_session_evt_packet_sent_t;

// ----------------------------------------------------------------

/**
 * \brief ATT manager.
 * \ingroup att_server
 *
 * \details A structure that glues all pieces together. There is only one instance of this structure allocated by dotstack.
 *          A pointer to the instance can be get with \c bt_att_get_mgr().
 *          
 */
typedef struct _bt_att_mgr_t
{
//	bt_byte state;

	bt_att_listener_t* listeners;
	bt_att_session_t* sessions;

	bt_byte* var_db;
	bt_uint var_db_len;
	const bt_byte* const_db;
	bt_uint const_db_len;

} bt_att_mgr_t;

/**
 * \brief Initialize the ATT layer.
 * \ingroup att_server
 *
 * \details This function initializes the ATT layer of the stack. It must be called prior to any other
 *          ATT function can be called.
 */
bt_bool bt_att_init(void);

/**
 * \brief Return a pointer to an instance of the ATT manager.
 * \ingroup att_server
 *
 * \details This function returns a pointer to an instance of the ATT manager. There is only one instance of the manager allocated by the stack. 
 */
bt_att_mgr_t* bt_att_get_mgr(void);

bt_bool bt_att_register_listener(bt_att_listener_t* listener);
void bt_att_unregister_listener(bt_att_listener_t* listener);

/**
 * \brief Request connection parameters update
 * \ingroup att_server
 *
 * \details This function sends a request to the client (central) to update connection parameters.
 *
 * \param session ATT session.
 * \param min_interval Minimum connection interval expressed in 1.25ms units.
 * \param max_interval Maximum connection interval expressed in 1.25ms units.
 * \param slave_latency Slave latency expressed as number of connection events.
 * \param supervision_timeout Link supervision timeout expressed in 10ms units.
 * 
 * \return
 *        \li \c TRUE if request has been sent to the central.
 *        \li \c FALSE otherwise.
 */
bt_bool bt_att_update_conn_parameters(
	bt_att_session_t* session,
	bt_uint min_interval, bt_uint max_interval,
	bt_uint slave_latency, bt_uint supervision_timeout);

/**
* \brief Get the address of the remote device this device is connected to.
* \ingroup att_server
*
* \param session ATT session.
*
* \return
*        \li \c A pointer to bt_bdaddr structure that contains the address of the remote device.
*/
bt_bdaddr_t* bt_att_get_remote_address(const bt_att_session_t* session);

#include "cdbt/att/att_private.h"
#include "cdbt/utils/uuid.h"

#ifdef __cplusplus
}
#endif

#endif // __ATT_H
