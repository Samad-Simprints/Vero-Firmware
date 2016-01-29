/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*
* SEARAN LLC is the exclusive licensee and developer of dotstack with
* all its modifications and enhancements.
*
* Contains proprietary and confidential information of CandleDragon and
* may not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2009, 2010, 2011 CandleDragon. All Rights Reserved.
*******************************************************************************/

#ifndef __HFP_H
#define __HFP_H

/**
* \defgroup hfp Hands Free Profile (HFP)
*/

#include "cdbt/sdp/sdp.h"
#include "cdbt/utils/at_parser.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HFP_ROLE_HF			0
#define HFP_ROLE_AG			1


#define HFP_SESSION_PHASE_IDLE						0
#define HFP_SESSION_PHASE_WAITING_RFCOMM_CONNECT	1
#define HFP_SESSION_PHASE_WAITING_SCO_CONNECT		2
#define HFP_SESSION_PHASE_CONNECTING				3
#define HFP_SESSION_PHASE_WAITING_CMD_RESPONSE		4

#define HFP_SESSION_STATE_FREE						0xffff
#define HFP_SESSION_STATE_DISCONNECTED				0x0000
#define HFP_SESSION_STATE_SLC_CONNECTED				0x0001
#define HFP_SESSION_STATE_SCO_CONNECTED				0x0002
#define HFP_SESSION_STATE_CONNECTED					0x0003
#define HFP_SESSION_STATE_SLC_SETUP					0x0004
#define HFP_SESSION_STATE_CONNECTING				0x0008
#define HFP_SESSION_STATE_CALL_REFRESH_IN_PROGRESS	0x0010
#define HFP_SESSION_STATE_CALL_REFRESH_PENDING		0x0020
#define HFP_SESSION_STATE_ENABLING_VR				0x0040
#define HFP_SESSION_STATE_SCO_DISCONNECTING			0x0080
#define HFP_SESSION_STATE_SCO_CONNECTING			0x0100
#define HFP_SESSION_STATE_SEARCHING_HEADSETS        0x0200
#define HFP_SESSION_STATE_SENDING_AUDIO_PACKET      0x0400
#define HFP_SESSION_STATE_SENDING_CALL_LIST         0x0800

#define HFP_CODEC_TYPE_PCM                          1
#define HFP_CODEC_TYPE_MSBC                         2

#define HFP_EVENT_NOTHING                             0
/**
* This event is reported when SLC connected or disconnected. 
* The event parameters are passed via evt_param parameter of the session callback function
* which points to bt_hfp_evt_slc_connection_state_changed_t structure.
*/
#define HFP_EVENT_SLC_CONNECTION_STATE_CHANGED        1	

/**
* This event is reported when audio connection connected or disconnected. 
* The event parameters are passed via evt_param parameter of the session callback function
* which points to bt_hfp_evt_audio_connection_state_changed_t structure.
*/
#define HFP_EVENT_AUDIO_CONNECTION_STATE_CHANGED      2

//#define HFP_EVENT_CMD_RESPONSE_RECEIVED               3

/**
* This event is reported when the AG is notifying the HF about an incoming call. 
* The event has no parameters.
*/
#define HFP_EVENT_RING                                4

/**
* This event is reported when audio data has been received from the AG. 
* The event parameters are passed via evt_param parameter of the session callback function
* which points to bt_hfp_evt_audio_data_t structure.
*/
#define HFP_EVENT_AUDIO_DATA_RECEIVED                 5

/**
* This event is reported if an attempt to connect to the AG has failed.
* The event has no parameters.
*/
#define HFP_EVENT_HEADSET_CONNECT_ATTEMPT_FAILED      6
	
/**
* This event is reported when te AG has changed the microphone gain. 
* The event parameters are passed via evt_param parameter of the session callback function
* which points to bt_hfp_evt_mic_volume_changed_t structure.
*/
#define HFP_EVENT_MIC_VOLUME_CHANGED                  7

/**
* This event is reported when the AG has changed the speaker volume. 
* The event parameters are passed via evt_param parameter of the session callback function
* which points to bt_hfp_evt_spk_volume_changed_t structure.
*/
#define HFP_EVENT_SPK_VOLUME_CHANGED                  8

/**
* This event is reported if an attempt to setup SLC has failed.
* The event has no parameters.
*/
#define HFP_EVENT_SLC_SETUP_ATTEMPT_FAILED            9

/**
* This event is reported when the AG has sent the HF an indicator update. 
* The event parameters are passed via evt_param parameter of the session callback function
* which points to bt_hfp_evt_indicator_received_t structure.
*/
#define HFP_EVENT_INDICATOR_RECEIVED                  10

/**
* This event is reported when the HF has received operator's name. 
* The event parameters are passed via evt_param parameter of the session callback function
* which points to bt_hfp_evt_query_operator_completed_t structure.
*/
#define HFP_EVENT_QUERY_OPERATOR_COMPLETED            11

/**
* This event is reported when the HF has received the number of an incoming call. 
* The event parameters are passed via evt_param parameter of the session callback function
* which points to bt_hfp_evt_clip_received_t structure.
*/
#define HFP_EVENT_CLIP_RECEIVED                       12

/**
* This event is reported when there is a waiting call. 
* The event parameters are passed via evt_param parameter of the session callback function
* which points to bt_hfp_evt_call_waiting_t structure.
*/
#define HFP_EVENT_CALL_WAITING                        13

/**
* This event is reported when the AG has completed executing a command sent to it by the HF. 
* The event parameters are passed via evt_param parameter of the session callback function
* which points to bt_hfp_evt_command_completed_t structure.
*/
#define HFP_EVENT_CMD_COMPLETED                       14

/**
* This event is reported when the AG has changed voice control status. 
* The event parameters are passed via evt_param parameter of the session callback function
* which points to bt_hfp_evt_voice_recognition_changed_t structure.
*/
#define HFP_EVENT_VOICE_RECOGNITION_CHANGED           15

/**
* This event is reported when the HF has received a list of calls from the AG. 
* The event has no parameters.
*/
#define HFP_EVENT_CALL_LIST_RECEIVED                  16

/**
* This event is reported when the AG has changed the status of in-band ringing. 
* The event parameters are passed via evt_param parameter of the session callback function
* which points to bt_hfp_evt_inband_ring_changed_t structure.
*/
#define HFP_EVENT_INBAND_RING_CHANGED                 17

/**
* This event is reported when a call has been connected. 
* The event has no parameters.
*/
#define HFP_EVENT_CALL_STARTED                        18

/**
* This event is reported when active call has ended. 
* The event has no parameters.
*/
#define HFP_EVENT_CALL_ENDED                          19

/**
* This event is reported if an attempt to setup audio connection has failed.
* The event has no parameters.
*/
#define HFP_EVENT_AUDIO_SETUP_ATTEMPT_FAILED          20

/**
* This event is reported when the HF has received a subscriber number from the AG. 
* The event has no parameters.
*/
#define HFP_EVENT_SUBSCRIBER_NUMBER_RECEIVED          21

/**
* This event is reported when the HF has sent a DTMF code to the  AG. 
* The event has no parameters.
*/
#define HFP_EVENT_DTMF_CODE_SENT                      22

/**
* This event is reported when the HF has sent an ATA command to answer incoming call. 
* The event has no parameters.
*/
#define HFP_AG_EVENT_ANSWER_CALL                      23

/**
* This event is reported when the HF has sent an ATA command to reject incoming call
* or to disconnect the call in progress.
* The event has no parameters.
*/
#define HFP_AG_EVENT_TERMINATE_CALL                   24


/**
* This event is reported when the HF has sent a command to originate a call.  Command
* includes dialing a number, dialing from the memory or last number redial.
* The event parameters are passed via evt_param parameter of the session callback function
* which points to bt_hfp_evt_dial_request_received_t structure.
*/
#define HFP_AG_EVENT_DIAL_COMMAND_RECEIVED            25

/**
* This event is reported when HF has sent a command to activate/deactivate voice recognition in AG.
* The event parameters are passed via evt_param parameter of the session callback function
* which points to bt_hfp_evt_activate_voice_recognition_t structure.
*/
#define HFP_AG_EVENT_ACTIVATE_VOICE_RECOGNITION       26

/**
* This event is reported when HF has sent a command to senda DTMF code.
* The event parameters are passed via evt_param parameter of the session callback function
* which points to bt_hfp_evt_dtmf_code_t structure.
*/
#define HFP_AG_EVENT_SEND_DTMF_CODE                   27

/**
* This event is reported when HF has changed the microphone gain. 
* The event parameters are passed via evt_param parameter of the session callback function
* which points to bt_hfp_evt_mic_volume_changed_t structure.
*/
#define HFP_AG_EVENT_MIC_VOLUME_CHANGED               HFP_EVENT_MIC_VOLUME_CHANGED

/**
* This event is reported when HF has changed the speaker volume. 
* The event parameters are passed via evt_param parameter of the session callback function
* which points to bt_hfp_evt_spk_volume_changed_t structure.
*/
#define HFP_AG_EVENT_SPK_VOLUME_CHANGED               HFP_EVENT_SPK_VOLUME_CHANGED

#define HFP_AG_EVENT_SEARCH_COMPLETED                 28

#define HFP_EVENT_AUDIO_PACKET_SENT                   29

#define HFP_AG_EVENT_START_INBAND_RING                30

#define HFP_AG_EVENT_STOP_INBAND_RING                 31

#define HFP_AG_EVENT_HOLD_CALL                        32

#define HFP_AG_EVENT_ACTIVATE_CALL                    33

/**
* This event is reported when NR * EC in the AG has been disabled. 
* This event has no parameters.
*/
#define HFP_EVENT_NREC_DISABLED                        34

#define HFP_HF_FEATURE_EC_NR                    0x00000001
#define HFP_HF_FEATURE_CALL_WAITING             0x00000002
#define HFP_HF_FEATURE_CLI_PRESENTATION         0x00000004
#define HFP_HF_FEATURE_VOICE_RECOGNITTION       0x00000008
#define HFP_HF_FEATURE_REMOTE_VOLUME_CONTROL    0x00000010
#define HFP_HF_FEATURE_ENHANCED_CALL_STATUS     0x00000020
#define HFP_HF_FEATURE_ENHANCED_CALL_CONTROL    0x00000040
#define HFP_HF_FEATURE_CODEC_NEGOTIATION        0x00000080
#define HFP_HF_FEATURE_ALL                      0x000000FF

#define HFP_AG_FEATURE_3_WAY_CALLING				0x00000001
#define HFP_AG_FEATURE_EC_NR						0x00000002
#define HFP_AG_FEATURE_VOICE_RECOGNITTION			0x00000004
#define HFP_AG_FEATURE_INBAND_RING_TONE				0x00000008
#define HFP_AG_FEATURE_ATTACH_NUMBER_TO_VOICE_TAG	0x00000010
#define HFP_AG_FEATURE_REJECT_CALL					0x00000020
#define HFP_AG_FEATURE_ENHANCED_CALL_STATUS			0x00000040
#define HFP_AG_FEATURE_ENHANCED_CALL_CONTROL		0x00000080
#define HFP_AG_FEATURE_EXTENDED_ERROR_CODED			0x00000100
#define HFP_AG_FEATURE_CODEC_NEGOTIATION			0x00000200

#define HFP_HLD_SVC_RELEASE_ALL_HELD_CALLS		0x01
#define HFP_HLD_SVC_RELEASE_ALL_ACTIVE_CALLS	0x02
#define HFP_HLD_SVC_RELEASE_CALL				0x04
#define HFP_HLD_SVC_HOLD_ACTIVE_CALLS			0x08
#define HFP_HLD_SVC_PRIVATE_CALL				0x10
#define HFP_HLD_SVC_ADD_HELD_CALL				0x20
#define HFP_HLD_SVC_EXPLICIT_CALL_TRANSFER		0x40

// Command IDs for the bt_hfp_hf_hold
#define HFP_HLD_CMD_RELEASE_ALL_HELD            0
#define HFP_HLD_CMD_USER_BUSY                   0
#define HFP_HLD_CMD_RELEASE_ALL_ACTIVE_CALLS    1
#define HFP_HLD_CMD_RELEASE_ACTIVE_CALL         1
#define HFP_HLD_CMD_HOLD_ACTIVE_CALLS           2
#define HFP_HLD_CMD_PRIVATE_CALL                2
#define HFP_HLD_CMD_ADD_HELD_CALL               3
#define HFP_HLD_CMD_EXPLICIT_CALL_TRANSFER      4

typedef enum _bt_hfp_cmd_id_enum
{
	AT_UNKNOWN = 0,
	AT_BRSF,
	AT_BINP,
	AT_BLDN,
	AT_BVRA,
	AT_NREC,
	AT_VGM,
	AT_VGS,
	AT_BSIR,
	AT_BTRH,
	AT_CCWA,
	AT_CHLD,
	AT_CHUP,
	AT_CIND,
	AT_CLCC,
	AT_COPS,
	AT_CMEE,
	AT_CLIP,
	AT_CMER,
	AT_CIEV,
	AT_CVTS,
	AT_CNUM,
	AT_VTS,
	AT_A,
	AT_D,
	AT_D_EXT,
    AT_BIA,
	AT_BAC,
	AT_BCS,
	AT_BCC

} bt_hfp_cmd_id_enum;

#define HFP_AT_CMD_TYPE_ACTION		0
#define HFP_AT_CMD_TYPE_READ		1
#define HFP_AT_CMD_TYPE_TEST		2
#define HFP_AT_CMD_TYPE_DIAL		3
#define HFP_AT_CMD_TYPE_OTHER		4

#define HFP_RES_TYPE_OK			0
#define HFP_RES_TYPE_ERROR		1
#define HFP_RES_TYPE_INFO		2

#define HFP_IND_SERVICE			0
#define HFP_IND_CALL			1
#define HFP_IND_CALLSETUP		2
#define HFP_IND_CALLHELD		3
#define HFP_IND_SIGNAL			4
#define HFP_IND_ROAM			5
#define HFP_IND_BATTCHG			6
#define HFP_IND_MAX				7

#define HFP_IND_CALL_NO_ACTIVE_CALL		0
#define HFP_IND_CALL_ACTIVE_CALL		1

#define HFP_IND_CALLSETUP_NO_CALL_SETUP					0
#define HFP_IND_CALLSETUP_INCOMING_CALL_SETUP			1
#define HFP_IND_CALLSETUP_OUTGOING_CALL_SETUP			2
#define HFP_IND_CALLSETUP_OUTGOING_CALL_ALERT_REMOTE	3

#define HFP_IND_CALLHELD_NO_CALLS_HELD		0
#define HFP_IND_CALLHELD_ACTIVE_HELD		1
#define HFP_IND_CALLHELD_ALL_CALLS_HELD		2

#define HFP_IND_SERVICE_NO_SERVICE			0
#define HFP_IND_SERVICE_PRESENT             1

#define HFP_IND_SIGNAL_LOW                  0
#define HFP_IND_SIGNAL_MAX                  5

#define HFP_IND_ROAM_NOT_ACTIVE				0
#define HFP_IND_ROAM_ACTIVE					1

#define HFP_IND_BATTCHG_LOW                 0
#define HFP_IND_BATTCHG_MAX                 5

#define HFP_REG_MODE_FORWARD                3

#define HFP_REG_IND_NO_REPORTING            0
#define HFP_REG_IND_EVENT_REPORTING         1
#define HFP_REG_IND_ALL_REPORTING           2

#define HFP_CALL_STATUS_FREE				0xFF
#define HFP_CALL_STATUS_IDLE				0
#define HFP_CALL_STATUS_ACTIVE				1
#define HFP_CALL_STATUS_HELD				2
#define HFP_CALL_STATUS_DIALING				3
#define HFP_CALL_STATUS_ALERTING			4
#define HFP_CALL_STATUS_INCOMING			5
#define HFP_CALL_STATUS_WAITING				6

#define HFP_CALL_DIR_OUTGOING				0
#define HFP_CALL_DIR_INCOMING				1

#define HFP_CALL_MODE_VOICE					0
#define HFP_CALL_MODE_DATA					1
#define HFP_CALL_MODE_FAX					2

#define HFP_CALL_MPTY_NOT_MULTIPARTY		0
#define HFP_CALL_MPTY_MULTIPARTY			1

#define HFP_CALL_STATUS_MASK_IDLE			0
#define HFP_CALL_STATUS_MASK_ACTIVE			0x1
#define HFP_CALL_STATUS_MASK_HELD			0x2
#define HFP_CALL_STATUS_MASK_DIALING		0x4
#define HFP_CALL_STATUS_MASK_ALERTING		0x8
#define HFP_CALL_STATUS_MASK_INCOMING		0x10
#define HFP_CALL_STATUS_MASK_WAITING		0x20

typedef enum _bt_hfp_dial_request_enum
{
    HFP_DIAL_REQUEST_UNKNWN,
    HFP_DIAL_REQUEST_NUMBER,
    HFP_DIAL_REQUEST_MEMORY_DIAL,
    HFP_DIAL_REQUEST_REDIAL
} bt_hfp_dial_request_enum;

#ifndef HFP_MAX_CALLS
#define HFP_MAX_CALLS    3
#endif

#ifndef HFP_MAX_NUMBER_LENGTH
#define HFP_MAX_NUMBER_LENGTH        20
#endif

#ifndef HFP_EVT_STR_BUFFER_LENGTH
#define HFP_EVT_STR_BUFFER_LENGTH    40
#endif

#define HFP_TX_PACKET_LEN			 160  

#define HFP_AG_RING_INTERVAL		 5

#define HFP_AG_MAX_OPERATOR_NAME_LEN 16

typedef struct _bt_hfp_session bt_hfp_session;

typedef struct _bt_hfp_audio_packet_s bt_hfp_audio_packet_t;

/**
* \brief Notify the application of changes in the HFP session
* \ingroup hfp
*
* \details This function is called by the HFP layer when various session parameters have changed.
* It is also called when commands from the remote device or 
* response codes to the commands sent to the the remote device are received.
*
* \param session HFP session.
* \param evt Specifies the id of the event. The value of this arguments is one of the values defined by HFP_EVENT_ constants.
*             Each event has a parameter that points to an event specific structure that clarifies the event. 
*             The event parameter is passed through the \c evt_param parameter.
*
*             \li HSP_SESSION_RFCOMM_CONNECTION_STATE_CHANGED - The RFCOMM connection state has changed. The application should examine the value of the bt_hsp_session::state member to determine weather the RFCOMM session has connected or disconnected. The RFCOMM session has connected if the HSP_SESSION_STATE_RFCOMM_CONNECTED bit is set in the bt_hsp_session::state member.
*             \li HSP_SESSION_SCO_CONNECTION_STATE_CHANGED - The SCO connection state has changed. The application should examine the value of the bt_hsp_session::state member to determine weather the RFCOMM session has connected or disconnected. The RFCOMM session has connected if the HSP_SESSION_STATE_SCO_CONNECTED bit is set in the bt_hsp_session::state member.
*             \li HSP_SESSION_CMD_RESPONSE_RECEIVED - The Headset has received a response to the command it sent to the Audio Gateway. The response is a string pointed to by the \c what_param parameter. It can be either HS_RES_OK or HS_RES_ERROR.
*             \li HSP_SESSION_RING - The Audio gateway has sent the RING unsolicited result code. The application must respond to this result code by sending the button press (call bt_hsp_hs_send_button_press).
*             \li HSP_SESSION_AUDIO_DATA_RECEIVED - The audio data has been received. The pointer to a buffer is passed in the \c what_param parameter. The length of the data is passed in the \c what_param2 parameter (cast to bt_int).
*             \li HSP_SESSION_HEADSET_CONNECT_ATTEMPT - The attempt to connect to the Audio Gateway has failed.
*             \li HSP_SESSION_MIC_VOLUME_CHANGED - The Audio Gateway has changed the microphone gain. The new value is passed in the \c what_param parameter (cast to bt_byte).
*             \li HSP_SESSION_SPK_VOLUME_CHANGED - The Audio Gateway has changed the speaker volume. The new value is passed in the \c what_param parameter (cast to bt_byte)
*
* \param evt_param The event's parameter.
* \param callback_param The value of the callback_param passed to the call of bt_hfp_listen or bt_hfp_connect.
* 
*/
typedef void (*bt_hfp_session_callback_pf)(bt_hfp_session* session, bt_byte evt, void* evt_param, void* callback_param);

/**
* \brief Notify the application of the result of searching for the Audio Gateway
* \ingroup hfp
*
* \details This function is called by the HFP layer when searching for the Audio Gateway on a remote device has completed.
*
* \param session HFP session.
* \param server_channel The RFCOMM server channel used by the Audio Gateway. The application must pass this value to bt_hfp_connect. The value is valid only if the \c found parameter is TRUE.
* \param found \c TRUE if Audio Gateway has been found on the remote device. \c FALSE otherwise.
*
*/
typedef void (*bt_hfp_find_remote_callback_pf)(bt_hfp_session* session, bt_byte server_channel, bt_bool found);

/**
* \brief Notify the application that audio data has been sent to a remote device
* \ingroup hfp
*
* \details This function is called by the HFP layer when audio data has been sent to a remote device.
*
* \param session HFP session.
* \param callback_param The value of the \c callback_param parameter passed to the call of bt_hfp_send_audio.
*
*/
typedef void (*bt_hfp_send_audio_callback_pf)(bt_hfp_session* session, void* callback_param);

// event parameters

/**
* \brief  HFP_EVENT_AUDIO_DATA_RECEIVED event parameter
* \ingroup hfp
*
* \details HFP layer calls the session callback when it received a chunk of audio data from the remote device
* and passes a pointer to \c bt_hfp_evt_audio_data_t which contains a pointer to the data and its length.
*
*
*/
typedef struct _bt_hfp_evt_audio_data_t
{
	bt_hfp_audio_packet_t* packet; ///< A pointer to an audio packet buffer.
} bt_hfp_evt_audio_data_t;

/**
* \brief  HFP_EVENT_SLC_CONNECTION_STATE_CHANGED event parameter
* \ingroup hfp
*
* \details HFP layer calls the session callback when the state of the service level connection (SLC) has changed
* and passes a pointer to \c bt_hfp_evt_slc_connection_state_changed_t which contains a flag identifying 
* the current state of SLC (connected or disconnected).
*/
typedef struct _bt_hfp_evt_slc_connection_state_changed_t
{
	bt_bool connected;      ///< Defines the state of SLC. TRUE - if SLC is connected, FALSE - if SLC is disconnected
} bt_hfp_evt_slc_connection_state_changed_t;

/**
* \brief  HFP_EVENT_AUDIO_CONNECTION_STATE_CHANGED event parameter
* \ingroup hfp
*
* \details HFP layer calls the session callback when the state of the audio connection has changed
* and passes a pointer to \c bt_hfp_evt_audio_connection_state_changed_t which contains a flag identifying 
* the current state of the audio connection (connected or disconnected).
*/
typedef struct _bt_hfp_evt_audio_connection_state_changed_t
{
	bt_bool connected;           ///< Defines the state of audio connection. TRUE - if audio is connected, FALSE - if audio is disconnected
	bt_hci_conn_state_t* hci_conn;	 ///< A pointer to \c HCI SCO connection if \c connected is TRUE, otherwise - NULL
	bt_uint codec_id;
} bt_hfp_evt_audio_connection_state_changed_t;


/**
* \brief  HFP_EVENT_MIC_VOLUME_CHANGED event parameter
* \ingroup hfp
*
* \details HFP layer calls the session callback when the microphone gain has been changed in the AG
* and passes a pointer to \c bt_hfp_evt_mic_volume_changed_t which contains 
* the new value for the microphone gain.
*/
typedef struct _bt_hfp_evt_mic_volume_changed_t
{
	bt_byte volume;             ///< The value of the microphone gain
} bt_hfp_evt_mic_volume_changed_t;

/**
* \brief  HFP_EVENT_SPK_VOLUME_CHANGED event parameter
* \ingroup hfp
*
* \details HFP layer calls the session callback when the speaker volume has been changed in the AG
* and passes a pointer to \c bt_hfp_evt_spk_volume_changed_t which contains 
* the new value for the speaker volume.
*/
typedef struct _bt_hfp_evt_spk_volume_changed_t
{
	bt_byte volume;             ///< The value of the speaker volume
} bt_hfp_evt_spk_volume_changed_t;

/**
* \brief  HFP_EVENT_INDICATOR_RECEIVED event parameter
* \ingroup hfp
*
* \details HFP layer calls the session callback when it received indicator from the AG
* and passes a pointer to \c bt_hfp_evt_indicator_received_t which contains 
* the id of the indicator and its value.
*/
typedef struct _bt_hfp_evt_indicator_received_t
{
	bt_byte id;                ///< Indicator ID. The possible values are defined by HFP_IND_ constants
	bt_byte value;             ///< The value of the indicator
} bt_hfp_evt_indicator_received_t;

/**
* \brief  HFP_EVENT_QUERY_OPERATOR_COMPLETED event parameter
* \ingroup hfp
*
* \details HFP layer calls the session callback when it received operator's name from the AG 
* during SLC setup or as a result of calling \c bt_hfp_hf_query_operator
* and passes a pointer to \c bt_hfp_evt_query_operator_completed_t which contains 
* the name of the operator.
*/
typedef struct _bt_hfp_evt_query_operator_completed_t
{
	bt_byte mode;              ///< Operator's mode
	bt_char* name;             ///< Pointer to 0-terminated string that contains the name of the operator
} bt_hfp_evt_query_operator_completed_t;

/**
* \brief  HFP_EVENT_CLIP_RECEIVED event parameter
* \ingroup hfp
*
* \details HFP layer calls the session callback when it received the number of an incoming call from the AG 
* and passes a pointer to \c bt_hfp_evt_clip_received_t which contains 
* the number of the incoming call.
*/
typedef struct _bt_hfp_evt_clip_received_t
{
	bt_char* number;          ///< Pointer to 0-terminated string that contains the number of the incoming call
	bt_ulong number_type;     ///< The type of the number
} bt_hfp_evt_clip_received_t;

/**
* \brief  HFP_EVENT_CALL_WAITING event parameter
* \ingroup hfp
*
* \details HFP layer calls the session callback when it received the number of 
* a waiting call (another incoming call while there is already ongoing call)
* from the AG and passes a pointer to \c bt_hfp_evt_call_waiting_t which contains 
* the number of the waiting call.
*/
typedef struct _bt_hfp_evt_call_waiting_t
{
	bt_char* number;         ///< Pointer to 0-terminated string that contains the number of the incoming call
	bt_ulong number_type;    ///< The type of the number
} bt_hfp_evt_call_waiting_t;

/**
* \brief  HFP_EVENT_CMD_COMPLETED event parameter
* \ingroup hfp
*
* \details HFP layer calls the session callback when a command sent by HF to AG has been processed by the AG 
* and a result (success or error) has been received.
* the callback is passed a pointer to \c bt_hfp_evt_command_completed_t which contains the id of the command
* and result.
*
*/
typedef struct _bt_hfp_evt_command_completed_t
{
	bt_hfp_cmd_id_enum id;   ///< The command ID
	bt_bool success;         ///< TRUE - if command succeeded, FALSE - if command failed
} bt_hfp_evt_command_completed_t;

/**
* \brief  HFP_EVENT_VOICE_RECOGNITION_CHANGED event parameter
* \ingroup hfp
*
* \details HFP layer calls the session callback when voice dialing in the AG has been turned on or off
* as a result of calling \c bt_hfp_hf_enable_voice_recognition.
* the callback is passed a pointer to \c bt_hfp_evt_voice_recognition_changed_t which contains 
* the current status of voice dialing in the AG.
*
*/
typedef struct _bt_hfp_evt_voice_recognition_changed_t
{
	bt_bool enabled;        ///< TRUE - if voice dialing is enabled, FALSE - otherwise
} bt_hfp_evt_voice_recognition_changed_t;

/**
* \brief  HFP_EVENT_INBAND_RING_CHANGED event parameter
* \ingroup hfp
*
* \details HFP layer calls the session callback when in-band ringing in the AG has been turned on or off
* the callback is passed a pointer to \c bt_hfp_evt_inband_ring_changed_t which contains 
* the current status of in-band ringing in the AG.
*
* if in-band ringing is enabled the AG will provide ringing tone. 
* However the HF may ignore this tone and generate its own
* upon receiving the HFP_EVENT_RING event.
*
* if in-band ringing is disabled the HF has to generate a ringing tone on its own.
* upon receiving the HFP_EVENT_RING event.
*
*/
typedef struct _bt_hfp_evt_inband_ring_changed_t
{
	bt_bool enabled;       ///< TRUE - if in-band ringing is enabled, FALSE - otherwise
} bt_hfp_evt_inband_ring_changed_t;

/**
* \brief  HFP_AG_EVENT_DIAL_REQUEST_RECEIVED event parameter
* \ingroup hfp
*
* \details HFP layer calls the session callback when it received a command from HF device with instruction 
* to initiate a voice call.  The callback is passed a pointer to \c _bt_hfp_evt_dial_request_received_t
* which contains the type of the call, including place a call with a phone number supplied by HF, 
* memory dial, or last number redial.  
*
* When application is able to place a call it should call HFP layer with corresponding call information.
*
*/
typedef struct _bt_hfp_evt_dial_request_received_s
{
    bt_hfp_dial_request_enum  request_type; ///< The dial request type
    union 
    {
	    bt_char number[HFP_MAX_NUMBER_LENGTH + 1];    ///< 0-terminated string that contains the number to dial or memory location
        bt_int  memory_location;                      ///< memory location to dial
    } uu;
} bt_hfp_evt_dial_request_received_t;

typedef struct _bt_hfp_call_s bt_hfp_call_t;

typedef struct _bt_hfp_evt_answer_call_s
{
	bt_hfp_call_t* call;
} bt_hfp_evt_answer_call_t;

typedef struct _bt_hfp_evt_terminate_call_s
{
	bt_hfp_call_t* call;
} bt_hfp_evt_terminate_call_t;

typedef struct _bt_hfp_evt_activate_voice_recognition_s
{
	bt_bool activate;
} bt_hfp_evt_activate_voice_recognition_t;

typedef struct _bt_hfp_evt_send_dtmf_code_s
{
	bt_byte code;
} bt_hfp_evt_send_dtmf_code_t;

typedef struct _bt_hfp_hf_t bt_hfp_hf_t;

typedef struct _bt_hfp_ag_evt_search_completed_t
{
	bt_hfp_hf_t* hfs; 
	bt_byte count;
} bt_hfp_ag_evt_search_completed_t;

typedef struct _bt_hfp_evt_audio_packet_sent_s
{
	bt_hfp_audio_packet_t* packet;
} bt_hfp_evt_audio_packet_sent_t;

typedef struct _bt_hfp_evt_chld_s
{
	bt_byte action;
	bt_byte call_index;
} bt_hfp_evt_chld_t;

/**
* \brief  Stores value of an indicator
* \ingroup hfp
*
*
*/
typedef struct _bt_hfp_ind
{
    union
    {
	    bt_byte id;        ///< HF indicator ID. The possible values are defined by HFP_IND_ constants
        bt_byte enabled;   ///< AG indicator enabled.  If 0 indicator has been disabled by HF device.
    } uu;
	bt_byte val;           ///< The indicator's value.
} bt_hfp_ind;

/**
* \brief  Stores value of HF registrations
* \ingroup hfp
*
*
*/
typedef struct _bt_hfp_event_register
{
	bt_byte mode;          ///< The registration mode. Only HFP_REG_MODE_FORWARD is supported by the current BT spec.
	bt_byte ind;           ///< The indicator's value.  The possible values are defined by HFP_REG_IND_ constants
} bt_hfp_event_register_t;

/**
* \brief  Stores information about a call
* \ingroup hfp
*
*
*/
struct _bt_hfp_call_s
{
	bt_byte index;        ///< Call index inside AG
	bt_byte direction;    ///< Call direction. HFP_CALL_DIR_OUTGOING or HFP_CALL_DIR_INCOMING
	bt_byte status;       ///< Call status.\n
						  ///< \li HFP_CALL_STATUS_MASK_IDLE
						  ///< \li HFP_CALL_STATUS_MASK_ACTIVE
                          ///< \li HFP_CALL_STATUS_MASK_HELD
                          ///< \li HFP_CALL_STATUS_MASK_DIALING
                          ///< \li HFP_CALL_STATUS_MASK_ALERTING
                          ///< \li HFP_CALL_STATUS_MASK_INCOMING
                          ///< \li HFP_CALL_STATUS_MASK_WAITING
	bt_byte mode;         ///< Call mode
                          ///< \li HFP_CALL_MODE_VOICE
                          ///< \li HFP_CALL_MODE_DATA
                          ///< \li HFP_CALL_MODE_FAX
	bt_byte multiparty;   ///< Number or parties in the call
                          ///< \li HFP_CALL_MPTY_NOT_MULTIPARTY
                          ///< \li HFP_CALL_MPTY_MULTIPARTY
	bt_char number[HFP_MAX_NUMBER_LENGTH + 1];	///< Phone number
	bt_int  type;         ///< Number type
};

typedef struct _bt_hf_subscriber_number_t
{
	bt_char number[HFP_MAX_NUMBER_LENGTH + 1];	///< Phone number
	bt_byte type;         ///< Number type
	bt_byte service;         ///< Service type
} bt_hf_subscriber_number_t;

struct _bt_hfp_hf_t
{
	bt_bdaddr_t address;
	bt_byte* name;
	bt_byte server_channel;
	bt_byte pg_scan_rpt_mode;
	bt_byte pg_scan_period_mode;
	bt_long cod;
	bt_int clock_offset;
	bt_byte rssi;
};

/**
* \brief  Parameter to HFP_EVENT_AUDIO_DATA_RECEIVED event
* \ingroup hfp
*
* \details HFP layer calls the session callback when it has received a chunk of audio data from the remote device.
* The callback is passed a pointer to \c bt_hfp_evt_audio_data_t which contains a pointer to this structure.
*/
struct _bt_hfp_audio_packet_s
{
	bt_hfp_audio_packet_t* next_packet;

	bt_uint data_len;        ///< The length of the audio data
	bt_uint max_data_len;    ///< Maximum length of the media data that can be stored in the buffer pointed by the \c data member.
	bt_byte* data;           ///< Pointer to a buffer to store audio data. This pointer must be allocated by the HFP consumer
                             ///< before adding this structure to receive (::bt_hfp_add_audio_rx_buffer) or send queue (::bt_hfp_add_audio_tx_buffer).
};

struct _bt_hfp_session
{
	bt_uint state;
	bt_byte phase;
	bt_byte role;
	bt_rfcomm_dlc_t* rfcomm_dlc;
	bt_hci_conn_state_t* hci_conn;
	bt_ulong hf_features;
	bt_ulong ag_features;
	bt_byte ag_hld_services;
	bt_hfp_cmd_id_enum cur_cmd_id;
	bt_hfp_cmd_id_enum cur_res_id;
	bt_byte cur_cmd_type;
	bt_byte call_count;
	bt_uint call_status_mask;
	bt_byte ring_interval;
	bt_byte speaker_volume;
	bt_byte mic_gain;
	bt_byte event_reporting_enabled;
	bt_bool ag_clip_enabled; 
	bt_bool ag_call_waiting_notification_enabled; 
	bt_bool ag_extented_errors_enabled;
	bt_bool ag_voice_recognition_enabled;
	bt_bool ag_inband_ringing_enabled;
	bt_byte operator_name[HFP_AG_MAX_OPERATOR_NAME_LEN + 1];
	bt_uint* available_codecs;
	bt_uint codec_count;
	bt_uint current_codec;

	bt_hfp_session_callback_pf session_callback;
	void* session_callback_param;

	bt_bdaddr_t remote_bdaddr;
	bt_byte remote_server_channel;
	bt_byte remote_remaining_connect_attemtps;
	bt_l2cap_channel_t* remote_sdp_channel;
	bt_sdp_data_element_t sdp_pattern;
	bt_sr_handle_t remote_service_record_handle;
	bt_bool remote_next_attr_is_server_channel;
	bt_hfp_find_remote_callback_pf remote_callback;

	bt_byte find_server_channel;
	bt_bool find_service_found;
	bt_byte find_remaining_connect_attemtps;
	bt_bdaddr_t find_device_address;
	bt_hci_ctrl_listener_t find_disconnect_listener;

	bt_hci_listener_t hci_disconnect_listener;

	bt_hfp_ind ind_map[HFP_IND_MAX];
	bt_hfp_call_t calls[HFP_MAX_CALLS];

	bt_at_parser_t atcmd_parser;
	bt_byte cur_param_index;

	bt_hf_subscriber_number_t subscriber_number;

	union
	{
		bt_hfp_evt_query_operator_completed_t query_operator;
		bt_hfp_evt_slc_connection_state_changed_t conn_state;
		bt_hfp_evt_mic_volume_changed_t mic_volume;
		bt_hfp_evt_spk_volume_changed_t spk_volume;
		bt_hfp_evt_indicator_received_t indicator;
		bt_hfp_evt_clip_received_t clip;
		bt_hfp_evt_voice_recognition_changed_t voice_recognition;
		bt_hfp_evt_inband_ring_changed_t inband_ring;
		bt_hfp_evt_command_completed_t cmd_completed;
		bt_hfp_evt_call_waiting_t call_waiting;
        bt_hfp_evt_dial_request_received_t dial_request;
        bt_hfp_event_register_t registration;
		bt_hfp_evt_answer_call_t answer;
		bt_hfp_evt_terminate_call_t terminate;
		bt_hfp_evt_activate_voice_recognition_t activate_voice_recognition;
		bt_hfp_evt_send_dtmf_code_t dtmf;
		bt_hfp_evt_chld_t chld;
	} evt_parameters;

	bt_char evt_str_buffer[HFP_EVT_STR_BUFFER_LENGTH + 1];

	bt_byte tx_buffer[HFP_TX_PACKET_LEN];

	bt_byte cur_hf;
	bt_byte hf_count;
	bt_hfp_hf_t* found_hfs;

	bt_byte clcc_call_index;

	bt_queue_element_t* audio_rx_queue;
	bt_queue_element_t* audio_tx_queue;

};

/**
* \brief Initialize HFP layer
* \ingroup hfp
*
* \details This function initializes the HFP layer of the stack. It must be called prior to any other
* HFP function can be called.
*/
bt_bool bt_hfp_init(void);

/**
* \brief Allocate HFP session
* \ingroup hfp
*
* \details This function allocates a new HFP session.
*
* \param role HFP_ROLE_HF or HFP_ROLE_AG.
*
* \return
*         \li A pointer to the new HFP session structure if the function succeeds.
*         \li \c NULL otherwise.
*/
bt_hfp_session* bt_hfp_allocate_session(bt_byte role);
	
/**
* \brief Allocate HFP session
* \ingroup hfp
*
* \details This function allocates a new HF HFP session.
*
* \return
*         \li A pointer to the new HFP session structure if the function succeeds.
*         \li \c NULL otherwise.
*/
#define bt_hfp_allocate_session_hf()		bt_hfp_allocate_session(HFP_ROLE_HF)

/**
* \brief Allocate HFP session
* \ingroup hfp
*
* \details This function allocates a new AG HFP session. Currently AG role is not implemented.
*
* \return
*         \li A pointer to the new HFP session structure if the function succeeds.
*         \li \c NULL otherwise.
*/
#define bt_hfp_allocate_session_ag()		bt_hfp_allocate_session(HFP_ROLE_AG)

/**
* \brief Release HFP session
* \ingroup hfp
*
* \details This function deallocated the specified HFP session.
* This function does not disconnect the session. It just frees the memory used by the bt_hfp_session structure.
* The session has to be disconnected by a remote device or by calling bt_hfp_hf_disconnect.
*
* \param session The HFP session to be deallocated.
*
*/
void bt_hfp_free_session(bt_hfp_session* psess);

/**
* \brief Listen for incoming connections
* \ingroup hfp
*
* \details This function enables incoming connections on the specified HFP session. Changes in
* the session state are reported through a callback function. 
*
* \param session HFP session.
* \param server_channel An RFCOMM server channel on which the RFCOMM session specified in the call to bt_hfp_allocate_session will be listening.
* \param callback A callback function that is called when session state changes or data (command or response) is received from the remote party.
* \param callback_param An arbitrary data pointer that will be passed to the callback function specified by
*              the \c callback parameter.
* 
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise. The callback function is not called in this case. 
*/
bt_bool bt_hfp_listen(bt_hfp_session* session, bt_byte server_channel, bt_hfp_session_callback_pf callback, void* callback_param);

/**
* \brief Connect to a remote device
* \ingroup hfp
*
* \details This function establishes a HFP connection with a remote device which is running in Audio Gateway mode.
* Changes in the session state are reported through a callback function.
*
* \param session HFP session.
* \param remote_addr Address of the remote device.
* \param callback A callback function that is called when session state changes.
* \param callback_param An arbitrary data pointer that will be passed to the callback function specified by
*              the \c callback parameter.
* 
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise. The callback function is not called in this case. 
*/
bt_bool bt_hfp_connect(
	  bt_hfp_session* session, bt_bdaddr_p pbdaddr_remote, 
	  bt_hfp_session_callback_pf callback, void* callback_param);

/**
* \brief Close connection
* \ingroup hfp
*
* \details This function closes a HFP connection with a remote device.
* Changes in the session state are reported through a callback function set when connection was created by calling bt_hfp_connect or bt_hfp_listen.
*
* \param session HSP session.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise.
*/
bt_bool bt_hfp_hf_disconnect(bt_hfp_session* session);

void bt_hfp_hf_set_features(bt_hfp_session* session, bt_ulong features);

bt_bool bt_hfp_register_codec(bt_hfp_session* session, bt_uint codec_id);

/**
* \brief Request operator's name from AG
* \ingroup hfp
*
* \details This function requests operator's name from the AG. The result of this call is reported to the app in the form of 
* the HFP_EVENT_QUERY_OPERATOR_COMPLETED event.\n
* There is usually no need to call this function directly as the operator's name is requested and reported to the app
* during SLC setup.
*
* \param session HSP session.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise.
*/
bt_bool bt_hfp_hf_query_operator(bt_hfp_session* session);

/**
* \brief Answer the incoming call
* \ingroup hfp
*
* \details If there is an incoming call in the AG the app may call this function to accept it.
* The result of calling this function will be HFP_EVENT_CMD_COMPLETED event followed by HFP_EVENT_CALL_STARTED event.
*
* \param session HSP session.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise.
*/
bt_bool bt_hfp_hf_answer(bt_hfp_session* session);


/**
* \brief Enhanced call control
* \ingroup hfp
*
* \details If there is an incoming call in the AG the app may call this function to reject it.
* The result of calling this function will be HFP_EVENT_CMD_COMPLETED event.
*
* \param session HFP session.
* \param cmd Command Id.
* \param call index Call index.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise.
*/
bt_bool bt_hfp_hf_hold(bt_hfp_session* session, bt_byte cmd, bt_byte call_index);

/**
* \brief Reject the incoming call
* \ingroup hfp
*
* \details If there is an incoming call in the AG the app may call this function to reject it.
* The result of calling this function will be HFP_EVENT_CMD_COMPLETED event.
*
* \param session HFP session.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise.
*/
bt_bool bt_hfp_hf_reject(bt_hfp_session* session);

/**
* \brief Terminate the active call
* \ingroup hfp
*
* \details If there is an active call in the AG the app may call this function to terminate it.
* The result of calling this function will be HFP_EVENT_CMD_COMPLETED event.
*
* \param session HFP session.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise.
*/
#define bt_hfp_hf_terminate(session)	bt_hfp_hf_reject(session)

/**
* \brief Place a call
* \ingroup hfp
*
* \details Intiate an outgoing call.
* The result of calling this function will be HFP_EVENT_CMD_COMPLETED event.
*
* \param session HFP session.
* \param number Phone number to dial.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise.
*/
bt_bool bt_hfp_hf_dial_number(bt_hfp_session* session, bt_char* number);

bt_bool bt_hfp_hf_dial_memory(bt_hfp_session* session, bt_int number);

/**
* \brief Redial last dialed number
* \ingroup hfp
*
* \details Request AG to dial last dialed number.
* The result of calling this function will be HFP_EVENT_CMD_COMPLETED event.
*
* \param session HFP session.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise.
*/
bt_bool bt_hfp_hf_redial(bt_hfp_session* session);

/**
* \brief Enable/disable call waiting notification
* \ingroup hfp
*
* \details Request AG to enable or disable call waiting notification. If call waiting notification is enabled, 
* the AG will notify the HF whenever an incoming call is waiting during an ongoing call.
* The result of calling this function will be HFP_EVENT_CMD_COMPLETED event.
* There is usually no need to call this function directly as the call waiting notification is enabled during SLC setup.
*
* \param session HFP session.
* \param enable TRUE - to enable call waiting notification, FALSE - to disable it.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise.
*/
bt_bool bt_hfp_hf_enable_call_waiting_notification(bt_hfp_session* session, bt_bool enable);

/**
* \brief Report microphone gain on the HF to the AG
* \ingroup hfp
*
* \details This function reports microphone gain on the HF to the AF. 
* This function is called by the HF upon SLC setup to notify the AG about the current level of the microphone gain on the HF.
* In case physical mechanisms (buttons, dials etc.) means are implemented on the HF to control the volume levels, 
* the HF calls this function to inform the AG of any changes in the microphone gain.
*
* \param session HFP session.
* \param gain Microphone gain (0 - 15)
* 
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise.
*/
bt_bool bt_hfp_set_mic_gain(bt_hfp_session* session, bt_byte gain);

/**
* \brief Report speaker volume on the HF to the AG
* \ingroup hfp
*
* \details This function reports speaker volume on the HF to the AG. 
* This function is called by the HF upon SLC setup to notify the AG about the current level of the speaker volume on the HF.
* In case physical mechanisms (buttons, dials etc.) means are implemented on the HS to control the volume levels, 
* the HF calls this function to inform the AG of any changes in the speaker volume.
*
* \param session HFP session.
* \param gain Speaker volume (0 - 15)
* 
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise.
*/
bt_bool bt_hfp_set_speaker_volume(bt_hfp_session* session, bt_byte volume);

/**
* \brief Request subscriber number information from the AG
* \ingroup hfp
*
* \details This function sends a command to the AG which causes it to send back subscriber number information.
* The result of calling this function will be HFP_EVENT_CMD_COMPLETED event followed 
* by HFP_EVENT_SUBSCRIBER_NUMBER_RECEIVED event. The caller can retrieve the information from the bt_hfp_session::subscriber_number.
*
* \param session HFP session.
* 
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise. The callback function is not called in this case. 
*/
bt_bool bt_hfp_hf_get_subscriber_number(bt_hfp_session* session);

/**
* \brief Get current value of an indicator
* \ingroup hfp
*
* \details This function returns a value stored on the local device.
*
* \param session HFP session.
* \param indicator_id the ID if the indicator
* 
* \return
*        \li \c Current indicator's value.
*/
bt_byte bt_hfp_get_indicator_value(bt_hfp_session* session, bt_byte indicator_id);

/**
* \brief Determines if there is one or more calls in the AG with the specified status.
* \ingroup hfp
*
* \details This function does not send any commands to the AG. It uses a list of calls stored on the local device.
*
* \param session HFP session.
* \param status a bit mask of HFP_CALL_STATUS_ constants that defines statuses of calls to be checked.
* 
* \return
*        \li \c TRUE if there are calls with specified statuses.
*        \li \c FALSE otherwise.
*/
bt_bool bt_hfp_has_call_with_status(bt_hfp_session* session, bt_byte status);

/**
* \brief Determines if there is one or more active calls in the AG.
* \ingroup hfp
*
* \details This is a define that calls bt_hfp_has_call_with_status with HFP_CALL_STATUS_MASK_ACTIVE as the value for the status parameter.
*
* \param session HFP session.
* 
* \return
*        \li \c TRUE if there are active calls.
*        \li \c FALSE otherwise.
*/
#define bt_hfp_has_active_call(session)		(session->call_status_mask & HFP_CALL_STATUS_MASK_ACTIVE)

/**
* \brief Determines if there is one or more held calls in the AG.
* \ingroup hfp
*
* \details This is a define that calls bt_hfp_has_call_with_status with HFP_CALL_STATUS_MASK_HELD as the value for the status parameter.
*
* \param session HFP session.
* 
* \return
*        \li \c TRUE if there are held calls.
*        \li \c FALSE otherwise.
*/
#define bt_hfp_has_held_call(session)		(session->call_status_mask & HFP_CALL_STATUS_MASK_HELD)

/**
* \brief Determines if there is one or more dialing calls in the AG.
* \ingroup hfp
*
* \details This is a define that calls bt_hfp_has_call_with_status with HFP_CALL_STATUS_MASK_DIALING as the value for the status parameter.
*
* \param session HFP session.
* 
* \return
*        \li \c TRUE if there are dialing calls.
*        \li \c FALSE otherwise.
*/
#define bt_hfp_has_dialing_call(session)	(session->call_status_mask & HFP_CALL_STATUS_MASK_DIALING)

/**
* \brief Determines if there is one or more alerting calls in the AG.
* \ingroup hfp
*
* \details This is a define that calls bt_hfp_has_call_with_status with HFP_CALL_STATUS_MASK_ALERTING as the value for the status parameter.
*
* \param session HFP session.
* 
* \return
*        \li \c TRUE if there are alerting calls.
*        \li \c FALSE otherwise.
*/
#define bt_hfp_has_alerting_call(session)	(session->call_status_mask & HFP_CALL_STATUS_MASK_ALERTING)

/**
* \brief Determines if there is one or more outgoing (dialing or alerting) calls in the AG.
* \ingroup hfp
*
* \details This is a define that calls bt_hfp_has_call_with_status 
* with HFP_CALL_STATUS_MASK_ALERTING | HFP_CALL_STATUS_MASK_DIALING as the value for the status parameter.
*
* \param session HFP session.
* 
* \return
*        \li \c TRUE if there are outgoing calls.
*        \li \c FALSE otherwise.
*/
#define bt_hfp_has_outgoing_call(session)	(session->call_status_mask & (HFP_CALL_STATUS_MASK_ALERTING | HFP_CALL_STATUS_MASK_DIALING))

/**
* \brief Determines if there is one or more incoming calls in the AG.
* \ingroup hfp
*
* \details This is a define that calls bt_hfp_has_call_with_status 
* with HFP_CALL_STATUS_MASK_INCOMING as the value for the status parameter.
*
* \param session HFP session.
* 
* \return
*        \li \c TRUE if there are incoming calls.
*        \li \c FALSE otherwise.
*/
#define bt_hfp_has_incoming_call(session)	(session->call_status_mask & HFP_CALL_STATUS_MASK_INCOMING)

/**
* \brief Determines if there is one or more waiting calls in the AG.
* \ingroup hfp
*
* \details This is a define that calls bt_hfp_has_call_with_status 
* with HFP_CALL_STATUS_MASK_WAITING as the value for the status parameter.
*
* \param session HFP session.
* 
* \return
*        \li \c TRUE if there are waiting calls.
*        \li \c FALSE otherwise.
*/
#define bt_hfp_has_waiting_call(session)	(session->call_status_mask & HFP_CALL_STATUS_MASK_WAITING)

#define bt_hfp_is_call_setup_in_progress(session)		(bt_hfp_get_indicator_value(session, HFP_IND_CALLSETUP) != HFP_IND_CALLSETUP_NO_CALL_SETUP)

#define bt_hfp_is_incoming_call_in_progress(session)	(bt_hfp_get_indicator_value(session, HFP_IND_CALLSETUP) == HFP_IND_CALLSETUP_INCOMING_CALL_SETUP)

#define bt_hfp_is_outgoing_call_in_progress(session)	(bt_hfp_get_indicator_value(session, HFP_IND_CALLSETUP) == HFP_IND_CALLSETUP_OUTGOING_CALL_SETUP)

#define bt_hfp_is_outgoing_call_remote_alerted(session)	(bt_hfp_get_indicator_value(session, HFP_IND_CALLSETUP) == HFP_IND_CALLSETUP_OUTGOING_CALL_ALERT_REMOTE)

#define bt_hfp_has_only_held_call(session)				(bt_hfp_get_indicator_value(session, HFP_IND_CALLHELD) == HFP_IND_CALLHELD_ALL_CALLS_HELD)

#define bt_hfp_has_active_held_call(session)			(bt_hfp_get_indicator_value(session, HFP_IND_CALLHELD) == HFP_IND_CALLHELD_ACTIVE_HELD)

#define bt_hfp_service_present(session)					(bt_hfp_get_indicator_value(session, HFP_IND_SERVICE) != HFP_IND_SERVICE_NO_SERVICE)

#define bt_hfp_roam_active(session)						(bt_hfp_get_indicator_value(session, HFP_IND_ROAM) != HFP_IND_ROAM_NOT_ACTIVE)

/**
* \brief Request a call list from the AG
* \ingroup hfp
*
* \details This function send a command to the AG which causes it to send back a list of all calls
* it currently has.
* The result of calling this function will be HFP_EVENT_CMD_COMPLETED event followed 
* by HFP_EVENT_CALL_LIST_RECEIVED event.
*
* \param session HFP session.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise.
*/
bt_bool bt_hfp_hf_refresh_call_list(bt_hfp_session* session);

/**
* \brief Enable/disable calling line identification notification
* \ingroup hfp
*
* \details Request AG to enable or disable calling line identification notification. 
* If calling line identification notification is enabled and the calling number is available from the network, 
* the AG will notify the HF whenever there is an incoming call. The application will receive HFP_EVENT_CLIP_RECEIVED event.
* The result of calling this function will be HFP_EVENT_CMD_COMPLETED event.
* There is usually no need to call this function directly as the calling line identification notification is enabled during SLC setup.
*
* \param session HFP session.
* \param enable TRUE - to enable calling line identification notification, FALSE - to disable it.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise.
*/
bt_bool bt_hfp_hf_enable_calling_line_identification(bt_hfp_session* session, bt_bool enable);

/**
* \brief Find AG
* \ingroup hfp
*
* \details This function connects to a remote device and tries to find the AG on it. If the remote device
* implements HFP AG this function returns (via callback) the RFCOMM channel which should be used to connect to it.
*
* \param session HFP session.
* \param deviceAddress Remote device's address.
* \param callback A pointer to a function which is called when AG search is complete.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise. the callback is not called in this case.
*/
bt_bool bt_hfp_hf_find_ag(bt_hfp_session* session, bt_bdaddr_t* deviceAddress, bt_hfp_find_remote_callback_pf callback);

/**
* \brief Enable/disable calling voice control in the AG
* \ingroup hfp
*
* \details Request AG to enable or disable voice control. 
* The result of calling this function will be HFP_EVENT_CMD_COMPLETED event.
*
* \param session HFP session.
* \param enable TRUE - to enable voice control, FALSE - to disable it.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise.
*/
bt_bool bt_hfp_hf_enable_voice_recognition(bt_hfp_session* session, bt_bool enable);

/**
* \brief Disable Echo Canceling and Noise Reduction functions in the AG.
* \ingroup hfp
*
* \details This function should be called by the application to disable any Echo Canceling and 
* Noise Reduction functions embedded in the AG.  
* The result of calling this function will be HFP_EVENT_NREC_DISABLED event.
*
* This function should be called when Service Level Connection has been established and before any Audio Connection between
* the HF and the AG is established.
*
* \param session HFP session.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE if parameter if out of range, or RFCOMM connection is not established.
*/
bt_bool bt_hfp_hf_disable_nrec(bt_hfp_session* session);

/**
* \brief Transfer audio connection from the AG to the HF.
* \ingroup hfp
*
* \details This function establishes a SCO connection to the AG. As a result the AG will route its audio path to the HF.  
* The result of calling this function will be HFP_EVENT_AUDIO_CONNECTION_STATE_CHANGED event.
*
* \param session HFP session.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise. the HFP_EVENT_AUDIO_CONNECTION_STATE_CHANGED event is not reported in this case.
*/
bt_bool bt_hfp_connect_audio(bt_hfp_session* session);

/**
* \brief Transfer audio connection from the HF to the AG.
* \ingroup hfp
*
* \details This function terminates the SCO connection to the AG. As a result the AG will route its audio path to itself.  
* The result of calling this function will be HFP_EVENT_AUDIO_CONNECTION_STATE_CHANGED event.
*
* \param session HFP session.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise. the HFP_EVENT_AUDIO_CONNECTION_STATE_CHANGED event is not reported in this case.
*/
bt_bool bt_hfp_disconnect_audio(bt_hfp_session* session);

/**
* \brief Get the address of the remote device this device is connected.
* \ingroup hfp
*
* \param session HFP session.
*
* \return
*        \li \c A pointer to bt_bdaddr structure that contains the address of the remote device.
*/
#define bt_hfp_get_remote_address(session)				(&session->rfcomm_dlc->psess->pch->hci_conn->bdaddr_remote)

bt_bool bt_hfp_hf_send_dtmf_code(bt_hfp_session* session, char code);

/**
* \brief HFP AG incoming call
* \ingroup hfp
*
* \details This function should be called when AG receives notification from the cellular baseband
* about an incoming call.  As a result HFP AG saves information about the call and send notification
* to all connected HF devices.
*
* \param session HFP session.
* \param number string type phone number of format specified by \c type
* \param type
*            \li \c 128 - both the type of number and the numbering plan are unknown
*            \li \c 129 - unknown type of number and ISDN/Telephony numbering plan
*            \li \c 145 - international type of number and ISDN/Telephony numbering plan (contains the character "+")
*
* \return
*         \li A pointer to the new HFP call structure if the function succeeds.
*         \li \c NULL otherwise.
*/
bt_hfp_call_t *bt_hfp_ag_incoming_call(bt_hfp_session* session, const bt_char* number, bt_int type);

/**
* \brief HFP AG outgoing call
* \ingroup hfp
*
* \details This function should be called when AG receives notification from the cellular baseband
* about an outgoing call.  As a result HFP AG saves information about the call and send notification
* to all connected HF devices.
*
* \param session HFP session.
* \param number string type phone number of format specified by \c type
*
* \return
*         \li A pointer to the new HFP call structure if the function succeeds.
*         \li \c NULL otherwise.
*/
bt_hfp_call_t *bt_hfp_ag_outgoing_call(bt_hfp_session* session, const bt_char* number);

/**
* \brief HFP AG reject outgoing call
* \ingroup hfp
*
* \details This function should be called by AG to abort a request from HF to dial a number stored in memory or 
* re-dial last number dialed.
*
* \param session HFP session.
*
*/
void bt_hfp_ag_reject_outgoing_call(bt_hfp_session* session);

/**
* \brief HFP AG outgoing call alerting
* \ingroup hfp
*
* \details This function should be called when AG receives notification that the outgoing call
* is alerting remote party.  As a result HFP AG sends notification to all connected HF devices.
*
* \param session HFP session.
* \param call HFP call.
*
*/
void bt_hfp_ag_outgoing_call_alerting(bt_hfp_session* session, bt_hfp_call_t* call);

/**
* \brief HFP AG call connected
* \ingroup hfp
*
* \details This function should be called when incoming call has been answered locally, or 
* outgoing call has been answered by remote party.  The call is in the connected state.
* As a result HFP AG sends notification to connected HF device.
*
* \param session HFP session.
* \param call HFP call.
*
*/
void bt_hfp_ag_call_connected(bt_hfp_session* session, bt_hfp_call_t *call);

/**
* \brief HFP AG call disconnected
* \ingroup hfp
*
* \details This function should be called when a call has been terminated.  It can be 
* local or remote termination of the outgoing or connected call, as well as rejection
* of the incoming call.  As a result HFP AG sends notification to connected HF device.
*
* \param session HFP session.
* \param call HFP call.
*
*/
void bt_hfp_ag_call_disconnected(bt_hfp_session* session, bt_hfp_call_t *call);

/**
* \brief Set Service State indicator value.
* \ingroup hfp
*
* \details This function should be called by the application during session initialization and every time service 
* state goes up or down.  
*
* \param session HFP session.
* \param ind_val Current service state.  Valid values are 1 for service present, 0 otherwise.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE if parameter if out of range.
*/
bt_bool bt_hfp_ag_set_service_state(bt_hfp_session* session, bt_byte ind_val);

/**
* \brief Set Signal Strength indicator value.
* \ingroup hfp
*
* \details This function should be called by the application during session initialization and every time signal
* strength value changes.  
*
* \param session HFP session.
* \param ind_val Current signal strength.  Valid values are between 0 for low signal and 5 for maximum signal.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE if parameter if out of range.
*/
bt_bool bt_hfp_ag_set_signal_strength(bt_hfp_session* session, bt_byte ind_val);

/**
* \brief Set Roaming State indicator value.
* \ingroup hfp
*
* \details This function should be called by the application during session initialization and every time roaming 
* state changes.  
*
* \param session HFP session.
* \param ind_val Current roaming state.  Valid values are 1 when the phone is roaming, 0 when phone is in home network.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE if parameter if out of range.
*/
bt_bool bt_hfp_ag_set_roam_state(bt_hfp_session* session, bt_byte ind_val);


/**
* \brief Set Battery Level indicator value.
* \ingroup hfp
*
* \details This function should be called by the application during session initialization and when battery 
* level changes.  
*
* \param session HFP session.
* \param ind_val Current battery level.  Valid values are between 0 for low battery and 5 when battery is full.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE if parameter if out of range.
*/
bt_bool bt_hfp_ag_set_battery_level(bt_hfp_session* session, bt_byte ind_val);

/**
* \brief Send Inband ring notification to HF device.
* \ingroup hfp
*
* \details This function should be called by the application to indicate to the HF that the in-band ring 
* tone setting has been locally changed.  
*
* This function should be called when Service Level Connection has been established.
*
* \param session HFP session.
* \param ind_val Indicator value.  0 - the AG provides no in-band ring tone. 1 - the AG provides an in-band ring tone.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE if parameter if out of range, or RFCOMM connection is not established.
*/
bt_bool bt_hfp_ag_set_inband_ring(bt_hfp_session* session, bt_byte ind_val);

/**
* \brief Send voice recognition activation notification to HF device.
* \ingroup hfp
*
* \details This function should be called by the application to indicate to the HF that 
* the voice recognition in the AG has been activated or deactivated.  
*
* This function should be called when Service Level Connection has been established.
*
* \param session HFP session.
* \param activate FALSE - voice recognition has been activated in the AG. TRUE - voice recognition has been deactivated in the AG.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE if parameter if out of range, or RFCOMM connection is not established.
*/
bt_bool bt_hfp_activate_voice_recognition(bt_hfp_session* session, bt_bool activate);

/**
* \brief Set HF microphone gain
* \ingroup hfp
*
* \details This function sets microphone gain on the HF. 
* The AG can call this function to set the HF's microphone gain.
*
* \param session HFP session.
* \param gain Microphone gain (0 - 15)
* 
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise.
*/
#define bt_hfp_ag_set_mic_gain(session, gain) bt_hfp_set_mic_gain(session, gain)

/**
* \brief Set HF speaker volume
* \ingroup hfp
*
* \details This function sets speaker volume on the HF. 
* The AG can call this function to set HF's speaker volume.
*
* \param session HFP session.
* \param gain Speaker volume (0 - 15)
* 
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise.
*/
#define bt_hfp_ag_set_speaker_volume(session, volume) bt_hfp_set_speaker_volume(session, volume)

/**
* \brief Set Operator name.
* \ingroup hfp
*
* \details This function should be called by the application during session initialization.
*
* \param session HFP session.
* \param name Operator's name.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE if parameter if out of range.
*/
bt_bool bt_hfp_ag_set_operator_name(bt_hfp_session* session, const bt_byte* name);

/**
* \brief Set Subscriber number.
* \ingroup hfp
*
* \details This function should be called by the application during session initialization.
*
* \param session HFP session.
* \param number string type phone number of format specified by \c type
* \param type
*            \li \c 128 - both the type of number and the numbering plan are unknown
*            \li \c 129 - unknown type of number and ISDN/Telephony numbering plan
*            \li \c 145 - international type of number and ISDN/Telephony numbering plan (contains the character "+")
* \param service Indicates which service this phone number relates to. Shall be either 4 (voice) or 5 (fax).
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE if parameter if out of range.
*/
bt_bool bt_hfp_ag_set_subscriber_number(bt_hfp_session* session, const bt_byte* number, bt_byte type, bt_byte service);

bt_bool bt_hfp_ag_find_headsets(bt_hfp_session* gw, bt_byte search_length);

bt_bool bt_hfp_add_audio_rx_buffer(bt_hfp_session* session, bt_hfp_audio_packet_t* buffer);

bt_bool bt_hfp_add_audio_tx_buffer(bt_hfp_session* session, bt_hfp_audio_packet_t* buffer);

bt_hfp_call_t* bt_hfp_ag_find_call_with_status(bt_hfp_session* session, bt_byte status);

#ifdef __cplusplus
}
#endif

#include "cdbt/hfp/hfp_private.h"

#endif // __HFP_H
