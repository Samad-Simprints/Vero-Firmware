/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __A2DP_H
#define __A2DP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup a2dp Advanced Audio Distribution Profile (A2DP)
 *
 * The Advanced Audio Distribution Profile (A2DP) defines the protocols and procedures 
 * that realize distribution of audio content of high-quality in mono or stereo on ACL channels.
 *
 */

#include "cdbt/l2cap/l2cap.h"
#include "cdbt/rfcomm/rfcomm.h"
#include "cdbt/sdp/sdp.h"
#include "cdbt/sdp/sdp_client.h"
#include "cdbt/bt/bt_signal.h"
#include "cdbt/avdtp/avdtp.h"

/** \addtogroup a2dp
* @{
*/
/** @name Source features
*
* \details The following is a list of features a source can support. The supported features are specified 
*          in the corresponding SDP service record and can be retrieved with ::bt_a2dp_find_source.
*/
/**@{
*/
#define A2DP_SOURCE_FEATURE_PLAYER        1    ///< Player
#define A2DP_SOURCE_FEATURE_MICROPHONE    2    ///< Mic
#define A2DP_SOURCE_FEATURE_TUNER         4    ///< Tuner
#define A2DP_SOURCE_FEATURE_MIXER         8    ///< Mixer
/**@}*/ 
/**@}*/

/** \addtogroup a2dp
* @{
*/
/** @name Sink features
*
* \details The following is a list of features a sink can support. The supported features are specified 
*          in the corresponding SDP service record and can be retrieved with ::bt_a2dp_find_sink.
*/
/**@{
*/
#define A2DP_SINK_FEATURE_HEADPHONE       1    ///< Headphone
#define A2DP_SINK_FEATURE_SPEAKER         2    ///< Speaker
#define A2DP_SINK_FEATURE_RECORDER        4    ///< Recorder
#define A2DP_SINK_FEATURE_AMPLIFIER       8    ///< Amplifier
/**@}*/ 
/**@}*/

#define A2DP_MANAGER_STATE_IDLE           0
#define A2DP_MANAGER_STATE_CONNECTING     1

#define A2DP_EVT_NOTHING                             AVDTP_EVT_NULL
/** \addtogroup a2dp
* @{
*/
/** @name Events
*
* \details The following is a list of events A2DP layer generates and can report to the upper layer when 
*          it completes executing an operation initiated by either local or remote device. 
*/
/**@{
*/
#define A2DP_EVT_CTRL_CHANNEL_CONNECTED              AVDTP_EVT_CTRL_CHANNEL_CONNECTED               ///< This event is generated when a control channel between two AVDTP entities has been established.
#define A2DP_EVT_CTRL_CHANNEL_DISCONNECTED           AVDTP_EVT_CTRL_CHANNEL_DISCONNECTED			///< This event is generated when a control channel between two AVDTP entities has been terminated.
#define A2DP_EVT_CTRL_CONNECTION_FAILED              AVDTP_EVT_CTRL_CONNECTION_FAILED				///< This event is generated when a local device failed to create a control channel between two AVDTP entities.
#define A2DP_EVT_DISCOVER_SEP_COMPLETED              AVDTP_EVT_DISCOVER_COMPLETED					///< This event is generated when a local device received a response (either positive or negative) to a "discover" request.
#define A2DP_EVT_GET_SEP_CAPABILITIES_COMPLETED      AVDTP_EVT_GET_SEP_CAPABILITIES_COMPLETED		///< This event is generated when a local device received a response (either positive or negative) to a "get SEP capabilities" request.
#define A2DP_EVT_SET_STREAM_CONFIGURATION_COMPLETED  AVDTP_EVT_SET_STREAM_CONFIGURATION_COMPLETED	///< This event is generated when a local device received a response (either positive or negative) to a "set stream configuration" request.
#define A2DP_EVT_GET_STREAM_CONFIGURATION_COMPLETED  AVDTP_EVT_GET_STREAM_CONFIGURATION_COMPLETED	///< This event is generated when a local device received a response (either positive or negative) to a "get stream configuration" request.
#define A2DP_EVT_RECONFIGURE_STREAM_COMPLETED        AVDTP_EVT_STREAM_RECONFIGURE_COMPLETED			///< This event is generated when a local device received a response (either positive or negative) to a "change stream configuration" request.
#define A2DP_EVT_OPEN_STREAM_COMPLETED               AVDTP_EVT_OPEN_STREAM_COMPLETED				///< This event is generated when a local device received a response (either positive or negative) to a "open stream" request.
#define A2DP_EVT_START_STREAM_COMPLETED              AVDTP_EVT_START_STREAM_COMPLETED				///< This event is generated when a local device received a response (either positive or negative) to a "start stream" request.
#define A2DP_EVT_CLOSE_STREAM_COMPLETED              AVDTP_EVT_CLOSE_STREAM_COMPLETED				///< This event is generated when a local device received a response (either positive or negative) to a "close stream" request.
#define A2DP_EVT_SUSPEND_STREAM_COMPLETED            AVDTP_EVT_SUSPEND_STREAM_COMPLETED				///< This event is generated when a local device received a response (either positive or negative) to a "suspend stream" request.
#define A2DP_EVT_STREAM_SECURITY_CONTROL_COMPLETED   AVDTP_EVT_STREAM_SECURITY_CONTROL_COMPLETED	///< This event is generated when a local device received a response (either positive or negative) to a "exchange content protection control data" request.
#define A2DP_EVT_ABORT_STREAM_COMPLETED              AVDTP_EVT_ABORT_STREAM_COMPLETED				///< This event is generated when a local device received a response (either positive or negative) to a "abort stream" request.
#define A2DP_EVT_SEP_INFO_RECEIVED                   AVDTP_EVT_SEP_INFO_RECEIVED					///< This event is generated for each SEP contained in a positive response to a "discover" request.
#define A2DP_EVT_SEP_CAPABILITIES_RECEIVED           AVDTP_EVT_SEP_CAPABILITIES_RECEIVED			///< This event is generated when a local device received a positive response to a "get SEP capabilities" request.
#define A2DP_EVT_STREAM_CONFIGURATION_RECEIVED       AVDTP_EVT_STREAM_CONFIGURATION_RECEIVED		///< This event is generated when a local device received a positive response to a "get stream configuration" request.
													   
// event generated when local device is acceptor   
#define A2DP_EVT_SET_STREAM_CONFIGURATION_REQUESTED  AVDTP_EVT_SET_STREAM_CONFIGURATION_REQUESTED   ///< This event is generated when a local device received "set stream configuration" request.
#define A2DP_EVT_OPEN_STREAM_REQUESTED               AVDTP_EVT_OPEN_STREAM_REQUESTED				///< This event is generated when a local device received "open stream" request.
#define A2DP_EVT_START_STREAM_REQUESTED              AVDTP_EVT_START_STREAM_REQUESTED				///< This event is generated when a local device received "start stream" request.
#define A2DP_EVT_CLOSE_STREAM_REQUESTED              AVDTP_EVT_CLOSE_STREAM_REQUESTED				///< This event is generated when a local device received "close stream" request.
#define A2DP_EVT_SUSPEND_STREAM_REQUESTED            AVDTP_EVT_SUSPEND_STREAM_REQUESTED				///< This event is generated when a local device received "suspend stream" request.
#define A2DP_EVT_ABORT_STREAM_REQUESTED              AVDTP_EVT_ABORT_STREAM_REQUESTED				///< This event is generated when a local device received "abort stream" request.
#define A2DP_EVT_RECONFIGURE_STREAM_REQUESTED        AVDTP_EVT_RECONFIGURE_STREAM_REQUESTED			///< This event is generated when a local device received "change stream configuration" request.
#define A2DP_EVT_MEDIA_PACKET_RECEIVED               AVDTP_EVT_MEDIA_PACKET_RECEIVED				///< This event is generated when a local device received a media packet.
#define A2DP_EVT_STREAM_CONFIGURED                   AVDTP_EVT_STREAM_CONFIGURED					///< This event is generated when a local device has successfully configured a stream. 
                                                                                                    ///< This event follows the A2DP_EVT_SET_STREAM_CONFIGURATION_REQUESTED if the upper layer has accepted it.
                                                                                                    ///< This event is not generated if stream configuration was initiated by the local device.
#define A2DP_EVT_STREAM_RECONFIGURED                 AVDTP_EVT_STREAM_RECONFIGURED                  ///< This event is generated when a local device has successfully reconfigured a stream. 
                                                                                                    ///< This event follows the A2DP_EVT_RECONFIGURE_STREAM_REQUESTED if the upper layer has accepted it.
                                                                                                    ///< This event is not generated if stream reconfiguration was initiated by the local device.
#define A2DP_EVT_STREAM_OPENED                       AVDTP_EVT_STREAM_OPENED                        ///< This event is generated when a local device has successfully opened a stream. 
                                                                                                    ///< This event follows the A2DP_EVT_OPEN_STREAM_REQUESTED if the upper layer has accepted it.
                                                                                                    ///< This event is not generated if stream opening was initiated by the local device.
#define A2DP_EVT_STREAM_STARTED                      AVDTP_EVT_STREAM_STARTED                       ///< This event is generated when a local device has successfully started a stream. 
                                                                                                    ///< This event follows the A2DP_EVT_START_STREAM_REQUESTED if the upper layer has accepted it.
                                                                                                    ///< This event is not generated if stream starting was initiated by the local device.
#define A2DP_EVT_STREAM_CLOSED                       AVDTP_EVT_STREAM_CLOSED                        ///< This event is generated when a local device has successfully closed a stream. 
                                                                                                    ///< This event follows the A2DP_EVT_CLOSE_STREAM_REQUESTED if the upper layer has accepted it.
                                                                                                    ///< This event is not generated if stream closing was initiated by the local device.
#define A2DP_EVT_STREAM_SUSPENDED                    AVDTP_EVT_STREAM_SUSPENDED                     ///< This event is generated when a local device has successfully suspended a stream. 
                                                                                                    ///< This event follows the A2DP_EVT_SUSPEND_STREAM_REQUESTED if the upper layer has accepted it.
                                                                                                    ///< This event is not generated if stream suspension was initiated by the local device.
#define A2DP_EVT_STREAM_ABORTED                      AVDTP_EVT_STREAM_ABORTED                       ///< This event is generated when a local device has successfully aborted a stream. 
                                                                                                    ///< This event follows the A2DP_EVT_ABORT_STREAM_REQUESTED if the upper layer has accepted it.
                                                                                                    ///< This event is not generated if stream abortion was initiated by the local device.
#define A2DP_EVT_MEDIA_PACKET_SENT                   AVDTP_EVT_MEDIA_PACKET_SENT                    ///< This event is generated when a local device sent a media packet.
#define A2DP_EVT_MEDIA_PACKET_SEND_FAILED            AVDTP_EVT_MEDIA_PACKET_SEND_FAILED             ///< This event is generated when a local device failed to send a media packet.

#define A2DP_EVT_OPEN_AND_START_STREAM_COMPLETED     (AVDTP_EVT_LAST + 1)                           ///< This event is generated when a local device completed "open and start" request.
/**@}*/ 
/**@}*/

typedef struct _bt_a2dp_mgr_t bt_a2dp_mgr_t;
typedef union _bt_a2dp_event_u bt_a2dp_event_t;

/**
* \brief Notify the application of the result of searching for a remote A2DP entity (sourse or sink)
* \ingroup a2dp
*
* \details This function is called by the A2DP layer when searching for an A2DP entity on a remote device has completed.
*
* \param supported_features Features supported by a remote A2DP entity.
* \param found \c TRUE if an A2DP entity has been found on the remote device. \c FALSE otherwise.
* \param param pointer to arbitrary data passed to the bt_a2dp_find_source() or bt_a2dp_find_sink function through its \c callback_param parameter.
*
*/
typedef void (*bt_a2dp_find_server_callback_fp)(bt_uint supported_features, bt_bool found, void* param);

/**
 * \brief A2DP application callback.
 * \ingroup a2dp
 *
 * \details In order to be notified of various events a consumer of the A2DP layer has to register a callback function. 
 *          The stack will call that function whenever a new event has been generated.
 *
 * \param mgr A2DP manager.
 *
 * \param evt A2DP event.
 *          The event can be one of the following values:
 *            @arg A2DP_EVT_CTRL_CHANNEL_CONNECTED:             Control channel connected.
 *            @arg A2DP_EVT_CTRL_CHANNEL_DISCONNECTED:          Control channel disconnected.
 *            @arg A2DP_EVT_CTRL_CONNECTION_FAILED:             Control channel connection failed (generated only if control connection has been initiated by the local device).
 *            @arg A2DP_EVT_DISCOVER_COMPLETED:                 Local device completed discovering remote SEPs.
 *            @arg A2DP_EVT_GET_SEP_CAPABILITIES_COMPLETED:     Local device received a response to Get SEP capabilities operation.
 *            @arg A2DP_EVT_SET_STREAM_CONFIGURATION_COMPLETED: Local device received a response to Set stream configuration operation.
 *            @arg A2DP_EVT_GET_STREAM_CONFIGURATION_COMPLETED: Local device received a response to Get stream configuration operation.
 *            @arg A2DP_EVT_STREAM_RECONFIGURE_COMPLETED:       Local device received a response to Reconfigure stream operation.
 *            @arg A2DP_EVT_OPEN_STREAM_COMPLETED:              Local device received a response to Open stream operation.
 *            @arg A2DP_EVT_START_STREAM_COMPLETED:             Local device received a response to Start stream operation.
 *            @arg A2DP_EVT_CLOSE_STREAM_COMPLETED:             Local device received a response to Close stream operation.
 *            @arg A2DP_EVT_SUSPEND_STREAM_COMPLETED:           Local device received a response to Suspend stream operation.
 *            @arg A2DP_EVT_STREAM_SECURITY_CONTROL_COMPLETED:  Local device received a response to Stream security control operation.
 *            @arg A2DP_EVT_ABORT_STREAM_COMPLETED:             Local device received a response to Abort stream operation.
 *            @arg A2DP_EVT_SEP_INFO_RECEIVED:                  SEP information received.
 *            @arg A2DP_EVT_SEP_CAPABILITIES_RECEIVED:          SEP capabilities received.
 *            @arg A2DP_EVT_STREAM_CONFIGURATION_RECEIVED:      Stream configuration received.
 *
 *            @arg A2DP_EVT_SET_STREAM_CONFIGURATION_REQUESTED: Remote device requested stream configuration.
 *            @arg A2DP_EVT_OPEN_STREAM_REQUESTED:              Remote device requested to open a stream.
 *            @arg A2DP_EVT_START_STREAM_REQUESTED:             Remote device requested to start a stream.
 *            @arg A2DP_EVT_CLOSE_STREAM_REQUESTED:             Remote device requested to close a stream.
 *            @arg A2DP_EVT_SUSPEND_STREAM_REQUESTED:           Remote device requested to suspend a stream.
 *            @arg A2DP_EVT_ABORT_STREAM_REQUESTED:             Remote device requested to abort a stream.
 *            @arg A2DP_EVT_RECONFIGURE_STREAM_REQUESTED:       Remote device requested to reconfigure a stream.
 *            @arg A2DP_EVT_MEDIA_PACKET_RECEIVED:              Remote device sent a media packet.
 *            @arg A2DP_EVT_STREAM_CONFIGURED:                  A stream has been configured (This event is generated right after A2DP_EVT_SET_STREAM_CONFIGURATION_REQUESTED if the local devices accepted the request).
 *            @arg A2DP_EVT_STREAM_RECONFIGURED:                A stream has been re-configured (This event is generated right after A2DP_EVT_RECONFIGURE_STREAM_REQUESTED if the local devices accepted the request).
 *            @arg A2DP_EVT_STREAM_OPENED:                      A stream has been opened (This event is generated as a result of local or remote stream opening request).
 *            @arg A2DP_EVT_STREAM_STARTED:                     A stream has been started (This event is generated right after A2DP_EVT_START_STREAM_REQUESTED if the local devices accepted the request).
 *            @arg A2DP_EVT_STREAM_CLOSED:                      A stream has been close (This event is generated right after A2DP_EVT_CLOSE_STREAM_REQUESTED if the local devices accepted the request).
 *            @arg A2DP_EVT_STREAM_SUSPENDED:                   A stream has been suspended (This event is generated right after A2DP_EVT_SUSPEND_STREAM_REQUESTED if the local devices accepted the request).
 *            @arg A2DP_EVT_STREAM_ABORTED:                     A stream has been aborted (This event is generated right after A2DP_EVT_SUSPEND_STREAM_REQUESTED 
 *                                                               if the local devices accepted the request. 
 *                                                               It is also generated if connection between devices has been terminated by means other than
 *                                                               AVDTP signaling, e.g. devices going out of rage).
 *            @arg A2DP_EVT_MEDIA_PACKET_SENT:                  The local device has successfully sent a media packet to the remote device.
 *            @arg A2DP_EVT_MEDIA_PACKET_SEND_FAILED:           The local device was not able to send a media packet to the remote device.
 *
 *            @arg A2DP_EVT_OPEN_AND_START_STREAM_COMPLETED     Local device completed "open and start" request.
 *
 * \param evt_param Event parameter.
 *          Which member of the bt_a2dp_event_t union is valid depends on the event:
 *            @arg A2DP_EVT_CTRL_CHANNEL_CONNECTED:             \c bt_avdtp_evt_ctrl_channel_connected_t               ctrl_channel_connected
 *            @arg A2DP_EVT_CTRL_CHANNEL_DISCONNECTED:          \c bt_avdtp_evt_ctrl_channel_disconnected_t            ctrl_channel_disconnected
 *            @arg A2DP_EVT_CTRL_CONNECTION_FAILED:             \c NULL
 *            @arg A2DP_EVT_DISCOVER_COMPLETED:                 \c bt_avdtp_evt_discover_completed_t                   discover_completed;
 *            @arg A2DP_EVT_GET_SEP_CAPABILITIES_COMPLETED:     \c bt_avdtp_evt_get_sep_capabilities_completed_t       get_sep_capabilities_completed
 *            @arg A2DP_EVT_SET_STREAM_CONFIGURATION_COMPLETED: \c bt_avdtp_evt_set_stream_configuration_completed_t   set_stream_configuration_completed
 *            @arg A2DP_EVT_GET_STREAM_CONFIGURATION_COMPLETED: \c bt_avdtp_evt_get_stream_configuration_completed_t   get_stream_configuration_completed
 *            @arg A2DP_EVT_STREAM_RECONFIGURE_COMPLETED:       \c bt_avdtp_evt_stream_reconfigure_completed_t         stream_reconfigure_completed
 *            @arg A2DP_EVT_OPEN_STREAM_COMPLETED:              \c bt_avdtp_evt_open_stream_completed_t                open_stream_completed
 *            @arg A2DP_EVT_START_STREAM_COMPLETED:             \c bt_avdtp_evt_start_stream_completed_t               start_stream_completed
 *            @arg A2DP_EVT_CLOSE_STREAM_COMPLETED:             \c bt_avdtp_evt_close_stream_completed_t               close_stream_completed
 *            @arg A2DP_EVT_SUSPEND_STREAM_COMPLETED:           \c bt_avdtp_evt_suspend_stream_completed_t             suspend_stream_completed
 *            @arg A2DP_EVT_STREAM_SECURITY_CONTROL_COMPLETED:  \c bt_avdtp_evt_stream_security_control_completed_t    security_control_completed
 *            @arg A2DP_EVT_ABORT_STREAM_COMPLETED:             \c bt_avdtp_evt_abort_stream_requested_t               abort_stream_requested
 *            @arg A2DP_EVT_SEP_INFO_RECEIVED:                  \c bt_avdtp_evt_sep_info_received_t                    sep_info_received
 *            @arg A2DP_EVT_SEP_CAPABILITIES_RECEIVED:          \c bt_avdtp_evt_sep_capabilities_received_t            sep_capabilities_received
 *            @arg A2DP_EVT_STREAM_CONFIGURATION_RECEIVED:      \c bt_avdtp_evt_sep_capabilities_received_t            sep_capabilities_received
 *
 *            @arg A2DP_EVT_SET_STREAM_CONFIGURATION_REQUESTED: \c bt_avdtp_evt_set_stream_configuration_requested_t   set_stream_configuration_requested
 *            @arg A2DP_EVT_OPEN_STREAM_REQUESTED:              \c bt_avdtp_evt_open_stream_requested_t                open_stream_requested
 *            @arg A2DP_EVT_START_STREAM_REQUESTED:             \c bt_avdtp_evt_start_stream_requested_t               start_stream_requested
 *            @arg A2DP_EVT_CLOSE_STREAM_REQUESTED:             \c bt_avdtp_evt_close_stream_requested_t               close_stream_requested
 *            @arg A2DP_EVT_SUSPEND_STREAM_REQUESTED:           \c bt_avdtp_evt_suspend_stream_requested_t             suspend_stream_requested
 *            @arg A2DP_EVT_ABORT_STREAM_REQUESTED:             \c bt_avdtp_evt_abort_stream_requested_t               abort_stream_requested
 *            @arg A2DP_EVT_RECONFIGURE_STREAM_REQUESTED:       \c bt_avdtp_evt_reconfigure_stream_requested_t         reconfigure_stream_requested
 *            @arg A2DP_EVT_MEDIA_PACKET_RECEIVED:              \c bt_avdtp_evt_media_packet_received_t                media_packet_received
 *            @arg A2DP_EVT_STREAM_CONFIGURED:                  \c bt_avdtp_evt_stream_configured_t                    stream_configured
 *            @arg A2DP_EVT_STREAM_RECONFIGURED:                \c bt_avdtp_evt_stream_reconfigured_t                  stream_reconfigured
 *            @arg A2DP_EVT_STREAM_OPENED:                      \c bt_avdtp_evt_stream_opened_t                        stream_opened
 *            @arg A2DP_EVT_STREAM_STARTED:                     \c bt_avdtp_evt_stream_started_t                       stream_started
 *            @arg A2DP_EVT_STREAM_CLOSED:                      \c bt_avdtp_evt_stream_closed_t                        stream_closed
 *            @arg A2DP_EVT_STREAM_SUSPENDED:                   \c bt_avdtp_evt_stream_suspended_t                     stream_suspended
 *            @arg A2DP_EVT_STREAM_ABORTED:                     \c bt_avdtp_evt_stream_aborted_t                       stream_aborted
 *            @arg A2DP_EVT_MEDIA_PACKET_SENT:                  \c bt_avdtp_evt_media_packet_sent_t                    media_packet_sent
 *            @arg A2DP_EVT_MEDIA_PACKET_SEND_FAILED:           \c bt_avdtp_evt_media_packet_send_failed_t             media_packet_send_failed
 *
 *            @arg A2DP_EVT_OPEN_AND_START_STREAM_COMPLETED     \c bt_a2dp_evt_open_and_start_stream_completed_t       open_and_start_stream_completed
 *
 * \param callback_param A pointer to an arbitrary data set by a call to bt_avdtp_register_callback.
 * 
 */
typedef void (*bt_a2dp_mgr_callback_fp)(bt_a2dp_mgr_t* mgr, bt_byte evt, bt_a2dp_event_t* evt_param, void* callback_param);

/**
* \brief  Parameter to A2DP_EVT_OPEN_AND_START_STREAM_COMPLETED event
* \ingroup a2dp
*
* \details A pointer to this structure is passed to the A2DP application callback as 
*          a valid member of the bt_a2dp_event_t union - bt_a2dp_event_t::open_and_start_stream_completed -
*          when A2DP completed a "open & start stream" request. 
*/
typedef struct _bt_a2dp_evt_open_and_start_stream_completed_s
{
	bt_byte failed_cmd;                                                  ///< ID of the failed command. This can be one of the following values:
                                                                         ///< \li AVDTP_CMD_SET_CONFIGURATION
                                                                         ///< \li AVDTP_CMD_OPEN
                                                                         ///< \li AVDTP_CMD_START
	bt_byte err_code;                                                    ///< The result of the request.
                                                                         ///< \li If the remote accepted all 3 requests sent during execution of the "open & start" request \c err_code == AVDTP_ERROR_SUCCESS.
                                                                         ///< \li Otherwise \c err_code == the error code returned by the remote party.
	bt_byte strm_handle;                                                 ///< Stream handle.
	union
	{
		bt_avdtp_evt_set_stream_configuration_completed_t set_config;    ///< If "set configuration" request failed this member contains description of an error returned by the remote party.
		bt_avdtp_evt_open_stream_completed_t open;                       ///< If open stream" request failed this member contains description of an error returned by the remote party.
		bt_avdtp_evt_start_stream_completed_t start;                     ///< If "start stream" request failed this member contains description of an error returned by the remote party.
	} evt_params;
} bt_a2dp_evt_open_and_start_stream_completed_t;

/**
* \brief  Parameter to an application callback.
* \ingroup a2dp
*
* \details This union is used to pass event specific data to the A2DP consumer. Which member of the union 
*          points to a valid structure depends on the event reported to the consumer. In general, each event
*          has a corresponding member in the union.
*/
union _bt_a2dp_event_u
{
	bt_a2dp_evt_open_and_start_stream_completed_t       open_and_start_stream_completed;      ///< Valid if event is A2DP_EVT_OPEN_AND_START_STREAM_COMPLETED

	// AVDTP event parameters
	bt_avdtp_evt_ctrl_channel_connected_t               ctrl_channel_connected;                ///< Valid if event is A2DP_EVT_CTRL_CHANNEL_CONNECTED
	bt_avdtp_evt_ctrl_channel_disconnected_t            ctrl_channel_disconnected;			   ///< Valid if event is A2DP_EVT_CTRL_CHANNEL_DISCONNECTED
	bt_avdtp_evt_discover_completed_t                   discover_completed;					   ///< Valid if event is A2DP_EVT_DISCOVER_COMPLETED
	bt_avdtp_evt_sep_info_received_t                    sep_info_received;					   ///< Valid if event is A2DP_EVT_SEP_INFO_RECEIVED
	bt_avdtp_evt_get_sep_capabilities_completed_t       get_sep_capabilities_completed;		   ///< Valid if event is A2DP_EVT_GET_SEP_CAPABILITIES_COMPLETED
	bt_avdtp_evt_sep_capabilities_received_t            sep_capabilities_received;			   ///< Valid if event is A2DP_EVT_SEP_CAPABILITIES_RECEIVED
	bt_avdtp_evt_set_stream_configuration_completed_t   set_stream_configuration_completed;	   ///< Valid if event is A2DP_EVT_SET_STREAM_CONFIGURATION_COMPLETED
	bt_avdtp_evt_get_stream_configuration_completed_t   get_stream_configuration_completed;	   ///< Valid if event is A2DP_EVT_GET_STREAM_CONFIGURATION_COMPLETED
	bt_avdtp_evt_stream_reconfigure_completed_t         stream_reconfigure_completed;		   ///< Valid if event is A2DP_EVT_STREAM_RECONFIGURE_COMPLETED
	bt_avdtp_evt_open_stream_completed_t                open_stream_completed;				   ///< Valid if event is A2DP_EVT_OPEN_STREAM_COMPLETED
	bt_avdtp_evt_start_stream_completed_t               start_stream_completed;				   ///< Valid if event is A2DP_EVT_START_STREAM_COMPLETED
	bt_avdtp_evt_close_stream_completed_t               close_stream_completed;				   ///< Valid if event is A2DP_EVT_CLOSE_STREAM_COMPLETED
	bt_avdtp_evt_suspend_stream_completed_t             suspend_stream_completed;			   ///< Valid if event is A2DP_EVT_SUSPEND_STREAM_COMPLETED
	bt_avdtp_evt_stream_security_control_completed_t    security_control_completed;			   ///< Valid if event is A2DP_EVT_STREAM_SECURITY_CONTROL_COMPLETED
	bt_avdtp_evt_set_stream_configuration_requested_t   set_stream_configuration_requested;	   ///< Valid if event is A2DP_EVT_SET_STREAM_CONFIGURATION_REQUESTED
	bt_avdtp_evt_reconfigure_stream_requested_t         reconfigure_stream_requested;		   ///< Valid if event is A2DP_EVT_RECONFIGURE_STREAM_REQUESTED
	bt_avdtp_evt_open_stream_requested_t                open_stream_requested;				   ///< Valid if event is A2DP_EVT_OPEN_STREAM_REQUESTED
	bt_avdtp_evt_start_stream_requested_t               start_stream_requested;				   ///< Valid if event is A2DP_EVT_START_STREAM_REQUESTED
	bt_avdtp_evt_suspend_stream_requested_t             suspend_stream_requested;			   ///< Valid if event is A2DP_EVT_SUSPEND_STREAM_REQUESTED
	bt_avdtp_evt_close_stream_requested_t               close_stream_requested;				   ///< Valid if event is A2DP_EVT_CLOSE_STREAM_REQUESTED
	bt_avdtp_evt_abort_stream_requested_t               abort_stream_requested;				   ///< Valid if event is A2DP_EVT_ABORT_STREAM_REQUESTED

	/////////////////////////////////////////////////////////////////////////////

	bt_avdtp_evt_stream_configured_t                    stream_configured;                     ///< Valid if event is A2DP_EVT_STREAM_CONFIGURED
	bt_avdtp_evt_stream_reconfigured_t                  stream_reconfigured;				   ///< Valid if event is A2DP_EVT_STREAM_RECONFIGURED
	bt_avdtp_evt_stream_opened_t                        stream_opened;						   ///< Valid if event is A2DP_EVT_STREAM_OPENED
	bt_avdtp_evt_stream_started_t                       stream_started;						   ///< Valid if event is A2DP_EVT_STREAM_STARTED
	bt_avdtp_evt_stream_suspended_t                     stream_suspended;					   ///< Valid if event is A2DP_EVT_STREAM_SUSPENDED
	bt_avdtp_evt_stream_closed_t                        stream_closed;						   ///< Valid if event is A2DP_EVT_STREAM_CLOSED
	bt_avdtp_evt_stream_aborted_t                       stream_aborted;						   ///< Valid if event is A2DP_EVT_STREAM_ABORTED

	/////////////////////////////////////////////////////////////////////////////

	bt_avdtp_evt_media_packet_received_t                media_packet_received;                 //< Valid if event is A2DP_EVT_MEDIA_PACKET_RECEIVED
	bt_avdtp_evt_media_packet_sent_t                    media_packet_sent;					   //< Valid if event is A2DP_EVT_MEDIA_PACKET_SENT
	bt_avdtp_evt_media_packet_send_failed_t             media_packet_send_failed;              //< Valid if event is A2DP_EVT_MEDIA_PACKET_SEND_FAILED
};


/**
 * \brief A2DP manager.
 * \ingroup a2dp
 *
 * \details A structure that glues all pieces together. There is only one instance of this structure allocated by dotstack.
 *          A pointer to the instance can be get with ::bt_a2dp_get_mgr.
 *          
 */
struct _bt_a2dp_mgr_t
{
	bt_byte state;                         ///< Manager state. This value can be one of the following values:
                                           ///< \li A2DP_MANAGER_STATE_IDLE
                                           ///< \li A2DP_MANAGER_STATE_CONNECTING
	bt_avdtp_mgr_t* avdtp_mgr;             ///< AVDTP manager.
	bt_avdtp_stream_t* connection_stream;  ///< Pointer to a stream being open with ::bt_a2dp_open_and_start_stream.

	bt_a2dp_mgr_callback_fp callback;      ///< Pointer to a function which a A2DP consumer must register in order to be notified of various events.
	void* callback_param;                  ///< Pointer to arbitrary data to be passed to the \c callback.
};

/**
 * \brief Return a pointer to an instance of the A2DP manager.
 * \ingroup a2dp
 *
 * \details This function returns a pointer to an instance of the A@DP manager. There is only one instance of the manager allocated by the stack. 
 *          The pointer is passed as the first parameter to all A2DP functions.
 */
bt_a2dp_mgr_t* bt_a2dp_get_mgr(void);

/**
 * \brief Initialize the A2DP layer.
 * \ingroup a2dp
 *
 * \details This function initializes the A2DP layer of the stack. It must be called prior to any other
 * A2DP function can be called.
 */
void bt_a2dp_init(void);

/**
 * \brief Start the A2DP layer.
 * \ingroup a2dp
 *
 * \details This function makes the A2DP layer ready to accept connection requests from remote device. 
 *          To make an outgoing connection calling this function is not required.
 *
 * \param mgr AVDTP manager.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
*/
bt_bool bt_a2dp_start(bt_a2dp_mgr_t* mgr);

/**
 * \brief Register a A2DP application callback.
 * \ingroup a2dp
 *
 * \details In order to be notified of various events a consumer of the A2DP layer has to register a callback function. 
 *          The stack will call this function whenever a new event has been generated passing the code of the event as the second parameter.
 *          The event can be one of the following values:
 * 
 *    @arg A2DP_EVT_CTRL_CHANNEL_CONNECTED:             Control channel connected.
 *    @arg A2DP_EVT_CTRL_CHANNEL_DISCONNECTED:          Control channel disconnected.
 *    @arg A2DP_EVT_CTRL_CONNECTION_FAILED:             Control channel connection failed (generated only if control connection has been initiated by the local device).
 *    @arg A2DP_EVT_DISCOVER_COMPLETED:                 Local device completed discovering remote SEPs.
 *    @arg A2DP_EVT_GET_SEP_CAPABILITIES_COMPLETED:     Local device received a response to Get SEP capabilities operation.
 *    @arg A2DP_EVT_SET_STREAM_CONFIGURATION_COMPLETED: Local device received a response to Set stream configuration operation.
 *    @arg A2DP_EVT_GET_STREAM_CONFIGURATION_COMPLETED: Local device received a response to Get stream configuration operation.
 *    @arg A2DP_EVT_STREAM_RECONFIGURE_COMPLETED:       Local device received a response to Reconfigure stream operation.
 *    @arg A2DP_EVT_OPEN_STREAM_COMPLETED:              Local device received a response to Open stream operation.
 *    @arg A2DP_EVT_START_STREAM_COMPLETED:             Local device received a response to Start stream operation.
 *    @arg A2DP_EVT_CLOSE_STREAM_COMPLETED:             Local device received a response to Close stream operation.
 *    @arg A2DP_EVT_SUSPEND_STREAM_COMPLETED:           Local device received a response to Suspend stream operation.
 *    @arg A2DP_EVT_STREAM_SECURITY_CONTROL_COMPLETED:  Local device received a response to Stream security control operation.
 *    @arg A2DP_EVT_ABORT_STREAM_COMPLETED:             Local device received a response to Abort stream operation.
 *    @arg A2DP_EVT_SEP_INFO_RECEIVED:                  SEP information received.
 *    @arg A2DP_EVT_SEP_CAPABILITIES_RECEIVED:          SEP capabilities received.
 *    @arg A2DP_EVT_STREAM_CONFIGURATION_RECEIVED:      Stream configuration received.
 *
 *    @arg A2DP_EVT_SET_STREAM_CONFIGURATION_REQUESTED: Remote device requested stream configuration.
 *    @arg A2DP_EVT_OPEN_STREAM_REQUESTED:              Remote device requested to open a stream.
 *    @arg A2DP_EVT_START_STREAM_REQUESTED:             Remote device requested to start a stream.
 *    @arg A2DP_EVT_CLOSE_STREAM_REQUESTED:             Remote device requested to close a stream.
 *    @arg A2DP_EVT_SUSPEND_STREAM_REQUESTED:           Remote device requested to suspend a stream.
 *    @arg A2DP_EVT_ABORT_STREAM_REQUESTED:             Remote device requested to abort a stream.
 *    @arg A2DP_EVT_RECONFIGURE_STREAM_REQUESTED:       Remote device requested to reconfigure a stream.
 *    @arg A2DP_EVT_MEDIA_PACKET_RECEIVED:              Remote device sent a media packet.
 *    @arg A2DP_EVT_STREAM_CONFIGURED:                  A stream has been configured (This event is generated right after A2DP_EVT_SET_STREAM_CONFIGURATION_REQUESTED if the local devices accepted the request).
 *    @arg A2DP_EVT_STREAM_RECONFIGURED:                A stream has been re-configured (This event is generated right after A2DP_EVT_RECONFIGURE_STREAM_REQUESTED if the local devices accepted the request).
 *    @arg A2DP_EVT_STREAM_OPENED:                      A stream has been opened (This event is generated as a result of local or remote stream opening request).
 *    @arg A2DP_EVT_STREAM_STARTED:                     A stream has been started (This event is generated right after A2DP_EVT_START_STREAM_REQUESTED if the local devices accepted the request).
 *    @arg A2DP_EVT_STREAM_CLOSED:                      A stream has been close (This event is generated right after A2DP_EVT_CLOSE_STREAM_REQUESTED if the local devices accepted the request).
 *    @arg A2DP_EVT_STREAM_SUSPENDED:                   A stream has been suspended (This event is generated right after A2DP_EVT_SUSPEND_STREAM_REQUESTED if the local devices accepted the request).
 *    @arg A2DP_EVT_STREAM_ABORTED:                     A stream has been aborted (This event is generated right after A2DP_EVT_SUSPEND_STREAM_REQUESTED 
 *                                                      if the local devices accepted the request. 
 *                                                      It is also generated if connection between devices has been terminated by means other than
 *                                                      A2DP signaling, e.g. devices going out of range).
 *    @arg A2DP_EVT_MEDIA_PACKET_SENT:                 The local device has successfully sent a media packet to the remote device.
 *    @arg A2DP_EVT_MEDIA_PACKET_SEND_FAILED:          The local device was not able to send a media packet to the remote device.
 *
 *    @arg A2DP_EVT_OPEN_AND_START_STREAM_COMPLETED     This event is generated when a local device completed "open and start" request.
 *
 * \param mgr AVDTP manager.
 * \param callback The callback function that will be called when the AVDTP generates an event.
 * \param callback_param A pointer to arbitrary data to be passed to the \c callback callback.
 * 
 */
void bt_a2dp_register_callback(bt_a2dp_mgr_t* mgr, bt_a2dp_mgr_callback_fp callback, void* callback_param);

/**
 * \brief Find a codec
 * \ingroup a2dp
 *
 * \details A2DP in theory can support any type of codec. Each codec uses its own format for exchanging capabilities and configuration information. 
 *          In order to make our implementation do not care about these formats we use a simple way of telling AVDTP how to parse and serialize 
 *          codec's configuration. The consumer of A2DP has to register a callback function (one per codec type) for each codec it wishes to support. 
 *          That callback has to perform two function. The first one is to read the configuration received from 
 *          the remote device and store it in a structure defined by the consumer. The second one is to serialize the data from a structure 
 *          to a format (in case of standard A2DP codecs the format is defined in A2DP specification, vendor specific codecs can define
 *          their own formats) suitable for sending as a part of a AVDTP request.
 *          This function returns a pointer to a structure that holds a pointer to a codec's callback function.
 *
 *
 * \param mgr A2DP manager.
 * \param codec_type Codec type.
 *          The codec_type can be one of the following values:
 *            @arg AVDTP_CODEC_TYPE_SBC:           SBC
 *            @arg AVDTP_CODEC_TYPE_MPEG1_2_AUDIO: MPEG-1,2 (used in MP3 files)
 *            @arg AVDTP_CODEC_TYPE_MPEG2_4_AAC:   MPEG-2,4 AAC (used in Apple products)
 *            @arg AVDTP_CODEC_TYPE_ATRAC:         ATRAC (used in Sony products)
 *            @arg AVDTP_CODEC_TYPE_NON_A2DP:      Non-A2DP Codec
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise. The function fails if a callback for a codec type specified in the \p codec parameter 
 *        \li has not been previously registered with ::bt_avdtp_register_codec.
 */
#define bt_a2dp_find_codec(mgr, codec_type)                     bt_avdtp_find_codec(mgr->avdtp_mgr, codec_type)

#define bt_a2dp_call_codec(codec, op, op_param, mgr)            codec->codec_handler(codec, op, op_param, mgr->avdtp_mgr)

/**
 * \brief Connect to a remote device.
 * \ingroup a2dp
 *
 * \details This function opens a control channel connection to a remote device specified by the \p remote_addr. If connection
 *          cannot be initiated for some reason, for example, there is not enough resources, it returns FALSE and not events are generated. Otherwise
 *          the result of an attempt to connect to the remote device is reported via the AVDTP callback. 
 *          The events generated will either be A2DP_EVT_CTRL_CHANNEL_CONNECTED or A2DP_EVT_CTRL_CHANNEL_CONNECTION_FAILED.
 *
 * \param mgr A2DP manager.
 * \param remote_addr The address of a remote device.
 * 
 * \return
 *        \li \c TRUE if connection establishment has been started.
 *        \li \c FALSE otherwise.
*/
#define bt_a2dp_connect(mgr, remote_addr)                       bt_avdtp_connect(mgr->avdtp_mgr, remote_addr)

/**
 * \brief Connect to a remote device.
 * \ingroup a2dp
 *
 * \details This function opens a control channel connection to a remote device specified by the \p remote_addr. If connection
 *          cannot be initiated for some reason, for example, there is not enough resources, it returns FALSE and not events are generated. Otherwise
 *          the result of an attempt to connect to the remote device is reported via the AVDTP callback. 
 *          The events generated will either be A2DP_EVT_CTRL_CHANNEL_CONNECTED or A2DP_EVT_CTRL_CHANNEL_CONNECTION_FAILED.
 *
 * \param mgr A2DP manager.
 * \param remote_addr The address of a remote device.
 * \param acl_config ACL link configuration. This can be a combination of the following values:
 *                   \li HCI_CONFIG_ENABLE_AUTHENTICATION
 *                   \li HCI_CONFIG_ENABLE_ENCRYPTION
 *                   \li HCI_CONFIG_BECOME_MASTER
 * 
 * \return
 *        \li \c TRUE if connection establishment has been started.
 *        \li \c FALSE otherwise.
*/
#define bt_a2dp_connect_ex(mgr, remote_addr, acl_config)	   bt_avdtp_connect_ex(mgr->avdtp_mgr, remote_addr, acl_config)

/**
 * \brief Disconnect from a remote device.
 * \ingroup a2dp
 *
 * \details This function closes a control and transport channels on all streams associated with the remote device specified by the \p remote_addr. 
 *          As a result of this operation the following events will be generated:
 *            @arg A2DP_EVT_MEDIA_PACKET_RECEIVED: if a stream's receive queue is not empty this event is generated for each buffer with bt_media_packet_t::data_len set to 0
 *            @arg A2DP_EVT_MEDIA_PACKET_SENT:     if a stream's send queue is not empty this event is generated for each buffer with bt_media_packet_t::data_len set to 0
 *            @arg A2DP_EVT_STREAM_CLOSED:         this event is generate if a stream is in "closing" state as a result of a request from the remote device or 
 *                                                 ::bt_a2dp_close_stream call before ::bt_a2dp_disconnect call.
 *            @arg A2DP_EVT_STREAM_ABORTED:        this event is generated if a stream is in "active" state at the time of bt_avdtp_disconnect call.
 *
 * \param mgr A2DP manager.
 * \param remote_addr The address of a remote device.
 * 
 * \return
 *        \li \c TRUE if disconnection has been started.
 *        \li \c FALSE otherwise. No events will be generated.
*/
#define bt_a2dp_disconnect(mgr, remote_addr)                    bt_avdtp_disconnect(mgr->avdtp_mgr, remote_addr)

/**
 * \brief Register a Sink SEP with the local A2DP manager.
 * \ingroup a2dp
 *
 * \details This function is used to add a sink SEP to a list of SEPs supported by the local A2DP entity. 
 *
 * \param mgr A2DP manager.
 * \param caps The capabilities of a SEP.
 * 
 * \return
 *        \li \c ID of a SEP if the function succeeds.
 *        \li \c FALSE otherwise.
*/
#define bt_a2dp_register_sink(mgr, caps)                        bt_avdtp_register_sep(mgr->avdtp_mgr, AVDTP_SEP_TYPE_SINK, caps)

/**
 * \brief Register a Source SEP with the local A2DP manager.
 * \ingroup a2dp
 *
 * \details This function is used to add a source SEP to a list of SEPs supported by the local A2DP entity. 
 *
 * \param mgr A2DP manager.
 * \param caps The capabilities of a SEP.
 * 
 * \return
 *        \li \c ID of a SEP if the function succeeds.
 *        \li \c FALSE otherwise.
*/
#define bt_a2dp_register_source(mgr, caps)                      bt_avdtp_register_sep(mgr->avdtp_mgr, AVDTP_SEP_TYPE_SOURCE, caps)

/**
 * \brief Discover SEPs on a remote device.
 * \ingroup a2dp
 *
 * \details This function asks the remote device to send a list of all available SEPs.
 *          As a result of this operation the following events will be generated:
 *            @arg A2DP_EVT_SEP_INFO_RECEIVED:   this event is generated for every SEP received from the remote device. the \p evt_param.sep_info_received contains SEP information.
 *            @arg A2DP_EVT_DISCOVER_COMPLETED:  this event is generated after the last A2DP_EVT_SEP_INFO_RECEIVED if the remote accepted the request and the \p evt_param.discover_completed.err_code == AVDTP_ERROR_SUCCESS.
 *                                               if the remote rejected the request the \p evt_param.discover_completed.err_code == the error code sent by the remote.
 *
 * \param mgr A2DP manager.
 * \param remote_addr The address of a remote device.
 * 
 * \return
 *        \li \c TRUE if discover request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
#define bt_a2dp_discover(mgr, remote_addr)                      bt_avdtp_discover(mgr->avdtp_mgr, remote_addr)

/**
 * \brief Get remote SEP capabilities.
 * \ingroup a2dp
 *
 * \details This function asks the remote device to send capabilities of a SEP specified by the \p seid_acp.
 *          As a result of this operation the following events will be generated:
 *            @arg A2DP_EVT_SEP_CAPABILITIES_RECEIVED:       this event is generated if the remote device accepted the request. the \p evt_param.sep_capabilities_received contains SEP capabilities.
 *            @arg A2DP_EVT_GET_SEP_CAPABILITIES_COMPLETED:  this event is generated right after A2DP_EVT_SEP_CAPABILITIES_RECEIVED 
 *                                                       if the remote accepted the request the \p evt_param.get_sep_capabilities_completed.err_code == AVDTP_ERROR_SUCCESS.
 *                                                       if the remote rejected the request the \p evt_param.get_sep_capabilities_completed.err_code == the error code sent by the remote.
 *
 * \param mgr A2DP manager.
 * \param remote_addr The address of a remote device.
 * \param seid_acp The ID of a remote SEP.
 * 
 * \return
 *        \li \c TRUE if discover request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
#define bt_a2dp_get_capabilities(mgr, remote_addr, seid_acp)    bt_avdtp_get_capabilities(mgr->avdtp_mgr, remote_addr, seid_acp)

/**
 * \brief Get remote SEP capabilities.
 * \ingroup a2dp
 *
 * \details This function asks the remote device to send capabilities of a SEP specified by the \p seid_acp.
 *          As a result of this operation the following events will be generated:
 *            @arg A2DP_EVT_SEP_CAPABILITIES_RECEIVED:       this event is generated if the remote device accepted the request. the \p evt_param.sep_capabilities_received contains SEP capabilities.
 *            @arg A2DP_EVT_GET_SEP_CAPABILITIES_COMPLETED:  this event is generated right after A2DP_EVT_SEP_CAPABILITIES_RECEIVED 
 *                                                       if the remote accepted the request the \p evt_param.get_sep_capabilities_completed.err_code == AVDTP_ERROR_SUCCESS.
 *                                                       if the remote rejected the request the \p evt_param.get_sep_capabilities_completed.err_code == the error code sent by the remote.
 *
 * \param mgr A2DP manager.
 * \param remote_addr The address of a remote device.
 * \param seid_acp The ID of a remote SEP.
 * 
 * \return
 *        \li \c TRUE if discover request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
#define bt_a2dp_get_all_capabilities(mgr, remote_addr, seid_acp)    bt_avdtp_get_all_capabilities(mgr->avdtp_mgr, remote_addr, seid_acp)

#define bt_a2dp_get_configuration(mgr, strm_handle)    bt_avdtp_get_configuration(mgr->avdtp_mgr, strm_handle)

/**
 * \brief Create a stream.
 * \ingroup a2dp
 *
 * \details This function allocates memory for storing stream's data and assigns a stream handle. 
 *          The stream handle is used to manipulate the stream - open, close, configure, suspend, abort.
 *
 * \param mgr A2DP manager.
 * 
 * \return
 *        \li \c Stream handle if the function succeeds.
 *        \li \c 0 otherwise.
*/
#define bt_a2dp_create_stream(mgr)                              bt_avdtp_create_stream(mgr->avdtp_mgr)

/**
 * \brief Destroy a stream.
 * \ingroup a2dp
 *
 * \details This function frees memory used by the stream. The stream has to exist and be in the "idle" state for this function to succeed. 
 *          I.e. the stream has to be closed or aborted before this function can be called.
 *
 * \param mgr A2DP manager.
 * \param strm_handle Stream handle.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
*/
#define bt_a2dp_destroy_stream(mgr, strm_handle)                bt_avdtp_destroy_stream(mgr->avdtp_mgr, strm_handle)

/**
 * \brief Listen for incoming connections.
 * \ingroup a2dp
 *
 * \details This function tells a stream that it can use a particular SEP to accept incoming requests to open it. The SEP can be associated
 *          with multiple streams but used with only one. The stream has to be closed before the SEP can be used with another stream.
 *          For outgoing connections this is not needed. Any SEP can be used with any stream given that the SEP is not already in use by
 *          another stream.
 *
 * \param mgr A2DP manager.
 * \param strm_handle Stream handle.
 * \param sep_id Local SEP ID.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
*/
#define bt_a2dp_listen(mgr, strm_handle, sep_id)                bt_avdtp_listen(mgr->avdtp_mgr, strm_handle, sep_id)

/**
 * \brief Cancel listening for incoming connections.
 * \ingroup a2dp
 *
 * \details This function removes a SEP from a list of SEPS which a stream can use for incoming requests.
 *
 * \param mgr A2DP manager.
 * \param strm_handle Stream handle.
 * \param sep_id Local SEP ID.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
*/
#define bt_a2dp_cancel_listen(mgr, strm_handle, sep_id)         bt_avdtp_cancel_listen(mgr->avdtp_mgr, strm_handle, sep_id)

/**
 * \brief Open & start a stream
 * \ingroup a2dp
 *
 * \details Opening a stream involves sending 3 requests to a remote device - "set configuration", "open stream" and "start stream". 
 *          Each event generates its own event which must be handled and acted accordingly by the application. 
 *          To make the use of API easier dotstack combines all these requests in one request called "open & start stream". dotstack 
 *          sends necessary requests in a proper sequence, handles responses and generates only one event (A2DP_EVT_OPEN_AND_START_STREAM_COMPLETED) 
 *          at the end. If any of the individual requests has failed the event's parameter bt_a2dp_event_t::open_and_start_stream_completed is
 *          populated with the error code and request id which caused it.
 *
 * \param mgr A2DP manager.
 * \param strm_handle Stream handle.
 * \param remote_addr The address of a remote device.
 * \param seid_int Local SEP ID.
 * \param seid_acp Remote SEP ID.
 * \param caps Stream configuration.
 *          
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise. No events will be generated.
 */
bt_bool bt_a2dp_open_and_start_stream(
	bt_a2dp_mgr_t* mgr, bt_byte strm_handle,
	bt_bdaddr_t* remote_addr, bt_byte seid_int, bt_byte seid_acp,
	const bt_avdtp_sep_capabilities_t* caps);

bt_bool bt_a2dp_set_configuration(
	bt_a2dp_mgr_t* mgr, bt_byte strm_handle,
	bt_bdaddr_t* remote_addr, bt_byte seid_int, bt_byte seid_acp,
	const bt_avdtp_sep_capabilities_t* caps);

#define bt_a2dp_open_stream(mgr, strm_handle)	                bt_avdtp_open_stream(mgr->avdtp_mgr, strm_handle)

/**
 * \brief Reconfigure stream.
 * \ingroup a2dp
 *
 * \details This function tries to change the stream's configuration. For this function to succeed the stream has to be open.
 *          As a result of this operation the A2DP_EVT_STREAM_RECONFIGURE_COMPLETED event will be generated. 
 *          If reconfiguration was a success the \p evt_param.stream_reconfigure_completed.err_code == AVDTP_ERROR_SUCCESS.
 *          Otherwise the \p evt_param.stream_reconfigure_completed.err_code == the error code sent by the remote.
 *
 * \param mgr A2DP manager.
 * \param strm_handle Stream handle.
 * \param caps New stream configuration.
 * 
 * \return
 *        \li \c TRUE if the function succeeds, i.e. the actual request has been sent to the remote party.
 *        \li \c FALSE otherwise. No events will be generated.
 */
#define bt_a2dp_reconfigure_stream(mgr, strm_handle, caps)      bt_avdtp_reconfigure_stream(mgr->avdtp_mgr, strm_handle, caps)

/**
 * \brief Get local stream state.
 * \ingroup a2dp
 *
 * \details This function returns local state of a stream specified by the \p strm_handle. No request is sent to the remote party.
 *
 * \param mgr A2DP manager.
 * \param strm_handle Stream handle.
 * 
 * \return The state of the stream. 
 *         The result will be one of the following values:
 *            @arg AVDTP_STREAM_STATE_IDLE:                 The stream is idle. This can mean two things. The stream specified by \p strm_handle does not exist or
 *                                                          the stream is closed.
 *            @arg AVDTP_STREAM_OPENING_TRANSPORT_CHANNELS: The stream is opening transport channels.
 *            @arg AVDTP_STREAM_CLOSING_TRANSPORT_CHANNELS: The stream is closing transport channels.
 *            @arg AVDTP_STREAM_STATE_CONFIGURED:           The stream has been configured.
 *            @arg AVDTP_STREAM_STATE_OPEN:                 The stream has been opened.
 *            @arg AVDTP_STREAM_STATE_STREAMING:            The stream has been started. Depending on the local SEP type (source or sink) it means
 *                                                          that the stream is can send or receive media packets.
 *            @arg AVDTP_STREAM_STATE_CLOSING:              The stream is closing. This means that all transport channels associated with the stream
 *                                                          are being closed. After they have been closed the stream goes to AVDTP_STREAM_STATE_IDLE state.
 *            @arg AVDTP_STREAM_STATE_ABORTING:             The stream is aborting. This means that all transport channels associated with the stream
 *                                                          are being closed. After they have been closed the stream goes to AVDTP_STREAM_STATE_IDLE state.
 */
#define bt_a2dp_get_stream_state(mgr, strm_handle)              bt_avdtp_get_stream_state(mgr->avdtp_mgr, strm_handle)

/**
 * \brief Get stream's local SEP ID.
 * \ingroup a2dp
 *
 * \details This function returns the ID of the local SEP associated with the stream.
 *
 * \param mgr A2DP manager.
 * \param strm_handle Stream handle.
 * 
 * \return \li The ID of the local SEP if strm_handle specifies a valid stream. 
 *         \li 0 otherwise.
 */
#define bt_a2dp_get_stream_local_sep_id(mgr, strm_handle)       bt_avdtp_get_stream_local_sep_id(mgr->avdtp_mgr, strm_handle)

/**
 * \brief Get stream's remote SEP ID.
 * \ingroup a2dp
 *
 * \details This function returns the ID of the remote SEP associated with the stream.
 *
 * \param mgr A2DP manager.
 * \param strm_handle Stream handle.
 * 
 * \return \li The ID of the remote SEP if strm_handle specifies a valid stream. 
 *         \li 0 otherwise.
 */
#define bt_a2dp_get_stream_remote_sep_id(mgr, strm_handle)      bt_avdtp_get_stream_remote_sep_id(mgr->avdtp_mgr, strm_handle)

/**
 * \brief Get stream's remote BT address.
 * \ingroup a2dp
 *
 * \details This function returns the address of the remote device associated with the stream.
 *
 * \param mgr A2DP manager.
 * \param strm_handle Stream handle.
 * 
 * \return \li The address of the remote device if strm_handle specifies a valid stream. 
 *         \li NULL otherwise.
 */
#define bt_a2dp_get_stream_remote_address(mgr, strm_handle)     bt_avdtp_get_stream_remote_address(mgr->avdtp_mgr, strm_handle)

/**
 * \brief Get the type of the codec currently used with the stream.
 * \ingroup a2dp
 *
 * \details This function returns the type of the codec currently used with the stream.
 *
 * \param mgr A2DP manager.
 * \param strm_handle Stream handle.
 * 
 * \return @arg The type of the codec if strm_handle specifies a valid stream and the stream is in one of the following states:
 *
 *           AVDTP_STREAM_STATE_CONFIGURED
 *           AVDTP_STREAM_STATE_OPEN
 *           AVDTP_STREAM_STATE_STREAMING
 *
 *         @arg The result will be one of the following values:
 *
 *            AVDTP_CODEC_TYPE_SBC:           SBC
 *            AVDTP_CODEC_TYPE_MPEG1_2_AUDIO: MPEG-1,2 (used in MP3 files)
 *            AVDTP_CODEC_TYPE_MPEG2_4_AAC:   MPEG-2,4 AAC (used in Apple products)
 *            AVDTP_CODEC_TYPE_ATRAC:         ATRAC (used in Sony products)
 *            AVDTP_CODEC_TYPE_NON_A2DP:      Non-A2DP Codec
 *
 *         @arg 0xFF otherwise.
 */
#define bt_a2dp_get_stream_codec_type(mgr, strm_handle)         bt_avdtp_get_stream_codec_type(mgr->avdtp_mgr, strm_handle)

/**
 * \brief Get the configuration of the codec currently used with the stream.
 * \ingroup a2dp
 *
 * \details This function returns a pointer to a structure that contains configuration of the codec currently used with the stream.
 *          The structure returned depends on the codec. The dotstack defines structures only for SBC, MPEG-1,2 and MPEG-2,4 AAC codecs:
 *              @arg SBC: bt_a2dp_sbc_config_t (defined in a2dp_sbc_codec.h)
 *              @arg MPEG-1,2: bt_a2dp_mpeg_config_t (defined in a2dp_mpeg_codec.h)
 *              @arg MPEG-2,4 AAC: bt_a2dp_aac_config_t (defined in a2dp_aac_codec.h)
 *
 * \param mgr A2DP manager.
 * \param strm_handle Stream handle.
 * 
 * \return \li The codec's configuration if strm_handle specifies a valid stream and the stream is in one of the following state:
 *
 *         AVDTP_STREAM_STATE_CONFIGURED
 *         AVDTP_STREAM_STATE_OPEN
 *         AVDTP_STREAM_STATE_STREAMING
 *
 *         \li NULL otherwise.
 */
#define bt_a2dp_get_stream_codec_config(mgr, strm_handle)       bt_avdtp_get_stream_codec_config(mgr->avdtp_mgr, strm_handle)

/**
 * \brief Get stream's configuration.
 * \ingroup a2dp
 *
 * \details This function returns a pointer to a structure holding current configuration of the stream.
 *
 * \param mgr A2DP manager.
 * \param strm_handle Stream handle.
 * 
 * \return \li The stream's configuration if strm_handle specifies a valid stream and the stream is in one of the following state:
 *
 *         AVDTP_STREAM_STATE_CONFIGURED
 *         AVDTP_STREAM_STATE_OPEN
 *         AVDTP_STREAM_STATE_STREAMING
 *
 *         \li NULL otherwise.
 */
#define bt_avdtp_get_stream_config(mgr, strm_handle)            bt_avdtp_get_stream_config(mgr->avdtp_mgr, strm_handle)

/**
 * \brief Start a stream.
 * \ingroup a2dp
 *
 * \details This function tries to start a stream by sending a request to the remote party. The stream has to be in 
 *          AVDTP_STREAM_STATE_OPEN state. The stream goes to this state as a result of successful configuration or suspension (both
 *          can be initiated by either party).
 *          As a result of this operation the A2DP_EVT_START_STREAM_COMPLETED event will be generated.
 *          If the stream has been open the \p evt_param.start_stream_requested.err_code == AVDTP_ERROR_SUCCESS.
 *          Otherwise, if the remote device for any reason cannot or does not wish to start the stream,
 *          the \p evt_param.start_stream_requested.err_code == the error code sent by the remote.
 *
 * \param mgr A2DP manager.
 * \param strm_handle Stream handle.
 * 
 * \return
 *        \li \c TRUE if the function succeeds, i.e. the actual request has been sent to the remote party.
 *        \li \c FALSE otherwise. No events will be generated.
 */
#define bt_a2dp_start_stream(mgr, strm_handle)                  bt_avdtp_start_stream(mgr->avdtp_mgr, strm_handle)

/**
 * \brief Close a stream.
 * \ingroup a2dp
 *
 * \details This function tries to close a stream by sending a request to the remote party. The stream has to be in 
 *          AVDTP_STREAM_STATE_OPEN or AVDTP_STREAM_STATE_STREAMING state.
 *          As a result of this operation the A2DP_EVT_CLOSE_STREAM_COMPLETED event will be generated.
 *          If the stream has been closed the \p evt_param.bt_avdtp_evt_close_stream_completed_t.err_code == AVDTP_ERROR_SUCCESS.
 *          Otherwise, if the remote device for any reason cannot or does not wish to close the stream,
 *          the \p evt_param.bt_avdtp_evt_close_stream_completed_t.err_code == the error code sent by the remote.
 *
 * \param mgr A2DP manager.
 * \param strm_handle Stream handle.
 * 
 * \return
 *        \li \c TRUE if the function succeeds, i.e. the actual request has been sent to the remote party.
 *        \li \c FALSE otherwise. No events will be generated.
 */
#define bt_a2dp_close_stream(mgr, strm_handle)                  bt_avdtp_close_stream(mgr->avdtp_mgr, strm_handle)

/**
 * \brief Suspend a stream.
 * \ingroup a2dp
 *
 * \details This function tries to suspend a stream by sending a request to the remote party. The stream has to be in 
 *          AVDTP_STREAM_STATE_STREAMING state.
 *          As a result of this operation the A2DP_EVT_SUSPEND_STREAM_COMPLETED event will be generated.
 *          If the stream has been suspended the \p evt_param.bt_avdtp_evt_suspend_stream_requested_t.err_code == AVDTP_ERROR_SUCCESS.
 *          Otherwise, if the remote device for any reason cannot or does not wish to suspend the stream,
 *          the \p evt_param.bt_avdtp_evt_suspend_stream_requested_t.err_code == the error code sent by the remote.
 *
 * \param mgr A2DP manager.
 * \param strm_handle Stream handle.
 * 
 * \return
 *        \li \c TRUE if the function succeeds, i.e. the actual request has been sent to the remote party.
 *        \li \c FALSE otherwise. No events will be generated.
 */
#define bt_a2dp_suspend_stream(mgr, strm_handle)                bt_avdtp_suspend_stream(mgr->avdtp_mgr, strm_handle)

/**
 * \brief Suspend a stream.
 * \ingroup a2dp
 *
 * \details This function tries to suspend a stream by sending a request to the remote party. The stream can be in any state state except AVDTP_STREAM_STATE_IDLE.
 *          As a result of this operation the A2DP_EVT_ABORT_STREAM_COMPLETED event will be generated.
 *          This operation cannot be rejected. The \p evt_param.abort_stream_requested.err_code is always == AVDTP_ERROR_SUCCESS.
 *
 * \param mgr A2DP manager.
 * \param strm_handle Stream handle.
 * 
 * \return
 *        \li \c TRUE if the function succeeds, i.e. the actual request has been sent to the remote party.
 *        \li \c FALSE otherwise. No events will be generated.
 */
#define bt_a2dp_abort_stream(mgr, strm_handle)                  bt_avdtp_abort_stream(mgr->avdtp_mgr, strm_handle)

/**
 * \brief Get HCI connection for a stream
 * \ingroup a2dp
 *
 * \details This function returns a pointer to a structure that describes an HCI connection a stream is open on. 
 *          The return value can be used to call various function from the HCI layer. For example, if an app
 *          wants to force disconnection from a remote device it can call ::bt_hci_disconnect.
 *
 * \param mgr A2DP manager.
 * \param strm_handle Stream handle.
 * 
 * \return
 *        \li \c Pointer to a structure that describes an HCI connection if the function succeeds.
 *        \li \c NULL otherwise. The function fails only if a stream specified by the \p strm_handle parameter 
 *        \li does not exist or there is no HCI connection between local and remote devices associated with the stream.
 *
 * \note This function has not been implemented.
 */
#define bt_a2dp_get_hci_connection(mgr, strm_handle)            bt_avdtp_get_hci_connection(mgr->avdtp_mgr, strm_handle)

/**
 * \brief Add a media packet buffer to a receive queue
 * \ingroup a2dp
 *
 * \details The consumer of A2DP is responsible for allocating and supplying A2DP with buffers used to store received packets. 
 *          A2DP itself only has a queue for storing pointers to buffers supplied by the consumer. When a packet comes in A2DP finds
 *          the first buffer large enough to hold the received packet, copies the packet to the buffer and generates 
 *          a A2DP_EVT_MEDIA_PACKET_RECEIVED event. The consumer then has to process the data in the buffer and return it back to the queue.
 *          If there is no buffers in the queue or none of the buffers is large enough the received packets is dropped. 
 *          Each buffer has a field (data_len) that holds the length of the received buffer. This field is never 0 if the buffer contains
 *          a packet. If a channel closed regardless of what has caused that and there are still buffers in the queue A2DP generates
 *          a A2DP_EVT_MEDIA_PACKET_RECEIVED event for each buffer and sets the data_len to 0. This is to inform the A2DP consumer that
 *          the buffer has not been used and can be, for example, deallocated.
 *          This function adds a buffer to the receive queue.
 *
 *
 * \param mgr A2DP manager.
 * \param strm_handle Stream handle.
 * \param buffer Pointer to a structure that holds the buffer and its parameters.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise. The function fails only if a stream specified by the \p strm_handle parameter 
 *        \li does not exist. The stream can be in any state to call this function.
 */
#define bt_a2dp_add_media_rx_buffer(mgr, strm_handle, buffer)   bt_avdtp_add_media_rx_buffer(mgr->avdtp_mgr, strm_handle, buffer)

/**
 * \brief Remove a media packet buffer from a receive queue
 * \ingroup a2dp
 *
 * \details The consumer of A2DP is responsible for allocating and supplying A2DP with buffers used to store received packets. 
 *          A2DP itself only has a queue for storing pointers to buffers supplied by the consumer. When a packet comes in A2DP finds
 *          the first buffer large enough to hold the received packet, copies the packet to the buffer and generates 
 *          a A2DP_EVT_MEDIA_PACKET_RECEIVED event. The consumer then has to process the data in the buffer and return it back to the queue.
 *          If there is no buffers in the queue or none of the buffers is large enough the received packets is dropped. 
 *          Each buffer has a field (data_len) that holds the length of the received buffer. This field is never 0 if the buffer contains
 *          a packet. If a channel closed regardless of what has caused that and there are still buffers in the queue A2DP generates
 *          a A2DP_EVT_MEDIA_PACKET_RECEIVED event for each buffer and sets the data_len to 0. This is to inform the A2DP consumer that
 *          the buffer has not been used and can be, for example, deallocated.
 *          This function removes a buffer from the receive queue.
 *
 *
 * \param mgr A2DP manager.
 * \param strm_handle Stream handle.
 * \param buffer Pointer to a structure that holds the buffer and its parameters.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise. The function fails only if a stream specified by the \p strm_handle parameter 
 *        \li does not exist. The stream can be in any state to call this function.
 */
#define bt_a2dp_remove_media_rx_buffer(mgr, strm_handle, buffer) bt_avdtp_remove_media_rx_buffer(mgr->avdtp_mgr, strm_handle, buffer)

/**
 * \brief Add a media packet buffer to a send queue
 * \ingroup a2dp
 *
 * \details When the consumer of A2DP wants to send a packet to a remote device it calls this function. The function adds the packet to a queue
 *          and tells A2DP that it has something to send. The packet will be send as soon as the stream goes to A2DP_STREAM_STATE_STREAMING
 *          state. When the packet has been successfully sent a A2DP_EVT_MEDIA_PACKET_SENT is generated. Otherwise a A2DP_EVT_MEDIA_PACKET_SEND_FAILED
 *          is generated. Regardless of the event generated the consumer can re-use the buffer as A2DP has removed it from the queue and gave up control
 *          over it. As in the case of received buffers, if a channel closed regardless of what has caused that and there are still buffers in the queue A2DP generates
 *          a A2DP_EVT_MEDIA_PACKET_SENT event for each buffer and sets the data_len field to 0. This is to inform the AVDTP consumer that
 *          the buffer has not been used and can be, for example, deallocated.
 *
 *
 * \param mgr A2DP manager.
 * \param strm_handle Stream handle.
 * \param buffer Pointer to a structure that holds the buffer and its parameters.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise. The function fails only if a stream specified by the \p strm_handle parameter 
 *        \li does not exist. The stream can be in any state to call this function.
 */
#define bt_a2dp_add_media_tx_buffer(mgr, strm_handle, buffer)   bt_avdtp_add_media_tx_buffer(mgr->avdtp_mgr, strm_handle, buffer)

/**
 * \brief Remove a media packet buffer from a send queue
 * \ingroup a2dp
 *
 * \details When the consumer of A2DP wants to send a packet to a remote device it calls bt_avdtp_add_media_tx_buffer function. 
 *          The function adds the packet to a queue and tells A2DP that it has something to send. The packet will be send as soon as the stream goes 
 *          to A2DP_STREAM_STATE_STREAMING state. The consumer has a chance to remove a packet from the queue before it has been sent to a remote device
 *          by calling ::bt_a2dp_remove_media_tx_buffer.
 *
 *
 * \param mgr A2DP manager.
 * \param strm_handle Stream handle.
 * \param buffer Pointer to a structure that holds the buffer and its parameters.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise. The function fails only if a stream specified by the \p strm_handle parameter 
 *        \li does not exist. The stream can be in any state to call this function.
 *
 * \note This function has not been implemented.
 */
#define bt_a2dp_remove_media_tx_buffer(mgr, strm_handle, buffer) bt_avdtp_remove_media_tx_buffer(mgr->avdtp_mgr, strm_handle, buffer)

bt_bool bt_a2dp_find_server(
	bt_bdaddr_t* deviceAddress, const bt_uuid_t* service_class_id, 
	bt_a2dp_find_server_callback_fp callback, 
	bt_sdp_client_callback_fp client_callback,
	void* callback_param);

/**
 * \brief Find source
 * \ingroup a2dp
 *
 * \details This function looks for a source on a remote device specified by \c deviceAddress 
 *          and, if found, returns features supported by the source.
 *
 * \param deviceAddress The address of a remote device.
 * \param callback The callback function that will be called when search has completed.
 * \param client_callback The optional callback function that an application can set if it wants to be notified of state changes of the SDP client.
 *                        The \c evt parameter of the callback can be one of the following values:
 *                        \li SDP_CLIENT_STATE_IDLE
 *                        \li SDP_CLIENT_STATE_CONNECTING
 *                        \li SDP_CLIENT_STATE_DISCONNECTING
 *                        \li SDP_CLIENT_STATE_CONNECTED
 *
 * \param callback_param A pointer to arbitrary data to be passed to the \c callback and \c client_callback callbacks.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise. 
 *
 */
bt_bool bt_a2dp_find_source(
	bt_bdaddr_t* deviceAddress, 
	bt_a2dp_find_server_callback_fp callback, 
	bt_sdp_client_callback_fp client_callback,
	void* callback_param);

/**
 * \brief Find sink
 * \ingroup a2dp
 *
 * \details This function looks for a sink on a remote device specified by \c deviceAddress 
 *          and, if found, returns features supported by the sink.
 *
 * \param deviceAddress The address of a remote device.
 * \param callback The callback function that will be called when search has completed.
 * \param client_callback The optional callback function that an application can set if it wants to be notified of state changes of the SDP client.
 *                        The \c evt parameter of the callback can be one of the following values:
 *                        \li SDP_CLIENT_STATE_IDLE
 *                        \li SDP_CLIENT_STATE_CONNECTING
 *                        \li SDP_CLIENT_STATE_DISCONNECTING
 *                        \li SDP_CLIENT_STATE_CONNECTED
 *
 * \param callback_param A pointer to arbitrary data to be passed to the \c callback and \c client_callback callbacks.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise. 
 *
 */
bt_bool bt_a2dp_find_sink(
	bt_bdaddr_t* deviceAddress, 
	bt_a2dp_find_server_callback_fp callback, 
	bt_sdp_client_callback_fp client_callback,
	void* callback_param);

/**
 * \brief Register default MPEG codec
 * \ingroup a2dp
 *
 * \details This function adds MPEG codec implemented by dotstack to the list of known codecs. 
 *          For more information about codecs see description of ::bt_avdtp_register_codec.
 *          The only codec A2DP is mandatory to support is SBC. All other codecs are optional.
 *          If an application wants to use MPEG-1,2 codec it must call this function when it is initializing.
 *
 * \note    dotstack codecs do not do actual encoding/decoding. their function is to parse and serialize
 *          codec's configuration.
 *          
 *
 * \param mgr A2DP manager.
 * 
 */
void bt_a2dp_register_mpeg_codec(bt_a2dp_mgr_t* mgr);

/**
 * \brief Register default AAC codec
 * \ingroup a2dp
 *
 * \details This function adds AAC codec implemented by dotstack to the list of known codecs. 
 *          For more information about codecs see description of ::bt_avdtp_register_codec.
 *          The only codec A2DP is mandatory to support is SBC. All other codecs are optional.
 *          If an application wants to use AAC codec it must call this function when it is initializing.
 *
 * \note    dotstack codecs do not do actual encoding/decoding. their function is to parse and serialize
 *          codec's configuration.
 *          
 *
 * \param mgr A2DP manager.
 * 
 */
void bt_a2dp_register_aac_codec(bt_a2dp_mgr_t* mgr);

#include "cdbt/a2dp/a2dp_private.h"

#ifdef __cplusplus
}
#endif

#endif // __A2DP_H
