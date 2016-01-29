/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __ATT_CLIENT_H
#define __ATT_CLIENT_H

/**
 * \defgroup att_client ATT Client
 * \ingroup att
 * 
 *  This module describes functions and data structures used to implement an ATT client (central).
 */

#include "cdbt/l2cap/l2cap.h"
#include "cdbt/sm/sm.h"

#define ATT_CLIENT_SESSION_STATE_FREE                 0x00
#define ATT_CLIENT_SESSION_STATE_DISCONNECTED         0x01
#define ATT_CLIENT_SESSION_STATE_CONNECTED            0x02
#define ATT_CLIENT_SESSION_STATE_TRAN_STARTED         0x04
#define ATT_CLIENT_SESSION_STATE_CONNECTING           0x08
#define ATT_CLIENT_SESSION_STATE_DISCONNECTING        0x10
#define ATT_CLIENT_SESSION_STATE_PACKET_PENDING       0x20
#define ATT_CLIENT_SESSION_STATE_AUTHENTICATING       0x40

#define ATT_CLIENT_SESSION_EVT_NOTHING                        0
/** \addtogroup att_client
* @{
*/
/** @name Events
*
*/
/**@{
*/
#define ATT_CLIENT_SESSION_EVT_SESSION_CONNECTED              1     ///< This event is generated when client connected to the server.
#define ATT_CLIENT_SESSION_EVT_SESSION_DISCONNECTED           2     ///< This event is generated when client disconnected from the server.
#define ATT_CLIENT_SESSION_EVT_CONNECTION_FAILED              3     ///< This event is generated when client failed to connect to the server.
#define ATT_CLIENT_SESSION_EVT_TRAN_TIMEOUT                   7     ///< This event is generated if operation (find, read, write) timed out.

#define ATT_CLIENT_EVT_EXCHANGE_MTU_RESPONSE                  10    ///< This event is generated when client received a response (either positive or negative) to a "exchange MTU" request.
#define ATT_CLIENT_EVT_FIND_INFO_RESPONSE                     11    ///< This event is generated when client received a response (either positive or negative) to a "find info" request.
#define ATT_CLIENT_EVT_FIND_BY_TYPE_VALUE_RESPONSE            12    ///< This event is generated when client received a response (either positive or negative) to a "find by type value" request.
#define ATT_CLIENT_EVT_READ_BY_TYPE_RESPONSE                  13    ///< This event is generated when client received a response (either positive or negative) to a "read by type" request.
#define ATT_CLIENT_EVT_READ_RESPONSE                          14    ///< This event is generated when client received a response (either positive or negative) to a "read" request.
#define ATT_CLIENT_EVT_READ_BLOB_RESPONSE                     15    ///< This event is generated when client received a response (either positive or negative) to a "read blob" request.
#define ATT_CLIENT_EVT_READ_MULTIPLE_RESPONSE                 16    ///< This event is generated when client received a response (either positive or negative) to a "read multiple" request.
#define ATT_CLIENT_EVT_READ_BY_GROUP_TYPE_RESPONSE            17    ///< This event is generated when client received a response (either positive or negative) to a "read by group type" request.

#define ATT_CLIENT_EVT_WRITE_RESPONSE                         18    ///< This event is generated when client received a response (either positive or negative) to a "write" request.
#define ATT_CLIENT_EVT_PREPARE_WRITE_RESPONSE                 19    ///< This event is generated when client received a response (either positive or negative) to a "preapare write" request.
#define ATT_CLIENT_EVT_EXECUTE_WRITE_RESPONSE                 20    ///< This event is generated when client received a response (either positive or negative) to a "execute write" request.

#define ATT_CLIENT_EVT_VALUE_NOTIFICATION                     21    ///< This event is generated when client received attribute value notification.
#define ATT_CLIENT_EVT_VALUE_INDICATION                       22    ///< This event is generated when client received attribute value indication.

#define ATT_CLIENT_EVT_CONN_PARAM_UPDATE_REQUEST              50    ///< This event is generated when client received a "connection parameter update" request.
#define ATT_CLIENT_EVT_CONN_PARAM_UPDATE_COMPLETED            51    ///< This event is generated after the new connection parameters have been set.
/**@}*/ 
/**@}*/

#define ATT_CLIENT_UUID_LEN_16     0
#define ATT_CLIENT_UUID_LEN_128    1

#define ATT_CHAR_PROP_BOADCAST              0x01
#define ATT_CHAR_PROP_READ                  0x02
#define ATT_CHAR_PROP_WRITE_NO_RESPONSE     0x04
#define ATT_CHAR_PROP_WRITE                 0x08
#define ATT_CHAR_PROP_NOTIFY                0x10
#define ATT_CHAR_PROP_INDICATE              0x20
#define ATT_CHAR_PROP_AUTH_SIGNED_WRITES    0x40
#define ATT_CHAR_PROP_EXT_PROPERTIES        0x80


#ifdef __cplusplus
extern "C" {
#endif

typedef struct _bt_att_find_info_response_s bt_att_find_info_response_t;
typedef struct _bt_att_find_by_type_value_response_s bt_att_find_by_type_value_response_t;
typedef struct _bt_att_read_by_type_response_s bt_att_read_by_type_response_t;
typedef struct _bt_att_read_by_group_type_response_s bt_att_read_by_group_type_response_t;

#include "cdbt/att/att_client_packet.h"
#include "cdbt/att/att_bridge.h"

/**
* \brief  Structure to store response to a "find info" request.
* \ingroup att_client
*
* \details An array of this structures is passed to bt_att_client_find_info().
*          It is application's responsibility to allocate memory for the array. 
*          When response is received the actual number of valid entries in the array
*          is stored in bt_att_client_evt_info_response_t::count.
*/
struct _bt_att_find_info_response_s
{
	bt_byte format;          ///< Length of the attribute UUID
	                         ///<   @arg 0x01 = 16-bit UUID
                             ///<   @arg 0x02 = 128-bit UUID
	bt_uint handle;          ///< Attribute handle
	union
	{
		bt_uint uuid16;      ///< 16-bit UUID
		bt_uuid_t uuid128;   ///< 128-bit UUID
	} type;                  ///< Attribute UUID
};

/**
* \brief  Structure to store response to a "find by type value" request.
* \ingroup att_client
*
* \details An array of this structures is passed to bt_att_client_find_by_type_value().
*          It is application's responsibility to allocate memory for the array. 
*          When response is received the actual number of valid entries in the array
*          is stored in bt_att_client_evt_find_by_type_value_response_t::count.
*/
struct _bt_att_find_by_type_value_response_s
{
	bt_uint handle;           ///< Found Attribute Handle
	bt_uint group_end_handle; ///< Group End Handle
};

/**
* \brief  Structure to store response to a "read by type" request.
* \ingroup att_client
*
* \details An array of this structures is passed to bt_att_client_read_by_type().
*          It is application's responsibility to allocate memory for the array. 
*          When response is received the actual number of valid entries in the array
*          is stored in bt_att_client_evt_read_by_type_response_t::count.
*/
struct _bt_att_read_by_type_response_s
{
	bt_uint handle;          ///< Attribute handle
	const bt_byte* value;    ///< Attribute value
	bt_byte len;             ///< Attribute value length
};

/**
* \brief  Structure to store response to a "read by group type" request.
* \ingroup att_client
*
* \details An array of this structures is passed to bt_att_client_read_by_group_type().
*          It is application's responsibility to allocate memory for the array. 
*          When response is received the actual number of valid entries in the array
*          is stored in bt_att_client_evt_read_by_group_type_response_t::count.
*/
struct _bt_att_read_by_group_type_response_s
{
	bt_uint handle;           ///< Attribute handle
	bt_uint end_group_handle; ///< Group End Handle
	const bt_byte* value;     ///< Attribute value
	bt_byte len;              ///< Attribute value length
};

/**
* \brief  Parameter to ATT_CLIENT_EVT_EXCHANGE_MTU_RESPONSE event
* \ingroup att_client
*
* \details A pointer to this structure is passed to the ATT client application callback 
*          when client received a response (either positive or negative) to a "exchange MTU" request.
*/
typedef struct _bt_att_client_evt_mtu_response_s
{
	bt_byte status;          ///< Operation status
	bt_uint mtu;             ///< Server's MTU
} bt_att_client_evt_mtu_response_t;

/**
* \brief  Parameter to ATT_CLIENT_EVT_FIND_INFO_RESPONSE event
* \ingroup att_client
*
* \details A pointer to this structure is passed to the ATT client application callback 
*          when client received a response (either positive or negative) to a "find info" request.
*/
typedef struct _bt_att_client_evt_info_response_s
{
	bt_byte status;          ///< Operation status
	bt_uint start_handle;    ///< First attribute handle found
	bt_uint end_handle;      ///< Last attribute handle found
	bt_uint count;           ///< The number of attribute handles found
} bt_att_client_evt_info_response_t;

/**
* \brief  Parameter to ATT_CLIENT_EVT_FIND_BY_TYPE_VALUE_RESPONSE event
* \ingroup att_client
*
* \details A pointer to this structure is passed to the ATT client application callback 
*          when client received a response (either positive or negative) to a "find by type value info" request.
*/
typedef struct _bt_att_client_evt_find_by_type_value_response_s
{
	bt_byte status;          ///< Operation status
	bt_uint start_handle;    ///< First attribute handle found
	bt_uint end_handle;      ///< Last attribute handle found
	bt_uint count;           ///< The number of attribute handles found
} bt_att_client_evt_find_by_type_value_response_t;

/**
* \brief  Parameter to ATT_CLIENT_EVT_READ_BY_TYPE_RESPONSE event
* \ingroup att_client
*
* \details A pointer to this structure is passed to the ATT client application callback 
*          when client received a response (either positive or negative) to a "read by type" request.
*/
typedef struct _bt_att_client_evt_read_by_type_response_s
{
	bt_byte status;          ///< Operation status
	bt_uint start_handle;    ///< First attribute handle found
	bt_uint end_handle;      ///< Last attribute handle found
	bt_uint count;           ///< The number of attribute handles found
} bt_att_client_evt_read_by_type_response_t;

/**
* \brief  Parameter to ATT_CLIENT_EVT_READ_RESPONSE event
* \ingroup att_client
*
* \details A pointer to this structure is passed to the ATT client application callback 
*          when client received a response (either positive or negative) to a "read" request.
*/
typedef struct _bt_att_client_evt_read_response_s
{
	bt_byte status;          ///< Operation status
	bt_uint handle;          ///< Attribute handle
	const bt_byte* value;    ///< Attribute value
	bt_uint len;             ///< Attribute value length
} bt_att_client_evt_read_response_t;

/**
* \brief  Parameter to ATT_CLIENT_EVT_READ_BLOB_RESPONSE event
* \ingroup att_client
*
* \details A pointer to this structure is passed to the ATT client application callback 
*          when client received a response (either positive or negative) to a "read blob" request.
*/
typedef struct _bt_att_client_evt_read_blob_response_s
{
	bt_byte status;          ///< Operation status
	bt_uint handle;          ///< Attribute handle
	bt_uint offset;          ///< The offset of the first octet.
	const bt_byte* value;    ///< Part of the attribute value
	bt_uint len;             ///< Part of the attribute value length
} bt_att_client_evt_read_blob_response_t;

/**
* \brief  Parameter to ATT_CLIENT_EVT_READ_MULTIPLE_RESPONSE event
* \ingroup att_client
*
* \details A pointer to this structure is passed to the ATT client application callback 
*          when client received a response (either positive or negative) to a "read multiple" request.
*/
typedef struct _bt_att_client_evt_read_multiple_response_s
{
	bt_byte status;          ///< Operation status
	union
	{
		struct
		{
			const bt_uint* handles;
			bt_uint count;
		} handle_list;
		
		bt_uint err_handle;
	} h;

	const bt_byte* values;   ///< Attribute values
	bt_uint len;             ///< The length of attribute values
} bt_att_client_evt_read_multiple_response_t;

/**
* \brief  Parameter to ATT_CLIENT_EVT_READ_BY_GROUP_TYPE_RESPONSE event
* \ingroup att_client
*
* \details A pointer to this structure is passed to the ATT client application callback 
*          when client received a response (either positive or negative) to a "read by group type" request.
*/
typedef struct _bt_att_client_evt_read_by_group_type_response_s
{
	bt_byte status;          ///< Operation status
	bt_uint start_handle;    ///< First attribute handle found
	bt_uint end_handle;      ///< Last attribute handle found
	bt_uint count;           ///< The number of attribute handles found
} bt_att_client_evt_read_by_group_type_response_t;

/**
* \brief  Parameter to ATT_CLIENT_EVT_WRITE_RESPONSE event
* \ingroup att_client
*
* \details A pointer to this structure is passed to the ATT client application callback 
*          when client received a response (either positive or negative) to a "write" request.
*/
typedef struct _bt_att_client_evt_write_response_s
{
	bt_byte status;          ///< Operation status
	bt_uint handle;          ///< The attribute handle
} bt_att_client_evt_write_response_t;

/**
* \brief  Parameter to ATT_CLIENT_EVT_PREPARE_WRITE_RESPONSE event
* \ingroup att_client
*
* \details A pointer to this structure is passed to the ATT client application callback 
*          when client received a response (either positive or negative) to a "prepare write" request.
*/
typedef struct _bt_att_client_evt_prepare_write_response_s
{
	bt_byte status;          ///< Operation status
	bt_uint err_handle;      ///< The attribute handle that generated error response
} bt_att_client_evt_prepare_write_response_t;

/**
* \brief  Parameter to ATT_CLIENT_EVT_EXECUTE_WRITE_RESPONSE event
* \ingroup att_client
*
* \details A pointer to this structure is passed to the ATT client application callback 
*          when client received a response (either positive or negative) to a "execute write" request.
*/
typedef struct _bt_att_client_evt_execute_write_response_s
{
	bt_byte status;          ///< Operation status
	bt_uint err_handle;      ///< The attribute handle that generated error response
} bt_att_client_evt_execute_write_response_t;

/**
* \brief  Parameter to ATT_CLIENT_EVT_VALUE_NOTIFICATION event
* \ingroup att_client
*
* \details A pointer to this structure is passed to the ATT client application callback 
*          when client received attribute value notification.
*/
typedef struct _bt_att_client_evt_value_notification_s
{
	bt_uint handle;          ///< Attribute handle
	const bt_byte* value;    ///< Attribute value
	bt_uint len;             ///< Attribute value length
} bt_att_client_evt_value_notification_t;

/**
* \brief  Parameter to ATT_CLIENT_EVT_VALUE_INDICATION event
* \ingroup att_client
*
* \details A pointer to this structure is passed to the ATT client application callback 
*          when client received attribute value indication.
*/
typedef struct _bt_att_client_evt_value_indication_s
{
	bt_uint handle;          ///< Attribute handle
	const bt_byte* value;    ///< Attribute value
	bt_uint len;             ///< Attribute value length
} bt_att_client_evt_value_indication_t;

/**
* \brief  Parameter to ATT_CLIENT_EVT_CONN_PARAM_UPDATE_REQUEST event
* \ingroup att_client
*
* \details A pointer to this structure is passed to the ATT client application callback 
*          when client received update connection parameters request.
*/
typedef struct _bt_att_client_evt_conn_param_update_s
{
	bt_uint min_interval;           ///< Minimum connection interval expressed in 1.25ms units
	bt_uint max_interval;           ///< Maximum connection interval expressed in 1.25ms units
	bt_uint slave_latency;          ///< Slave latency expressed as number of connection events
	bt_uint supervision_timeout;    ///< Link supervision timeout expressed in 10ms units.
} bt_att_client_evt_conn_param_update_t;

/**
* \brief  Parameter to ATT_CLIENT_EVT_CONN_PARAM_UPDATE_COMPLETED event
* \ingroup att_client
*
* \details A pointer to this structure is passed to the ATT client application callback 
*          after the new connection parameters have been set.
*/
typedef struct _bt_att_client_evt_conn_param_update_completed_s
{
	bt_byte status;          ///< Operation status
	bt_int hci_status;       ///< HCI command status
} bt_att_client_evt_conn_param_update_completed_t;

/**
* \brief  Parameter to ATT client application callback.
* \ingroup att_client
*
* \details This union is used to pass event specific data to the ATT client consumer. Which member of the union 
*          points to a valid structure depends on the event reported to the consumer. In general, each event
*          has a corresponding member in the union.
*/
typedef union _bt_att_client_evt_u
{
	bt_att_client_evt_mtu_response_t                mtu;                           ///< Valid if event is ATT_CLIENT_EVT_EXCHANGE_MTU_RESPONSE
	bt_att_client_evt_info_response_t               find_info;                     ///< Valid if event is ATT_CLIENT_EVT_FIND_INFO_RESPONSE
	bt_att_client_evt_find_by_type_value_response_t find_by_type_value;            ///< Valid if event is ATT_CLIENT_EVT_FIND_BY_TYPE_VALUE_RESPONSE
	bt_att_client_evt_read_by_type_response_t       read_by_type;                  ///< Valid if event is ATT_CLIENT_EVT_READ_BY_TYPE_RESPONSE
	bt_att_client_evt_read_response_t               read;                          ///< Valid if event is ATT_CLIENT_EVT_READ_RESPONSE
	bt_att_client_evt_read_blob_response_t          read_blob;                     ///< Valid if event is ATT_CLIENT_EVT_READ_BLOB_RESPONSE
	bt_att_client_evt_read_multiple_response_t      read_multiple;                 ///< Valid if event is ATT_CLIENT_EVT_READ_MULTIPLE_RESPONSE
	bt_att_client_evt_read_by_group_type_response_t read_by_group_type;            ///< Valid if event is ATT_CLIENT_EVT_READ_BY_GROUP_TYPE_RESPONSE
	bt_att_client_evt_write_response_t              write;                         ///< Valid if event is ATT_CLIENT_EVT_WRITE_RESPONSE
	bt_att_client_evt_prepare_write_response_t      prepare_write;                 ///< Valid if event is ATT_CLIENT_EVT_PREPARE_WRITE_RESPONSE
	bt_att_client_evt_execute_write_response_t      execute_write;                 ///< Valid if event is ATT_CLIENT_EVT_EXECUTE_WRITE_RESPONSE
	bt_att_client_evt_value_notification_t          value_notification;            ///< Valid if event is ATT_CLIENT_EVT_VALUE_NOTIFICATION
	bt_att_client_evt_value_indication_t            value_indication;              ///< Valid if event is ATT_CLIENT_EVT_VALUE_INDICATION
	bt_att_client_evt_conn_param_update_t           conn_param_update;             ///< Valid if event is ATT_CLIENT_EVT_CONN_PARAM_UPDATE_REQUEST
	bt_att_client_evt_conn_param_update_completed_t conn_param_update_completed;   ///< Valid if event is ATT_CLIENT_EVT_CONN_PARAM_UPDATE_COMPLETED
} bt_att_client_evt_t;

typedef struct _bt_att_client_session_s bt_att_client_session_t;
typedef void (*bt_att_client_session_callback_fp)(bt_att_client_session_t* session, bt_int evt, bt_att_client_evt_t* evt_param, void* param);

typedef void (*bt_att_client_err_handler_fp)(bt_att_client_session_t* session, bt_uint handle, bt_byte errcode);

struct _bt_att_session_s;
struct _bt_att_bridge_s;

struct _bt_att_client_session_s
{
	bt_byte state;
	bt_l2cap_channel_t* channel;
	bt_uint mtu;
	bt_uint pending_value_confirmation;
	bt_att_client_packet_t packet;

	bt_att_client_session_callback_fp callback;
	void* callback_param;

	bt_long tran_start_time;

	struct
	{
		bt_byte result;
		bt_byte cmd_id;
		bt_att_client_evt_conn_param_update_t evt;
		bt_uint min_ce_length;
		bt_uint max_ce_length;
	} conn_param_update;

	struct _bt_att_bridge_s* bridge;

	bt_sm_listener_t sm_listener;
	bt_att_client_err_handler_fp last_error_handler;
	bt_uint last_error_handle;
	bt_byte last_error_errcode;
};

typedef struct _bt_att_client_mgr_t
{
	bt_att_client_session_t* sessions;

} bt_att_client_mgr_t;

/**
 * \brief Initialize the ATT client.
 * \ingroup att_client
 *
 * \details This function initializes the ATT client of the stack. It must be called prior to any other
 *          ATT client function can be called.
 */
bt_bool bt_att_client_init(void);

/**
 * \brief Return a pointer to an instance of the ATT client manager.
 * \ingroup att_client
 *
 * \details This function returns a pointer to an instance of the ATT client manager. There is only one instance of the manager allocated by the stack. 
 *
 * \return
 *         \li A pointer to the ATT client manager.
 */
bt_att_client_mgr_t* bt_att_get_client_mgr(void);

/**
 * \brief Allocate ATT client session
 * \ingroup att_client
 *
 * \details This function allocates a new ATT client session.
 *
 * \param callback The callback function that will be called when the ATT client generates an event.
 * \param callback_param A pointer to arbitrary data to be passed to the \c callback callback.
 *
 * \return
 *         \li A pointer to the new ATT client session structure if the function succeeds.
 *         \li \c NULL otherwise.
 */
bt_att_client_session_t* bt_att_client_allocate_session(bt_att_client_session_callback_fp callback, void* callback_param);

/**
 * \brief Destroy ATT client session.
 * \ingroup att_client
 *
 * \details This function frees memory used by the session. The session has to exist and be in the "idle" state for this function to succeed. 
 *          I.e. the session has to be disconnected before this function can be called.
 *
 * \param session ATT session.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
*/
void bt_att_client_free_session(bt_att_client_session_t* session);

/**
 * \brief Connect to a remote device (peripheral).
 * \ingroup att_client
 *
 * \details This function establishes a connection to a remote device specified by the \p addr. If connection
 *          cannot be initiated for some reason, for example, there is not enough resources, it returns FALSE and no events are generated. Otherwise
 *          the result of an attempt to connect to the remote device is reported via the ATT client callback. 
 *          The events generated will either be ATT_CLIENT_SESSION_EVT_SESSION_CONNECTED or ATT_CLIENT_SESSION_EVT_CONNECTION_FAILED.
 *
 * \param session ATT session.
 * \param addr The address of a remote device.
 * 
 * \return
 *        \li \c TRUE if connection establishment has been started.
 *        \li \c FALSE otherwise.
*/
bt_bool bt_att_client_connect(bt_att_client_session_t* session, bt_bdaddr_t* addr);

/**
 * \brief Disconnect from a remote device.
 * \ingroup att_client
 *
 * \details This function closes a connection to a remote device. After the connection has been terminated 
 *          the ATT client callback is called with ATT_CLIENT_SESSION_EVT_SESSION_CONNECTED event.
 *
 * \param session ATT session.
 * 
 * \return
 *        \li \c TRUE if disconnection has been started.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_att_client_disconnect(bt_att_client_session_t* session);

bt_bool bt_att_client_attach_connection(bt_att_client_session_t* session, struct _bt_att_session_s* server_session);

bt_bool bt_att_client_detach_connection(bt_att_client_session_t* session);

/**
 * \brief Exchange MTU.
 * \ingroup att_client
 *
 * \details This function informs the server about the client's MTU. In response to the "exchange MTU" request 
 *         the server sends its MTU to the client.
 *
 * \param session ATT session.
 * \param mtu Client's MTU.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_att_client_exchange_mtu(bt_att_client_session_t* session, bt_uint mtu);

/**
 * \brief Find Information.
 * \ingroup att_client
 *
 * \details This function sends a "find information request to the client. ATT_CLIENT_EVT_FIND_INFO_RESPONSE event is generated
 *          when the response from the client has been received.
 *
 * \param session ATT session.
 * \param start_handle First requested handle number.
 * \param end_handle Last requested handle number.
 * \param result Pointer to a buffer where response will be stored.
 * \param max_result The maximum number of responses that \p result can store.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_att_client_find_info(
	bt_att_client_session_t* session, 
	bt_uint start_handle, bt_uint end_handle,
	bt_att_find_info_response_t* result, bt_uint max_results);

/**
 * \brief Find By Type Value.
 * \ingroup att_client
 *
 * \details This function sends a "find by type value request to the client. ATT_CLIENT_EVT_FIND_BY_TYPE_VALUE_RESPONSE event is generated
 *          when the response from the client has been received.
 *
 * \param session ATT session.
 * \param start_handle First requested handle number.
 * \param end_handle Last requested handle number.
 * \param type Attribute type.
 * \param value Attribute value to find.
 * \param len Length of the \p value.
 * \param result Pointer to a buffer where response will be stored.
 * \param max_result The maximum number of responses that \p result can store.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_att_client_find_by_type_value(
	bt_att_client_session_t* session, 
	bt_uint start_handle, bt_uint end_handle,
	bt_uint type, const bt_byte* value, bt_uint len,
	bt_att_find_by_type_value_response_t* result, bt_uint max_results);

/**
 * \brief Read By Type (16-bit).
 * \ingroup att_client
 *
 * \details This function sends a "read by type request to the client. ATT_CLIENT_EVT_READ_BY_TYPE_RESPONSE event is generated
 *          when the response from the client has been received.
 *
 * \param session ATT session.
 * \param start_handle First requested handle number.
 * \param end_handle Last requested handle number.
 * \param type 16-bit attribute type.
 * \param result Pointer to a buffer where response will be stored.
 * \param max_result The maximum number of responses that \p result can store.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_att_client_read_by_type(
	bt_att_client_session_t* session, 
	bt_uint start_handle, bt_uint end_handle,
	bt_uint type,
	bt_att_read_by_type_response_t* result, bt_uint max_results);

/**
 * \brief Read By Type (128-bit).
 * \ingroup att_client
 *
 * \details This function sends a "read by type request to the client. ATT_CLIENT_EVT_READ_BY_TYPE_RESPONSE event is generated
 *          when the response from the client has been received.
 *
 * \param session ATT session.
 * \param start_handle First requested handle number.
 * \param end_handle Last requested handle number.
 * \param type 128-bit attribute type.
 * \param result Pointer to a buffer where response will be stored.
 * \param max_result The maximum number of responses that \p result can store.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_att_client_read_by_type_80(
	bt_att_client_session_t* session, 
	bt_uint start_handle, bt_uint end_handle,
	bt_uuid_t* type,
	bt_att_read_by_type_response_t* result, bt_uint max_results);

/**
 * \brief Read Attribute value.
 * \ingroup att_client
 *
 * \details This function sends a "read request to the client. ATT_CLIENT_EVT_READ_RESPONSE event is generated
 *          when the response from the client has been received.
 *
 * \param session ATT session.
 * \param handle The handle of the attribute to be read.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_att_client_read(
   bt_att_client_session_t* session, bt_uint handle);

/**
 * \brief Read Blob.
 * \ingroup att_client
 *
 * \details This function sends a "read blob request to the client. ATT_CLIENT_EVT_READ_BLOB_RESPONSE event is generated
 *          when the response from the client has been received.
 *
 * \param session ATT session.
 * \param handle The handle of the attribute to be read.
 * \param offset The offset of the first octet to be read.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_att_client_read_blob(
	bt_att_client_session_t* session, bt_uint handle, bt_uint offset);

/**
 * \brief Read Multiple.
 * \ingroup att_client
 *
 * \details This function sends a "read blob request to the client. ATT_CLIENT_EVT_READ_MULTIPLE_RESPONSE event is generated
 *          when the response from the client has been received.
 *
 * \param session ATT session.
 * \param handles The list of two or more handles.
 * \param count The number of handles in \p handles.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_att_client_read_multiple(
	bt_att_client_session_t* session, 
	const bt_uint* handles, bt_uint count);

/**
 * \brief Read By Group Type (16-bit).
 * \ingroup att_client
 *
 * \details This function sends a "read by group type request to the client. ATT_CLIENT_EVT_READ_BY_GROUP_TYPE_RESPONSE event is generated
 *          when the response from the client has been received.
 *
 * \param session ATT session.
 * \param start_handle First requested handle number.
 * \param end_handle Last requested handle number.
 * \param group_type 16-bit Attribute Group Type.
 * \param result Pointer to a buffer where response will be stored.
 * \param max_result The maximum number of responses that \p result can store.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_att_client_read_by_group_type(
	bt_att_client_session_t* session, 
	bt_uint start_handle, bt_uint end_handle,
	bt_uint group_type,
	bt_att_read_by_group_type_response_t* result, bt_uint max_results);

/**
 * \brief Read By Group Type (128-bit).
 * \ingroup att_client
 *
 * \details This function sends a "read by group type request to the client. ATT_CLIENT_EVT_READ_BY_GROUP_TYPE_RESPONSE event is generated
 *          when the response from the client has been received.
 *
 * \param session ATT session.
 * \param start_handle First requested handle number.
 * \param end_handle Last requested handle number.
 * \param group_type 128-bit Attribute Group Type.
 * \param result Pointer to a buffer where response will be stored.
 * \param max_result The maximum number of responses that \p result can store.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_att_client_read_by_group_type_80(
	bt_att_client_session_t* session, 
	bt_uint start_handle, bt_uint end_handle,
	bt_uuid_t* group_type,
	bt_att_read_by_group_type_response_t* result, bt_uint max_results);

/**
 * \brief Write Attribute Value.
 * \ingroup att_client
 *
 * \details This function sends a "write request to the client. ATT_CLIENT_EVT_WRITE_RESPONSE event is generated
 *          when the response from the client has been received.
 *
 * \param session ATT session.
 * \param handle The handle of the attribute to be written.
 * \param value The value to be written to the attribute.
 * \param len The length of the \p value.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_att_client_write(
	bt_att_client_session_t* session, bt_uint handle,
	const bt_byte* value, bt_uint len);

/**
 * \brief Write Command.
 * \ingroup att_client
 *
 * \details This function sends a "write command request to the client. No event is generated.
 *
 * \param session ATT session.
 * \param handle The handle of the attribute to be written.
 * \param value The value to be written to the attribute.
 * \param len The length of the \p value.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_att_client_write_cmd(
	bt_att_client_session_t* session, bt_uint handle,
	const bt_byte* value, bt_uint len);

/**
 * \brief Signed Write Command.
 * \ingroup att_client
 *
 * \details This function sends a "signed write command request to the client. No event is generated.
 *
 * \param session ATT session.
 * \param handle The handle of the attribute to be written.
 * \param value The value to be written to the attribute.
 * \param len The length of the \p value.
 * \param signature Authentication signature (12 bytes).
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_att_client_signed_write(
	bt_att_client_session_t* session, bt_uint handle,
	const bt_byte* value, bt_uint len,
	const bt_byte* signature);

/**
 * \brief Prepare Write.
 * \ingroup att_client
 *
 * \details This function sends a "prepare write request to the client. ATT_CLIENT_EVT_PREPARE_WRITE_RESPONSE event is generated
 *          when the response from the client has been received.
 *
 * \param session ATT session.
 * \param handle The handle of the attribute to be written.
 * \param value_offset The offset of the first octet to be written.
 * \param value The value to be written to the attribute.
 * \param len The length of the \p value.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_att_client_prepare_write(
	bt_att_client_session_t* session, 
	bt_uint handle, bt_uint value_offset,
	const bt_byte* value, bt_uint len);

/**
 * \brief Execute Write.
 * \ingroup att_client
 *
 * \details This function sends a "execute write request to the client. ATT_CLIENT_EVT_EXECUTE_WRITE_RESPONSE event is generated
 *          when the response from the client has been received.
 *
 * \param session ATT session.
 * \param flags This can be one of the following values:
 *                @arg 0x00 - Cancel all prepared writes.
 *                @arg 0x01 - Immediately write all pending prepared values.
 *
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_att_client_execute_write(bt_att_client_session_t* session, bt_byte flags);

/**
 * \brief Accept connection parameters update.
 * \ingroup att_client
 *
 * \details When a server sends "connection parameters update" request a ATT_CLIENT_EVT_CONN_PARAM_UPDATE_REQUEST event is generated.
 *          Client has to either accept the request or deny it. \c bt_att_client_accept_conn_param_update is used to accept the request.
 *
 * \param session ATT session.
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
bt_bool bt_att_client_accept_conn_param_update(
	bt_att_client_session_t* session, 
	bt_uint min_interval, bt_uint max_interval,
	bt_uint slave_latency, bt_uint supervision_timeout,
	bt_uint min_ce_length, bt_uint max_ce_length);

/**
 * \brief Reject connection parameters update.
 * \ingroup att_client
 *
 * \details When a server sends "connection parameters update" request a ATT_CLIENT_EVT_CONN_PARAM_UPDATE_REQUEST event is generated.
 *          Client has to either accept the request or deny it. \c bt_att_client_reject_conn_param_update is used to deny the request.
 *
 * \param session ATT session.
 * 
 * \return
 *        \li \c TRUE if a request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
void bt_att_client_reject_conn_param_update(bt_att_client_session_t* session);

/**
* \brief Get the address of the remote device this device is connected to.
* \ingroup att_client
*
* \param session ATT client session.
*
* \return
*        \li \c A pointer to bt_bdaddr structure that contains the address of the remote device.
*/
bt_bdaddr_t* bt_att_client_get_remote_address(const bt_att_client_session_t* session);

#include "cdbt/att/att_client_private.h"
#include "cdbt/utils/uuid.h"

#ifdef __cplusplus
}
#endif

#endif //__ATT_CLIENT_H
