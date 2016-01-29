/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __AVRCP_H
#define __AVRCP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup avrcp Audio/Video Remote Control Profile (AVRCP)
 *
 * The Audio/Video Remote Control Profile (AVRCP) defines the features and procedures
 * required in order to ensure interoperability between Bluetooth devices with audio/video
 * control functions in the Audio/Video distribution scenarios.
 */

#include "cdbt/sdp/sdp.h"
#include "cdbt/sdp/sdp_client.h"
#include "cdbt/avctp/avctp.h"

#define AVRCP_BTSIG_COMPANY_ID    0x001958

#define AVRCP_MANAGER_STATE_IDLE             0

#define AVRCP_MANAGER_FLAG_SEARCHING         1

#define AVRCP_CHANNEL_STATE_FREE             0
#define AVRCP_CHANNEL_STATE_IDLE             1
#define AVRCP_CHANNEL_STATE_CONNECTING       2
#define AVRCP_CHANNEL_STATE_CONNECTED        3
#define AVRCP_CHANNEL_STATE_DISCONNECTING    4

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
#define AVRCP_EVT_NOTHING                         0
#define AVRCP_EVT_CONTROL_CHANNEL_CONNECTED       1    ///< This event is generated when a control channel between two AVRCP entities has been established.
#define AVRCP_EVT_CONTROL_CHANNEL_DISCONNECTED    2    ///< This event is generated when a control channel between two AVRCP entities has been terminated.
#define AVRCP_EVT_CONTROL_CONNECTION_FAILED       3    ///< This event is generated when a local device failed to create a control channel between two AVRCP entities.
#define AVRCP_EVT_BROWSING_CHANNEL_CONNECTED      4    ///< This event is generated when a browsing channel between two AVRCP entities has been established.
#define AVRCP_EVT_BROWSING_CHANNEL_DISCONNECTED   5    ///< This event is generated when a browsing channel between two AVRCP entities has been terminated.
#define AVRCP_EVT_BROWSING_CONNECTION_FAILED      6    ///< This event is generated when a local device failed to create a browsing channel between two AVRCP entities.
#define AVRCP_EVT_SEARCH_COMPLETED                7    ///< This event is generated when a local device completed searching for nearby targets.

#define AVRCP_EVT_PANEL_RESPONSE_RECEIVED                     50    ///< This event is generated when a local device received a response to a PASS THROUGH command.
#define AVRCP_EVT_COMPANY_ID_LIST_RECEIVED                    51    ///< This event is generated when a local device received a response to a "get company id" request.
#define AVRCP_EVT_EVENT_ID_LIST_RECEIVED                      52    ///< This event is generated when a local device received a response to a "get supported events" request.
#define AVRCP_EVT_PLAYER_SETTING_ATTRIBUTES_RECEIVED          53    ///< This event is generated when a local device received a response to a "get supported player setting attributes" request.
#define AVRCP_EVT_PLAYER_SETTING_VALUES_RECEIVED              54    ///< This event is generated when a local device received a response to a "get player setting attribute values" request.
#define AVRCP_EVT_PLAYER_CURRENT_SETTING_VALUES_RECEIVED      55    ///< This event is generated when a local device received a response to a "get current player setting attribute values" request.
#define AVRCP_EVT_SET_PLAYER_SETTING_VALUES_COMPLETED         56    ///< This event is generated when a local device received a response to a "set player setting attribute values" request.
#define AVRCP_EVT_PLAYER_SETTING_ATTRIBUTES_TEXT_RECEIVED     57    ///< This event is generated when a local device received a response to a "get player setting attributes displayable text" request.
#define AVRCP_EVT_PLAYER_SETTING_VALUES_TEXT_RECEIVED         58    ///< This event is generated when a local device received a response to a "get player setting attribute values displayable text" request.
#define AVRCP_EVT_INFORM_DISPLAYABLE_CHARACTER_SET_COMPLETED  59    ///< This event is generated when a local device received a response to a "inform displayable character set" request.
#define AVRCP_EVT_INFORM_BATTERY_STATUS_OF_CT_COMPLETED       60    ///< This event is generated when a local device received a response to a "inform battery status" request.
#define AVRCP_EVT_GET_ELEMENT_ATTRIBUTES_RECEIVED             61    ///< This event is generated when a local device received a response to a "get media element attributes" request.
#define AVRCP_EVT_GET_PLAY_STATUS_RECEIVED                    62    ///< This event is generated when a local device received a response to a "get play status" request.
#define AVRCP_EVT_PLAYBACK_STATUS_CHANGED                     63    ///< This event is generated when a local device received a "play status changed" notification.
#define AVRCP_EVT_TRACK_CHANGED                               64    ///< This event is generated when a local device received a "track changed changed" notification.
#define AVRCP_EVT_TRACK_REACHED_END                           65    ///< This event is generated when a local device received a "track reached end" notification.
#define AVRCP_EVT_TRACK_REACHED_START                         66    ///< This event is generated when a local device received a "track reached start" notification.
#define AVRCP_EVT_PLAYBACK_POS_CHANGED                        67    ///< This event is generated when a local device received a "playback position changed" notification.
#define AVRCP_EVT_BATT_STATUS_CHANGED                         68    ///< This event is generated when a local device received a "battery status changed" notification.
#define AVRCP_EVT_SYSTEM_STATUS_CHANGED                       69    ///< This event is generated when a local device received a "system status changed" notification.
#define AVRCP_EVT_PLAYER_APPLICATION_SETTING_CHANGED          70    ///< This event is generated when a local device received a "player application setting changed" notification.
#define AVRCP_EVT_NOW_PLAYING_CONTENT_CHANGED                 71    ///< This event is generated when a local device received a "now playing content changed" notification.
#define AVRCP_EVT_AVAILABLE_PLAYERS_CHANGED                   72    ///< This event is generated when a local device received a "available players changed" notification.
#define AVRCP_EVT_ADDRESSED_PLAYER_CHANGED                    73    ///< This event is generated when a local device received a "addressed player changed" notification.
#define AVRCP_EVT_UIDS_CHANGED                                74    ///< This event is generated when a local device received a "UIDs changed" notification.
#define AVRCP_EVT_VOLUME_CHANGED                              75    ///< This event is generated when a local device received a "volume changed" notification.
#define AVRCP_EVT_SET_ABSOLUTE_VOLUME_COMPLETED               76    ///< This event is generated when a local device received a response to a "set absolute volume" request.
#define AVRCP_EVT_SET_ADDRESSED_PLAYER_COMPLETED              77    ///< This event is generated when a local device received a response to a "set addressed player" request.
#define AVRCP_EVT_PLAY_ITEM_COMPLETED                         78    ///< This event is generated when a local device received a response to a "play item" request.
#define AVRCP_EVT_ADD_TO_NOW_PLAYING_COMPLETED                79    ///< This event is generated when a local device received a response to a "add to now playing" request.
#define AVRCP_EVT_REGISTER_NOTIFICATIONS_COMPLETED            80    ///< This event is generated when a local device received a response to a "register notification" request.

#define AVRCP_EVT_PANEL_COMMAND_RECEIVED                      81    ///< This event is generated when a local device received a PASS THROUGH command.
#define AVRCP_EVT_SET_ABSOLUTE_VOLUME_REQUESTED               82    ///< This event is generated when a local device received a "set absolute volume" request.
#define AVRCP_EVT_BATTERY_STATUS_OF_CT_RECEIVED               83    ///< This event is generated when a local device received a "battery status of controller" command.
#define AVRCP_EVT_DISPLAYABLE_CHARACTER_SET_RECEIVED          84    ///< This event is generated when a local device received a "displayable chracter set command" request.
#define AVRCP_EVT_ELEMENT_ATTRIBUTES_REQUESTED                85    ///< This event is generated when a local device received a "get element attributes" request.
#define AVRCP_EVT_REGISTER_NOTIFICATION_REQUESTED             86    ///< This event is generated when a local device received a "register notification" request.
/**@}*/ 
/**@}*/

#define AVRCP_CHANNEL_FLAG_LISTENING                   1
#define AVRCP_CHANNEL_FLAG_SENDING                     2
#define AVRCP_CHANNEL_FLAG_REGISTERING_NOTIFICATIONS   4
#define AVRCP_CHANNEL_FLAG_PLAY_STATUS_REQUESTED       8

#define AVCTP_ERROR_SUCCESS                            0

#define AVCTP_ERROR_BAD_STATE                          0x31

#define AVCTP_MESSAGE_PACKET_TYPE_SINGLE      0
#define AVCTP_MESSAGE_PACKET_TYPE_START       1
#define AVCTP_MESSAGE_PACKET_TYPE_CONTINUE    2
#define AVCTP_MESSAGE_PACKET_TYPE_END         3

#define AVCTP_MESSAGE_TYPE_COMMAND            0
#define AVCTP_MESSAGE_TYPE_RESPONSE           1

typedef struct _bt_avrcp_mgr_t       bt_avrcp_mgr_t;
typedef struct _bt_avrcp_channel_t   bt_avrcp_channel_t;
/*
typedef struct _bt_avctp_message_t   bt_avctp_message_t;
*/

#include "cdbt/avrcp/avrcp_command.h"

typedef struct _bt_avrcp_device_s
{
	bt_bdaddr_t address;
	bt_byte* name;
	bt_byte pg_scan_rpt_mode;
	bt_byte pg_scan_period_mode;
	bt_long cod;
	bt_int clock_offset;
	bt_byte rssi;
} bt_avrcp_device_t;

/**
* \brief  Parameter to AVRCP_EVT_SEARCH_COMPLETED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::device_search -
*          when searching for nearby devices has finished.
*/
typedef struct _bt_avrcp_evt_search_completed_s
{
	bt_avrcp_device_t* devices;    ///< list of found devices
	bt_byte count;                 ///< the number of found devices
} bt_avrcp_evt_search_completed_t;

/**
* \brief  Parameter to AVRCP_EVT_CONTROL_CHANNEL_CONNECTED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::channel_connected -
*          when a control channel between two devices has been established.
*/
typedef struct _bt_avrcp_evt_channel_connected_t
{
	bt_avrcp_channel_t* channel;    ///< AVRCP channel
} bt_avrcp_evt_channel_connected_t;

/**
* \brief  Parameter to AVRCP_EVT_CONTROL_CHANNEL_DISCONNECTED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::channel_disconnected -
*          when a control channel between two devices has been terminated.
*/
typedef struct _bt_avrcp_evt_channel_disconnected_t
{
	bt_avrcp_channel_t* channel;    ///< AVRCP channel
} bt_avrcp_evt_channel_disconnected_t;

/**
* \brief  Parameter to AVRCP_EVT_CONTROL_CONNECTION_FAILED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::connection_failed -
*          when a local device failed to create a control channel between two AVRCP entities.
*/
typedef struct _bt_avrcp_evt_connection_failed_t
{
	bt_avrcp_channel_t* channel;    ///< AVRCP channel
} bt_avrcp_evt_connection_failed_t;

/**
* \brief  Parameter to AVRCP_EVT_PANEL_RESPONSE_RECEIVED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::panel_response_received -
*          when a local device received a response to a PASS THROUGH command.
*/
typedef struct _bt_avrcp_evt_panel_response_received_t
{
	bt_avrcp_channel_t* channel;    ///< AVRCP channel
	bt_byte ctype;                  ///< Response type
	bt_byte button_status;          ///< Button status
	bt_byte opcode;                 ///< Operation Id
	bt_byte* params;                ///< Operation parameters
	bt_byte params_len;             ///< Length of the operation parameters

} bt_avrcp_evt_panel_response_received_t;

/**
* \brief  Parameter to AVRCP_EVT_PANEL_COMMAND_RECEIVED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::panel_command_received -
*          when a local device received a PASS THROUGH command.
*/
typedef struct _bt_avrcp_evt_panel_command_received_t
{
	bt_avrcp_channel_t* channel;    ///< AVRCP channel
	bt_byte ctype;				    ///< Command type
	bt_byte button_status;		    ///< Button status
	bt_byte opcode;				    ///< Operation Id
	bt_byte* params;			    ///< Operation parameters
	bt_byte params_len;			    ///< Length of the operation parameters

} bt_avrcp_evt_panel_command_received_t;

typedef struct _bt_avrcp_evt_register_events_completed_t
{
	bt_avrcp_channel_t* channel;    ///< AVRCP channel

} bt_avrcp_evt_register_events_completed_t;

/**
* \brief  Parameter to an application callback.
* \ingroup avrcp
*
* \details This union is used to pass event specific data to the AVRCP consumer. Which member of the union 
*          points to a valid structure depends on the event reported to the consumer. In general, each event
*          has a corresponding member in the union.
*/
typedef union _bt_avrcp_event_u
{
	bt_avrcp_evt_channel_connected_t channel_connected;                     ///< Valid if event is AVRCP_EVT_CONTROL_CHANNEL_CONNECTED
	bt_avrcp_evt_channel_disconnected_t channel_disconnected;               ///< Valid if event is AVRCP_EVT_CONTROL_CHANNEL_DISCONNECTED
	bt_avrcp_evt_connection_failed_t connection_failed;                     ///< Valid if event is AVRCP_EVT_CONTROL_CONNECTION_FAILED
	bt_avrcp_evt_panel_response_received_t panel_response_received;         ///< Valid if event is AVRCP_EVT_PANEL_RESPONSE_RECEIVED
	bt_avrcp_evt_search_completed_t device_search;                          ///< Valid if event is AVRCP_EVT_SEARCH_COMPLETED
	bt_av_response_t response;                                              
	bt_av_capability_company_id_t company_id;                               ///< Valid if event is AVRCP_EVT_COMPANY_ID_LIST_RECEIVED
	bt_av_capability_event_id_t supported_event_id;                         ///< Valid if event is AVRCP_EVT_EVENT_ID_LIST_RECEIVED
	bt_av_player_settings_t player_settings;                                ///< Valid if event is AVRCP_EVT_PLAYER_SETTING_ATTRIBUTES_RECEIVED
	bt_av_player_setting_values_t player_setting_values;                    ///< Valid if event is AVRCP_EVT_PLAYER_SETTING_VALUES_RECEIVED
	bt_av_player_setting_current_values_t player_setting_current_values;    ///< Valid if event is AVRCP_EVT_PLAYER_CURRENT_SETTING_VALUES_RECEIVED
	bt_av_player_settings_text_t player_settings_text;                      ///< Valid if event is AVRCP_EVT_PLAYER_SETTING_ATTRIBUTES_TEXT_RECEIVED
	bt_av_player_setting_values_text_t player_setting_values_text;          ///< Valid if event is AVRCP_EVT_PLAYER_SETTING_VALUES_TEXT_RECEIVED
	bt_av_element_attributes_t element_attributes;                          ///< Valid if event is AVRCP_EVT_GET_ELEMENT_ATTRIBUTES_RECEIVED
	bt_av_play_status_t play_status;                                        ///< Valid if event is AVRCP_EVT_GET_PLAY_STATUS_RECEIVED
	bt_av_set_absolute_volume_t absolute_volume;                            ///< Valid if event is AVRCP_EVT_SET_ABSOLUTE_VOLUME_COMPLETED
	bt_av_set_addressed_player_t addressed_player;                          ///< Valid if event is AVRCP_EVT_SET_ADDRESSED_PLAYER_COMPLETED
	bt_av_play_item_t play_item_status;                                     ///< Valid if event is AVRCP_EVT_PLAY_ITEM_COMPLETED
	bt_av_add_to_now_playing_t add_to_now_playing_status;                   ///< Valid if event is AVRCP_EVT_ADD_TO_NOW_PLAYING_COMPLETED
	bt_av_notification_t notification;                                      ///< Valid if event is on of the following:
                                                                            ///< \li AVRCP_EVT_PLAYBACK_STATUS_CHANGED                    
                                                                            ///< \li AVRCP_EVT_TRACK_CHANGED                              
                                                                            ///< \li AVRCP_EVT_TRACK_REACHED_END                          
                                                                            ///< \li AVRCP_EVT_TRACK_REACHED_START                        
                                                                            ///< \li AVRCP_EVT_PLAYBACK_POS_CHANGED                       
                                                                            ///< \li AVRCP_EVT_BATT_STATUS_CHANGED                        
                                                                            ///< \li AVRCP_EVT_SYSTEM_STATUS_CHANGED                      
                                                                            ///< \li AVRCP_EVT_PLAYER_APPLICATION_SETTING_CHANGED         
                                                                            ///< \li AVRCP_EVT_NOW_PLAYING_CONTENT_CHANGED                
                                                                            ///< \li AVRCP_EVT_AVAILABLE_PLAYERS_CHANGED                  
                                                                            ///< \li AVRCP_EVT_ADDRESSED_PLAYER_CHANGED                   
                                                                            ///< \li AVRCP_EVT_UIDS_CHANGED                               
                                                                            ///< \li AVRCP_EVT_VOLUME_CHANGED                             
	bt_avrcp_evt_register_events_completed_t register_events;               ///< Valid if event AVRCP_EVT_REGISTER_NOTIFICATIONS_COMPLETED

	bt_avrcp_evt_panel_command_received_t panel_command_received;           ///< Valid if event is AVRCP_EVT_PANEL_COMMAND_RECEIVED
	bt_av_battery_status_of_ct_t battery_status_of_ct;                      ///< Valid if event is AVRCP_EVT_BATTERY_STATUS_OF_CT_RECEIVED
	bt_av_displayable_character_set_t displayable_character_set;            ///< Valid if event is AVRCP_EVT_DISPLAYABLE_CHARACTER_SET_RECEIVED
	bt_av_get_element_attributes_t get_element_attributes;                  ///< Valid if event is AVRCP_EVT_ELEMENT_ATTRIBUTES_REQUESTED
	bt_av_register_notification_t register_notification;                    ///< Valid if event is AVRCP_EVT_REGISTER_NOTIFICATION_REQUESTED
} bt_avrcp_event_t;

///////////////////////////////////////////////////////////////////////////////

/**
* \brief  AVRCP channel description
* \ingroup avrcp
*
* \details This structure is used to hold information about an AVRCP channel.
*/
struct _bt_avrcp_channel_t
{
	bt_byte state;
	bt_byte flags;
	bt_uint cur_event_mask;
	bt_uint new_event_mask;
	bt_byte play_status;
	bt_uint req_play_status_interval;
	bt_ulong play_status_req_time;
	bt_avctp_channel_t* control_channel;
	bt_avctp_channel_t* browsing_channel;
	bt_avrcp_mgr_t* avrcp_mgr;
	bt_queue_element_t*    send_cq_head;
	bt_queue_element_t*    ack_cq_head;

	// target data
	bt_uint tg_event_mask;
	bt_uint tg_interim_event_mask;
	bt_uint tg_signalled_event_mask;
	bt_ulong notify_song_position;
	bt_byte register_notification_tran_id[AVC_MAXEVENTS];
	bt_byte tg_next_event_id;
	bt_byte volume;
};

/**
 * \brief AVRCP application callback.
 * \ingroup avrcp
 *
 * \details In order to be notified of various events a consumer of the AVRCP layer has to register a callback function (done with bt_avrcp_start()). 
 *          The stack will call that function whenever a new event has been generated.
 *
 * \param mgr AVRCP manager.
 *
 * \param evt AVRCP event.
 *          The event can be one of the following values:
 *            @arg AVRCP_EVT_CONTROL_CHANNEL_CONNECTED                   This event is generated when a control channel between two AVRCP entities has been established.
 *            @arg AVRCP_EVT_CONTROL_CHANNEL_DISCONNECTED                This event is generated when a control channel between two AVRCP entities has been terminated.
 *            @arg AVRCP_EVT_CONTROL_CONNECTION_FAILED                   This event is generated when a local device failed to create a control channel between two AVRCP entities.
 *            @arg AVRCP_EVT_BROWSING_CHANNEL_CONNECTED                  This event is generated when a browsing channel between two AVRCP entities has been established.
 *            @arg AVRCP_EVT_BROWSING_CHANNEL_DISCONNECTED               This event is generated when a browsing channel between two AVRCP entities has been terminated.
 *            @arg AVRCP_EVT_BROWSING_CONNECTION_FAILED                  This event is generated when a local device failed to create a browsing channel between two AVRCP entities.
 *            @arg AVRCP_EVT_SEARCH_COMPLETED                            This event is generated when a local device completed searching for nearby targets.
 *
 *            @arg AVRCP_EVT_PANEL_RESPONSE_RECEIVED                     This event is generated when a local device received a response to a PASS THROUGH command.
 *            @arg AVRCP_EVT_COMPANY_ID_LIST_RECEIVED                    This event is generated when a local device received a response to a "get company id" request.
 *            @arg AVRCP_EVT_EVENT_ID_LIST_RECEIVED                      This event is generated when a local device received a response to a "get supported events" request.
 *            @arg AVRCP_EVT_PLAYER_SETTING_ATTRIBUTES_RECEIVED          This event is generated when a local device received a response to a "get supported player setting attributes" request.
 *            @arg AVRCP_EVT_PLAYER_SETTING_VALUES_RECEIVED              This event is generated when a local device received a response to a "get player setting attribute values" request.
 *            @arg AVRCP_EVT_PLAYER_CURRENT_SETTING_VALUES_RECEIVED      This event is generated when a local device received a response to a "get current player setting attribute values" request.
 *            @arg AVRCP_EVT_SET_PLAYER_SETTING_VALUES_COMPLETED         This event is generated when a local device received a response to a "set player setting attribute values" request.
 *            @arg AVRCP_EVT_PLAYER_SETTING_ATTRIBUTES_TEXT_RECEIVED     This event is generated when a local device received a response to a "get player setting attributes displayable text" request.
 *            @arg AVRCP_EVT_PLAYER_SETTING_VALUES_TEXT_RECEIVED         This event is generated when a local device received a response to a "get player setting attribute values displayable text" request.
 *            @arg AVRCP_EVT_INFORM_DISPLAYABLE_CHARACTER_SET_COMPLETED  This event is generated when a local device received a response to a "inform displayable character set" request.
 *            @arg AVRCP_EVT_INFORM_BATTERY_STATUS_OF_CT_COMPLETED       This event is generated when a local device received a response to a "inform battery status" request.
 *            @arg AVRCP_EVT_GET_ELEMENT_ATTRIBUTES_RECEIVED             This event is generated when a local device received a response to a "get media element attributes" request.
 *            @arg AVRCP_EVT_GET_PLAY_STATUS_RECEIVED                    This event is generated when a local device received a response to a "get play status" request.
 *            @arg AVRCP_EVT_PLAYBACK_STATUS_CHANGED                     This event is generated when a local device received a "play status changed" notification.
 *            @arg AVRCP_EVT_TRACK_CHANGED                               This event is generated when a local device received a "track changed changed" notification.
 *            @arg AVRCP_EVT_TRACK_REACHED_END                           This event is generated when a local device received a "track reached end" notification.
 *            @arg AVRCP_EVT_TRACK_REACHED_START                         This event is generated when a local device received a "track reached start" notification.
 *            @arg AVRCP_EVT_PLAYBACK_POS_CHANGED                        This event is generated when a local device received a "playback position changed" notification.
 *            @arg AVRCP_EVT_BATT_STATUS_CHANGED                         This event is generated when a local device received a "battery status changed" notification.
 *            @arg AVRCP_EVT_SYSTEM_STATUS_CHANGED                       This event is generated when a local device received a "system status changed" notification.
 *            @arg AVRCP_EVT_PLAYER_APPLICATION_SETTING_CHANGED          This event is generated when a local device received a "player application setting changed" notification.
 *            @arg AVRCP_EVT_NOW_PLAYING_CONTENT_CHANGED                 This event is generated when a local device received a "now playing content changed" notification.
 *            @arg AVRCP_EVT_AVAILABLE_PLAYERS_CHANGED                   This event is generated when a local device received a "available players changed" notification.
 *            @arg AVRCP_EVT_ADDRESSED_PLAYER_CHANGED                    This event is generated when a local device received a "addressed player changed" notification.
 *            @arg AVRCP_EVT_UIDS_CHANGED                                This event is generated when a local device received a "UIDs changed" notification.
 *            @arg AVRCP_EVT_VOLUME_CHANGED                              This event is generated when a local device received a "volume changed" notification.
 *            @arg AVRCP_EVT_SET_ABSOLUTE_VOLUME_COMPLETED               This event is generated when a local device received a response to a "set absolute volume" request.
 *            @arg AVRCP_EVT_SET_ADDRESSED_PLAYER_COMPLETED              This event is generated when a local device received a response to a "set addressed player" request.
 *            @arg AVRCP_EVT_PLAY_ITEM_COMPLETED                         This event is generated when a local device received a response to a "play item" request.
 *            @arg AVRCP_EVT_ADD_TO_NOW_PLAYING_COMPLETED                This event is generated when a local device received a response to a "add to now playing" request.
 *            @arg AVRCP_EVT_REGISTER_NOTIFICATIONS_COMPLETED            This event is generated when a local device received a response to a "register notification" request.
 *
 *            @arg AVRCP_EVT_PANEL_COMMAND_RECEIVED                      This event is generated when a local device received a PASS THROUGH command.
 *            @arg AVRCP_EVT_SET_ABSOLUTE_VOLUME_REQUESTED               This event is generated when a local device received a "set absolute volume" request.
 *            @arg AVRCP_EVT_BATTERY_STATUS_OF_CT_RECEIVED               This event is generated when a local device received a "battery status of controller" command.
 *            @arg AVRCP_EVT_DISPLAYABLE_CHARACTER_SET_RECEIVED          This event is generated when a local device received a "displayable chracter set command" request.
 *            @arg AVRCP_EVT_ELEMENT_ATTRIBUTES_REQUESTED                This event is generated when a local device received a "get element attributes" request.
 *
 * \param evt_param Event parameter.
 *          Which member of the bt_avrcp_event_t union is valid depends on the event:
 *            @arg AVRCP_EVT_CONTROL_CHANNEL_CONNECTED                  \c bt_avrcp_evt_channel_connected_t        channel_connected
 *            @arg AVRCP_EVT_CONTROL_CHANNEL_DISCONNECTED               \c bt_avrcp_evt_channel_disconnected_t     channel_disconnected
 *            @arg AVRCP_EVT_CONTROL_CONNECTION_FAILED                  \c bt_avrcp_evt_connection_failed_t        connection_failed
 *            @arg AVRCP_EVT_PANEL_RESPONSE_RECEIVED                    \c bt_avrcp_evt_panel_response_received_t  panel_response_received
 *            @arg AVRCP_EVT_SEARCH_COMPLETED                           \c bt_avrcp_evt_search_completed_t         device_search
 *            @arg AVRCP_EVT_COMPANY_ID_LIST_RECEIVED                   \c bt_av_capability_company_id_t           company_id
 *            @arg AVRCP_EVT_EVENT_ID_LIST_RECEIVED                     \c bt_av_capability_event_id_t             supported_event_id
 *            @arg AVRCP_EVT_PLAYER_SETTING_ATTRIBUTES_RECEIVED         \c bt_av_player_settings_t                 player_settings
 *            @arg AVRCP_EVT_PLAYER_SETTING_VALUES_RECEIVED             \c bt_av_player_setting_values_t           player_setting_values
 *            @arg AVRCP_EVT_PLAYER_CURRENT_SETTING_VALUES_RECEIVED     \c bt_av_player_setting_current_values_t   player_setting_current_values
 *            @arg AVRCP_EVT_PLAYER_SETTING_ATTRIBUTES_TEXT_RECEIVED    \c bt_av_player_settings_text_t            player_settings_text
 *            @arg AVRCP_EVT_PLAYER_SETTING_VALUES_TEXT_RECEIVED        \c bt_av_player_setting_values_text_t      player_setting_values_text
 *            @arg AVRCP_EVT_GET_ELEMENT_ATTRIBUTES_RECEIVED            \c bt_av_element_attributes_t              element_attributes
 *            @arg AVRCP_EVT_GET_PLAY_STATUS_RECEIVED                   \c bt_av_play_status_t                     play_status
 *            @arg AVRCP_EVT_SET_ABSOLUTE_VOLUME_COMPLETED              \c bt_av_set_absolute_volume_t             absolute_volume
 *            @arg AVRCP_EVT_SET_ADDRESSED_PLAYER_COMPLETED             \c bt_av_set_addressed_player_t            addressed_player
 *            @arg AVRCP_EVT_PLAY_ITEM_COMPLETED                        \c bt_av_play_item_t                       play_item_status
 *            @arg AVRCP_EVT_ADD_TO_NOW_PLAYING_COMPLETED               \c bt_av_add_to_now_playing_t              add_to_now_playing_status
 *            @arg AVRCP_EVT_PLAYBACK_STATUS_CHANGED                    \c bt_av_notification_t                    notification
 *            @arg AVRCP_EVT_TRACK_CHANGED                              \c bt_av_notification_t                    notification
 *            @arg AVRCP_EVT_TRACK_REACHED_END                          \c bt_av_notification_t                    notification
 *            @arg AVRCP_EVT_TRACK_REACHED_START                        \c bt_av_notification_t                    notification
 *            @arg AVRCP_EVT_PLAYBACK_POS_CHANGED                       \c bt_av_notification_t                    notification
 *            @arg AVRCP_EVT_BATT_STATUS_CHANGED                        \c bt_av_notification_t                    notification
 *            @arg AVRCP_EVT_SYSTEM_STATUS_CHANGED                      \c bt_av_notification_t                    notification
 *            @arg AVRCP_EVT_PLAYER_APPLICATION_SETTING_CHANGED         \c bt_av_notification_t                    notification
 *            @arg AVRCP_EVT_NOW_PLAYING_CONTENT_CHANGED                \c bt_av_notification_t                    notification
 *            @arg AVRCP_EVT_AVAILABLE_PLAYERS_CHANGED                  \c bt_av_notification_t                    notification
 *            @arg AVRCP_EVT_ADDRESSED_PLAYER_CHANGED                   \c bt_av_notification_t                    notification
 *            @arg AVRCP_EVT_UIDS_CHANGED                               \c bt_av_notification_t                    notification
 *            @arg AVRCP_EVT_VOLUME_CHANGED                             \c bt_av_notification_t                    notification
 *
 *            @arg AVRCP_EVT_PANEL_COMMAND_RECEIVED                     \c bt_avrcp_evt_panel_command_received_t    panel_command_received
 *            @arg AVRCP_EVT_BATTERY_STATUS_OF_CT_RECEIVED              \c bt_av_battery_status_of_ct_t             battery_status_of_ct
 *            @arg AVRCP_EVT_DISPLAYABLE_CHARACTER_SET_RECEIVED         \c bt_av_displayable_character_set_t        displayable_character_set
 *            @arg AVRCP_EVT_ELEMENT_ATTRIBUTES_REQUESTED               \c bt_av_get_element_attributes_t           get_element_attributes
 *
 * \param callback_param A pointer to an arbitrary data set by a call to bt_avrcp_start.
 * 
 */
typedef void (*bt_avrcp_mgr_callback_fp)(bt_avrcp_mgr_t* mgr, bt_byte evt, bt_avrcp_event_t* evt_param, void* callback_param);

/**
 * \brief Find Controller/Target callback.
 * \ingroup avrcp
 *
 * \details This callback is called when search for Controller/Target has finished.
 *
 * \param found This can be one of the following values:
 *              \li \c TRUE if Controller/Target was found.
 *              \li \c FALSE otherwise.
 * \param callback_param A pointer to an arbitrary data set by a call to bt_avrcp_find_target/bt_avrcp_find_controller.
 * 
 */
typedef void (*bt_avrcp_find_callback_fp)(bt_bool found, void* param);

/**
 * \brief AVRCP manager.
 * \ingroup avrcp
 *
 * \details A structure that glues all pieces together. There is only one instance of this structure allocated by dotstack.
 *          A pointer to the instance can be get with \c bt_avrcp_get_mgr().
 *          
 */
struct _bt_avrcp_mgr_t
{
	bt_byte state;                          ///< Manager state. This value can be one of the following values:
                                            ///< \li AVRCP_MANAGER_STATE_IDLE
                                            ///< \li AVRCP_MANAGER_STATE_CONNECTING
                                            ///< \li AVRCP_MANAGER_STATE_DISCONNECTING
	bt_byte flags;                          ///< Additional manager state. This value can be a combination of the following values:
                                            ///< \li AVRCP_MANAGER_FLAG_SEARCHING
	bt_ulong company_id;                    ///< The 24-bit unique ID obtained from the IEEE Registration Authority Committee.
                                            ///< If the vendor of a TG device does not have the unique ID, the value 0xFFFFFF may be used.
	bt_uint supported_events;               ///< Events supported by the target. This value can be a combination of the following values:
                                            ///< \li AVC_EVENT_FLAG_PLAYBACK_STATUS_CHANGED
                                            ///< \li AVC_EVENT_FLAG_TRACK_CHANGED
                                            ///< \li AVC_EVENT_FLAG_TRACK_REACHED_END
                                            ///< \li AVC_EVENT_FLAG_TRACK_REACHED_START
                                            ///< \li AVC_EVENT_FLAG_PLAYBACK_POS_CHANGED
                                            ///< \li AVC_EVENT_FLAG_BATT_STATUS_CHANGED
                                            ///< \li AVC_EVENT_FLAG_SYSTEM_STATUS_CHANGED
                                            ///< \li AVC_EVENT_FLAG_PLAYER_APPLICATION_SETTING_CHANGED
                                            ///< \li AVC_EVENT_FLAG_NOW_PLAYING_CONTENT_CHANGED
                                            ///< \li AVC_EVENT_FLAG_AVAILABLE_PLAYERS_CHANGED
                                            ///< \li AVC_EVENT_FLAG_ADDRESSED_PLAYER_CHANGED
                                            ///< \li AVC_EVENT_FLAG_UIDS_CHANGED
                                            ///< \li AVC_EVENT_FLAG_VOLUME_CHANGED
	bt_ulong song_length;                   ///< Current song length
	bt_ulong song_position;                 ///< Current song poistion
	bt_byte play_status;                    ///< Current playback status
	bt_byte volume;                         ///< Current volumne
	bt_byte battery_status;                 ///< Current battery statusThis can be one of the following values:
                                            ///< \li AVC_BATTERY_STATUS_NORMAL
                                            ///< \li AVC_BATTERY_STATUS_WARNING
                                            ///< \li AVC_BATTERY_STATUS_CRITICAL
                                            ///< \li AVC_BATTERY_STATUS_EXTERNAL
                                            ///< \li AVC_BATTERY_STATUS_FULL_CHARGE
	bt_byte system_status;                  ///< Current system status
	bt_av_element_id_t current_track_id;    ///< Current track id
	bt_uint uid_counter;                    ///< The number of media items in the target.

	bt_avrcp_channel_t* channels;           ///< List of available AVRCP channels.

	bt_avrcp_mgr_callback_fp callback;      ///< Pointer to a function used to notify the AVRCP consumer about various events.
	void* callback_param;                   ///< Pointer to arbitrary data to be passed to the \c callback.

//	bt_avrcp_channel_t* opening_channel;
};


/**
 * \brief Return a pointer to an instance of the AVRCP manager.
 * \ingroup avrcp
 *
 * \details This function returns a pointer to an instance of the AVRCP manager. 
 *          There is only one instance of the manager allocated by the stack. 
 */
bt_avrcp_mgr_t* bt_avrcp_get_mgr(void);

/**
 * \brief Initialize AVRCP to be used in target mode.
 * \ingroup avrcp
 *
 * \details This function initializes the AVRCP layer of the stack in target mode. It must be called prior to any other
 *          AVRCP function can be called.
 *
 * \param company_id The 24-bit unique ID obtained from the IEEE Registration Authority Committee.
 *                   If the vendor of a TG device does not have the unique ID, the value 0xFFFFFF may be used.
 * \param supported_events Bitmask that specifies events supported by the target. This value can be a combination of the following values:
 *                         \li AVC_EVENT_FLAG_PLAYBACK_STATUS_CHANGED
 *                         \li AVC_EVENT_FLAG_TRACK_CHANGED
 *                         \li AVC_EVENT_FLAG_TRACK_REACHED_END
 *                         \li AVC_EVENT_FLAG_TRACK_REACHED_START
 *                         \li AVC_EVENT_FLAG_PLAYBACK_POS_CHANGED
 *                         \li AVC_EVENT_FLAG_BATT_STATUS_CHANGED
 *                         \li AVC_EVENT_FLAG_SYSTEM_STATUS_CHANGED
 *                         \li AVC_EVENT_FLAG_PLAYER_APPLICATION_SETTING_CHANGED
 *                         \li AVC_EVENT_FLAG_NOW_PLAYING_CONTENT_CHANGED
 *                         \li AVC_EVENT_FLAG_AVAILABLE_PLAYERS_CHANGED
 *                         \li AVC_EVENT_FLAG_ADDRESSED_PLAYER_CHANGED
 *                         \li AVC_EVENT_FLAG_UIDS_CHANGED
 *                         \li AVC_EVENT_FLAG_VOLUME_CHANGED
 */
void bt_avrcp_init_target(bt_ulong company_id, bt_uint supported_events);

/**
 * \brief Initialize AVRCP to be used in controller mode.
 * \ingroup avrcp
 *
 * \details This function initializes the AVRCP layer of the stack in controller mode. It must be called prior to any other
 *          AVRCP function can be called.
 */
void bt_avrcp_init_controller(void);

/**
 * \brief Start the AVRCP layer.
 * \ingroup avrcp
 *
 * \details In order to be notified of various events a consumer of the AVRCP layer has to register a callback function. 
 *          The stack will call the callback function whenever a new event has been generated passing the code of the event as the second parameter.
 *          bt_avrcp_start() stores pointers to the \c callback and \c callback_param in the bt_avrcp_mgr_t structure.
 *
 * \param mgr AVRCP manager.
 * \param callback The callback function that will be called when the AVRCP generates an event.
 * \param callback_param A pointer to arbitrary data to be passed to the \c callback callback.
 * 
*/
void bt_avrcp_start(bt_avrcp_mgr_t* mgr, bt_avrcp_mgr_callback_fp callback, void* callback_param);

/**
* \brief Allocate AVRCP channel
* \ingroup avrcp
*
* \details This function allocates a new incoming AVRCP channel. The channel is intended to be used to accept a connection
*          from a remote device. There can be only one incoming channel.
*
* \param mgr AVRCP manager.
* \param create_browsing_channel Defines weather a browsing channel will be created.
*
* \return
*         \li A pointer to the new AVRCP channel if the function succeeds.
*         \li \c NULL otherwise.
*/
bt_avrcp_channel_t* bt_avrcp_create_channel(bt_avrcp_mgr_t* mgr, bt_bool create_browsing_channel);

/**
* \brief Allocate AVRCP channel
* \ingroup avrcp
*
* \details This function allocates a new outgoing AVRCP channel. The channel is intended to be used to create a connection
*          to a remote device. There can be multiple outgoing channels.
*
* \param mgr AVRCP manager.
* \param create_browsing_channel Defines weather a browsing channel will be created.
*
* \return
*         \li A pointer to the new AVRCP channel if the function succeeds.
*         \li \c NULL otherwise.
*/
bt_avrcp_channel_t* bt_avrcp_create_outgoing_channel(bt_avrcp_mgr_t* mgr, bt_bool create_browsing_channel);

/**
 * \brief Destroy AVRCP channel.
 * \ingroup avrcp
 *
 * \details This function frees memory used by the channel. The channel has to exist and be in the "idle" state for this function to succeed. 
 *          I.e. the channel has to be disconnected before this function can be called.
 *
 * \param channel AVRCP channel.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
*/
bt_bool bt_avrcp_destroy_channel(bt_avrcp_channel_t* channel);

/**
 * \brief Listen for incoming connections.
 * \ingroup avrcp
 *
 * \details This function enables incoming connections on the specified AVRCP channel.
 *
 * \param channel AVRCP channel.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
*/
bt_bool bt_avrcp_listen(bt_avrcp_channel_t* channel);

/**
 * \brief Cancel listening for incoming connections.
 * \ingroup avrcp
 *
 * \details This function stops listening for incoming connections on a specified channel.
 *
 * \param channel AVRCP channel.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
*/
void bt_avrcp_cancel_listen(bt_avrcp_channel_t* channel);

/**
 * \brief Get AVCTP control channel state.
 * \ingroup avrcp
 *
 * \details This function returns status of the AVCTP control channel.
 *
 * \param channel AVRCP channel.
 * 
 * \return  Returns of the following values:
 *          \li AVCTP_CHANNEL_STATE_FREE
 *          \li AVCTP_CHANNEL_STATE_IDLE
 *          \li AVCTP_CHANNEL_STATE_CONNECTING
 *          \li AVCTP_CHANNEL_STATE_CONNECTED
 *          \li AVCTP_CHANNEL_STATE_DISCONNECTING
*/
bt_byte bt_avrcp_get_control_channel_state(bt_avrcp_channel_t* channel);

/**
 * \brief Get AVCTP browsing channel state.
 * \ingroup avrcp
 *
 * \details This function returns status of the AVCTP browsing channel.
 *
 * \param channel AVRCP channel.
 * 
 * \return  Returns of the following values:
 *          \li AVCTP_CHANNEL_STATE_FREE
 *          \li AVCTP_CHANNEL_STATE_IDLE
 *          \li AVCTP_CHANNEL_STATE_CONNECTING
 *          \li AVCTP_CHANNEL_STATE_CONNECTED
 *          \li AVCTP_CHANNEL_STATE_DISCONNECTING
*/
bt_byte bt_avrcp_get_browsing_channel_state(bt_avrcp_channel_t* channel);

/**
 * \brief Get channel's remote BT address.
 * \ingroup avrcp
 *
 * \details This function returns the address of the remote device associated with the channel.
 *
 * \param channel AVRCP channel.
 * 
 * \return \li The address of the remote device if channel is connected. 
 *         \li NULL otherwise.
 */
bt_bdaddr_t* bt_avrcp_get_channel_remote_address(bt_avrcp_channel_t* channel);

/**
 * \brief Connect to a remote device.
 * \ingroup avrcp
 *
 * \details This function establishes a connection to a remote device specified by the \p remote_address. If connection
 *          cannot be initiated for some reason, for example, there is not enough resources, it returns FALSE and no events are generated. Otherwise
 *          the result of an attempt to connect to the remote device is reported via the AVRCP callback. 
 *          The events generated will either be AVRCP_EVT_CONTROL_CHANNEL_CONNECTED or AVRCP_EVT_CONTROL_CONNECTION_FAILED.
 *
 * \param channel AVRCP channel.
 * \param remote_address The address of a remote device.
 * 
 * \return
 *        \li \c TRUE if connection establishment has been started.
 *        \li \c FALSE otherwise.
*/
bt_bool bt_avrcp_connect(bt_avrcp_channel_t* channel, bt_bdaddr_t* remote_address);

/**
 * \brief Disconnect from a remote device.
 * \ingroup avrcp
 *
 * \details This function closes a connection to a remote device. 
 *
 * \param channel AVRCP channel.
 * 
 * \return
 *        \li \c TRUE if disconnection has been started.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_avrcp_disconnect(bt_avrcp_channel_t* channel);

/**
 * \brief Get HCI connection for a channel
 * \ingroup avrcp
 *
 * \details This function returns a pointer to a structure that describes an HCI connection a channel is open on. 
 *          The return value can be used to call various function from the HCI layer. For example, if an app
 *          wants to force disconnection from a remote device it can call ::bt_hci_disconnect.
 *
 * \param channel AVRCP channel.
 * 
 * \return
 *        \li \c Pointer to a structure that describes an HCI connection if the function succeeds.
 *        \li \c NULL otherwise. The function fails only if a channel specified by the \p channel parameter 
 *        \li does not exist or there is no HCI connection between local and remote devices associated with the channel.
 */
bt_hci_conn_state_p* bt_avrcp_get_hci_connection(bt_avrcp_channel_t* channel);

/**
 * \brief Send AVRCP command
 * \ingroup avrcp
 *
 * \details This function sends a command to the remote device.
 *
 * \param channel AVRCP channel.
 * \param command Command to be sent.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
 */
bt_bool bt_avrcp_send_cmd(bt_avrcp_channel_t* channel, bt_av_command_t* command);

/**
 * \brief Add to "now playing" list.
 * \ingroup avrcp
 *
 * \details This function adds a media element specified by \p element_id to the "now playing" list on the target.
 *
 * \param channel AVRCP channel.
 * \param scope The scope in which the \p element_id is valid. This value can be on the following values:
 *              \li AVC_SCOPE_MEDIA_PLAYER_LIST
 *              \li AVC_MEDIA_PLAYER_VIRTUAL_FILESYSTEM
 *              \li AVC_SEARCH
 *              \li AVC_NOW_PLAYING
 *
 * \param element_id UID of the media element to be added to the "now playing" list.
 * \param counter UID counter.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
 */
bt_bool bt_avrcp_add_to_now_playing(bt_avrcp_channel_t* channel, bt_byte scope,
									bt_av_element_id_t* element_id, bt_uint counter);

/**
 * \brief Get Company ID list.
 * \ingroup avrcp
 *
 * \details This function requests a list of company id's supported by the remote device
 *
 * \param channel AVRCP channel.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
 */
bt_bool bt_avrcp_get_company_id_list(bt_avrcp_channel_t* channel);

/**
 * \brief Get supported events.
 * \ingroup avrcp
 *
 * \details This function requests a list of events supported by the remote device
 *
 * \param channel AVRCP channel.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
 */
bt_bool bt_avrcp_get_supported_event_id_list(bt_avrcp_channel_t* channel);

/**
 * \brief Get current player setting values.
 * \ingroup avrcp
 *
 * \details This function requests a list of current set values for the player application on the target. 
 *          The list of attribute ids whose values have to be returned is passed via the \p response_buffer parameter. 
 *          The caller has to set bt_av_player_setting_current_values_t::setting_id_list to a list of 
 *          player setting attribute ids, bt_av_player_setting_current_values_t::count to the number
 *          of entries in the list, bt_av_player_setting_current_values_t::setting_value_id_list to a buffer 
 *          where returned values will be stored.
 *
 * \param channel AVRCP channel.
 * \param response_buffer Pointer to bt_av_player_setting_current_values_t structure initialized as stated above.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
 */
bt_bool bt_avrcp_get_current_player_application_setting_value(
	bt_avrcp_channel_t* channel, bt_av_player_setting_current_values_t* response_buffer);

/**
 * \brief Get media element attributes.
 * \ingroup avrcp
 *
 * \details This function requests the attributes of the element specified with \p element_id. 
 *
 * \note         Currently \p element_id is ignored. The AVRCP specification defines that only UID 0
 *               can be used to return the attributes of the current track.
 *
 * \param channel AVRCP channel.
 * \param element_id UID of the media element whose attributes are requested.
 * \param attr_mask Bitmask that defines which attributes are requested. This value can be a combination of the following values:
 *                  \li AVC_MEDIA_ATTR_FLAG_TITLE
 *                  \li AVC_MEDIA_ATTR_FLAG_ARTIST
 *                  \li AVC_MEDIA_ATTR_FLAG_ALBUM
 *                  \li AVC_MEDIA_ATTR_FLAG_NUMBER
 *                  \li AVC_MEDIA_ATTR_FLAG_TOTAL_NUMBER
 *                  \li AVC_MEDIA_ATTR_FLAG_GENRE
 *                  \li AVC_MEDIA_ATTR_FLAG_PLAYING_TIME
 *                  \li AVC_MEDIA_ATTR_FLAG_ALL
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
 */
bt_bool bt_avrcp_get_element_attributes(bt_avrcp_channel_t* channel, bt_av_element_id_t* element_id,
										bt_uint attr_mask);

/**
 * \brief Get playback status.
 * \ingroup avrcp
 *
 * \details This function requests the status of the currently playing media at the target.
 *
 * \param channel AVRCP channel.
 * \param repeat_interval Interval in milliseconds at which AVRCP polls the target for playback status. 
 *                        If 0 is passed polling is stopped.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
 */
bt_bool bt_avrcp_get_play_status(bt_avrcp_channel_t* channel, bt_uint repeat_interval);

/**
 * \brief Get player setting attribute text.
 * \ingroup avrcp
 *
 * \details This function requests the target device to provide supported player application setting attribute displayable text 
 *          for the provided player application setting attributes.
 *          The list of attribute ids whose displayable text have to be returned is passed via the \p response_buffer parameter. 
 *          The caller has to set bt_av_player_settings_text_t::setting_id_list to a list of 
 *          player setting attribute ids, bt_av_player_settings_text_t::count to the number
 *          of entries in the list, bt_av_player_settings_text_t::setting_text_list to a buffer 
 *          where returned values will be stored.
 *
 * \param channel AVRCP channel.
 * \param response_buffer Pointer to bt_av_player_settings_text_t structure initialized as stated above.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
 */
bt_bool bt_avrcp_get_player_application_setting_attr_text(
	bt_avrcp_channel_t* channel, bt_av_player_settings_text_t* response_buffer);

/**
 * \brief Get player setting value text.
 * \ingroup avrcp
 *
 * \details This function request the target device to provide target supported player application setting value 
 *          displayable text for the provided player application setting attribute values.
 *          The list of attribute ids whose value displayable text have to be returned is passed via the \p response_buffer parameter. 
 *          The caller has to set bt_av_player_setting_values_text_t::setting_value_id_list to a list of 
 *          player setting attribute value ids, bt_av_player_setting_values_text_t::count to the number
 *          of entries in the list, bt_av_player_setting_values_text_t::setting_value_text_list to a buffer 
 *          where returned values will be stored.
 *
 * \param channel AVRCP channel.
 * \param response_buffer Pointer to bt_av_player_setting_values_text_t structure initialized as stated above.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
 */
bt_bool bt_avrcp_get_player_application_setting_value_text(
	bt_avrcp_channel_t* channel, bt_byte attr_id,
	bt_av_player_setting_values_text_t* response_buffer);

/**
 * \brief Inform controller's battery status.
 * \ingroup avrcp
 *
 * \details This function is used to inform the target about the controller's battery status.
 *
 * \param channel AVRCP channel.
 * \param status Battery status. This can be one of the following values:
 *               \li AVC_BATTERY_STATUS_NORMAL
 *               \li AVC_BATTERY_STATUS_WARNING
 *               \li AVC_BATTERY_STATUS_CRITICAL
 *               \li AVC_BATTERY_STATUS_EXTERNAL
 *               \li AVC_BATTERY_STATUS_FULL_CHARGE
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
 */
bt_bool bt_avrcp_inform_battery_status(bt_avrcp_channel_t* channel, bt_byte status);

/**
 * \brief Inform displayable character set.
 * \ingroup avrcp
 *
 * \details This function informs the list of character set supported by the controller to the target.
 *
 * \param channel AVRCP channel.
 * \param charset_list List of displayable character sets.
 * \param charset_count Number of entries in the list.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
 */
bt_bool bt_avrcp_inform_displayable_character_set(
	bt_avrcp_channel_t* channel, bt_uint* charset_list, bt_byte charset_count);

/**
 * \brief Get supported player setting attributes.
 * \ingroup avrcp
 *
 * \details This function request the target device to provide target supported player application setting attributes.
 *          The list of attribute ids is stored in the setting_id_list member of the \p response_buffer parameter. 
 *          The caller has to set bt_av_player_settings_t::setting_id_list to a buffer 
 *          where returned values will be stored and bt_av_player_settings_t::count to the number
 *          of entries in the list.
 *
 * \param channel AVRCP channel.
 * \param response_buffer Pointer to bt_av_player_settings_t structure initialized as stated above.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
 */
bt_bool bt_avrcp_list_player_application_setting_attributes(
	bt_avrcp_channel_t* channel, bt_av_player_settings_t* response_buffer);

/**
 * \brief Get player setting attribute values.
 * \ingroup avrcp
 *
 * \details This function requests the target device to list the set of possible values 
 *          for the requested player application setting attribute.
 *          The list of attribute value ids is stored in the setting_value_id_list member of the \p response_buffer parameter. 
 *          The caller has to set bt_av_player_setting_values_t::setting_value_id_list to a buffer 
 *          where returned values will be stored and bt_av_player_setting_values_t::count to the number
 *          of entries in the list.
 *
 * \param channel AVRCP channel.
 * \param response_buffer Pointer to bt_av_player_setting_values_t structure initialized as stated above.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
 */
bt_bool bt_avrcp_list_player_application_setting_values(
	bt_avrcp_channel_t* channel, bt_byte attr_id, 
	bt_av_player_setting_values_t* response_buffer);

/**
 * \brief Play media item.
 * \ingroup avrcp
 *
 * \details This function starts playing an item indicated by the UID.
 *
 * \param channel AVRCP channel.
 * \param scope The scope in which the \p element_id is valid. This value can be on the following values:
 *              \li AVC_SCOPE_MEDIA_PLAYER_LIST
 *              \li AVC_MEDIA_PLAYER_VIRTUAL_FILESYSTEM
 *              \li AVC_SEARCH
 *              \li AVC_NOW_PLAYING
 * \param element_id UID of the media element to be played.
 * \param counter UID counter.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
 */
bt_bool bt_avrcp_play_item(bt_avrcp_channel_t* channel, bt_byte scope,
						   bt_av_element_id_t* element_id, bt_uint counter);

/**
 * \brief Register notification.
 * \ingroup avrcp
 *
 * \details This function registers with the target to receive notifications 
 *          asynchronously based on specific events occurring. 
 *
 * \param channel AVRCP channel.
 * \param event_id Event Id. This value can be one of the following values:
 *                           \li AVC_EVENT_PLAYBACK_STATUS_CHANGED
 *                           \li AVC_EVENT_TRACK_CHANGED
 *                           \li AVC_EVENT_TRACK_REACHED_END
 *                           \li AVC_EVENT_TRACK_REACHED_START
 *                           \li AVC_EVENT_PLAYBACK_POS_CHANGED
 *                           \li AVC_EVENT_BATT_STATUS_CHANGED
 *                           \li AVC_EVENT_SYSTEM_STATUS_CHANGED
 *                           \li AVC_EVENT_PLAYER_APPLICATION_SETTING_CHANGED
 *                           \li AVC_EVENT_NOW_PLAYING_CONTENT_CHANGED
 *                           \li AVC_EVENT_AVAILABLE_PLAYERS_CHANGED
 *                           \li AVC_EVENT_ADDRESSED_PLAYER_CHANGED
 *                           \li AVC_EVENT_UIDS_CHANGED
 *                           \li AVC_EVENT_VOLUME_CHANGED
 * \param playback_interval The time interval (in seconds) at which the change in playback position will be notified.
 *                          Applicable for AVC_EVENT_PLAYBACK_POS_CHANGED event.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
 */
bt_bool bt_avrcp_register_notification(bt_avrcp_channel_t* channel, bt_byte event_id, 
									   bt_ulong playback_interval);

/**
 * \brief Register notifications.
 * \ingroup avrcp
 *
 * \details This function registers with the target to receive notifications 
 *          asynchronously based on specific events occurring. This function is used
 *          to register multiple notifications with one call. 
 *
 * \note    This function cannot be used to register for AVC_EVENT_PLAYBACK_POS_CHANGED event.
 *
 * \param channel AVRCP channel.
 * \param event_mask Bitmask that specifies which events to register for. This value can be a combination of the following values:
 *                           \li AVC_EVENT_FLAG_PLAYBACK_STATUS_CHANGED
 *                           \li AVC_EVENT_FLAG_TRACK_CHANGED
 *                           \li AVC_EVENT_FLAG_TRACK_REACHED_END
 *                           \li AVC_EVENT_FLAG_TRACK_REACHED_START
 *                           \li AVC_EVENT_FLAG_BATT_STATUS_CHANGED
 *                           \li AVC_EVENT_FLAG_SYSTEM_STATUS_CHANGED
 *                           \li AVC_EVENT_FLAG_PLAYER_APPLICATION_SETTING_CHANGED
 *                           \li AVC_EVENT_FLAG_NOW_PLAYING_CONTENT_CHANGED
 *                           \li AVC_EVENT_FLAG_AVAILABLE_PLAYERS_CHANGED
 *                           \li AVC_EVENT_FLAG_ADDRESSED_PLAYER_CHANGED
 *                           \li AVC_EVENT_FLAG_UIDS_CHANGED
 *                           \li AVC_EVENT_FLAG_VOLUME_CHANGED
 *                           \li AVC_EVENT_FLAG_ALL
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
 */
bt_bool bt_avrcp_register_notifications(bt_avrcp_channel_t* channel, bt_uint event_mask); 

/**
 * \brief Set absolute volume.
 * \ingroup avrcp
 *
 * \details This function is used to set an absolute volume to be used by the rendering device.
 *
 * \param channel AVRCP channel.
 * \param volume Volume
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
 */
bt_bool bt_avrcp_set_absolute_volume(bt_avrcp_channel_t* channel, bt_byte volume);

/**
 * \brief Set addressed player.
 * \ingroup avrcp
 *
 * \details This function is used to inform the target of which media player the controller wishes to control.
 *
 * \param channel AVRCP channel.
 * \param player_id Player Id.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
 */
bt_bool bt_avrcp_set_addressed_player(bt_avrcp_channel_t* channel, bt_uint player_id);

/**
 * \brief Set player setting attribute values.
 * \ingroup avrcp
 *
 * \details This function requests to set the player application setting list of player application setting values 
 *          on the target device for the corresponding defined list of setting attributes.
 *          for the requested player application setting attribute.
 *          The list of attribute value ids is stored in the setting_value_id_list member of the \p response_buffer parameter. 
 *          The caller has to set bt_av_player_setting_values_t::setting_value_id_list to a buffer 
 *          where returned values will be stored and bt_av_player_setting_values_t::count to the number
 *          of entries in the list.
 *
 * \param channel AVRCP channel.
 * \param attr_id_list List of setting attribute ids.
 * \param attr_value_list List of setting attribute value ids.
 * \param attr_id_count The number of entries in both lists.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
 */
bt_bool bt_avrcp_set_player_application_setting_value(
	bt_avrcp_channel_t* channel, bt_byte* attr_id_list, 
	bt_byte* attr_value_list, bt_byte attr_id_count);

/**
* \brief Find Targets
* \ingroup avrcp
*
* \details This function looks for targets on nearby devices.
*          The AVRCP_EVT_SEARCH_COMPLETED event is generated when the search has completed.
*
* \param search_length The amount of time the search will be performed for.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise. the callback is not called in this case.
*/
bt_bool bt_avrcp_find_targets(bt_byte search_length);

/**
* \brief Cancel finding Targets
* \ingroup avrcp
*
* \details This function stops AVRCP layer looking for targets on nearby devices.
*          As a result of this operation the AVRCP_EVT_SEARCH_COMPLETED event will be generated.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise. the callback is not called in this case.
*/
bt_bool bt_avrcp_cancel_find(void);

/**
* \brief Find Target
* \ingroup avrcp
*
* \details This function looks for a target on a remote device specified by \p deviceAddress.
*
* \param deviceAddress The address of a remote device.
* \param callback The callback function that will be called when search has completed.
* \param client_callback The optional callback function that an application can set if it wants to be notified of state changes of the SDP client.
*                        The \c evt parameter of the callback can be one of the following values:
*                        \li SDP_CLIENT_STATE_IDLE
*                        \li SDP_CLIENT_STATE_CONNECTING
*                        \li SDP_CLIENT_STATE_DISCONNECTING
*                        \li SDP_CLIENT_STATE_CONNECTED
* \param callback_param A pointer to arbitrary data to be passed to the \p callback and \p client_callback callbacks.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise. the callback is not called in this case.
*/
bt_bool bt_avrcp_find_target(
	bt_bdaddr_t* deviceAddress, 
	bt_avrcp_find_callback_fp callback, 
	bt_sdp_client_callback_fp client_callback,
	void* callback_param);

/**
* \brief Find Controller
* \ingroup avrcp
*
* \details This function looks for a controller on a remote device specified by \p deviceAddress.
*
* \param deviceAddress The address of a remote device.
* \param callback The callback function that will be called when search has completed.
* \param client_callback The optional callback function that an application can set if it wants to be notified of state changes of the SDP client.
*                        The \c evt parameter of the callback can be one of the following values:
*                        \li SDP_CLIENT_STATE_IDLE
*                        \li SDP_CLIENT_STATE_CONNECTING
*                        \li SDP_CLIENT_STATE_DISCONNECTING
*                        \li SDP_CLIENT_STATE_CONNECTED
* \param callback_param A pointer to arbitrary data to be passed to the \p callback and \p client_callback callbacks.
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise. the callback is not called in this case.
*/
bt_bool bt_avrcp_find_controller(
	bt_bdaddr_t* deviceAddress, 
	bt_avrcp_find_callback_fp callback, 
	bt_sdp_client_callback_fp client_callback,
	void* callback_param);

// local target control
/**
* \brief Set playback status
* \ingroup avrcp
*
* \details This function is used to set the playback status when AVRCP is running in target mode.
*          If there are active connections with controllers, the ones that registered 
*          for AVC_EVENT_PLAYBACK_STATUS_CHANGED event will be notified.
*
* \param song_length The length of the current track.
* \param song_position The position of the current track.
* \param play_status Playback status. This value can be one of the following values:
*        \li AVC_PLAY_STATUS_STOPPED
*        \li AVC_PLAY_STATUS_PLAYING
*        \li AVC_PLAY_STATUS_PAUSED
*        \li AVC_PLAY_STATUS_FW_SEEK
*        \li AVC_PLAY_STATUS_REV_SEEK
*/
void bt_avrcp_tg_set_play_status(bt_ulong song_length, bt_ulong song_position, bt_byte play_status);

/**
* \brief Set current track
* \ingroup avrcp
*
* \details This function is used to set the current track when AVRCP is running in target mode.
*          If there are active connections with controllers, the ones that registered 
*          for AVC_EVENT_TRACK_CHANGED event will be notified.
*
* \param track_id Track Id
* \param song_length The length of the current track.
* \param song_position The position of the current track.
*/
void bt_avrcp_tg_set_current_track(bt_av_element_id_t* track_id, bt_ulong song_length, bt_ulong song_position);

/**
* \brief Set absolute volume
* \ingroup avrcp
*
* \details This function is used to set the absolute volume when AVRCP is running in target mode.
*          If there are active connections with controllers, the ones that registered 
*          for AVC_EVENT_VOLUME_CHANGED event will be notified.
*
* \param track_id Track Id
* \param song_length The length of the current track.
* \param song_position The position of the current track.
*/
void bt_avrcp_tg_set_absolute_volume(bt_byte volume);

void bt_avrcp_tg_set_channel_absolute_volume(bt_avrcp_channel_t* channel, bt_byte volume);

/**
* \brief Set battery status
* \ingroup avrcp
*
* \details This function is used to set the battery status when AVRCP is running in target mode.
*          If there are active connections with controllers, the ones that registered 
*          for AVC_EVENT_BATT_STATUS_CHANGED event will be notified.
*
* \param status Battery status
*/
void bt_avrcp_tg_set_battery_status(bt_byte status);

/**
* \brief Set system status
* \ingroup avrcp
*
* \details This function is used to set the system status when AVRCP is running in target mode.
*          If there are active connections with controllers, the ones that registered 
*          for AVC_EVENT_SYSTEM_STATUS_CHANGED event will be notified.
*
* \param status Battery status
*/
void bt_avrcp_tg_set_system_status(bt_byte status);

/**
* \brief Send media element attributes
* \ingroup avrcp
*
* \details This function is used to send the system status when AVRCP is running in target mode.
*          If there are active connections with controllers, the ones that registered 
*          for AVC_EVENT_SYSTEM_STATUS_CHANGED event will be notified.
*
* \param status Battery status
*/
bt_bool bt_avrcp_tg_send_element_attributes(
	bt_avrcp_channel_t* channel, bt_byte tran_id,
	bt_av_element_attribute_t* attrs, bt_byte attr_count);

#ifdef __cplusplus
}
#endif

#include "cdbt/avrcp/avrcp_private.h"

#endif // __AVRCP_H
