/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __AVRCP_COMMAND_H
#define __AVRCP_COMMAND_H

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup avrcp
* @{
*/
/** @name Command types
*
*/
/**@{
*/
#define AVC_CTYPE_CONTROL            0
#define AVC_CTYPE_STATUS             1
#define AVC_CTYPE_SPECIFIC_IQUIRY    2
#define AVC_CTYPE_NOTIFY             3
#define AVC_CTYPE_GENERAL_INQUORY    4
/**@}*/ 
/**@}*/

/** \addtogroup avrcp
* @{
*/
/** @name Response types
*
*/
/**@{
*/
#define AVC_RESPONSE_NOT_IMPLEMENTED    0x8
#define AVC_RESPONSE_ACCEPTED           0x9
#define AVC_RESPONSE_REJECTED           0xA
#define AVC_RESPONSE_IN_TRANSITION      0xB
#define AVC_RESPONSE_STABLE             0xC
#define AVC_RESPONSE_IMPLEMENTED        0xC
#define AVC_RESPONSE_CHANGED            0xD
#define AVC_RESPONSE_INTERIM            0xF
#define AVC_RESPONSE_TIMEOUT            0xF0
/**@}*/ 
/**@}*/


/** \addtogroup avrcp
* @{
*/
/** @name Subunit types
*
*/
/**@{
*/
#define AVC_SUBUNIT_TYPE_MONITOR                  0x00
#define AVC_SUBUNIT_TYPE_AUDIO                    0x01
#define AVC_SUBUNIT_TYPE_PRINTER                  0x02
#define AVC_SUBUNIT_TYPE_DISC                     0x03
#define AVC_SUBUNIT_TYPE_TAPE_RECORDER_PLAYER     0x04
#define AVC_SUBUNIT_TYPE_TUNER                    0x05
#define AVC_SUBUNIT_TYPE_CA                       0x06
#define AVC_SUBUNIT_TYPE_CAMERA                   0x07
#define AVC_SUBUNIT_TYPE_PANEL                    0x09
#define AVC_SUBUNIT_TYPE_BULLETIN_BOARD           0x0A
#define AVC_SUBUNIT_TYPE_CAMERA_STORAGE           0x0B
#define AVC_SUBUNIT_TYPE_VENDOR_UNIQUE            0x1C
#define AVC_SUBUNIT_TYPE_EXTENDED_TO_NEXT_BYTE    0x1E
#define AVC_SUBUNIT_TYPE_UNIT                     0x1F
/**@}*/ 
/**@}*/

#define AVC_SUBUNIT_ID_EXTENDED_TO_NEXT_BYTE      0x5
#define AVC_SUBUNIT_ID_IGNORE                     0x7

#define AVC_CMD_GENERAL_POWER                     0xB2
#define AVC_CMD_GENERAL_UNIT_INFO                 0x30
#define AVC_CMD_GENERAL_SUBUNIT_INFO              0x31
#define AVC_CMD_GENERAL_RESERVE                   0x01
#define AVC_CMD_GENERAL_VERSION                   0xB0
#define AVC_CMD_GENERAL_VENDOR_DEPENDENT          0x00

#define AVC_CMD_PANEL_PASS_THROUGH                0x7C

/** \addtogroup avrcp
* @{
*/
/** @name AV/C Panel PASS THROUGH operation IDs
*
*/
/**@{
*/
#define AVC_PANEL_OPID_SELECT                          0x00
#define AVC_PANEL_OPID_UP                              0x01
#define AVC_PANEL_OPID_DOWN                            0x02
#define AVC_PANEL_OPID_LEFT                            0x03
#define AVC_PANEL_OPID_RIGHT                           0x04
#define AVC_PANEL_OPID_RIGHT_UP                        0x05
#define AVC_PANEL_OPID_RIGHT_DOWN                      0x06
#define AVC_PANEL_OPID_LEFT_UP                         0x07
#define AVC_PANEL_OPID_LEFT_DOWN                       0x08
#define AVC_PANEL_OPID_ROOT_MENU                       0x09
#define AVC_PANEL_OPID_SETUP_MENU                      0x0A
#define AVC_PANEL_OPID_CONTENTS_MENU                   0x0B
#define AVC_PANEL_OPID_FAVORITE_MENU                   0x0C
#define AVC_PANEL_OPID_EXIT                            0x0D
#define AVC_PANEL_OPID_ON_DEMAND_MENU                  0x0E
#define AVC_PANEL_OPID_APPS_MENU                       0x0F
/*
#define AVC_PANEL_OPID_RESERVER                        0x10
#define AVC_PANEL_OPID_RESERVER                        0x11
#define AVC_PANEL_OPID_RESERVER                        0x12
#define AVC_PANEL_OPID_RESERVER                        0x13
#define AVC_PANEL_OPID_RESERVER                        0x14
#define AVC_PANEL_OPID_RESERVER                        0x15
#define AVC_PANEL_OPID_RESERVER                        0x16
#define AVC_PANEL_OPID_RESERVER                        0x17
#define AVC_PANEL_OPID_RESERVER                        0x18
#define AVC_PANEL_OPID_RESERVER                        0x19
#define AVC_PANEL_OPID_RESERVER                        0x1A
#define AVC_PANEL_OPID_RESERVER                        0x1B
#define AVC_PANEL_OPID_RESERVER                        0x1C
#define AVC_PANEL_OPID_RESERVER                        0x1D
#define AVC_PANEL_OPID_RESERVER                        0x1E
#define AVC_PANEL_OPID_RESERVER                        0x1F
*/
#define AVC_PANEL_OPID_0                               0x20
#define AVC_PANEL_OPID_1                               0x21
#define AVC_PANEL_OPID_2                               0x22
#define AVC_PANEL_OPID_3                               0x23
#define AVC_PANEL_OPID_4                               0x24
#define AVC_PANEL_OPID_5                               0x25
#define AVC_PANEL_OPID_6                               0x26
#define AVC_PANEL_OPID_7                               0x27
#define AVC_PANEL_OPID_8                               0x28
#define AVC_PANEL_OPID_9                               0x29
#define AVC_PANEL_OPID_DOT                             0x2A
#define AVC_PANEL_OPID_ENTER                           0x2B
#define AVC_PANEL_OPID_CLEAR                           0x2C
/*
#define AVC_PANEL_OPID_RESRVED                         0x2D
#define AVC_PANEL_OPID_RESRVED                         0x2E
#define AVC_PANEL_OPID_RESRVED                         0x2F
*/
#define AVC_PANEL_OPID_CHANNEL_UP                      0x30
#define AVC_PANEL_OPID_CHANNEL_DOWN                    0x31
#define AVC_PANEL_OPID_PREVIOUS_CHANNEL                0x32
#define AVC_PANEL_OPID_SOUND_SELECT                    0x33
#define AVC_PANEL_OPID_INPUT_SELECT                    0x34
#define AVC_PANEL_OPID_DISPLAY_INFORMATION             0x35
#define AVC_PANEL_OPID_HELP                            0x36
#define AVC_PANEL_OPID_PAGE_UP                         0x37
#define AVC_PANEL_OPID_PAGE_DOWN                       0x38
#define AVC_PANEL_OPID_LIVE_TV                         0x39
#define AVC_PANEL_OPID_ZOOM                            0x3A
#define AVC_PANEL_OPID_LOCK                            0x3B
#define AVC_PANEL_OPID_SKIP                            0x3C
#define AVC_PANEL_OPID_NEXT_DAY                        0x3D
#define AVC_PANEL_OPID_PREVIOUS_DAY                    0x3E
#define AVC_PANEL_OPID_LINKED_CONTENT                  0x3F
#define AVC_PANEL_OPID_POWER_TOGGLE                    0x40
#define AVC_PANEL_OPID_VOLUME_UP                       0x41
#define AVC_PANEL_OPID_VOLUME_DOWN                     0x42
#define AVC_PANEL_OPID_MUTE                            0x43
#define AVC_PANEL_OPID_PLAY                            0x44
#define AVC_PANEL_OPID_STOP                            0x45
#define AVC_PANEL_OPID_PAUSE                           0x46
#define AVC_PANEL_OPID_RECORD                          0x47
#define AVC_PANEL_OPID_REWIND                          0x48
#define AVC_PANEL_OPID_FAST_FORWARD                    0x49
#define AVC_PANEL_OPID_EJECT                           0x4A
#define AVC_PANEL_OPID_FORWARD                         0x4B
#define AVC_PANEL_OPID_BACKWARD                        0x4C
#define AVC_PANEL_OPID_LIST                            0x4D
/*
#define AVC_PANEL_OPID_RESERVED                        0x4E
#define AVC_PANEL_OPID_RESERVED                        0x4F
*/
#define AVC_PANEL_OPID_ANGLE                           0x50
#define AVC_PANEL_OPID_SUBPICTURE                      0x51
#define AVC_PANEL_OPID_PIP_MOVE                        0x52
#define AVC_PANEL_OPID_PIP_DOWN                        0x53
#define AVC_PANEL_OPID_PIP_UP                          0x54
#define AVC_PANEL_OPID_RF_BYPASS                       0x55
/*
#define AVC_PANEL_OPID_RESERVED                        0x56
#define AVC_PANEL_OPID_RESERVED                        0x57
#define AVC_PANEL_OPID_RESERVED                        0x58
#define AVC_PANEL_OPID_RESERVED                        0x5A
#define AVC_PANEL_OPID_RESERVED                        0x5B
#define AVC_PANEL_OPID_RESERVED                        0x5C
#define AVC_PANEL_OPID_RESERVED                        0x5D
#define AVC_PANEL_OPID_RESERVED                        0x5E
#define AVC_PANEL_OPID_RESERVED                        0x5F
*/
#define AVC_PANEL_OPID_PLAY_FUNCTION                   0x60
#define AVC_PANEL_OPID_PAUSE_PLAY_FUNCTION             0x61
#define AVC_PANEL_OPID_RECORD_FUNCTION                 0x62
#define AVC_PANEL_OPID_PAUSE_RECORD_FUNCTION           0x63
#define AVC_PANEL_OPID_STOP_FUNCTION                   0x64
#define AVC_PANEL_OPID_MUTE_FUNCTION                   0x65
#define AVC_PANEL_OPID_RESTORE_FOLUME_FUNCTION         0x66
#define AVC_PANEL_OPID_TUNE_FUNCTION                   0x67
#define AVC_PANEL_OPID_SELECT_DISK_FUNCTION            0x68
#define AVC_PANEL_OPID_SELECT_AV_INPUT_FUNCTION        0x69
#define AVC_PANEL_OPID_SELECT_AUDIO_INPUT_FUNCTION     0x6A
#define AVC_PANEL_OPID_POWER_STATE_FUNCTION            0x6B
#define AVC_PANEL_OPID_KEYBORD_FUNCTION                0x6C
/*
#define AVC_PANEL_OPID_RESERVED                        0x6D
#define AVC_PANEL_OPID_RESERVED                        0x6E
#define AVC_PANEL_OPID_RESERVED                        0x6F
#define AVC_PANEL_OPID_RESERVED                        0x70
*/
#define AVC_PANEL_OPID_F1                              0x71
#define AVC_PANEL_OPID_F2                              0x72
#define AVC_PANEL_OPID_F3                              0x73
#define AVC_PANEL_OPID_F4                              0x74
#define AVC_PANEL_OPID_F5                              0x75
#define AVC_PANEL_OPID_F6                              0x76
#define AVC_PANEL_OPID_F7                              0x77
#define AVC_PANEL_OPID_F8                              0x78
#define AVC_PANEL_OPID_F9                              0x79
#define AVC_PANEL_OPID_A                               0x7A
#define AVC_PANEL_OPID_B                               0x7B
#define AVC_PANEL_OPID_C                               0x7C
#define AVC_PANEL_OPID_D                               0x7D
#define AVC_PANEL_OPID_VENDOR_UNIQUE                   0x7E
/**@}*/ 
/**@}*/

#define AVC_PANEL_BUTTON_PRESSED    0
#define AVC_PANEL_BUTTON_RELEASED   1

#define AVC_FLAG_PANEL_CLICK        1
#define AVC_FLAG_BROWSING_CMD       2
#define AVC_FLAG_RESPONSE           4

#define AVC_PDUID_GETCAPABILITIES                               0x10
#define AVC_PDUID_LIST_PLAYER_APPLICATION_SETTING_ATTRIBUTES    0x11
#define AVC_PDUID_LIST_PLAYER_APPLICATION_SETTING_VALUES        0x12
#define AVC_PDUID_GET_CURRENT_PLAYER_APPLICATION_SETTING_VALUE  0x13
#define AVC_PDUID_SET_PLAYER_APPLICATION_SETTING_VALUE          0x14
#define AVC_PDUID_GET_PLAYER_APPLICATION_SETTING_ATTRIBUTE_TEXT 0x15
#define AVC_PDUID_GET_PLAYER_APPLICATION_SETTING_VALUE_TEXT     0x16
#define AVC_PDUID_INFORM_DISPLAYABLE_CHARACTER_SET              0x17
#define AVC_PDUID_INFORM_BATTERY_STATUS_OF_CT                   0x18
#define AVC_PDUID_GET_ELEMENT_ATTRIBUTES                        0x20
#define AVC_PDUID_GET_PLAY_STATUS                               0x30
#define AVC_PDUID_REGISTER_NOTIFICATION                         0x31
#define AVC_PDUID_REQUEST_CONTINUING_RESPONSE                   0x40
#define AVC_PDUID_ABORT_CONTINUING_RESPONSE                     0x41
#define AVC_PDUID_SET_ABSOLUTE_VOLUME                           0x50
#define AVC_PDUID_SET_ADDRESSED_PLAYER                          0x60
#define AVC_PDUID_GET_FOLDER_ITEMS                              0x71
#define AVC_PDUID_SET_BROWSED_PLAYER                            0x70
#define AVC_PDUID_CHANGE_PATH                                   0x72
#define AVC_PDUID_GET_ITEM_ATTRIBUTES                           0x73
#define AVC_PDUID_PLAY_ITEM                                     0x74
#define AVC_PDUID_SEARCH                                        0x80
#define AVC_PDUID_ADD_TO_NOW_PLAYING                            0x90
#define AVC_PDUID_GENERAL_REJECT                                0xa0

#define AVC_CAPABILITY_COMPANY_ID          2
#define AVC_CAPABILITY_EVENTS_SUPPORTED    3

/** \addtogroup avrcp
* @{
*/
/** @name Battery status
*
*/
/**@{
*/
#define AVC_BATTERY_STATUS_NORMAL          0
#define AVC_BATTERY_STATUS_WARNING         1
#define AVC_BATTERY_STATUS_CRITICAL        2
#define AVC_BATTERY_STATUS_EXTERNAL        3
#define AVC_BATTERY_STATUS_FULL_CHARGE     4
/**@}*/ 
/**@}*/

/** \addtogroup avrcp
* @{
*/
/** @name Media attribute IDs
*
*/
/**@{
*/
#define AVC_MEDIA_ATTR_ID_TITLE           1
#define AVC_MEDIA_ATTR_ID_ARTIST          2
#define AVC_MEDIA_ATTR_ID_ALBUM           3
#define AVC_MEDIA_ATTR_ID_NUMBER          4
#define AVC_MEDIA_ATTR_ID_TOTAL_NUMBER    5
#define AVC_MEDIA_ATTR_ID_GENRE           6
#define AVC_MEDIA_ATTR_ID_PLAYING_TIME    7
/**@}*/ 
/**@}*/

/** \addtogroup avrcp
* @{
*/
/** @name Media attribute bitmask
*
*/
/**@{
*/
#define AVC_MEDIA_ATTR_FLAG_TITLE         0x01
#define AVC_MEDIA_ATTR_FLAG_ARTIST        0x02
#define AVC_MEDIA_ATTR_FLAG_ALBUM         0x04
#define AVC_MEDIA_ATTR_FLAG_NUMBER        0x08
#define AVC_MEDIA_ATTR_FLAG_TOTAL_NUMBER  0x10
#define AVC_MEDIA_ATTR_FLAG_GENRE         0x20
#define AVC_MEDIA_ATTR_FLAG_PLAYING_TIME  0x40
#define AVC_MEDIA_ATTR_FLAG_ALL           0x7F
/**@}*/ 
/**@}*/

/** \addtogroup avrcp
* @{
*/
/** @name Play status
*
*/
/**@{
*/
#define AVC_PLAY_STATUS_STOPPED     0
#define AVC_PLAY_STATUS_PLAYING     1
#define AVC_PLAY_STATUS_PAUSED      2
#define AVC_PLAY_STATUS_FW_SEEK     3
#define AVC_PLAY_STATUS_REV_SEEK    4
#define AVC_PLAY_STATUS_ERROR       0xFF
/**@}*/ 
/**@}*/

/** \addtogroup avrcp
* @{
*/
/** @name Notifications
*
*/
/**@{
*/
#define AVC_EVENT_PLAYBACK_STATUS_CHANGED            0x01 ///< Change in playback status of the current track.
#define AVC_EVENT_TRACK_CHANGED                      0x02 ///< Change of current track
#define AVC_EVENT_TRACK_REACHED_END                  0x03 ///< Reached end of a track
#define AVC_EVENT_TRACK_REACHED_START                0x04 ///< Reached start of a track
#define AVC_EVENT_PLAYBACK_POS_CHANGED               0x05 ///< Change in playback position. Returned after the specified playback notification change notification interval
#define AVC_EVENT_BATT_STATUS_CHANGED                0x06 ///< Change in battery status
#define AVC_EVENT_SYSTEM_STATUS_CHANGED              0x07 ///< Change in system status
#define AVC_EVENT_PLAYER_APPLICATION_SETTING_CHANGED 0x08 ///< Change in player application setting
#define AVC_EVENT_NOW_PLAYING_CONTENT_CHANGED        0x09 ///< The content of the Now Playing list has changed, see 6.9.5.
#define AVC_EVENT_AVAILABLE_PLAYERS_CHANGED          0x0a ///< The available players have changed, see 6.9
#define AVC_EVENT_ADDRESSED_PLAYER_CHANGED           0x0b ///< The Addressed Player has been changed, see 6.9.2.
#define AVC_EVENT_UIDS_CHANGED                       0x0c ///< The UIDs have changed, see 6.10.3.3.
#define AVC_EVENT_VOLUME_CHANGED                     0x0d ///< The volume has been changed locally on the TG, see 6.13.3
/**@}*/ 
/**@}*/
#define AVC_MAXEVENTS                                0x0d

/** \addtogroup avrcp
* @{
*/
/** @name Notifications mask
*
*/
/**@{
*/
#define AVC_EVENT_FLAG_PLAYBACK_STATUS_CHANGED            0x0001
#define AVC_EVENT_FLAG_TRACK_CHANGED                      0x0002
#define AVC_EVENT_FLAG_TRACK_REACHED_END                  0x0004
#define AVC_EVENT_FLAG_TRACK_REACHED_START                0x0008
#define AVC_EVENT_FLAG_PLAYBACK_POS_CHANGED               0x0010
#define AVC_EVENT_FLAG_BATT_STATUS_CHANGED                0x0020
#define AVC_EVENT_FLAG_SYSTEM_STATUS_CHANGED              0x0040
#define AVC_EVENT_FLAG_PLAYER_APPLICATION_SETTING_CHANGED 0x0080
#define AVC_EVENT_FLAG_NOW_PLAYING_CONTENT_CHANGED        0x0100
#define AVC_EVENT_FLAG_AVAILABLE_PLAYERS_CHANGED          0x0200
#define AVC_EVENT_FLAG_ADDRESSED_PLAYER_CHANGED           0x0400
#define AVC_EVENT_FLAG_UIDS_CHANGED                       0x0800
#define AVC_EVENT_FLAG_VOLUME_CHANGED                     0x1000
#define AVC_EVENT_FLAG_ALL                                0x1FFF
/**@}*/ 
/**@}*/

/** \addtogroup avrcp
* @{
*/
/** @name Media navigation scope
*
*/
/**@{
*/
//                                                  Valid Browseable                 Items Description                       Applicable Player
#define AVC_SCOPE_MEDIA_PLAYER_LIST         0x00 // Media Player Item                Contains all available media players    None
#define AVC_MEDIA_PLAYER_VIRTUAL_FILESYSTEM 0x01 // Folder Item, Media Element Item  The virtual filesystem containing the   Browsed
                                                 //                                  media content of the browsed player
#define AVC_SEARCH                          0x02 // Media Element Item               The results of a search operation       Browsed
                                                 //                                  on the browsed player
#define AVC_NOW_PLAYING                     0x03 // Media Element Item               The Now Playing list (or queue)         Addressed
                                                 //                                  of the addressed player
/**@}*/ 
/**@}*/

#define AVRCP_COMMAND_TYPE_CONTROL     0
#define AVRCP_COMMAND_TYPE_BROWSING    1

#define AVC_PLAYER_SETTING_EQUALIZER_STATUS      1
#define AVC_PLAYER_SETTING_REPEAT_MODE_STATUS    2
#define AVC_PLAYER_SETTING_SUFFLE_STATUS         3
#define AVC_PLAYER_SETTING_SCAN_STATUS           4

#define AVC_PLAYER_SETTING_EQUALIZER_OFF         1
#define AVC_PLAYER_SETTING_EQUALIZER_ON          2

#define AVC_PLAYER_SETTING_REPEAT_MODE_OFF       1
#define AVC_PLAYER_SETTING_REPEAT_SINGLE_TRACK   2
#define AVC_PLAYER_SETTING_REPEAT_ALL_TRACKS     3
#define AVC_PLAYER_SETTING_REPEAT_GROUP          4		

#define AVC_PLAYER_SETTING_SUFFLE_OFF            1
#define AVC_PLAYER_SETTING_SUFFLE_ALL_TRACKS     2
#define AVC_PLAYER_SETTING_SUFFLE_GROUP          3

#define AVC_PLAYER_SETTING_SCAN_OFF              1
#define AVC_PLAYER_SETTING_SCAN_ALL_TRACKS       2
#define AVC_PLAYER_SETTING_SCAN_GROUP            3

#define AVC_VOLUME_MIN    0 // 0%
#define AVC_VOLUME_MAX    0x7F // 100%

#define AVC_PACKET_TYPE_SINGLE      0
#define AVC_PACKET_TYPE_START       1
#define AVC_PACKET_TYPE_CONTINUE    2
#define AVC_PACKET_TYPE_END         3

typedef struct _bt_av_command_t bt_av_command_t;

/**
* \brief  AV/C response header
* \ingroup avrcp
*
* \details This structure is used to store fields present in every AV/C response.
*/
typedef struct _bt_av_response_t
{
	bt_avrcp_channel_t* channel;    ///< AVRCP channel
	bt_byte ctype;                  ///< Response type. This can be one of the following values:
                                    ///< \li AVC_RESPONSE_NOT_IMPLEMENTED
                                    ///< \li AVC_RESPONSE_ACCEPTED
                                    ///< \li AVC_RESPONSE_REJECTED
                                    ///< \li AVC_RESPONSE_IN_TRANSITION
                                    ///< \li AVC_RESPONSE_STABLE
                                    ///< \li AVC_RESPONSE_IMPLEMENTED
                                    ///< \li AVC_RESPONSE_CHANGED
                                    ///< \li AVC_RESPONSE_INTERIM
                                    ///< \li AVC_RESPONSE_TIMEOUT
	bt_byte tran_id;                ///< Transaction Id.
} bt_av_response_t;

struct _bt_av_command_t
{
	bt_av_command_t* next_command;

	bt_byte cmd_type;
	bt_byte flags;
	bt_byte ctype;
	bt_byte subunit_type;
	bt_byte subunit_id;
	bt_byte opcode;
	bt_byte pdu_id;
	bt_byte* parameters;
	bt_uint params_len;
	bt_av_response_t* response_buffer;
	bt_long send_time;
	bt_byte tran_id;
};

/**
* \brief  Media element UID
* \ingroup avrcp
*
* \details This structure is used to store media element UID.
*/
typedef struct _bt_av_element_id_s
{
	bt_ulong id_lo;    ///< 4 least significant bytes of UID.
	bt_ulong id_hi;    ///< 4 most significant bytes of UID.
} bt_av_element_id_t;

/**
* \brief  Parameter to AVRCP_EVT_COMPANY_ID_LIST_RECEIVED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::company_id -
*          when a local device received a response to a "get company id" request.
*/
typedef struct _bt_av_capability_company_id_s
{
	bt_av_response_t header;      ///< Common response header.
	bt_byte count;                ///< The number of supported company ids.
	bt_ulong* company_id_list;    ///< List of supported company ids.
} bt_av_capability_company_id_t;

/**
* \brief  Parameter to AVRCP_EVT_EVENT_ID_LIST_RECEIVED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::supported_event_id -
*          when a local device received a response to a "get supported events" request.
*/
typedef struct _bt_av_capability_event_id_s
{
	bt_av_response_t header;     ///< Common response header.
	bt_byte count;               ///< The number of supported events ids.
	bt_byte* event_id_list;      ///< List of supported event ids.
} bt_av_capability_event_id_t;


/**
* \brief  Parameter to AVRCP_EVT_PLAYER_SETTING_ATTRIBUTES_RECEIVED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::player_settings -
*          when a local device received a response to a "get supported player setting attributes" request.
*/
typedef struct _bt_av_player_settings_s
{
	bt_av_response_t header;     ///< Common response header.
	bt_byte count;               ///< The number of supported player setting attribute ids.
	bt_byte* setting_id_list;    ///< List of supported player setting attribute ids.
} bt_av_player_settings_t;

/**
* \brief  Parameter to AVRCP_EVT_PLAYER_SETTING_VALUES_RECEIVED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::player_setting_values -
*          when a local device received a response to a "get player setting attribute values" request.
*/
typedef struct _bt_av_player_setting_values_s
{
	bt_av_response_t header;         ///< Common response header.
	bt_byte count;					 ///< The number of supported player setting attribute value ids.
	bt_byte* setting_value_id_list;	 ///< List of supported player setting attribute value ids.
} bt_av_player_setting_values_t;

/**
* \brief  Parameter to AVRCP_EVT_PLAYER_CURRENT_SETTING_VALUES_RECEIVED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::player_setting_current_values -
*          when a local device received a response to a "get current player setting attribute values" request.
*/
typedef struct _bt_av_player_setting_current_values_s
{
	bt_av_response_t header;          ///< Common response header.
	bt_byte count;                    ///< The number of player setting attribute ids to be returned from the target.
	bt_byte* setting_id_list;         ///< List of player setting attribute ids to be returned from the target.
	bt_byte* setting_value_id_list;   ///< List of current player setting attribute value ids.
} bt_av_player_setting_current_values_t;

typedef struct _bt_av_player_text_s
{
	bt_uint charset;
	bt_byte len;
	bt_byte* text;

} bt_av_player_text_t;

/**
* \brief  Parameter to AVRCP_EVT_PLAYER_SETTING_ATTRIBUTES_TEXT_RECEIVED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::player_settings_text -
*          when a local device received a response to a "get player setting attributes displayable text" request.
*/
typedef struct _bt_av_player_settings_text_s
{
	bt_av_response_t header;                   ///< Common response header.
	bt_byte count;                             ///< The number of player setting attribute ids for which displayable text is requested.
	bt_byte* setting_id_list;                  ///< List of player setting attribute ids for which displayable text is requested.
	bt_av_player_text_t* setting_text_list;    ///< List of player setting attributes displayable text.
} bt_av_player_settings_text_t;

/**
* \brief  Parameter to AVRCP_EVT_PLAYER_SETTING_VALUES_TEXT_RECEIVED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::player_setting_values_text -
*          when a local device received a response to a "get player setting attribute values displayable text" request.
*/
typedef struct _bt_av_player_setting_values_text_s
{
	bt_av_response_t header;                        ///< Common response header.
	bt_byte count;                                  ///< The number of player setting attribute value ids for which displayable text is requested.
	bt_byte* setting_value_id_list;                 ///< List of player setting attribute value ids for which displayable text is requested.
	bt_av_player_text_t* setting_value_text_list;   ///< List of player setting attribute values displayable text.
} bt_av_player_setting_values_text_t;

/**
* \brief  Media element attribute
* \ingroup avrcp
*
* \details This structure is used to store media element attribute.
*/
typedef struct _bt_av_element_attribute_s
{
	bt_ulong id;         ///< Attribute Id.
	bt_uint charset;     ///< Charcater set.
	bt_uint len;         ///< Value length.
	bt_byte* value;      ///< Attribute value.
} bt_av_element_attribute_t;

/**
* \brief  Parameter to AVRCP_EVT_GET_ELEMENT_ATTRIBUTES_RECEIVED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::element_attributes -
*          when a local device received a response to a "get media element attributes" request.
*/
typedef struct _bt_av_element_attributes_s
{
	bt_av_response_t header;                 ///< Common response header.
	bt_byte count;                           ///< The number of attributes returned.
	bt_av_element_attribute_t* attr_list;    ///< List of attribute values
} bt_av_element_attributes_t;

/**
* \brief  Parameter to AVRCP_EVT_GET_PLAY_STATUS_RECEIVED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::play_status -
*          when a local device received a response to a "get play status" request.
*/
typedef struct _bt_av_play_status_s
{
	bt_av_response_t header;    ///< Common response header.
	bt_ulong song_length;       ///< Current track length
	bt_ulong song_position;     ///< Current track position
	bt_byte play_status;        ///< Playback status. This can be one of the following values:
                                ///< \li AVC_PLAY_STATUS_STOPPED
                                ///< \li AVC_PLAY_STATUS_PLAYING
                                ///< \li AVC_PLAY_STATUS_PAUSED
                                ///< \li AVC_PLAY_STATUS_FW_SEEK
                                ///< \li AVC_PLAY_STATUS_REV_SEEK
                                ///< \li AVC_PLAY_STATUS_ERROR
} bt_av_play_status_t;

/**
* \brief  Parameter to AVRCP_EVT_BATTERY_STATUS_OF_CT_RECEIVED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::battery_status_of_ct -
*          when a local device received a "battery status of controller" command.
*/
typedef struct _bt_av_battery_status_of_ct_s
{
	bt_av_response_t header;    ///< Common response header.
	bt_byte status;             ///< Battery status. This can be one of the following values:
                                ///< \li AVC_BATTERY_STATUS_NORMAL
                                ///< \li AVC_BATTERY_STATUS_WARNING
                                ///< \li AVC_BATTERY_STATUS_CRITICAL
                                ///< \li AVC_BATTERY_STATUS_EXTERNAL
                                ///< \li AVC_BATTERY_STATUS_FULL_CHARGE

} bt_av_battery_status_of_ct_t;

/**
* \brief  Parameter to AVRCP_EVT_DISPLAYABLE_CHARACTER_SET_RECEIVED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::displayable_character_set -
*          when a local device received a "displayable chracter set command" request.
*/
typedef struct _bt_av_displayable_character_set_s
{
	bt_av_response_t header;    ///< Common response header.
	bt_byte count;              ///< The number of supported characters sets by the controller.
	bt_uint* charset_list;      ///< List of supported characters sets by the controller.
} bt_av_displayable_character_set_t;

/**
* \brief  Parameter to AVRCP_EVT_PLAYBACK_STATUS_CHANGED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::notification::params:play_status -
*          when a local device received a "play status changed" notification. 
*/
typedef struct _bt_av_notification_playback_status_changed_s
{
	bt_av_response_t header;    ///< Common response header.
	bt_byte status;             ///< Play status. This can be on of the following values:
                                ///< \li AVC_PLAY_STATUS_STOPPED
                                ///< \li AVC_PLAY_STATUS_PLAYING
                                ///< \li AVC_PLAY_STATUS_PAUSED
                                ///< \li AVC_PLAY_STATUS_FW_SEEK
                                ///< \li AVC_PLAY_STATUS_REV_SEEK
                                ///< \li AVC_PLAY_STATUS_ERROR
} bt_av_notification_playback_status_changed_t;

/**
* \brief  Parameter to AVRCP_EVT_TRACK_CHANGED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::notification::params:track -
*          when a local device received a "track changed" notification. 
*/
typedef struct _bt_av_notification_track_changed_s
{
	bt_av_element_id_t id;     ///< New track UID
} bt_av_notification_track_changed_t;

/**
* \brief  Parameter to AVRCP_EVT_PLAYBACK_POS_CHANGED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::notification::params:playback_pos -
*          when a local device received a "playback position changed" notification. 
*/
typedef struct _bt_av_notification_playback_pos_changed_s
{
	bt_ulong position;         ///< New playback position
} bt_av_notification_playback_pos_changed_t;

/**
* \brief  Parameter to AVRCP_EVT_BATT_STATUS_CHANGED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::notification::params:battery_status -
*          when a local device received a "battery status changed" notification. 
*/
typedef struct _bt_av_notification_battery_status_s
{
	bt_byte status;            ///< New battery status.This can be one of the following values:
                               ///< \li AVC_BATTERY_STATUS_NORMAL
                               ///< \li AVC_BATTERY_STATUS_WARNING
                               ///< \li AVC_BATTERY_STATUS_CRITICAL
                               ///< \li AVC_BATTERY_STATUS_EXTERNAL
                               ///< \li AVC_BATTERY_STATUS_FULL_CHARGE
} bt_av_notification_battery_status_t;

/**
* \brief  Parameter to AVRCP_EVT_SYSTEM_STATUS_CHANGED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::notification::params:system_status -
*          when a local device received a "system status changed" notification. 
*/
typedef struct _bt_av_notification_system_status_changed_s
{
	bt_byte status;             ///< New system status
} bt_av_notification_system_status_changed_t;

/**
* \brief  Parameter to AVRCP_EVT_ADDRESSED_PLAYER_CHANGED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::notification::params:addressed_player -
*          when a local device received a "addressed player changed" notification. 
*/
typedef struct _bt_av_notification_addressed_player_changed_s
{
	bt_uint player_id;         ///< New player Id
	bt_uint uid_counter;       ///< UID counter
} bt_av_notification_addressed_player_changed_t;

/**
* \brief  Parameter to AVRCP_EVT_UIDS_CHANGED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::notification::params:uids -
*          when a local device received a "UIDs changed" notification. 
*/
typedef struct _bt_av_notification_uids_changed_s
{
	bt_uint uid_counter;       ///< UID counter
} bt_av_notification_uids_changed_t;

/**
* \brief  Parameter to AVRCP_EVT_VOLUME_CHANGED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::notification::params:volume -
*          when a local device received a "UIDs changed" notification. 
*/
typedef struct _bt_av_notification_volume_changed_s
{
	bt_byte volume;            ///< Volume
} bt_av_notification_volume_changed_t;

typedef struct _bt_av_notification_app_setting_changed_s
{
	bt_byte setting_id;         
	bt_byte setting_value_id;

} bt_av_notification_app_setting_changed_t;

/**
* \brief  Parameter to the following events:
*             \li AVRCP_EVT_PLAYBACK_STATUS_CHANGED                    
*             \li AVRCP_EVT_TRACK_CHANGED                              
*             \li AVRCP_EVT_PLAYBACK_POS_CHANGED                       
*             \li AVRCP_EVT_BATT_STATUS_CHANGED                        
*             \li AVRCP_EVT_SYSTEM_STATUS_CHANGED                      
*             \li AVRCP_EVT_NOW_PLAYING_CONTENT_CHANGED                
*             \li AVRCP_EVT_AVAILABLE_PLAYERS_CHANGED                  
*             \li AVRCP_EVT_ADDRESSED_PLAYER_CHANGED                   
*             \li AVRCP_EVT_UIDS_CHANGED                               
*             \li AVRCP_EVT_VOLUME_CHANGED                             
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::notification -
*          when a local device received one of the following notifications from the target:
*             \li Play status changed
*             \li Track changed changed
*             \li Playback position changed
*             \li Battery status changed
*             \li System status changed
*             \li Addressed player changed
*             \li UIDs changed
*             \li Volume changed
*             \li Player application setting changed
*         The notification code defines which member of the bt_av_notification_t::params union is valid
*             @arg AVRCP_EVT_PLAYBACK_STATUS_CHANGED              \c bt_av_notification_playback_status_changed_t     play_status
*             @arg AVRCP_EVT_TRACK_CHANGED                        \c bt_av_notification_track_changed_t               track
*             @arg AVRCP_EVT_PLAYBACK_POS_CHANGED                 \c bt_av_notification_playback_pos_changed_t        playback_pos
*             @arg AVRCP_EVT_BATT_STATUS_CHANGED                  \c bt_av_notification_battery_status_t              battery_status
*             @arg AVRCP_EVT_SYSTEM_STATUS_CHANGED                \c bt_av_notification_system_status_changed_t       system_status
*             @arg AVRCP_EVT_ADDRESSED_PLAYER_CHANGED             \c bt_av_notification_addressed_player_changed_t    addressed_player               
*             @arg AVRCP_EVT_UIDS_CHANGED                         \c bt_av_notification_uids_changed_t                uids
*             @arg AVRCP_EVT_VOLUME_CHANGED                       \c bt_av_notification_volume_changed_t              volume
*             @arg AVRCP_EVT_PLAYER_APPLICATION_SETTING_CHANGED   \c bt_av_notification_app_setting_changed_t         app_setting
*/
typedef struct _bt_av_notification_s
{
	bt_av_response_t header;                                             ///< Common response header.
	union
	{
		bt_av_notification_playback_status_changed_t play_status;        ///< Valid if notification is AVRCP_EVT_PLAYBACK_STATUS_CHANGED 
		bt_av_notification_track_changed_t track;                        ///< Valid if notification is AVRCP_EVT_TRACK_CHANGED 
		bt_av_notification_playback_pos_changed_t playback_pos;          ///< Valid if notification is AVRCP_EVT_PLAYBACK_POS_CHANGED 
		bt_av_notification_battery_status_t battery_status;              ///< Valid if notification is AVRCP_EVT_BATT_STATUS_CHANGED 
		bt_av_notification_system_status_changed_t system_status;        ///< Valid if notification is AVRCP_EVT_SYSTEM_STATUS_CHANGED 
		bt_av_notification_addressed_player_changed_t addressed_player;  ///< Valid if notification is AVRCP_EVT_ADDRESSED_PLAYER_CHANGED 
		bt_av_notification_uids_changed_t uids;                          ///< Valid if notification is AVRCP_EVT_UIDS_CHANGED 
		bt_av_notification_volume_changed_t volume;                      ///< Valid if notification is AVRCP_EVT_VOLUME_CHANGED 
		bt_av_notification_app_setting_changed_t app_setting;            ///< Valid if notification is AVRCP_EVT_PLAYER_APPLICATION_SETTING_CHANGED 
	} params;
} bt_av_notification_t;

/**
* \brief  Parameter to AVRCP_EVT_SET_ABSOLUTE_VOLUME_COMPLETED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::absolute_volume -
*          when a local device received a response to a "set absolute volume" request.
*/
typedef struct _bt_av_set_absolute_volume_s
{
	bt_av_response_t header;    ///< Common response header.
	bt_byte volume;             ///< Volume
} bt_av_set_absolute_volume_t;

/**
* \brief  Parameter to AVRCP_EVT_SET_ADDRESSED_PLAYER_COMPLETED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::addressed_player -
*          when a local device received a response to a "set addressed player" request.
*/
typedef struct _bt_av_set_addressed_player_s
{
	bt_av_response_t header;    ///< Common response header.
	bt_byte status;             ///< The result of changing the addressed player.
} bt_av_set_addressed_player_t;

/**
* \brief  Parameter to AVRCP_EVT_PLAY_ITEM_COMPLETED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::play_item_status -
*          when a local device received a response to a "play item" request.
*/
typedef struct _bt_av_play_item_s
{
	bt_av_response_t header;    ///< Common response header.
	bt_byte status;             ///< The result of the request.
} bt_av_play_item_t;

/**
* \brief  Parameter to AVRCP_EVT_ADD_TO_NOW_PLAYING_COMPLETED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::add_to_now_playing_status -
*          when a local device received a response to a "add to now playing" request.
*/
typedef struct _bt_av_add_to_now_playing_s
{
	bt_av_response_t header;    ///< Common response header.
	bt_byte status;             ///< The result of the request.
} bt_av_add_to_now_playing_t;

/**
* \brief  Parameter to AVRCP_EVT_ELEMENT_ATTRIBUTES_REQUESTED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::get_element_attributes -
*          when a local device received a "get element attributes" request.
*/
typedef struct _bt_av_get_element_attributes_s
{
	bt_av_response_t header;    ///< Common response header.
	bt_av_element_id_t id;      ///< Media element UID
	bt_byte attributes;         ///< Bitmask that defines attributes requested. This can be a combination of the following values:
                                ///< \li AVC_MEDIA_ATTR_FLAG_TITLE
                                ///< \li AVC_MEDIA_ATTR_FLAG_ARTIST
                                ///< \li AVC_MEDIA_ATTR_FLAG_ALBUM
                                ///< \li AVC_MEDIA_ATTR_FLAG_NUMBER
                                ///< \li AVC_MEDIA_ATTR_FLAG_TOTAL_NUMBER
                                ///< \li AVC_MEDIA_ATTR_FLAG_GENRE
                                ///< \li AVC_MEDIA_ATTR_FLAG_PLAYING_TIME
} bt_av_get_element_attributes_t;

/**
* \brief  Parameter to AVRCP_EVT_REGISTER_NOTIFICATION_REQUESTED event
* \ingroup avrcp
*
* \details A pointer to this structure is passed to the AVRCP application callback as 
*          a valid member of the bt_avrcp_event_t union - bt_avrcp_event_t::register_notification -
*          when a local device received a "register notification" request.
*/
typedef struct _bt_av_register_notification_t
{
	bt_av_response_t header;    ///< Common response header.
	bt_byte event_id;           ///< Event Id. This can be one of the following values:
                                ///< \li AVC_EVENT_PLAYBACK_STATUS_CHANGED
                                ///< \li AVC_EVENT_TRACK_CHANGED
                                ///< \li AVC_EVENT_TRACK_REACHED_END
                                ///< \li AVC_EVENT_TRACK_REACHED_START
                                ///< \li AVC_EVENT_PLAYBACK_POS_CHANGED
                                ///< \li AVC_EVENT_BATT_STATUS_CHANGED
                                ///< \li AVC_EVENT_SYSTEM_STATUS_CHANGED
                                ///< \li AVC_EVENT_PLAYER_APPLICATION_SETTING_CHANGED
                                ///< \li AVC_EVENT_NOW_PLAYING_CONTENT_CHANGED
                                ///< \li AVC_EVENT_AVAILABLE_PLAYERS_CHANGED
                                ///< \li AVC_EVENT_ADDRESSED_PLAYER_CHANGED
                                ///< \li AVC_EVENT_UIDS_CHANGED
                                ///< \li AVC_EVENT_VOLUME_CHANGED
	bt_ulong playback_pos;      ///< Playback position. Used only if \c event_id is AVC_EVENT_PLAYBACK_POS_CHANGED

} bt_av_register_notification_t;

/**
* \brief Get unit info 
* \ingroup avrcp
*
* \details This function is used to request unit info from the target.
*
* \param channel AVRCP channel.
* 
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise.
*/
bt_bool bt_avrcp_get_unit_info(bt_avrcp_channel_t* channel);

/**
* \brief Get subunit info 
* \ingroup avrcp
*
* \details This function is used to request subunit info from the target.
*
* \param channel AVRCP channel.
* 
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise.
*/
bt_bool bt_avrcp_get_subuint_info(bt_avrcp_channel_t* channel);

/**
* \brief Send AV/C Panel Subunit PASS THROUGH command
* \ingroup avrcp
*
* \details This function is used to send AV/C Panel Subunit PASS THROUGH command to the target.
*          
* \param channel AVRCP channel.
* \param ctype Command type. This value can be on of the following values:
*              \li AVC_CTYPE_CONTROL            0
*              \li AVC_CTYPE_STATUS             1
*              \li AVC_CTYPE_SPECIFIC_IQUIRY    2
*              \li AVC_CTYPE_NOTIFY             3
*              \li AVC_CTYPE_GENERAL_INQUORY    4
* \param opid Operation Id. This value can be on of the AVC_PANEL_OPID_... constants
* \param button_state Button state. This can be on of the following values:
*              \li AVC_PANEL_BUTTON_PRESSED
*              \li AVC_PANEL_BUTTON_RELEASED
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise.
*/
bt_bool bt_avrcp_send_simple_panel_cmd(
	bt_avrcp_channel_t* channel, bt_byte ctype, 
	bt_byte opid, bt_byte button_state);

/**
* \brief Send AV/C Panel Subunit "pressed" PASS THROUGH command
* \ingroup avrcp
*
* \details This function is used to send AV/C Panel Subunit PASS THROUGH command
*          with button state set to AVC_PANEL_BUTTON_PRESSED.
*          
* \param channel AVRCP channel.
* \param opid Operation Id. This value can be on of the AVC_PANEL_OPID_... constants
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise.
*/
bt_bool bt_avrcp_send_press_panel_control(bt_avrcp_channel_t* channel, bt_byte opid);

/**
* \brief Send AV/C Panel Subunit "released" PASS THROUGH command
* \ingroup avrcp
*
* \details This function is used to send AV/C Panel Subunit PASS THROUGH command
*          with button state set to AVC_PANEL_BUTTON_RELEASED.
*          
* \param channel AVRCP channel.
* \param opid Operation Id. This value can be on of the AVC_PANEL_OPID_... constants
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise.
*/
bt_bool bt_avrcp_send_release_panel_control(bt_avrcp_channel_t* channel, bt_byte opid);

/**
* \brief Send AV/C Panel Subunit "control" PASS THROUGH command
* \ingroup avrcp
*
* \details This function is used to send AV/C Panel Subunit PASS THROUGH command 
*          with command type set to AVC_CTYPE_CONTROL.
*          
* \param channel AVRCP channel.
* \param opid Operation Id. This value can be on of the AVC_PANEL_OPID_... constants
* \param button_state Button state. This can be on of the following values:
*              \li AVC_PANEL_BUTTON_PRESSED
*              \li AVC_PANEL_BUTTON_RELEASED
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise.
*/
bt_bool bt_avrcp_send_panel_control(bt_avrcp_channel_t* channel, bt_byte opid, bt_byte button_state);

/**
* \brief Send AV/C Panel Subunit "click" PASS THROUGH command
* \ingroup avrcp
*
* \details This function is used to send a button click. Two PATH THROUGTH commands are sent.
*          The first command has button state set to AVC_PANEL_BUTTON_PRESSED. The second command
*          gas button state set to AVC_PANEL_BUTTON_RELEASED
*          
* \param channel AVRCP channel.
* \param button_id Operation Id. This value can be on of the AVC_PANEL_OPID_... constants
*
* \return
*        \li \c TRUE if the function succeeds.
*        \li \c FALSE otherwise.
*/
bt_bool bt_avrcp_send_button_click(bt_avrcp_channel_t* channel, bt_byte button_id);

// Panel operations

#define bt_avrcp_select_press(channel)             bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_SELECT)
#define bt_avrcp_select_release(channel)           bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_SELECT)
#define bt_avrcp_select_click(channel)             bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_SELECT)

#define bt_avrcp_up_press(channel)                 bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_UP)
#define bt_avrcp_up_release(channel)               bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_UP)
#define bt_avrcp_up_click(channel)                 bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_UP)

#define bt_avrcp_down_press(channel)               bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_DOWN)
#define bt_avrcp_down_release(channel)             bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_DOWN)
#define bt_avrcp_down_click(channel)               bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_DOWN)

#define bt_avrcp_left_press(channel)               bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_LEFT)
#define bt_avrcp_left_release(channel)             bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_LEFT)
#define bt_avrcp_left_click(channel)               bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_LEFT)

#define bt_avrcp_right_press(channel)              bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_RIGHT)
#define bt_avrcp_right_release(channel)            bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_RIGHT)
#define bt_avrcp_right_click(channel)              bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_RIGHT)

#define bt_avrcp_right_up_press(channel)           bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_RIGHT_UP)
#define bt_avrcp_right_up_release(channel)         bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_RIGHT_UP)
#define bt_avrcp_right_up_click(channel)           bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_RIGHT_UP)

#define bt_avrcp_right_down_press(channel)         bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_RIGHT_DOWN)
#define bt_avrcp_right_down_release(channel)       bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_RIGHT_DOWN)
#define bt_avrcp_right_down_click(channel)         bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_RIGHT_DOWN)

#define bt_avrcp_left_up_press(channel)            bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_LEFT_UP)
#define bt_avrcp_left_up_release(channel)          bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_LEFT_UP)
#define bt_avrcp_left_up_click(channel)            bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_LEFT_UP)

#define bt_avrcp_left_down_press(channel)          bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_LEFT_DOWN)
#define bt_avrcp_left_down_release(channel)        bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_LEFT_DOWN)
#define bt_avrcp_left_down_click(channel)          bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_LEFT_DOWN)

#define bt_avrcp_root_menu_press(channel)          bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_ROOT_MENU)
#define bt_avrcp_root_menu_release(channel)        bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_ROOT_MENU)
#define bt_avrcp_root_menu_click(channel)          bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_ROOT_MENU)

#define bt_avrcp_setup_menu_press(channel)         bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_SETUP_MENU)
#define bt_avrcp_setup_menu_release(channel)       bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_SETUP_MENU)
#define bt_avrcp_setup_menu_click(channel)         bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_SETUP_MENU)

#define bt_avrcp_contents_menu_press(channel)      bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_CONTENTS_MENU)
#define bt_avrcp_content_menu_release(channel)     bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_CONTENTS_MENU)
#define bt_avrcp_content_menu_click(channel)       bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_CONTENTS_MENU)

#define bt_avrcp_favorive_menu_press(channel)      bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_FAVORIVE_MENU)
#define bt_avrcp_favorite_menu_release(channel)    bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_FAVORIVE_MENU)
#define bt_avrcp_favorite_menu_click(channel)      bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_FAVORITE_MENU)

#define bt_avrcp_exit_press(channel)               bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_EXIT)
#define bt_avrcp_exit_release(channel)             bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_EXIT)
#define bt_avrcp_exit_click(channel)               bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_EXIT)

#define bt_avrcp_0_press(channel)                  bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_0)
#define bt_avrcp_0_release(channel)                bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_0)
#define bt_avrcp_0_click(channel)                  bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_0)

#define bt_avrcp_1_press(channel)                  bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_1)
#define bt_avrcp_1_release(channel)                bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_1)
#define bt_avrcp_1_click(channel)                  bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_1)

#define bt_avrcp_2_press(channel)                  bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_2)
#define bt_avrcp_2_release(channel)                bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_2)
#define bt_avrcp_2_click(channel)                  bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_2)

#define bt_avrcp_3_press(channel)                  bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_3)
#define bt_avrcp_3_release(channel)                bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_3)
#define bt_avrcp_3_click(channel)                  bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_3)

#define bt_avrcp_4_press(channel)                  bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_4)
#define bt_avrcp_4_release(channel)                bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_4)
#define bt_avrcp_4_click(channel)                  bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_4)

#define bt_avrcp_5_press(channel)                  bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_5)
#define bt_avrcp_5_release(channel)                bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_5)
#define bt_avrcp_5_click(channel)                  bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_5)

#define bt_avrcp_6_press(channel)                  bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_6)
#define bt_avrcp_6_release(channel)                bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_6)
#define bt_avrcp_6_click(channel)                  bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_6)

#define bt_avrcp_7_press(channel)                  bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_7)
#define bt_avrcp_7_release(channel)                bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_7)
#define bt_avrcp_7_click(channel)                  bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_7)

#define bt_avrcp_8_press(channel)                  bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_8)
#define bt_avrcp_8_release(channel)                bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_8)
#define bt_avrcp_8_click(channel)                  bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_8)

#define bt_avrcp_9_press(channel)                  bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_9)
#define bt_avrcp_9_release(channel)                bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_9)
#define bt_avrcp_9_click(channel)                  bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_9)

#define bt_avrcp_dot_press(channel)                bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_DOT)
#define bt_avrcp_dot_release(channel)              bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_DOT)
#define bt_avrcp_dot_click(channel)                bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_DOT)

#define bt_avrcp_enter_press(channel)              bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_ENTER)
#define bt_avrcp_enter_release(channel)            bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_ENTER)
#define bt_avrcp_enter_click(channel)              bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_ENTER)

#define bt_avrcp_clear_press(channel)              bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_CLEAR)
#define bt_avrcp_clear_release(channel)            bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_CLEAR)
#define bt_avrcp_clear_click(channel)                  bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_CLEAR)

#define bt_avrcp_channel_up_press(channel)         bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_CHANNEL_UP)
#define bt_avrcp_channel_up_release(channel)       bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_CHANNEL_UP)
#define bt_avrcp_channel_up_click(channel)         bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_CHANNEL_UP)

#define bt_avrcp_channel_down_press(channel)       bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_CHANNEL_DOWN)
#define bt_avrcp_channel_down_release(channel)     bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_CHANNEL_DOWN)
#define bt_avrcp_channel_down_click(channel)       bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_CHANNEL_DOWN)

#define bt_avrcp_previous_channel_press(channel)   bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_PREVIOUS_CHANNEL)
#define bt_avrcp_previous_channel_release(channel) bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_PREVIOUS_CHANNEL)
#define bt_avrcp_previous_channel_click(channel)   bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_PREVIOUS_CHANNEL)

#define bt_avrcp_sound_select_press(channel)       bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_SOUND_SELECT)
#define bt_avrcp_sound_select_release(channel)     bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_SOUND_SELECT)
#define bt_avrcp_sound_select_click(channel)       bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_SOUND_SELECT)

#define bt_avrcp_input_select_press(channel)       bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_INPUT_SELECT)
#define bt_avrcp_input_select_release(channel)     bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_INPUT_SELECT)
#define bt_avrcp_input_select_click(channel)       bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_INPUT_SELECT)

#define bt_avrcp_display_info_press(channel)       bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_DISPLAY_INFORMATION)
#define bt_avrcp_display_info_release(channel)     bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_DISPLAY_INFORMATION)
#define bt_avrcp_display_info_click(channel)       bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_DISPLAY_INFORMATION)

#define bt_avrcp_help_press(channel)               bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_HELP)
#define bt_avrcp_help_release(channel)             bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_HELP)
#define bt_avrcp_help_click(channel)               bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_HELP)

#define bt_avrcp_page_up_press(channel)            bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_PAGE_UP)
#define bt_avrcp_page_up_release(channel)          bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_PAGE_UP)
#define bt_avrcp_page_up_click(channel)            bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_PAGE_UP)

#define bt_avrcp_page_down_press(channel)          bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_PAGE_DOWN)
#define bt_avrcp_page_down_release(channel)        bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_PAGE_DOWN)
#define bt_avrcp_page_down_click(channel)          bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_PAGE_DOWN)

#define bt_avrcp_power_press(channel)              bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_POWER_TOGGLE)
#define bt_avrcp_power_release(channel)            bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_POWER_TOGGLE)
#define bt_avrcp_power_click(channel)              bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_POWER_TOGGLE)

#define bt_avrcp_volume_up_press(channel)          bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_VOLUME_UP)
#define bt_avrcp_volume_up_release(channel)        bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_VOLUME_UP)
#define bt_avrcp_volume_up_click(channel)          bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_VOLUME_UP)

#define bt_avrcp_volume_down_press(channel)        bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_VOLUME_DOWN)
#define bt_avrcp_volume_down_release(channel)      bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_VOLUME_DOWN)
#define bt_avrcp_volume_down_click(channel)        bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_VOLUME_DOWN)

#define bt_avrcp_mute_press(channel)               bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_MUTE)
#define bt_avrcp_mute_release(channel)             bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_MUTE)
#define bt_avrcp_mute_click(channel)               bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_MUTE)

#define bt_avrcp_play_press(channel)               bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_PLAY)
#define bt_avrcp_play_release(channel)             bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_PLAY)
#define bt_avrcp_play_click(channel)               bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_PLAY)

#define bt_avrcp_stop_press(channel)               bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_STOP)
#define bt_avrcp_stop_release(channel)             bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_STOP)
#define bt_avrcp_stop_click(channel)               bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_STOP)

#define bt_avrcp_pause_press(channel)              bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_PAUSE)
#define bt_avrcp_pause_release(channel)            bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_PAUSE)
#define bt_avrcp_pause_click(channel)              bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_PAUSE)

#define bt_avrcp_record_press(channel)             bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_RECORD)
#define bt_avrcp_record_release(channel)           bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_RECORD)
#define bt_avrcp_record_click(channel)             bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_RECORD)

#define bt_avrcp_rewind_press(channel)             bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_REWIND)
#define bt_avrcp_rewind_release(channel)           bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_REWIND)
#define bt_avrcp_rewind_click(channel)             bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_REWIND)

#define bt_avrcp_fast_forward_press(channel)       bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_FAST_FORWARD)
#define bt_avrcp_fast_forward_release(channel)     bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_FAST_FORWARD)
#define bt_avrcp_fast_forward_click(channel)       bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_FAST_FORWARD)

#define bt_avrcp_eject_press(channel)              bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_EJECT)
#define bt_avrcp_eject_release(channel)            bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_EJECT)
#define bt_avrcp_eject_click(channel)              bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_EJECT)

#define bt_avrcp_forward_press(channel)            bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_FORWARD)
#define bt_avrcp_forward_release(channel)          bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_FORWARD)
#define bt_avrcp_forward_click(channel)            bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_FORWARD)

#define bt_avrcp_backward_press(channel)           bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_BACKWARD)
#define bt_avrcp_backward_release(channel)         bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_BACKWARD)
#define bt_avrcp_backward_click(channel)           bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_BACKWARD)

#define bt_avrcp_angle_press(channel)              bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_ANGLE)
#define bt_avrcp_angle_release(channel)            bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_ANGLE)
#define bt_avrcp_angle_click(channel)              bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_ANGLE)

#define bt_avrcp_subpicture_press(channel)         bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_SUBPICTURE)
#define bt_avrcp_subpicture_release(channel)       bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_SUBPICTURE)
#define bt_avrcp_subpicture_click(channel)         bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_SUBPICTURE)

#define bt_avrcp_f1_press(channel)                 bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_F1)
#define bt_avrcp_f1_release(channel)               bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_F1)
#define bt_avrcp_f1_click(channel)                 bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_F1)

#define bt_avrcp_f2_press(channel)                 bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_F2)
#define bt_avrcp_f2_release(channel)               bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_F2)
#define bt_avrcp_f2_click(channel)                 bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_F2)

#define bt_avrcp_f3_press(channel)                 bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_F3)
#define bt_avrcp_f3_release(channel)               bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_F3)
#define bt_avrcp_f3_click(channel)                 bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_F3)

#define bt_avrcp_f4_press(channel)                 bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_F4)
#define bt_avrcp_f4_release(channel)               bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_F4)
#define bt_avrcp_f4_click(channel)                 bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_F4)

#define bt_avrcp_f5_press(channel)                 bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_F5)
#define bt_avrcp_f5_release(channel)               bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_F5)
#define bt_avrcp_f5_click(channel)                 bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_F5)

#define bt_avrcp_f6_press(channel)                 bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_F6)
#define bt_avrcp_f6_release(channel)               bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_F6)
#define bt_avrcp_f6_click(channel)                 bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_F6)

#define bt_avrcp_f7_press(channel)                 bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_F7)
#define bt_avrcp_f7_release(channel)               bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_F7)
#define bt_avrcp_f7_click(channel)                 bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_F7)

#define bt_avrcp_f8_press(channel)                 bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_F8)
#define bt_avrcp_f8_release(channel)               bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_F8)
#define bt_avrcp_f8_click(channel)                 bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_F8)

#define bt_avrcp_f9_press(channel)                 bt_avrcp_send_press_panel_control(channel, AVC_PANEL_OPID_F9)
#define bt_avrcp_f9_release(channel)               bt_avrcp_send_release_panel_control(channel, AVC_PANEL_OPID_F9)
#define bt_avrcp_f9_click(channel)                 bt_avrcp_send_button_click(channel, AVC_PANEL_OPID_F9)

#ifdef __cplusplus
}
#endif

#endif // __AVRCP_COMMAND_H
