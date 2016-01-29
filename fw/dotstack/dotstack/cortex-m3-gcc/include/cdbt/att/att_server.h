/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __ATT_SERVER_H
#define __ATT_SERVER_H

/**
 * \defgroup att_server ATT Server
 * \ingroup att
 * 
 *  This module describes functions and data structures used to implement an ATT server (peripheral).
 */

#define ATT_SERVER_EVT_NOTHING                           ATT_MGR_EVT_NOTHING
/** \addtogroup att_server 
* @{
*/
/** @name Events
*
*/
/**@{
*/
#define ATT_SERVER_EVT_SESSION_CONNECTED                 ATT_MGR_EVT_SESSION_CONNECTED               ///< A client connected to the server.
#define ATT_SERVER_EVT_SESSION_DISCONNECTED              ATT_MGR_EVT_SESSION_DISCONNECTED            ///< A client disconnected from the server.
#define ATT_SERVER_EVT_ATTR_VALUE_CHANGED_BY_SERVER      ATT_MGR_EVT_ATTR_VALUE_CHANGED_BY_SERVER    ///< Attribute's value has been changed locally on the server.
#define ATT_SERVER_EVT_ATTR_VALUE_CHANGED_BY_CLIENT      ATT_MGR_EVT_ATTR_VALUE_CHANGED_BY_CLIENT    ///< A client has changed the attribute's value.
#define ATT_SERVER_EVT_ATTR_VALUE_NOTIFIED               ATT_MGR_EVT_ATTR_VALUE_NOTIFIED             ///< A value notification has bee sent to the client.
#define ATT_SERVER_EVT_ATTR_VALUE_INDICATED              ATT_MGR_EVT_ATTR_VALUE_INDICATED            ///< A value indication has been sent to the client.
#define ATT_SERVER_EVT_TRAN_TIMEOUT                      ATT_MGR_EVT_TRAN_TIMEOUT                    ///< Operation timed out.
#define ATT_SERVER_EVT_AUTHORIZATION_REQUESTED           ATT_MGR_EVT_AUTHORIZATION_REQUESTED         ///< Authorization is required in order to access the attribute's value.
#define ATT_SERVER_EVT_ATTR_VALUE_READ_BY_CLIENT         ATT_MGR_EVT_ATTR_VALUE_READ_BY_CLIENT       ///< A client has read the attribute's value.
#define ATT_SERVER_EVT_STOPPED                           50                                          ///< Server stooped.
/**@}*/ 
/**@}*/

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*bt_att_server_callback_fp)(bt_int evt, void* evt_param, void* param);

/**
 * \brief Start ATT server
 * \ingroup att_server
 *
 * \details This function starts the ATT server.
 *
 * \param att_var_db A pointer to ATT database that holds writable attributes.
 * \param att_var_db_len The length of the writable ATT database.
 * \param att_const_db A pinter to ATT database that holds read-only attributes.
 * \param att_const_db_len The length of the read-only ATT database.
 * \param listen_on_dynamic_channel Specifies whether ATT server should accept connections on BR/EDR links.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
 */
bt_bool bt_att_server_start(
	bt_byte* att_var_db, bt_uint att_var_db_len,
	const bt_byte* att_const_db, bt_uint att_const_db_len,
	bt_bool listen_on_dynamic_channel);

/**
 * \brief Stop ATT server
 * \ingroup att_server
 *
 * \details This function stops the ATT server.
 *
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
 */
void bt_att_server_stop(void);

/**
 * \brief Register a ATT application callback.
 * \ingroup att_server
 *
 * \details In order to be notified of various events a consumer of the ATT layer has to register a callback function. 
 *          The stack will call this function whenever a new event has been generated passing the code of the event as the first parameter.
 *          The event can be one of the following values:
 * 
 *    @arg ATT_SERVER_EVT_SESSION_CONNECTED                 A client connected to the server
 *    @arg ATT_SERVER_EVT_SESSION_DISCONNECTED              A client disconnected from the server
 *    @arg ATT_SERVER_EVT_ATTR_VALUE_CHANGED_BY_SERVER      Attribute's value has been changed locally on the server
 *    @arg ATT_SERVER_EVT_ATTR_VALUE_CHANGED_BY_CLIENT      A client has changed the attribute's value
 *    @arg ATT_SERVER_EVT_ATTR_VALUE_NOTIFIED               A value notification has bee sent to the client
 *    @arg ATT_SERVER_EVT_ATTR_VALUE_INDICATED              A value indication has been sent to the client
 *    @arg ATT_SERVER_EVT_TRAN_TIMEOUT                      Transaction timed out
 *    @arg ATT_SERVER_EVT_AUTHORIZATION_REQUESTED           Authorization is required in order to access the attribute's value
 *    @arg ATT_SERVER_EVT_STOPPED                           Server stooped
 *
 * \param callback The callback function that will be called when ATT generates an event.
 * \param callback_param A pointer to arbitrary data to be passed to the \c callback callback.
 * 
 */
void bt_att_server_register_callback(bt_att_server_callback_fp callback, void* param);

/**
 * \brief Write attribute's value.
 * \ingroup att_server
 *
 * \details This function writes attribute's value. 
 *
 * \param handle Attribute handle.
 * \param value Attribute value.
 * \param value_len Attribute value length.
 * \param offset Offset from which to write the value.
 * 
 * \return Error code. This can be one of the following values:
 *           @arg ATT_ERROR_SUCCESS
 *           @arg ATT_ERROR_INVALID_HANDLE
 *           @arg ATT_ERROR_READ_NOT_PERMITTED
 *           @arg ATT_ERROR_WRITE_NOT_PERMITTED
 *           @arg ATT_ERROR_INVALID_PDU
 *           @arg ATT_ERROR_INSUFFICIENT_AUTHENTICATION
 *           @arg ATT_ERROR_REQUEST_NOT_SUPPORTED
 *           @arg ATT_ERROR_INVALID_OFFSET
 *           @arg ATT_ERROR_INSUFFICIENT_AUTHORIZATION
 *           @arg ATT_ERROR_PREPARE_QUEUE_FULL
 *           @arg ATT_ERROR_ATTRIBUTE_NOT_FOUND
 *           @arg ATT_ERROR_ATTRIBUTE_NOT_LONG
 *           @arg ATT_ERROR_INSUFFICIENT_ENCRYPTION_KEY_SIZE
 *           @arg ATT_ERROR_INVALID_ATTRIBUTE_VALUE_LENGTH
 *           @arg ATT_ERROR_UNLIKELY_ERROR
 *           @arg ATT_ERROR_INSUFFICIENT_ENCRYPTION
 *           @arg ATT_ERROR_UNSUPPORTED_GROUP_TYPE
 *           @arg ATT_ERROR_INSUFFICIENT_RESOURCES
 *
*/
bt_byte bt_att_server_write_attribute_value(
	bt_uint handle, const bt_byte* value, bt_uint value_len, bt_uint offset);

/**
 * \brief Disconnect from a remote device.
 * \ingroup att_server
 *
 * \details This function closes a connection to a remote device. 
 *
 * \param session ATT session.
 * 
 * \return
 *        \li \c TRUE if disconnection has been started.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_att_server_disconnect(bt_att_session_t* session);

/**
 * \brief Indicate attribute's value to all connected clients.
 * \ingroup att_server
 *
 * \details This function sends the attribute's value specified with the \c attr parameter to all connected clients. 
 *
 * \param attr Attribute.
 * 
*/
void bt_att_server_indicate_to_all(bt_att_attribute_t* attr);

/**
 * \brief Indicate attribute's value.
 * \ingroup att_server
 *
 * \details This function sends the attribute's value specified with 
 *          the \c attr parameter to a client sepecified with the \c session parameter. 
 *
 * \param session ATT session.
 * \param attr Attribute.
 * 
*/
void bt_att_server_indicate(bt_att_session_t* session, bt_att_attribute_t* attr);

/**
 * \brief Notify attribute's value to all connected clients.
 * \ingroup att_server
 *
 * \details This function sends the attribute's value specified with the \c attr parameter to all connected clients. 
 *
 * \param attr Attribute.
 * 
*/
void bt_att_server_notify(bt_att_attribute_t* attr);

/**
 * \brief Authorize access to an attribute
 * \ingroup att_server
 *
 * \details If an attribute requires authorization before its value can be read or written, 
 *          ATT generates a ATT_SERVER_EVT_AUTHORIZATION_REQUESTED event. In response to this 
 *          event the application should obtain authorization (how this is done is implementation specific)
 *          and call bt_att_server_authorize_access.
 *
 * \param session ATT session.
 * \param attr Attribute.
 * \param opcode Attribute Opcode. The opcode that requires authorization is passed in the ATT_SERVER_EVT_AUTHORIZATION_REQUESTED event.
 *               The application should use the passed opcode when calling bt_att_server_authorize_access.
 *               The opcode can be one the following values:
 *                 @arg ATT_OPCODE_READ_REQUEST
 *                 @arg ATT_OPCODE_READ_BLOB_REQUEST
 *                 @arg ATT_OPCODE_READ_BY_TYPE_REQUEST
 *                 @arg ATT_OPCODE_READ_BY_GROUP_TYPE_REQUEST
 *                 @arg ATT_OPCODE_READ_MULTIPLE_REQUEST
 *                 @arg ATT_OPCODE_WRITE_REQUEST
 *                 @arg ATT_OPCODE_PREPARE_WRITE_REQUEST
 *
 * \param authorize Specifies whether access to the attribute has been authorized or not.
 * 
*/
void bt_att_server_authorize_access(bt_att_session_t* session, bt_att_attribute_t* attr, bt_byte opcode, bt_bool authorize);

#ifdef __cplusplus
}
#endif

#endif // __ATT_SERVER_H
