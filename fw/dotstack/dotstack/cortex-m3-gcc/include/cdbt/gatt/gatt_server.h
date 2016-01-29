/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __GATT_SERVER_H
#define __GATT_SERVER_H

#define GATT_SERVER_EVT_NOTHING                      ATT_SERVER_EVT_NOTHING
/** \addtogroup gatt_server 
* @{
*/
/** @name Events
*
*/
/**@{
*/
#define GATT_SERVER_EVT_SESSION_CONNECTED            ATT_SERVER_EVT_SESSION_CONNECTED         ///< A client connected to the server.
#define GATT_SERVER_EVT_SESSION_DISCONNECTED         ATT_SERVER_EVT_SESSION_DISCONNECTED      ///< A client disconnected from the server.
#define GATT_SERVER_EVT_ATTR_VALUE_NOTIFIED          ATT_SERVER_EVT_ATTR_VALUE_NOTIFIED       ///< A value notification has bee sent to the client.
#define GATT_SERVER_EVT_ATTR_VALUE_INDICATED         ATT_SERVER_EVT_ATTR_VALUE_INDICATED      ///< A value indication has been sent to the client.
#define GATT_SERVER_EVT_AUTHORIZATION_REQUESTED      ATT_SERVER_EVT_AUTHORIZATION_REQUESTED   ///< Authorization is required in order to access the attribute's value.
#define GATT_SERVER_EVT_CLIENT_CONFIG_CHANGED        100                                      ///< Characteristic's client configuration changed
#define GATT_SERVER_EVT_EXTENDED_PROPERTIES_CHANGED  101                                      ///< Characteristic's extended properties changed
#define GATT_SERVER_EVT_SERVER_CONFIG_CHANGED        102                                      ///< Characteristic's server configuration changed
#define GATT_SERVER_EVT_VALUE_CHANGED                103                                      ///< Characteristic's value changed
#define GATT_SERVER_EVT_VALUE_READ                   104                                      ///< Characteristic's value has been read
/**@}*/ 
/**@}*/

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*bt_gatt_server_callback_fp)(bt_int evt, void* evt_param, void* param);

typedef struct _bt_gatt_listener_t
{
	struct _bt_gatt_listener_t* next_listener;

	bt_gatt_server_callback_fp callback;
	void* callback_param;
} bt_gatt_listener_t;

/**
* \brief  Common to all event parameters header 
* \ingroup gatt_server
*
* \details This structure holds characteristic's identifier - service type, service id, and characteristic id.
*/
typedef struct _bt_gatt_evt_header_s
{
	bt_uuid_t service_type;        ///< 128-bit service type UUID.
	bt_uuid_t service_id;          ///< 128-bit service id UUID.
	bt_uuid_t characteristic_id;   ///< 128-bit characteristic id UUID.
} bt_gatt_evt_header_t;

/**
* \brief  Parameter to GATT_SERVER_EVT_CLIENT_CONFIG_CHANGED event
* \ingroup gatt_server
*
* \details A pointer to this structure is passed to the GATT application callback 
*          when the characteristic's client configuration changed. 
*/
typedef struct _bt_gatt_evt_client_config_changed_s
{
	bt_gatt_evt_header_t header;    ///< Common header
	bt_byte client_config;          ///< New client configuration
} bt_gatt_evt_client_config_changed_t;

/**
* \brief  Parameter to GATT_SERVER_EVT_SERVER_CONFIG_CHANGED event
* \ingroup gatt_server
*
* \details A pointer to this structure is passed to the GATT application callback 
*          when the characteristic's server configuration changed. 
*/
typedef struct _bt_gatt_evt_server_config_changed_s
{
	bt_gatt_evt_header_t header;    ///< Common header
	bt_byte server_config;          ///< New server configuration
} bt_gatt_evt_server_config_changed_t;

/**
* \brief  Parameter to GATT_SERVER_EVT_EXTENDED_PROPERTIES_CHANGED event
* \ingroup gatt_server
*
* \details A pointer to this structure is passed to the GATT application callback 
*          when the characteristic's extended properties changed. 
*/
typedef struct _bt_gatt_evt_ext_properties_changed_s
{
	bt_gatt_evt_header_t header;    ///< Common header
	bt_byte ext_properties;         ///< New extended properties
} bt_gatt_evt_ext_properties_changed_t;

/**
* \brief  Parameter to GATT_SERVER_EVT_VALUE_CHANGED event
* \ingroup gatt_server
*
* \details A pointer to this structure is passed to the GATT application callback 
*          when the characteristic's value changed. 
*/
typedef struct _bt_gatt_evt_value_changed_s
{
	bt_gatt_evt_header_t header;    ///< Common header
	const bt_byte* value;           ///< New value
	bt_uint len;                    ///< New value length
} bt_gatt_evt_value_changed_t;

/**
* \brief  Parameter to GATT_SERVER_EVT_VALUE_READ event
* \ingroup gatt_server
*
* \details A pointer to this structure is passed to the GATT application callback 
*          when the characteristic's value has been read by a client. 
*/
typedef struct _bt_gatt_evt_value_read_s
{
	bt_gatt_evt_header_t header;    ///< Common header
	const bt_byte* value;           ///< Value
	bt_uint len;                    ///< Value length
	bt_uint offset;                 ///< Offset from which the value has been read
} bt_gatt_evt_value_read_t;

/**
 * \brief Start GATT server (extended version)
 * \ingroup gatt_server
 *
 * \details This function starts the GATT server. This function can be used to start the server on BR/EDR links.
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
bt_bool bt_gatt_server_start_ex(
	bt_byte* att_var_db, bt_uint att_var_db_len,
	const bt_byte* att_const_db, bt_uint att_const_db_len,
	bt_bool listen_on_dynamic_channel);

/**
 * \brief Start GATT server
 * \ingroup gatt_server
 *
 * \details This function starts the GATT server. This function start the server only on LE links.
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
#define bt_gatt_server_start(att_var_db, att_var_db_len, att_const_db, att_const_db_len)	\
	bt_gatt_server_start_ex(att_var_db, att_var_db_len, att_const_db, att_const_db_len, BT_FALSE)

/**
 * \brief Register a GATT application callback.
 * \ingroup gatt_server
 *
 * \details In order to be notified of various events a consumer of the GATT layer has to register a callback function. 
 *          The stack will call this function whenever a new event has been generated passing the code of the event as the first parameter.
 *          The event can be one of the following values:
 * 
 *          @arg GATT_SERVER_EVT_SESSION_CONNECTED A client connected to the server.
 *          @arg GATT_SERVER_EVT_SESSION_DISCONNECTED  A client disconnected from the server.
 *          @arg GATT_SERVER_EVT_ATTR_VALUE_NOTIFIED  A value notification has bee sent to the client.
 *          @arg GATT_SERVER_EVT_ATTR_VALUE_INDICATED  A value indication has been sent to the client.
 *          @arg GATT_SERVER_EVT_AUTHORIZATION_REQUESTED  Authorization is required in order to access the attribute's value.
 *          @arg GATT_SERVER_EVT_CLIENT_CONFIG_CHANGED  Characteristic's client configuration changed
 *          @arg GATT_SERVER_EVT_EXTENDED_PROPERTIES_CHANGED  Characteristic's extended properties changed
 *          @arg GATT_SERVER_EVT_SERVER_CONFIG_CHANGED  Characteristic's server configuration changed
 *          @arg GATT_SERVER_EVT_VALUE_CHANGED  Characteristic's value changed
 *
 * \param callback The callback function that will be called when GATT generates an event.
 * \param callback_param A pointer to arbitrary data to be passed to the \c callback callback.
 * 
 */
void bt_gatt_server_register_callback(bt_gatt_server_callback_fp callback, void* param);

/**
 * \brief Register a GATT event listener.
 * \ingroup gatt_server
 *
 * \details In order to be notified of various events a consumer of the GATT layer may register an event listener. 
 *          The listener is a structure that contains a pointer to a callback and callback parameter - a pointer to arbitrary data 
 *          to be passed to the callback .
 *          The stack will call the callback whenever a new event has been generated passing the code of the event as the first parameter.
 *          The event can be one of the following values:
 * 
 *          @arg GATT_SERVER_EVT_SESSION_CONNECTED A client connected to the server.
 *          @arg GATT_SERVER_EVT_SESSION_DISCONNECTED  A client disconnected from the server.
 *          @arg GATT_SERVER_EVT_ATTR_VALUE_NOTIFIED  A value notification has bee sent to the client.
 *          @arg GATT_SERVER_EVT_ATTR_VALUE_INDICATED  A value indication has been sent to the client.
 *          @arg GATT_SERVER_EVT_AUTHORIZATION_REQUESTED  Authorization is required in order to access the attribute's value.
 *          @arg GATT_SERVER_EVT_CLIENT_CONFIG_CHANGED  Characteristic's client configuration changed
 *          @arg GATT_SERVER_EVT_EXTENDED_PROPERTIES_CHANGED  Characteristic's extended properties changed
 *          @arg GATT_SERVER_EVT_SERVER_CONFIG_CHANGED  Characteristic's server configuration changed
 *          @arg GATT_SERVER_EVT_VALUE_CHANGED  Characteristic's value changed
 *
 * \param listener Listener.
 * 
 */
bt_bool bt_gatt_server_register_listener(bt_gatt_listener_t* listener);

/**
 * \brief Unregister a GATT event listener.
 * \ingroup gatt_server
 *
 * \details Remove a listener previosuly registered with bt_gatt_server_register_listener. 
 *
 * \param listener Listener.
 * 
 */
void bt_gatt_server_unregister_listener(bt_gatt_listener_t* listener);

/**
 * \brief Write characteristic's value (16-bit).
 * \ingroup gatt_server
 *
 * \details This function updates characteristic's value. If the client configured characteristic to be 
 *          notified or indicated this function sends new value to the client.
 *
 * \param service_type 16-bit service type UUID.
 * \param service_id 16-bit service type UUID.
 * \param characteristic_type 16-bit characteristic type UUID.
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
*/
bt_byte bt_gatt_server_write_char_value(
	bt_uuid16 service_type, 
	bt_uuid16 service_id, 
	bt_uuid16 characteristic_type,
	const bt_byte* value, bt_uint value_len, bt_uint offset);

/**
 * \brief Write characteristic's value (128-bit).
 * \ingroup gatt_server
 *
 * \details This function updates characteristic's value. If the client configured characteristic to be 
 *          notified or indicated this function sends new value to the client.
 *          In order to use standard 16-bit UUIDs with this function they have to be converted to 128-bit UUID by combining with a base UUID which is
 *             00000000 (this part is replaced with 16-bit UUID) - 00001000 - 80000080 - 5F9B34FB. 
 *
 * \param service_type 128-bit service type UUID.
 * \param service_id 128-bit service type UUID.
 * \param characteristic_type 128-bit characteristic type UUID.
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
*/
bt_byte bt_gatt_server_write_char_value_80(
	bt_uuid_t* service_type, 
	bt_uuid_t* service_id, 
	bt_uuid_t* characteristic_type,
	const bt_byte* value, bt_uint value_len, bt_uint offset);

/**
 * \brief Notify characteristic's value (16-bit).
 * \ingroup gatt_server
 *
 * \details This function sends current characteristic's value to the client. The value is sent regardless of
 *          characteristic's client configuration. GATT_SERVER_EVT_ATTR_VALUE_NOTIFIED is generated right after
 *          the value has been sent.
 *
 * \param service_type 16-bit service type UUID.
 * \param service_id 16-bit service type UUID.
 * \param characteristic_type 16-bit characteristic type UUID.
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
*/
bt_byte bt_gatt_server_notify_char_value(
	bt_uuid16 service_type, 
	bt_uuid16 service_id, 
	bt_uuid16 characteristic_type);

/**
 * \brief Notify characteristic's value (128-bit).
 * \ingroup gatt_server
 *
 * \details This function sends current characteristic's value to the client. The value is sent regardless of
 *          characteristic's client configuration. GATT_SERVER_EVT_ATTR_VALUE_NOTIFIED is generated right after
 *          the value has been sent.
 *          In order to use standard 16-bit UUIDs with this function they have to be converted to 128-bit UUID by combining with a base UUID which is
 *             00000000 (this part is replaced with 16-bit UUID) - 00001000 - 80000080 - 5F9B34FB. 
 *
 * \param service_type 128-bit service type UUID.
 * \param service_id 128-bit service type UUID.
 * \param characteristic_type 128-bit characteristic type UUID.
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
*/
bt_byte bt_gatt_server_notify_char_value_80(
	bt_uuid_t* service_type, 
	bt_uuid_t* service_id, 
	bt_uuid_t* characteristic_type);

/**
 * \brief Indicate characteristic's value (16-bit).
 * \ingroup gatt_server
 *
 * \details This function sends current characteristic's value to the client. The value is sent regardless of
 *          characteristic's client configuration. GATT_SERVER_EVT_ATTR_VALUE_INDICATED event is generated
 *          after receiving confirmation from the client.
 *
 * \param service_type 16-bit service type UUID.
 * \param service_id 16-bit service type UUID.
 * \param characteristic_type 16-bit characteristic type UUID.
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
*/
bt_byte bt_gatt_server_indicate_char_value(
	bt_uuid16 service_type, 
	bt_uuid16 service_id, 
	bt_uuid16 characteristic_type);

/**
 * \brief Indicate characteristic's value (128-bit).
 * \ingroup gatt_server
 *
 * \details This function sends current characteristic's value to the client. The value is sent regardless of
 *          characteristic's client configuration. GATT_SERVER_EVT_ATTR_VALUE_INDICATED event is generated
 *          after receiving confirmation from the client.
 *          In order to use standard 16-bit UUIDs with this function they have to be converted to 128-bit UUID by combining with a base UUID which is
 *             00000000 (this part is replaced with 16-bit UUID) - 00001000 - 80000080 - 5F9B34FB. 
 *
 * \param service_type 128-bit service type UUID.
 * \param service_id 128-bit service type UUID.
 * \param characteristic_type 128-bit characteristic type UUID.
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
*/
bt_byte bt_gatt_server_indicate_char_value_80(
	bt_uuid_t* service_type, 
	bt_uuid_t* service_id, 
	bt_uuid_t* characteristic_type);

/**
 * \brief Authorize access to an attribute
 * \ingroup gatt_server
 *
 * \details If an attribute requires authorization before its value can be read or written, 
 *          GATT generates a GATT_SERVER_EVT_AUTHORIZATION_REQUESTED event. In response to this 
 *          event the application should obtain authorization (how this is done is implementation specific)
 *          and call bt_gatt_server_authorize_access.
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
#define bt_gatt_server_authorize_access(session, attr, opcode, authorize)	\
	bt_att_server_authorize_access(session, attr, opcode, authorize)

#ifdef __cplusplus
}
#endif

#endif // __GATT_SERVER_H
