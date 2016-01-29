/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __AVCTP_H
#define __AVCTP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup avctp Audio/Video Control Transport Protocol (AVCTP)
 *
 * AVCTP is the transport mechanisms used to exchange messages for controlling Audio and/or Video devices. 
 * The actual message contents are defined in the applicable A/V control profiles.
 *
 */

#include "cdbt/l2cap/l2cap.h"
#include "cdbt/avctp/avctp_packet.h"

#define AVCTP_MANAGER_STATE_IDLE          0

#define AVDTP_MANAGER_FLAG_SENDING_MEDIA_PACKET        1

/** \addtogroup avrcp
* @{
*/
/** @name Events
*
* \details The following is a list of events AVRCP layer generates and can report to the upper layer when 
*          it completes executing an operation initiated by either local or remote device. 
*/
/**@{
*/
#define AVCTP_EVT_NOTHING                      0   
#define AVCTP_EVT_CHANNEL_CONNECTED            1   ///< This event is generated when a channel between two AVCTP entities has been established.
#define AVCTP_EVT_CHANNEL_DISCONNECTED         2   ///< This event is generated when a channel between two AVCTP entities has been terminated.
#define AVCTP_EVT_CONNECTION_FAILED            3   ///< This event is generated when a local device failed to create a channel between two AVCTP entities.

#define AVCTP_EVT_COMMAND_RECEIVED             50  ///< This event is generated when a local device received a command.
#define AVCTP_EVT_RESPONE_RECEIVED             51  ///< This event is generated when a local device received a response.
#define AVCTP_EVT_COMMAND_SENT                 52  ///< This event is generated when a local device finished sending a command.
#define AVCTP_EVT_RESPONSE_SENT                53  ///< This event is generated when a local device finished sending a response.
#define AVCTP_EVT_COMMAND_CANCELLED            54  ///< This event is generated when a command has been canceled.
#define AVCTP_EVT_RESPONSE_CANCELLED           55  ///< This event is generated when a response has been canceled.
/**@}*/ 
/**@}*/

#define AVCTP_CHANNEL_STATE_FREE                       0
#define AVCTP_CHANNEL_STATE_IDLE                       1
#define AVCTP_CHANNEL_STATE_CONNECTING                 2
#define AVCTP_CHANNEL_STATE_CONNECTED                  3
#define AVCTP_CHANNEL_STATE_DISCONNECTING              4

#define AVCTP_CHANNEL_FLAG_LISTENING                   1
#define AVCTP_CHANNEL_FLAG_SENDING                     2

#define AVCTP_TRANSPORT_STATE_FREE                     0
#define AVCTP_TRANSPORT_STATE_IDLE                     1
#define AVCTP_TRANSPORT_STATE_CONNECTING               2
#define AVCTP_TRANSPORT_STATE_CONNECTED                3
#define AVCTP_TRANSPORT_STATE_DISCONNECTING            4

#define AVCTP_TRANSPORT_FLAG_SENDING                   2
#define AVCTP_TRANSPORT_FLAG_RX_MESSAGE_STARTED        4

#define AVCTP_ERROR_SUCCESS                            0

#define AVCTP_ERROR_BAD_STATE                          0x31

#define AVCTP_MESSAGE_PACKET_TYPE_SINGLE      0
#define AVCTP_MESSAGE_PACKET_TYPE_START       1
#define AVCTP_MESSAGE_PACKET_TYPE_CONTINUE    2
#define AVCTP_MESSAGE_PACKET_TYPE_END         3

#define AVCTP_MESSAGE_TYPE_COMMAND            0
#define AVCTP_MESSAGE_TYPE_RESPONSE           1

typedef struct _bt_avctp_mgr_t       bt_avctp_mgr_t;
typedef struct _bt_avctp_channel_t   bt_avctp_channel_t;
typedef struct _bt_avctp_message_t   bt_avctp_message_t;
typedef struct _bt_avctp_transport_t bt_avctp_transport_t;

/**
* \brief  Parameter to AVCTP_EVT_CHANNEL_CONNECTED event
* \ingroup avctp
*
* \details A pointer to this structure is passed to the AVCTP application callback as 
*          a valid member of the bt_avctp_event_t union - bt_avctp_event_t::channel_connected -
*          when a channel between two devices has been established.
*/
typedef struct _bt_avctp_evt_channel_connected_t
{
	bt_avctp_channel_t* channel;    ///< AVCTP channel
} bt_avctp_evt_channel_connected_t;

/**
* \brief  Parameter to AVCTP_EVT_CHANNEL_DISCONNECTED event
* \ingroup avctp
*
* \details A pointer to this structure is passed to the AVCTP application callback as 
*          a valid member of the bt_avctp_event_t union - bt_avctp_event_t::channel_disconnected -
*          when a channel between two devices has been terminated.
*/
typedef struct _bt_avctp_evt_channel_disconnected_t
{
	bt_avctp_channel_t* channel;    ///< AVCTP channel
} bt_avctp_evt_channel_disconnected_t;

/**
* \brief  Parameter to AVCTP_EVT_CONNECTION_FAILED event
* \ingroup avctp
*
* \details A pointer to this structure is passed to the AVCTP application callback as 
*          a valid member of the bt_avctp_event_t union - bt_avctp_event_t::connection_failed -
*          when a channel between two devices could not be established.
*/
typedef struct _bt_avctp_evt_connection_failed_t
{
	bt_avctp_channel_t* channel;    ///< AVCTP channel
} bt_avctp_evt_connection_failed_t;

/**
* \brief  Parameter to AVCTP_EVT_COMMAND_RECEIVED event
* \ingroup avctp
*
* \details A pointer to this structure is passed to the AVCTP application callback as 
*          a valid member of the bt_avctp_event_t union - bt_avctp_event_t::command_received -
*          when a local device received a command message.
*/
typedef struct _bt_avctp_evt_command_received_t
{
	bt_avctp_channel_t* channel;    ///< AVCTP channel
	bt_avctp_message_t* command;    ///< AVCTP command message
} bt_avctp_evt_command_received_t;

/**
* \brief  Parameter to AVCTP_EVT_RESPONSE_RECEIVED event
* \ingroup avctp
*
* \details A pointer to this structure is passed to the AVCTP application callback as 
*          a valid member of the bt_avctp_event_t union - bt_avctp_event_t::response_received -
*          when a local device received a response message.
*/
typedef struct _bt_avctp_evt_response_received_t
{
	bt_avctp_channel_t* channel;    ///< AVCTP channel
	bt_avctp_message_t* response;   ///< AVCTP response message
} bt_avctp_evt_response_received_t;

/**
* \brief  Parameter to AVCTP_EVT_COMMAND_SENT event
* \ingroup avctp
*
* \details A pointer to this structure is passed to the AVCTP application callback as 
*          a valid member of the bt_avctp_event_t union - bt_avctp_event_t::command_sent -
*          when a local device finished sending a command message.
*/
typedef struct _bt_avctp_evt_command_sent_t
{
	bt_avctp_channel_t* channel;    ///< AVCTP channel
	bt_avctp_message_t* command;    ///< AVCTP command message
} bt_avctp_evt_command_sent_t;

/**
* \brief  Parameter to AVCTP_EVT_COMMAND_CANCELLED event
* \ingroup avctp
*
* \details A pointer to this structure is passed to the AVCTP application callback as 
*          a valid member of the bt_avctp_event_t union - bt_avctp_event_t::command_cancelled -
*          when sending a command message has been canceled.
*/
typedef struct _bt_avctp_evt_command_cancelled_t
{
	bt_avctp_channel_t* channel;    ///< AVCTP channel
	bt_avctp_message_t* command;    ///< AVCTP command message
} bt_avctp_evt_command_cancelled_t;

/**
* \brief  Parameter to AVCTP_EVT_RESPONSE_SENT event
* \ingroup avctp
*
* \details A pointer to this structure is passed to the AVCTP application callback as 
*          a valid member of the bt_avctp_event_t union - bt_avctp_event_t::response_sent -
*          when a local device finished sending a response message.
*/
typedef struct _bt_avctp_evt_response_sent_t
{
	bt_avctp_channel_t* channel;    ///< AVCTP channel
	bt_avctp_message_t* response;   ///< AVCTP response message
} bt_avctp_evt_response_sent_t;

/**
* \brief  Parameter to AVCTP_EVT_RESPONSE_CANCELLED event
* \ingroup avctp
*
* \details A pointer to this structure is passed to the AVCTP application callback as 
*          a valid member of the bt_avctp_event_t union - bt_avctp_event_t::response_cancelled -
*          when sending a response message has been canceled.
*/
typedef struct _bt_avctp_evt_response_cancelled_t
{
	bt_avctp_channel_t* channel;    ///< AVCTP channel
	bt_avctp_message_t* response;   ///< AVCTP response message
} bt_avctp_evt_response_cancelled_t;

/**
* \brief  Parameter to an application callback.
* \ingroup avctp
*
* \details This union is used to pass event specific data to the AVCTP consumer. Which member of the union 
*          points to a valid structure depends on the event reported to the consumer. In general, each event
*          has a corresponding member in the union.
*/
typedef union _bt_avctp_event_u
{
	bt_avctp_evt_channel_connected_t channel_connected;          ///< Valid if event is AVCTP_EVT_CHANNEL_CONNECTED
	bt_avctp_evt_channel_disconnected_t channel_disconnected;    ///< Valid if event is AVCTP_EVT_CHANNEL_DISCONNECTED
	bt_avctp_evt_connection_failed_t connection_failed;          ///< Valid if event is AVCTP_EVT_CONNECTION_FAILED
	bt_avctp_evt_command_received_t command_received;            ///< Valid if event is AVCTP_EVT_COMMAND_RECEIVED
	bt_avctp_evt_response_received_t response_received;          ///< Valid if event is AVCTP_EVT_RESPONE_RECEIVED
	bt_avctp_evt_command_sent_t command_sent;                    ///< Valid if event is AVCTP_EVT_COMMAND_SENT
	bt_avctp_evt_response_sent_t response_sent;                  ///< Valid if event is AVCTP_EVT_RESPONSE_SENT
	bt_avctp_evt_command_cancelled_t command_cancelled;          ///< Valid if event is AVCTP_EVT_COMMAND_CANCELLED
	bt_avctp_evt_response_cancelled_t response_cancelled;        ///< Valid if event is AVCTP_EVT_RESPONSE_CANCELLED

} bt_avctp_event_t;

///////////////////////////////////////////////////////////////////////////////

/**
* \brief  AVCTP message description
* \ingroup avctp
*
* \details This structure is used to hold information about an AVCTP message.
*/
struct _bt_avctp_message_t
{
	bt_avctp_message_t* next_message;

	bt_byte tran_id;
	bt_byte packet_type;
	bt_byte num_packets;
	bt_byte message_type;
	bt_byte ipid;
	bt_uint profile_id;
	bt_byte* data;
	bt_uint data_len;
};

/**
* \brief  AVCTP transport description
* \ingroup avctp
*
* \details This structure is used to hold information about an AVCTP transport.
*/
struct _bt_avctp_transport_t
{
	bt_byte state;
	bt_byte flags;
	bt_bdaddr_t remote_addr;
	bt_l2cap_channel_t* l2cap_channel;
	bt_avctp_packet_t packet;
	bt_byte* tx_buffer;
	bt_byte* rx_buffer;
	bt_uint rx_message_len;
	bt_byte ref_count;

	bt_queue_element_t*    send_cq_head;
	bt_queue_element_t*    ack_cq_head;
	bt_byte next_transaction_id;

	bt_avctp_message_t* tx_message;
	bt_uint tx_message_remaining_data_len;

	bt_avctp_message_t rx_message;

	bt_avctp_mgr_t* avctp_mgr;
};

/**
* \brief  AVCTP channel description
* \ingroup avctp
*
* \details This structure is used to hold information about an AVCTP channel.
*/
struct _bt_avctp_channel_t
{
	bt_byte state;
	bt_byte flags;
	bt_avctp_transport_t* transport;
	bt_uint profile_id;
	bt_int psm;
	bt_byte l2cap_mode;

	bt_avctp_mgr_t* avctp_mgr;
};

/**
 * \brief AVCTP application callback.
 * \ingroup avctp
 *
 * \details In order to be notified of various events a consumer of the AVCTP layer has to register a callback function (done with bt_avctp_set_callback()). 
 *          The stack will call that function whenever a new event has been generated.
 *
 * \param mgr AVCTP manager.
 *
 * \param evt AVCTP event.
 *          The event can be one of the following values:
 *            @arg AVCTP_EVT_CHANNEL_CONNECTED             Channel connected.
 *            @arg AVCTP_EVT_CHANNEL_DISCONNECTED          Channel disconnected.
 *            @arg AVCTP_EVT_CONNECTION_FAILED             Channel connection failed (generated only if connection has been initiated by the local device).
 *
 *            @arg AVCTP_EVT_COMMAND_RECEIVED              Command received.
 *            @arg AVCTP_EVT_RESPONE_RECEIVED              Response received.
 *            @arg AVCTP_EVT_COMMAND_SENT                  Command sent.
 *            @arg AVCTP_EVT_RESPONSE_SENT                 Response sent.
 *            @arg AVCTP_EVT_COMMAND_CANCELLED             Command canceled.
 *            @arg AVCTP_EVT_RESPONSE_CANCELLED            Response canceled.
 *
 * \param evt_param Event parameter.
 *          Which member of the bt_avctp_event_t union is valid depends on the event:
 *            @arg AVCTP_EVT_CHANNEL_CONNECTED                          \c bt_avctp_evt_channel_connected_t        channel_connected
 *            @arg AVCTP_EVT_CHANNEL_DISCONNECTED                       \c bt_avctp_evt_channel_disconnected_t     channel_disconnected
 *            @arg AVCTP_EVT_CONNECTION_FAILED                          \c bt_avctp_evt_connection_failed_t        connection_failed
 *            @arg AVCTP_EVT_COMMAND_RECEIVED                           \c bt_avctp_evt_command_received_t         command_received
 *            @arg AVCTP_EVT_RESPONE_RECEIVED                           \c bt_avctp_evt_response_received_t        response_received
 *            @arg AVCTP_EVT_COMMAND_SENT                               \c bt_avctp_evt_command_sent_t             command_sent
 *            @arg AVCTP_EVT_RESPONSE_SENT                              \c bt_avctp_evt_response_sent_t            response_sent
 *            @arg AVCTP_EVT_COMMAND_CANCELLED                          \c bt_avctp_evt_command_cancelled_t        command_cancelled
 *            @arg AVCTP_EVT_RESPONSE_CANCELLED                         \c bt_avctp_evt_response_cancelled_t       response_cancelled
 *
 * \param callback_param A pointer to an arbitrary data set by a call to bt_avctp_set_callback.
 * 
 */
typedef void (*bt_avctp_mgr_callback_fp)(struct _bt_avctp_mgr_t* mgr, bt_byte evt, bt_avctp_event_t* evt_param, void* callback_param);

/**
 * \brief AVCTP manager.
 * \ingroup avctp
 *
 * \details A structure that glues all pieces together. There is only one instance of this structure allocated by dotstack.
 *          A pointer to the instance can be get with \c bt_avctp_get_mgr().
 *          
 */
struct _bt_avctp_mgr_t
{
	bt_byte state;                            ///< Manager state. This value can be one of the following values:
                                              ///< \li AVCTP_MANAGER_STATE_IDLE
                                              ///< \li AVCTP_MANAGER_STATE_CONNECTING 
	bt_byte flags;                            ///< Additional manager state. This value can be a combination of the following values:
                                              ///< \li AVDTP_MANAGER_FLAG_SENDING_MEDIA_PACKET

	bt_avctp_channel_t* channels;             ///< List of available AVCTP channels.
	bt_avctp_transport_t* transports;         ///< List of available AVCTP transports.

	bt_avctp_mgr_callback_fp callback;        ///< Pointer to a function used to notify the AVCTP consumer about various events.
	void* callback_param;                     ///< Pointer to arbitrary data to be passed to the \c callback.

//	bt_avdtp_control_cmd_t* pending_command;

	bt_avctp_channel_t* opening_channel;      ///< Pointer to a channle being open.
};


/**
 * \brief Return a pointer to an instance of the AVCTP manager.
 * \ingroup avctp
 *
 * \details This function returns a pointer to an instance of the AVCTP manager. 
 *          There is only one instance of the manager allocated by the stack. 
 */
bt_avctp_mgr_t* bt_avctp_get_mgr(void);

/**
 * \brief Initialize the AVCTP layer.
 * \ingroup avctp
 *
 * \details This function initializes the AVCTP layer of the stack. It must be called prior to any other
 *          AVCTP function can be called.
 */
void bt_avctp_init(void);

bt_bool bt_avctp_start(bt_avctp_mgr_t* mgr);

/**
 * \brief Register a AVCTP application callback.
 * \ingroup avctp
 *
 * \details In order to be notified of various events a consumer of the AVCTP layer has to register a callback function. 
 *          The stack will call this function whenever a new event has been generated passing the code of the event as the second parameter.
 *          The event can be one of the following values:
 *              @arg AVCTP_EVT_CHANNEL_CONNECTED             Channel connected.
 *              @arg AVCTP_EVT_CHANNEL_DISCONNECTED          Channel disconnected.
 *              @arg AVCTP_EVT_CONNECTION_FAILED             Channel connection failed (generated only if connection has been initiated by the local device).
 *
 *              @arg AVCTP_EVT_COMMAND_RECEIVED              Command received.
 *              @arg AVCTP_EVT_RESPONE_RECEIVED              Response received.
 *              @arg AVCTP_EVT_COMMAND_SENT                  Command sent.
 *              @arg AVCTP_EVT_RESPONSE_SENT                 Response sent.
 *              @arg AVCTP_EVT_COMMAND_CANCELLED             Command canceled.
 *              @arg AVCTP_EVT_RESPONSE_CANCELLED            Response canceled.
 *
 * \param mgr AVCTP manager.
 * \param callback The callback function that will be called when the AVCTP generates an event.
 * \param callback_param A pointer to arbitrary data to be passed to the \c callback callback.
 * 
 */
void bt_avctp_set_callback(bt_avctp_mgr_t* mgr, bt_avctp_mgr_callback_fp callback, void* callback_param);

/**
* \brief Allocate AVCTP channel
* \ingroup avctp
*
* \details This function allocates a new incoming AVCTP channel. The channel is intended to be used to accept a connection
*          from a remote device. There can be only one channel for each combination of \c profile_id and \c psm.
*
* \param mgr AVCTP manager.
* \param profile_id Profile Id
* \param psm The PSM on which the underlying L2CAP channel will listen and accept incoming connections.
* \param l2cap_mode Underlying L2CAP channel mode. This currently can only be CMODE_BASIC.
*
* \return
*         \li A pointer to the new AVCTP channel if the function succeeds.
*         \li \c NULL otherwise.
*/
bt_avctp_channel_t* bt_avctp_create_channel(bt_avctp_mgr_t* mgr, bt_uint profile_id, bt_int psm, bt_byte l2cap_mode);

/**
* \brief Allocate AVCTP channel
* \ingroup avctp
*
* \details This function allocates a new outgoing AVCTP channel. The channel is intended to be used to create a connection
*          to a remote device. There can be multiple channels with the same \c profile_id and \c psm.
*
* \param mgr AVCTP manager.
* \param profile_id Profile Id
* \param psm The PSM on which the underlying L2CAP channel will listen and accept incoming connections.
* \param l2cap_mode Underlying L2CAP channel mode. This currently can only be CMODE_BASIC.
*
* \return
*         \li A pointer to the new AVCTP channel if the function succeeds.
*         \li \c NULL otherwise.
*/
bt_avctp_channel_t* bt_avctp_create_outgoing_channel(bt_avctp_mgr_t* mgr, bt_uint profile_id, bt_int psm, bt_byte l2cap_mode);

/**
 * \brief Destroy AVCTP channel.
 * \ingroup avctp
 *
 * \details This function frees memory used by the channel. The channel has to exist and be in the "idle" state for this function to succeed. 
 *          I.e. the channel has to be disconnected before this function can be called.
 *
 * \param channel AVCTP channel.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
*/
bt_bool bt_avctp_destroy_channel(bt_avctp_channel_t* channel);

/**
 * \brief Listen for incoming connections.
 * \ingroup avctp
 *
 * \details This function enables incoming connections on the specified AVCTP channel.
 *
 * \param channel AVCTP channel.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
*/
bt_bool bt_avctp_listen(bt_avctp_channel_t* channel);

/**
 * \brief Cancel listening for incoming connections.
 * \ingroup avctp
 *
 * \details This function stops listening for incoming connections on the specified channel.
 *
 * \param channel AVCTP channel.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
*/
void bt_avctp_cancel_listen(bt_avctp_channel_t* channel);

/**
 * \brief Get channel state
 * \ingroup avctp
 *
 * \details This function return current state of the specified channel
 *
 * \param channel AVCTP channel.
 * 
 * \return
 *        \li AVCTP_CHANNEL_STATE_FREE
 *        \li AVCTP_CHANNEL_STATE_IDLE
 *        \li AVCTP_CHANNEL_STATE_CONNECTING
 *        \li AVCTP_CHANNEL_STATE_CONNECTED
 *        \li AVCTP_CHANNEL_STATE_DISCONNECTING
*/
bt_byte bt_avctp_get_channel_state(bt_avctp_channel_t* channel);

/**
 * \brief Get channel's remote BT address.
 * \ingroup avctp
 *
 * \details This function returns the address of the remote device associated with the channel.
 *
 * \param channel AVCTP channel.
 * 
 * \return \li The address of the remote device if channel is connected. 
 *         \li NULL otherwise.
 */
bt_bdaddr_t* bt_avctp_get_channel_remote_address(bt_avctp_channel_t* channel);

/**
 * \brief Connect to a remote device.
 * \ingroup avctp
 *
 * \details This function establishes a connection to a remote device specified by the \p remote_address. If connection
 *          cannot be initiated for some reason, for example, there is not enough resources, it returns FALSE and no events are generated. Otherwise
 *          the result of an attempt to connect to the remote device is reported via the AVCTP callback. 
 *          The events generated will either be AVCTP_EVT_CHANNEL_CONNECTED or AVCTP_EVT_CONNECTION_FAILED.
 *
 * \param channel AVCTP channel.
 * \param remote_address The address of a remote device.
 * \param acl_config ACL link configuration. This can be a combination of the following values:
 *                   \li HCI_CONFIG_ENABLE_AUTHENTICATION
 *                   \li HCI_CONFIG_ENABLE_ENCRYPTION
 *                   \li HCI_CONFIG_BECOME_MASTER
 * 
 * \return
 *        \li \c TRUE if connection establishment has been started.
 *        \li \c FALSE otherwise.
*/
bt_bool bt_avctp_connect(bt_avctp_channel_t* channel, bt_bdaddr_t* remote_address, bt_uint acl_config);

/**
 * \brief Disconnect from a remote device.
 * \ingroup avctp
 *
 * \details This function closes a connection to a remote device. 
 *
 * \param channel AVCTP channel.
 * 
 * \return
 *        \li \c TRUE if disconnection has been started.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_avctp_disconnect(bt_avctp_channel_t* channel);

/**
 * \brief Send a command message to a remote device.
 * \ingroup avctp
 *
 * \details This function sends a command message to a remote device.
 *
 * \param channel AVCTP channel.
 * \param data Message body.
 * \param data_len Message body length.
 * \param tran_id Pointer to a bt_byte where AVRCP will write transaction id assigned to the message.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_avctp_send_command(bt_avctp_channel_t* channel, bt_byte* data, bt_uint data_len, bt_byte* tran_id);

/**
 * \brief Send a response message to a remote device.
 * \ingroup avctp
 *
 * \details This function sends a response message to a remote device.
 *
 * \param channel AVCTP channel.
 * \param tran_id Transaction Id. This value is obtained by calling bt_avctp_send_command.
 * \param data Message body.
 * \param data_len Message body length.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_avctp_send_response(bt_avctp_channel_t* channel, bt_byte tran_id, bt_byte* data, bt_uint data_len);

/**
 * \brief Cancel a command message.
 * \ingroup avctp
 *
 * \details If a message has not yet been sent to the remote device, it can be canceled (i.e. removed from send queue) by calling this function.
 *
 * \param channel AVCTP channel.
 * \param tran_id Transaction Id. This value is obtained by calling bt_avctp_send_command.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise. No events will be generated.
*/
void bt_avctp_cancel_command(bt_avctp_channel_t* channel, bt_byte tran_id);

/**
 * \brief Cancel a response message.
 * \ingroup avctp
 *
 * \details If a message has not yet been sent to the remote device, it can be canceled (i.e. removed from send queue) by calling this function.
 *
 * \param channel AVCTP channel.
 * \param tran_id Transaction Id. This value is obtained by calling bt_avctp_send_command.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise. No events will be generated.
*/
void bt_avctp_cancel_response(bt_avctp_channel_t* channel, bt_byte tran_id);

/**
 * \brief Get HCI connection for a channel
 * \ingroup avctp
 *
 * \details This function returns a pointer to a structure that describes an HCI connection a channel is open on. 
 *          The return value can be used to call various function from the HCI layer. For example, if an app
 *          wants to force disconnection from a remote device it can call ::bt_hci_disconnect.
 *
 * \param channel AVCTP channel.
 * 
 * \return
 *        \li \c Pointer to a structure that describes an HCI connection if the function succeeds.
 *        \li \c NULL otherwise. The function fails only if a channel specified by the \p channel parameter 
 *        \li does not exist or there is no HCI connection between local and remote devices associated with the channel.
 */
bt_hci_conn_state_t* bt_avctp_get_hci_connection(bt_avctp_channel_t* channel);

#ifdef __cplusplus
}
#endif

#include "cdbt/avctp/avctp_private.h"

#endif // __AVCTP_H
