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

#ifndef __HSP_H
#define __HSP_H

/**
* \defgroup hsp HSP
*/

#include "cdbt/sdp/sdp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _bt_hsp_session_phase_e
{
	HSP_SESSION_PHASE_IDLE,
	HSP_SESSION_PHASE_WAITING_RFCOMM_CONNECT,
	HSP_SESSION_PHASE_WAITING_SCO_CONNECT,
	HSP_SESSION_PHASE_CONNECTING,
	HSP_SESSION_PHASE_WAITING_CMD_RESPONSE,
} bt_hsp_session_phase_e;

#define HSP_SESSION_STATE_FREE				      0xffff
#define HSP_SESSION_STATE_DISCONNECTED		      0x00
#define HSP_SESSION_STATE_RFCOMM_CONNECTED	      0x01
#define HSP_SESSION_STATE_SCO_CONNECTED		      0x02
#define HSP_SESSION_STATE_CONNECTED			      0x03
#define HSP_SESSION_STATE_CONNECTING		      0x04
#define HSP_SESSION_STATE_SCO_DISCONNECTING       0x08
#define HSP_SESSION_STATE_SENDING_AUDIO_PACKET    0x10

typedef enum _bt_hsp_session_event_e
{
	HSP_SESSION_NOTHING,
	HSP_SESSION_RFCOMM_CONNECTION_STATE_CHANGED,
	HSP_SESSION_SCO_CONNECTION_STATE_CHANGED,
	HSP_SESSION_CMD_RESPONSE_RECEIVED,
	HSP_SESSION_RING,
	HSP_SESSION_AUDIO_DATA_RECEIVED,
	HSP_SESSION_HEADSET_CONNECT_ATTEMPT_FAILED,
	HSP_SESSION_MIC_VOLUME_CHANGED,
	HSP_SESSION_SPK_VOLUME_CHANGED,
	HSP_SESSION_AUDIO_DATA_SENT,

} bt_hsp_session_event_e;

#define HS_CMD_BUTTON_PRESS	(const bt_byte*)"AT+CKPD=200\r"
#define HS_CMD_BUTTON_PRESS_LEN	12

#define HS_CMD_REMOTE_SET_MIC_VOLUME		"\r\n+VGM="
#define HS_CMD_REMOTE_SET_MIC_VOLUME_LEN	7
#define HS_CMD_REMOTE_SET_SPK_VOLUME		"\r\n+VGS="
#define HS_CMD_REMOTE_SET_SPK_VOLUME_LEN	7

#define HS_CMD_LOCAL_SET_MIC_VOLUME		(const bt_byte*)"AT+VGM="
#define HS_CMD_LOCAL_SET_MIC_VOLUME_LEN	7
#define HS_CMD_LOCAL_SET_SPK_VOLUME		(const bt_byte*)"AT+VGS="
#define HS_CMD_LOCAL_SET_SPK_VOLUME_LEN	7

#define HS_MIN_VOLUME	0
#define HS_MAX_VOLUME	15

#define HS_RES_OK		"\r\nOK\r\n"
#define HS_RES_ERROR	"\r\nERROR\r\n"
#define HS_RES_RING		"\r\nRING\r\n"

#define HSP_BUFFER_LENGTH    10

#define HSP_CODEC_TYPE_PCM   0

struct _bt_hsp_session_t;

typedef struct _bt_hsp_audio_packet_s bt_hsp_audio_packet_t;

/**
* \brief Notify the application of changes in the HSP session
* \ingroup hsp
*
* \details This function is called by the HSP layer when various session parameters have changed.
* It is also called when commands from the remote device or 
* response codes to the commands sent to the the remote device are received.
*
* \param session HSP session.
* \param evt Specifies the id of the event. The value of this arguments is one of the constants defined in bt_hsp_session_change_e enumeration.
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
* \param what_param The first what parameter.
* \param what_param2 The second what parameter.
* \param callback_param The value of the callback_param passed to the call of bt_hsp_hs_listen or bt_hsp_hs_connect.
* 
*/
typedef void (*bt_hsp_session_callback_pf)(struct _bt_hsp_session_t* session, bt_hsp_session_event_e evt, void* evt_param, void* callback_param);

/**
* \brief Notify the application of the result of searching for the Audio Gateway
* \ingroup hsp
*
* \details This function is called by the HSP layer when searching for the Audio Gateway on a remote device has completed.
*
* \param session HSP session.
* \param server_channel The RFCOMM server channel used by the Audio Gateway. The application must pass this value to bt_hsp_hs_connect. The value is valid only if the \c found parameter is TRUE.
* \param found \c TRUE if Audio Gateway has been found on the remote device. \c FALSE otherwise.
*
*/
typedef void (*bt_hsp_find_ag_callback_pf)(struct _bt_hsp_session_t* session, bt_byte server_channel, bt_bool found);

// event parameters

typedef struct _bt_hsp_evt_rfcomm_connection_state_changed_t
{
	bt_bool connected;
} bt_hsp_evt_rfcomm_connection_state_changed_t;

typedef struct _bt_hsp_evt_audio_connection_state_changed_t
{
	bt_bool connected;           ///< Defines the state of audio connection. TRUE - if audio is connected, FALSE - if audio is disconnected
	bt_hci_conn_state_t* hci_conn;	 ///< A pointer to \c HCI SCO connection if \c connected is TRUE, otherwise - NULL
} bt_hsp_evt_audio_connection_state_changed_t;

typedef struct _bt_hsp_evt_mic_volume_changed_t
{
	bt_byte volume;             ///< The value of the microphone gain
} bt_hsp_evt_mic_volume_changed_t;

typedef struct _bt_hsp_evt_spk_volume_changed_t
{
	bt_byte volume;             ///< The value of the speaker volume
} bt_hsp_evt_spk_volume_changed_t;

/**
* \brief  HSP_SESSION_AUDIO_DATA_RECEIVED event parameter
* \ingroup hsp
*
* \details HSP layer calls the session callback when it receives a chunk of audio data from the remote device
* and passes a pointer to \c bt_hsp_evt_audio_data_t which contains a pointer to the data and its length.
*
*
*/
typedef struct _bt_hsp_evt_audio_data_t
{
	bt_hsp_audio_packet_t* packet; ///< A pointer to an audio packet buffer.
} bt_hsp_evt_audio_data_t;


typedef struct _bt_hsp_evt_audio_packet_sent_s
{
	bt_hsp_audio_packet_t* packet;
} bt_hsp_evt_audio_packet_sent_t;

/**
* \brief  Parameter to HSP_SESSION_AUDIO_DATA_RECEIVED & HSP_SESSION_AUDIO_DATA_SENT events
* \ingroup hsp
*
* \details HSP layer calls the session callback when it has received a chunk of audio data from the remote device.
* The callback is passed a pointer to \c bt_hsp_evt_audio_data_t which contains a pointer to this structure.
*/
struct _bt_hsp_audio_packet_s
{
	bt_hsp_audio_packet_t* next_packet;

	bt_uint data_len;        ///< The length of the audio data
	bt_uint max_data_len;    ///< Maximum length of the media data that can be stored in the buffer pointed by the \c data member.
	bt_byte* data;           ///< Pointer to a buffer to store audio data. This pointer must be allocated by the HSP consumer
	                         ///< before adding this structure to receive (::bt_hsp_add_audio_rx_buffer) or send queue (::bt_hsp_add_audio_tx_buffer).
};

typedef struct _bt_hsp_session_t
{
	bt_uint state;
	bt_hsp_session_phase_e phase;
	bt_rfcomm_dlc_t* rfcomm_dlc;
	bt_hci_conn_state_t* hci_conn;

	bt_hsp_session_callback_pf session_callback;
	void* session_callback_param;

	bt_hci_listener_t hci_disconnect_listener;

	bt_bdaddr_t ag_bdaddr;
	bt_byte ag_server_channel;
	bt_byte ag_remaining_connect_attemtps;
	bt_l2cap_channel_t* ag_sdp_channel;
	bt_sdp_data_element_t ag_sdp_pattern;
	bt_sr_handle_t ag_service_record_handle;
	bt_bool ag_next_attr_is_server_channel;
	bt_hsp_find_ag_callback_pf ag_callback;

	bt_byte buffer[HSP_BUFFER_LENGTH];

	bt_queue_element_t* audio_rx_queue;
	bt_queue_element_t* audio_tx_queue;

	bt_byte find_server_channel;
	bt_bool find_service_found;
	bt_byte find_remaining_connect_attemtps;
	bt_bdaddr_t find_device_address;
	bt_hci_ctrl_listener_t find_disconnect_listener;

} bt_hsp_session_t;

/**
* \brief Initialize HSP layer
* \ingroup hsp
*
* \details This function initializes the HSP layer of the stack. It must be called prior to any other
* HSP function can be called.
*/
bt_bool bt_hsp_init(void);

/**
* \brief Allocate HSP session
* \ingroup hsp
*
* \details This function allocates a new HSP session.
*
* \param l2cap_mgr The L2CAP manager on which the HSP session is to be created.
*
* \return
*         \li A pointer to the new HSP session structure if the function succeeds.
*         \li \c NULL otherwise.
*/
bt_hsp_session_t* bt_hsp_allocate_session(bt_l2cap_mgr_t* l2cap_mgr);

/**
* \brief Release HSP session
* \ingroup hsp
*
* \details This function deallocated the specified HSP session.
* This function does not disconnect the session. It just frees the memory used by the bt_hsp_session structure.
* The session has to be disconnected by a remote device or by calling bt_hsp_hs_disconnect.
*
* \param session The HSP session to be deallocated.
*
*/
void bt_hsp_free_session(bt_hsp_session_t* psess);

/**
* \brief Listen for incoming connections
* \ingroup hsp
*
* \details This function enables incoming connections (local device is running in Head Set mode) on the specified HSP session. Changes in
* the session state are reported through a callback function. 
*
* \param session HSP session.
* \param server_channel An RFCOMM server channel on which the RFCOMM session specified in the call to bt_hsp_allocate_session will be listening.
* \param callback A callback function that is called when session state changes.
* \param param An arbitrary data pointer that will be passed to the callback function specified by
*              the \c callback parameter.
* 
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise. The callback function is not called in this case. 
*/
bt_bool bt_hsp_hs_listen(bt_hsp_session_t* session, bt_byte server_channel, bt_hsp_session_callback_pf callback, void* callback_param);

/**
* \brief Connect to a remote device
* \ingroup hsp
*
* \details This function establishes a HSP connection with a remote device which is running in Audio Gateway mode.
* Changes in the session state are reported through a callback function.
*
* \param session HSP session.
* \param remote_addr Address of the remote device.
* \param callback A callback function that is called when session state changes.
* \param param An arbitrary data pointer that will be passed to the callback function specified by
*              the \c callback parameter.
* 
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise. The callback function is not called in this case. 
*/
bt_bool bt_hsp_hs_connect(
	bt_hsp_session_t* session, bt_bdaddr_p pbdaddr_remote, 
	bt_hsp_session_callback_pf callback, void* param);

/**
* \brief Close connection
* \ingroup hsp
*
* \details This function closes a HSP connection with a remote device.
* Changes in the session state are reported through a callback function set when connection was created by calling bt_hsp_hs_connect or bt_hsp_hs_listen.
*
* \param session HSP session.
*/
bt_bool bt_hsp_hs_disconnect(bt_hsp_session_t* session);

/**
* \brief Send a headset button press to a remote device
* \ingroup hsp
*
* \details This function sends the "AT+CKPD=200\r" command to the Audio Gateway(AG). This command is sent to the AG when answering an incoming call or transferring connection from the AG to the Headset.
*
* \param session HSP session.
*/
bt_bool bt_hsp_hs_send_button_press(bt_hsp_session_t* session);

/**
* \brief Transfer audio connection from Audio Gateway to Headset.
* \ingroup hsp
*
* \details This function sends the "AT+CKPD=200\r" command to the Audio Gateway(AG). 
* This function is a macro which calls bt_hsp_hs_send_button_press. 
* If at the time of calling this function the AG owns the audio connection (i.e. the sound goes to the AG's speaker) 
* it will establish a SCO connection to the Headset and route the sound to the HS.
*
* \param session HSP session.
*/
#define bt_hsp_hs_transfer_audio(session)	bt_hsp_hs_send_button_press(session)

/**
* \brief Find an Audio Gateway on a remote device
* \ingroup hsp
*
* \details This function tries to find an Audio Gateway on a remote device. 
* If the remote device support the HSP AG mode this function will return (through a \c callback function) the server channel for establishing an RFCOMM connection.
*
* \param session HSP session.
* \param remote_addr Address of the remote device.
* \param callback A callback function that is called when search is completed.
* 
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise. The callback function is not called in this case. 
*/
bt_bool bt_hsp_hs_find_ag(bt_hsp_session_t* session, bt_bdaddr_t* deviceAddress, bt_hsp_find_ag_callback_pf callback);

/**
* \brief Report microphone gain on the Headset to the Audio Gateway
* \ingroup hsp
*
* \details This function reports microphone gain on the Headset to the Audio Gateway. 
* This function is called by the Headset upon connection establishment to notify the AG about the current level of the microphone gain on the Headset.
* In case physical mechanisms (buttons, dials etc.) means are implemented on the HS to control the volume levels, 
* the HS calls this function to inform the AG of any changes in the microphone gain.
*
* \param session HSP session.
* \param gain Microphone gain (0 - 15)
* 
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise.
*/
bt_bool bt_hsp_set_mic_gain(bt_hsp_session_t* session, bt_byte gain);

/**
* \brief Report speaker volume on the Headset to the Audio Gateway
* \ingroup hsp
*
* \details This function reports speaker volume on the Headset to the Audio Gateway. 
* This function is called by the Headset upon connection establishment to notify the AG about the current level of the speaker volume on the Headset.
* In case physical mechanisms (buttons, dials etc.) means are implemented on the HS to control the volume levels, 
* the HS calls this function to inform the AG of any changes in the speaker volume.
*
* \param session HSP session.
* \param gain Speaker volume (0 - 15)
* 
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise.
*/
bt_bool bt_hsp_set_speaker_volume(bt_hsp_session_t* session, bt_byte volume);

bt_bool bt_hsp_add_audio_rx_buffer(bt_hsp_session_t* session, bt_hsp_audio_packet_t* buffer);

/**
* \brief Send audio to a remote device
* \ingroup hsp
*
* \details This function sends audio data to a remote device.
*
* \param session HSP session.
* \param buffer Pointer to a buffer with the audio data
* 
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise. The callback function is not called in this case. 
*/
bt_bool bt_hsp_add_audio_tx_buffer(bt_hsp_session_t* session, bt_hsp_audio_packet_t* buffer);

/**
* \brief Get the address of the remote device this device is connected.
* \ingroup hsp
*
* \param session HSP session.
*
* \return
*        \li \c A pointer to bt_bdaddr structure that contains the address of the remote device.
*/
#define bt_hsp_get_remote_address(session)				(&session->rfcomm_dlc->psess->pch->hci_conn->bdaddr_remote)

#ifdef __cplusplus
}
#endif

#include "cdbt/hsp/hsp_private.h"

#endif
