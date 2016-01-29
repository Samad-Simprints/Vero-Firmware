/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __AVDTP_H
#define __AVDTP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup avdtp Audio/Video Distribution Protocol (AVDP)
 *
 * AVDTP is the transport protocol for audio and/or video distribution
 * connections and streaming of audio or video media over the Bluetooth air interface.
 *
 */

#include "cdbt/l2cap/l2cap.h"
#include "cdbt/avdtp/avdtp_control.h"

#define AVDTP_MANAGER_STATE_IDLE          0
#define AVDTP_MANAGER_STATE_CONNECTING    1

#define AVDTP_MANAGER_FLAG_SENDING_MEDIA_PACKET        1

#define AVDTP_EVT_NULL                                0
/** \addtogroup avdtp 
* @{
*/
/** @name Events
*
* \details The following is a list of events AVDTP layer generates and can report to the upper layer when 
*          it completes executing an operation initiated by either local or remote device. 
*/
/**@{
*/
#define AVDTP_EVT_CTRL_CHANNEL_CONNECTED              1     ///< This event is generated when a control channel between two AVDTP entities has been established.
#define AVDTP_EVT_CTRL_CHANNEL_DISCONNECTED           2     ///< This event is generated when a control channel between two AVDTP entities has been terminated.
#define AVDTP_EVT_CTRL_CONNECTION_FAILED              3     ///< This event is generated when a local device failed to create a control channel between two AVDTP entities.
#define AVDTP_EVT_DISCOVER_COMPLETED                  4     ///< This event is generated when a local device received a response (either positive or negative) to a "discover" request.
#define AVDTP_EVT_GET_SEP_CAPABILITIES_COMPLETED      5     ///< This event is generated when a local device received a response (either positive or negative) to a "get SEP capabilities" request.
#define AVDTP_EVT_SET_STREAM_CONFIGURATION_COMPLETED  6     ///< This event is generated when a local device received a response (either positive or negative) to a "set stream configuration" request.
#define AVDTP_EVT_GET_STREAM_CONFIGURATION_COMPLETED  7     ///< This event is generated when a local device received a response (either positive or negative) to a "get stream configuration" request.
#define AVDTP_EVT_STREAM_RECONFIGURE_COMPLETED        8     ///< This event is generated when a local device received a response (either positive or negative) to a "change stream configuration" request.
#define AVDTP_EVT_OPEN_STREAM_COMPLETED               9     ///< This event is generated when a local device received a response (either positive or negative) to a "open stream" request.
#define AVDTP_EVT_START_STREAM_COMPLETED              10    ///< This event is generated when a local device received a response (either positive or negative) to a "start stream" request.
#define AVDTP_EVT_CLOSE_STREAM_COMPLETED              11    ///< This event is generated when a local device received a response (either positive or negative) to a "close stream" request.
#define AVDTP_EVT_SUSPEND_STREAM_COMPLETED            12    ///< This event is generated when a local device received a response (either positive or negative) to a "suspend stream" request.
#define AVDTP_EVT_STREAM_SECURITY_CONTROL_COMPLETED   13    ///< This event is generated when a local device received a response (either positive or negative) to a "exchange content protection control data" request.
#define AVDTP_EVT_ABORT_STREAM_COMPLETED              14    ///< This event is generated when a local device received a response (either positive or negative) to a "abort stream" request.
#define AVDTP_EVT_SEP_INFO_RECEIVED                   15    ///< This event is generated for each SEP contained in a positive response to a "discover" request.
#define AVDTP_EVT_SEP_CAPABILITIES_RECEIVED           16    ///< This event is generated when a local device received a positive response to a "get SEP capabilities" request.
#define AVDTP_EVT_STREAM_CONFIGURATION_RECEIVED       17    ///< This event is generated when a local device received a positive response to a "get stream configuration" request.
#define AVDTP_EVT_DELAYREPORT_COMPLETED               18    ///< This event is generated when a local device received a response (either positive or negative) to a "delay report" request.

// event generated when local device is acceptor
#define AVDTP_EVT_SET_STREAM_CONFIGURATION_REQUESTED  50    ///< This event is generated when a local device received "set stream configuration" request.
#define AVDTP_EVT_OPEN_STREAM_REQUESTED               51    ///< This event is generated when a local device received "open stream" request.
#define AVDTP_EVT_START_STREAM_REQUESTED              52    ///< This event is generated when a local device received "start stream" request.
#define AVDTP_EVT_CLOSE_STREAM_REQUESTED              53    ///< This event is generated when a local device received "close stream" request.
#define AVDTP_EVT_SUSPEND_STREAM_REQUESTED            54    ///< This event is generated when a local device received "suspend stream" request.
#define AVDTP_EVT_ABORT_STREAM_REQUESTED              55    ///< This event is generated when a local device received "abort stream" request.
#define AVDTP_EVT_RECONFIGURE_STREAM_REQUESTED        56    ///< This event is generated when a local device received "change stream configuration" request.
#define AVDTP_EVT_MEDIA_PACKET_RECEIVED               57    ///< This event is generated when a local device received a media packet.
#define AVDTP_EVT_STREAM_CONFIGURED                   58    ///< This event is generated when a local device has successfully configured a stream. 
	                                                        ///< This event follows the AVDTP_EVT_SET_STREAM_CONFIGURATION_REQUESTED if the upper layer has accepted it.
	                                                        ///< This event is not generated if stream configuration was initiated by the local device.
#define AVDTP_EVT_STREAM_RECONFIGURED                 59    ///< This event is generated when a local device has successfully reconfigured a stream. 
                                                            ///< This event follows the AVDTP_EVT_RECONFIGURE_STREAM_REQUESTED if the upper layer has accepted it.
                                                            ///< This event is not generated if stream reconfiguration was initiated by the local device.
#define AVDTP_EVT_STREAM_OPENED                       60    ///< This event is generated when a local device has successfully opened a stream. 
                                                            ///< This event follows the AVDTP_EVT_OPEN_STREAM_REQUESTED if the upper layer has accepted it.
                                                            ///< This event is not generated if stream opening was initiated by the local device.
#define AVDTP_EVT_STREAM_STARTED                      61    ///< This event is generated when a local device has successfully started a stream. 
                                                            ///< This event follows the AVDTP_EVT_START_STREAM_REQUESTED if the upper layer has accepted it.
                                                            ///< This event is not generated if stream starting was initiated by the local device.
#define AVDTP_EVT_STREAM_CLOSED                       62    ///< This event is generated when a local device has successfully closed a stream. 
                                                            ///< This event follows the AVDTP_EVT_CLOSE_STREAM_REQUESTED if the upper layer has accepted it.
                                                            ///< This event is not generated if stream closing was initiated by the local device.
#define AVDTP_EVT_STREAM_SUSPENDED                    63    ///< This event is generated when a local device has successfully suspended a stream. 
                                                            ///< This event follows the AVDTP_EVT_SUSPEND_STREAM_REQUESTED if the upper layer has accepted it.
                                                            ///< This event is not generated if stream suspension was initiated by the local device.
#define AVDTP_EVT_STREAM_ABORTED                      64    ///< This event is generated when a local device has successfully aborted a stream. 
                                                            ///< This event follows the AVDTP_EVT_ABORT_STREAM_REQUESTED if the upper layer has accepted it.
                                                            ///< This event is not generated if stream abortion was initiated by the local device.
#define AVDTP_EVT_MEDIA_PACKET_SENT                   65    ///< This event is generated when a local device sent a media packet.
#define AVDTP_EVT_MEDIA_PACKET_SEND_FAILED            66    ///< This event is generated when a local device failed to send a media packet.
/**@}*/ 
/**@}*/

#define AVDTP_EVT_LAST                                99

#define AVDTP_SEP_STATE_FREE          0
#define AVDTP_SEP_STATE_IDLE          1

/** \addtogroup avdtp 
* @{
*/
/** @name Stream States
*
*/
/**@{
*/
#define AVDTP_STREAM_STATE_IDLE                      0      ///< The stream is idle. This can mean two things. The stream specified by strm_handle does not exist 
	                                                        ///< or the stream is closed.
#define AVDTP_STREAM_OPENING_TRANSPORT_CHANNELS      1      ///< The stream is opening transport channels.
#define AVDTP_STREAM_CLOSING_TRANSPORT_CHANNELS      2      ///< The stream is closing transport channels.
#define AVDTP_STREAM_STATE_CONFIGURED                3      ///< The stream has been configured.
#define AVDTP_STREAM_STATE_OPEN                      4      ///< The stream has been opened.
#define AVDTP_STREAM_STATE_STREAMING                 5      ///< The stream has been started. Depending on the local SEP type (source or sink) it means 
                                                            ///< that the stream can send or receive media packets.
#define AVDTP_STREAM_STATE_CLOSING                   6      ///< The stream is closing. This means that all transport channels associated with the stream are being closed. 
                                                            ///< After they have been closed the stream goes to AVDTP_STREAM_STATE_IDLE state.
#define AVDTP_STREAM_STATE_ABORTING                  7      ///< The stream is aborting. This means that all transport channels associated with the stream are being closed. 
                                                            ///< After they have been closed the stream goes to AVDTP_STREAM_STATE_IDLE state.
/**@}*/ 
/**@}*/

#define AVDTP_STREAM_FLAG_LISTENING                  1

/** \addtogroup avdtp 
* @{
*/
/** @name SEP Type
*
* @details The following is a list of SEP types. 
*
*/
/**@{
*/
#define AVDTP_SEP_TYPE_SOURCE    0 ///< Source (usually a device like a phone, desktop or laptop).
#define AVDTP_SEP_TYPE_SINK      1 ///< Sink (usually a device like a headphones or BMW).
/**@}*/ 
/**@}*/


/** \addtogroup avdtp 
* @{
*/
/** @name Service Categories
*
* @details The following is a list of service categories a SEP supports. 
*
* @note dotstack supports only AVDTP_SEP_SERVICE_CAPABILITY_MEDIA_TRANSPORT and AVDTP_SEP_SERVICE_CAPABILITY_MEDIA_CODEC.
* @note These constants define values that are transfered OTA. They are not use in API. Constants for initializing
*       bt_avdtp_sep_capabilities_t structure that is used to define a SEP's capabilities
*       are defined with AVDTP_SEP_CAPABILITY_FLAG_... constants.
*
*/
/**@{
*/
#define AVDTP_SEP_SERVICE_CAPABILITY_MEDIA_TRANSPORT       1 ///< Media. A SEP is capable of transferring media (audio, video or both) packets.
#define AVDTP_SEP_SERVICE_CAPABILITY_REPORTING             2 ///< Reporting. A SEP is capable of transferring reporting packets.
#define AVDTP_SEP_SERVICE_CAPABILITY_RECOVERY              3 ///< Recovery. A SEP is capable of transferring recovery packets.
#define AVDTP_SEP_SERVICE_CAPABILITY_CONTENT_PROTECTION    4 ///< Content Prortection. A SEP is capable of transferring content protection packets.
#define AVDTP_SEP_SERVICE_CAPABILITY_HEADER_COMPRESSION    5 ///< Header Compression. A SEP can use header compression for transferring Media or Recovery packets.
#define AVDTP_SEP_SERVICE_CAPABILITY_MULTIPLEXING          6 ///< Multiplexing. Multiple transport sessions, belonging to the same or to a different stream, can share a common transport (L2CAP) channel.
#define AVDTP_SEP_SERVICE_CAPABILITY_MEDIA_CODEC           7 ///< Media Codec. Defines which codec a SEP supports. A SEP can support only one codec.
#define AVDTP_SEP_SERVICE_CAPABILITY_DELAY_REPORTING       8 ///< Delay Reporting.
/**@}*/ 
/**@}*/

/** \addtogroup avdtp 
* @{
*/
/** @name Service Categories Flags
*
* @details The following is a list of constants that can be used to initialize bt_avdtp_sep_capabilities_t::categories.
*          A combination of these constants defines service capabilities exposed by a SEP to a remote party.
*
* @note dotstack supports only AVDTP_SEP_CAPABILITY_FLAG_MEDIA_TRANSPORT and AVDTP_SEP_CAPABILITY_FLAG_MEDIA_CODEC.
        All other service capabilities are ignored. They are defined here only for completeness.
*
*/
/**@{
*/
#define AVDTP_SEP_CAPABILITY_FLAG_MEDIA_TRANSPORT       0x01 ///< Media. A SEP is capable of transferring media (audio, video or both) packets.
#define AVDTP_SEP_CAPABILITY_FLAG_REPORTING             0x02 ///< Reporting. A SEP is capable of transferring reporting packets.
#define AVDTP_SEP_CAPABILITY_FLAG_RECOVERY              0x04 ///< Recovery. A SEP is capable of transferring recovery packets.
#define AVDTP_SEP_CAPABILITY_FLAG_CONTENT_PROTECTION    0x08 ///< Content Prortection. A SEP is capable of transferring content protection packets.
#define AVDTP_SEP_CAPABILITY_FLAG_HEADER_COMPRESSION    0x10 ///< Header Compression. A SEP can use header compression for transferring Media or Recovery packets.
#define AVDTP_SEP_CAPABILITY_FLAG_MULTIPLEXING          0x20 ///< Multiplexing. Multiple transport sessions, belonging to the same or to a different stream, can share a common transport (L2CAP) channel.
#define AVDTP_SEP_CAPABILITY_FLAG_MEDIA_CODEC           0x40 ///< Media Codec. Defines which codec a SEP supports. A SEP can support only one codec.
#define AVDTP_SEP_CAPABILITY_FLAG_DELAY_REPORTING       0x80 ///< Delat reporitng.
/**@}*/ 
/**@}*/

#define AVDTP_TRANSPORT_CHANNEL_TYPE_DEDICATED    0
#define AVDTP_TRANSPORT_CHANNEL_TYPE_SHARED       1

#define AVDTP_MAX_STREAM_TRANSPORT_SESSION    3

/** \addtogroup avdtp 
* @{
*/
/** @name Transport Session Types
*
* @details The following is a list of transport sessions a SEP supports. 
*
*   @note The only transport session cyrrently supported by dotstack is AVDTP_TRANSPORT_SESSION_TYPE_MEDIA.
*/
/**@{
*/
#define AVDTP_TRANSPORT_SESSION_TYPE_MEDIA        0 ///< Media (audio or video). 
#define AVDTP_TRANSPORT_SESSION_TYPE_REPORTING    1 ///< Reporting (currently not supported). 
#define AVDTP_TRANSPORT_SESSION_TYPE_RECOVERY     2 ///< Recovery (currently not supported). 
/**@}*/ 
/**@}*/

/** \addtogroup avdtp 
* @{
*/
/** @name Codec Types
*
* @details The following is a list of codecs a SEP supports. 
*
*/
/**@{
*/
#define AVDTP_CODEC_TYPE_SBC              0x00 ///< SBC (mandatory to support in A2DP profile). 
#define AVDTP_CODEC_TYPE_MPEG1_2_AUDIO    0x01 ///< MPEG-1,2 (optional).
#define AVDTP_CODEC_TYPE_MPEG2_4_AAC      0x02 ///< MPEG-2,4 AAC (optional, used in Apple's products).
#define AVDTP_CODEC_TYPE_ATRAC            0x04 ///< ATRAC (proprietary codec owned by Sony Corporation).
#define AVDTP_CODEC_TYPE_NON_A2DP         0xFF ///< Vendor specific.
/**@}*/ 
/**@}*/

/** \addtogroup avdtp 
* @{
*/
/** @name Media Types
*
* @details The following is a list of media types a SEP can support. 
*
*/
/**@{
*/
#define AVDTP_MEDIA_TYPE_AUDIO         0 ///< Audio.
#define AVDTP_MEDIA_TYPE_VIDEO         1 ///< Video.
#define AVDTP_MEDIA_TYPE_MULTIMEDIA    2 ///< Both Audio & Video.
/**@}*/ 
/**@}*/


/** \addtogroup avdtp 
* @{
*/
/** @name Codec Handler Operations
*
*/
/**@{
*/
#define AVDTP_CODEC_OPCODE_PARSE_CONFIG        0          ///< Parse codec configuration.
#define AVDTP_CODEC_OPCODE_SERIALIZE_CONFIG    1          ///< Serialize codec configuration.
/**@}*/ 
/**@}*/
#define AVDTP_CODEC_OPCODE_PARSE_PACKET        2


#define AVDTP_CONTENT_PROTECTION_METHOD_SCMS_T 2

#define AVDTP_SCMS_T_L_BIT                     1
#define AVDTP_SCMS_T_CP_BIT                    2

/** \addtogroup avdtp 
* @{
*/
/** @name Error Codes
*
*/
/**@{
*/
#define AVDTP_ERROR_SUCCESS                       0       ///< The operation completed with no errors.

#define AVDTP_ERROR_BAD_HEADER_FORMAT             0x01    ///< The request packet header format is invalid.
#define AVDTP_ERROR_BAD_LENGTH                    0x11    ///< The request packet length is not match the assumed length.
#define AVDTP_ERROR_BAD_ACP_SEID                  0x12    ///< The requested command indicates an invalid ACP SEP ID (not addressable)
#define AVDTP_ERROR_SEP_IN_USE                    0x13    ///< The SEP is in use.
#define AVDTP_ERROR_SEP_NOT_IN_USE                0x14    ///< The SEP is not in use.
#define AVDTP_ERROR_BAD_SERV_CATEGORY             0x17    ///< The value of Service Category in the request packet is not defined in AVDTP.
#define AVDTP_ERROR_BAD_PAYLOAD_FORMAT            0x18    ///< The requested command has an incorrect payload format.
#define AVDTP_ERROR_NOT_SUPPORTED_COMMAND         0x19    ///< The requested command is not supported by the device.
#define AVDTP_ERROR_INVALID_CAPABILITIES          0x1a    ///< The reconfigure command is an attempt to reconfigure a transport service capabilities of the SEP. Reconfigure is only permitted for application service capabilities.

#define AVDTP_ERROR_BAD_RECOVERY_TYPE             0x22    ///< The requested Recovery Type is not defined in AVDTP.
#define AVDTP_ERROR_BAD_MEDIA_TRANSPORT_FORMAT    0x23    ///< The format of Media Transport Capability is not correct.
#define AVDTP_ERROR_BAD_RECOVERY_FORMAT           0x25    ///< The format of Recovery Service Capability is not correct.
#define AVDTP_ERROR_BAD_ROHC_FORMAT               0x26    ///< The format of Header Compression Service Capability is not correct.
#define AVDTP_ERROR_BAD_CP_FORMAT                 0x27    ///< The format of Content Protection Service Capability is not correct.
#define AVDTP_ERROR_BAD_MULTIPLEXING_FORMAT       0x28    ///< The format of Multiplexing Service Capability is not correct.
#define AVDTP_ERROR_UNSUPPORTED_CONFIGURAION      0x29    ///< Configuration not supported.

#define AVDTP_ERROR_BAD_STATE                     0x31    ///< The stream is in state that does not permit executing commands.

#define AVDTP_ERROR_FAILED_TO_CONNECT_TRANSPORT   0x40    ///< An attempt to esctablish a transport channel has failed.
#define AVDTP_ERROR_FAILED_TO_CONNECT_CONTROL     0x41    ///< An attempt to esctablish a control channel has failed.
/**@}*/ 
/**@}*/

typedef struct _bt_media_packet_t bt_media_packet_t;

typedef struct _bt_cp_header_s bt_cp_header_t;

struct _bt_cp_header_s
{
	union
	{
		bt_byte scmst;

		struct
		{
			bt_byte* value;
			bt_uint length;
		} unknown;
	} header;
};


/**
* \brief  Media packet buffer
* \ingroup avdtp
*
* \details This structure is used to receive and send media packet from/to the remote device.
*          See more information about usage of this structure in 
*          descriptions of ::bt_avdtp_add_media_rx_buffer and ::bt_avdtp_add_media_tx_buffer.
*/
struct _bt_media_packet_t
{
	bt_media_packet_t* next_packet;    ///< Pointer to next buffer

	bt_byte version;                   ///< Version of the RTP implementation.
	bt_byte csrc_count;                ///< The CSRC count contains the number of CSRC identifiers that follow the fixed header.
	bt_bool marker;                    ///< The interpretation of the marker is defined by a profile. It is
                                       ///< intended to allow significant events such as frame boundaries to
	                                   ///< be marked in the packet stream.
	bt_byte payload_type;              ///< This field identifies the format of the RTP payload and determines its interpretation by the application. 
                                       ///< A profile specifies default static mapping of payload type codes to payload formats.
	bt_uint seq_number;                ///< The sequence number increments by one for each media packet sent, and may be used by the receiver to detect packet loss and to restore packet sequence.
	bt_ulong timestamp;                ///< The Time Stamp reflects the sampling instant of the first octet in the media packet.
	bt_ulong ssrc;                     ///< The SSRC field identifies the synchronization source. This
                                       ///< identifier is chosen randomly, with the intent that no two synchronization
                                       ///< sources, within the same media transport
                                       ///< session, shall have the same SSRC identifier.
	bt_ulong* csrc_list;               ///< The CSRC list identifies the contributing sources for the payload contained in this packet.

	bt_byte* data;                     ///< Pointer to a buffer to store media data. This pointer must be allocated by the AVDTP consumer
                                       ///< before adding this structure to receive (::bt_avdtp_add_media_rx_buffer) or send queue (::bt_avdtp_add_media_tx_buffer).
	bt_int data_len;                   ///< Lentgth of the media data.
	bt_int max_data_len;               ///< Maximum length of the media data that can be stored in the buffer pointed by the \c data member.

	bt_cp_header_t cp_header;          ///< Content protection header.
};


/**
* \brief  SEP capabilities
* \ingroup avdtp
*
* \details This structure is used to hold SEP capabilities.
*/
typedef struct _bt_avdtp_sep_capabilities_t
{
	bt_byte categories;                  ///< Defines service capabilities exposed by a SEP to a remote party. This can be a combination of the following values:
                                         ///< \li AVDTP_SEP_CAPABILITY_FLAG_MEDIA_TRANSPORT
                                         ///< \li AVDTP_SEP_CAPABILITY_FLAG_REPORTING
                                         ///< \li AVDTP_SEP_CAPABILITY_FLAG_RECOVERY
                                         ///< \li AVDTP_SEP_CAPABILITY_FLAG_CONTENT_PROTECTION
                                         ///< \li AVDTP_SEP_CAPABILITY_FLAG_HEADER_COMPRESSION
                                         ///< \li AVDTP_SEP_CAPABILITY_FLAG_MULTIPLEXING
                                         ///< \li AVDTP_SEP_CAPABILITY_FLAG_MEDIA_CODEC

	bt_byte media_type;                  ///< Type of media supported by this SEP. This can be on of the following values:
                                         ///< \li AVDTP_MEDIA_TYPE_AUDIO
                                         ///< \li AVDTP_MEDIA_TYPE_VIDEO
                                         ///< \li AVDTP_MEDIA_TYPE_MULTIMEDIA
	bt_byte codec_type;                  ///< Codec type supported by this SEP.  This can be on of the following values:
                                         ///< \li AVDTP_CODEC_TYPE_SBC
                                         ///< \li AVDTP_CODEC_TYPE_MPEG1_2_AUDIO
                                         ///< \li AVDTP_CODEC_TYPE_MPEG2_4_AAC
                                         ///< \li AVDTP_CODEC_TYPE_ATRAC
                                         ///< \li AVDTP_CODEC_TYPE_NON_A2DP
	void* codec_config;                  ///< Pointer to a buffer that holds codec specific capabilities

	bt_uint cp_type;                     ///< Type of content protection supported by this SEP.
                                         ///< \note Content protection is currently not supported by dotstack.
	bt_byte* cp_specific;                ///< Pointer to a buffer holding content protection specific data.
                                         ///< \note Content protection is currently not supported by dotstack.
	bt_byte  cp_specific_len;            ///< Length of the content protection specific data.
                                         ///< \note Content protection is currently not supported by dotstack.

	bt_byte recovery_type;               ///< Type of recovery supported by this SEP. 
                                         ///< \note Recovery is currently not supported by dotstack.
	bt_byte max_recovery_window;         ///< Recovery window size.
                                         ///< \note Recovery is currently not supported by dotstack.
	bt_byte max_parity_code_packets;     ///< Maximum number of parity codec packets.
                                         ///< \note Recovery is currently not supported by dotstack.


	bt_byte header_compression_options;  ///< Header compression configuration
                                         ///< \note Header compression is currently not supported by dotstack.

	bt_byte multiplexing_options;        ///< Multiplexing configuration
                                         ///< \note Multiplexing is currently not supported by dotstack.

	bt_byte tsid_media;                  ///< ID of the media transport session
	bt_byte tcid_media;                  ///< ID of the media transport channel
	bt_byte tsid_reporting;              ///< ID of the reporting transport session
	bt_byte tcid_reporting;              ///< ID of the reporting transport channel
	bt_byte tsid_recovery;               ///< ID of the recovery transport session
	bt_byte tcid_recovery;               ///< ID of the recovery transport channel

} bt_avdtp_sep_capabilities_t;

typedef struct _bt_avdtp_sep_t bt_avdtp_sep_t;

/**
* \brief  SEP description
* \ingroup avdtp
*
* \details This structure is used to hold information about SEPs available on a local device.
*/
struct _bt_avdtp_sep_t
{
	bt_avdtp_sep_t* next_sep;                  ///< Pointer to next SEP

	bt_byte id;                                ///< ID of the SEP
	bt_byte type;                              ///< Type of the SEP. This can be one of the following values:
                                               ///< \li AVDTP_SEP_TYPE_SOURCE
                                               ///< \li AVDTP_SEP_TYPE_SINK
	const bt_avdtp_sep_capabilities_t* caps;   ///< SEP capabilities
	bt_byte state;                             ///< State of the SEP buffer. This can be one of the following values:
                                               ///< \li AVDTP_SEP_STATE_FREE
                                               ///< \li AVDTP_SEP_STATE_IDLE
};

/**
* \brief  Parameter to AVDTP_EVT_CTRL_CHANNEL_CONNECTED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::ctrl_channel_connected -
*          when a control channel between two devices has been established.
*/
typedef struct _bt_avdtp_evt_ctrl_channel_connected_s
{
	bt_bdaddr_t* bdaddr;    ///< the address of a remote device
} bt_avdtp_evt_ctrl_channel_connected_t;

/**
* \brief  Parameter to AVDTP_EVT_CTRL_CHANNEL_DISCONNECTED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::ctrl_channel_disconnected -
*          when a control channel between two devices has been terminated.
*/
typedef struct _bt_avdtp_evt_ctrl_channel_disconnected_s
{
	bt_bdaddr_t* bdaddr;    ///< the address of a remote device
} bt_avdtp_evt_ctrl_channel_disconnected_t;

/**
* \brief  Parameter to AVDTP_EVT_DISCOVER_COMPLETED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::discover_completed -
*          when AVDTP completed discovering SEPs available on a remote device.
*/
typedef struct _bt_avdtp_evt_discover_completed_t
{
	bt_byte err_code;    ///< The result of discovering.
	                     ///< \li If the remote accepted the request \c err_code == AVDTP_ERROR_SUCCESS.
	                     ///< \li Otherwise \c err_code == the error code returned by the remote party.
} bt_avdtp_evt_discover_completed_t;

/**
* \brief  Parameter to AVDTP_EVT_SEP_INFO_RECEIVED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::sep_info_received -
*          when AVDTP received positive result to a "discover" request. 
*          AVDTP_EVT_SEP_INFO_RECEIVED is generated for every SEP received from the remote device.
*/
typedef struct _bt_avdtp_evt_sep_info_received_t
{
	bt_byte sep_id;      ///< SEP ID.
	bt_byte sep_type;    ///< SEP type. This can be either AVDTP_SEP_TYPE_SOURCE or AVDTP_SEP_TYPE_SINK.
	bt_bool in_use;      ///< A flag indicating if a SEP is already being used.
	bt_byte media_type;  ///< Type of media supported by this SEP. This can be on of the following values:
	                     ///< \li AVDTP_MEDIA_TYPE_AUDIO
                         ///< \li AVDTP_MEDIA_TYPE_VIDEO
	                     ///< \li AVDTP_MEDIA_TYPE_MULTIMEDIA
} bt_avdtp_evt_sep_info_received_t;

/**
* \brief  Parameter to AVDTP_EVT_GET_SEP_CAPABILITIES_COMPLETED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::get_sep_capabilities_completed -
*          when AVDTP received a response to a "get SEP capabilities" request. 
*          AVDTP_EVT_GET_SEP_CAPABILITIES_COMPLETED only informs the status of the request - success or failure.
*          In case of success another event - AVDTP_EVT_SEP_CAPABILITIES_RECEIVED - is generate with a pointer
*          to a structure that holds actual SEP's capabilities.
*/
typedef struct _bt_avdtp_evt_get_sep_capabilities_completed_t
{
	bt_byte err_code;    ///< The result of the request.
                         ///< \li If the remote accepted the request \c err_code == AVDTP_ERROR_SUCCESS.
                         ///< \li Otherwise \c err_code == the error code returned by the remote party.
} bt_avdtp_evt_get_sep_capabilities_completed_t;

/**
* \brief  Parameter to AVDTP_EVT_SEP_CAPABILITIES_RECEIVED and AVDTP_EVT_STREAM_CONFIGURATION_RECEIVED events
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::sep_capabilities_received -
*          when AVDTP received a positive response to a "get SEP capabilities" or "get stream configuration" request. 
*/
typedef struct _bt_avdtp_evt_sep_capabilities_received_t
{
	bt_avdtp_sep_capabilities_t* caps;    ///< SEP capabilities or stream configuration.
} bt_avdtp_evt_sep_capabilities_received_t;

/**
* \brief  Parameter to AVDTP_EVT_SET_STREAM_CONFIGURATION_COMPLETED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::set_stream_configuration_completed -
*          when AVDTP received a response to a "set stream configuration" request. 
*/
typedef struct _bt_avdtp_evt_set_stream_configuration_completed_t
{
	bt_byte err_code;        ///< The result of the request.
                             ///< \li If the remote accepted the request \c err_code == AVDTP_ERROR_SUCCESS.
                             ///< \li Otherwise \c err_code == the error code returned by the remote party.
	bt_byte strm_handle;     ///< Stream handle.
	bt_byte svc_category;    ///< The value of the first Service Category to fail if the remote rejected the request.
} bt_avdtp_evt_set_stream_configuration_completed_t;

/**
* \brief  Parameter to AVDTP_EVT_SET_STREAM_CONFIGURATION_COMPLETED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::get_stream_configuration_completed -
*          when AVDTP received a response to a "get stream configuration" request. 
*          AVDTP_EVT_SET_STREAM_CONFIGURATION_COMPLETED only informs the status of the request - success or failure.
*          In case of success another event - AVDTP_EVT_STREAM_CONFIGURATION_RECEIVED - is generate with a pointer
*          to a structure that hold actual stream's configuration.
*/
typedef struct _bt_avdtp_evt_get_stream_configuration_completed_t
{
	bt_byte err_code;
	bt_byte strm_handle;
} bt_avdtp_evt_get_stream_configuration_completed_t;

/**
* \brief  Parameter to AVDTP_EVT_STREAM_RECONFIGURE_COMPLETED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::stream_reconfigure_completed -
*          when AVDTP received a response to a "change stream configuration" request. 
*/
typedef struct _bt_avdtp_evt_stream_reconfigure_completed_t
{
	bt_byte err_code;        ///< The result of the request.
                             ///< \li If the remote accepted the request \c err_code == AVDTP_ERROR_SUCCESS.
                             ///< \li Otherwise \c err_code == the error code returned by the remote party.
	bt_byte strm_handle;     ///< Stream handle.
	bt_byte svc_category;    ///< The value of the first Service Category to fail if the remote rejected the request.
} bt_avdtp_evt_stream_reconfigure_completed_t;

/**
* \brief  Parameter to AVDTP_EVT_OPEN_STREAM_COMPLETED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::open_stream_completed -
*          when AVDTP received a response to a "open stream" request. 
*/
typedef struct _bt_avdtp_evt_open_stream_completed_t
{
	bt_byte err_code;       ///< The result of the request.
                            ///< \li If the remote accepted the request \c err_code == AVDTP_ERROR_SUCCESS.
                            ///< \li Otherwise \c err_code == the error code returned by the remote party.
	bt_byte strm_handle;    ///< Stream handle.
} bt_avdtp_evt_open_stream_completed_t;

/**
* \brief  Parameter to AVDTP_EVT_START_STREAM_COMPLETED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::start_stream_completed -
*          when AVDTP received a response to a "start stream" request. 
*/
typedef struct _bt_avdtp_evt_start_stream_completed_t
{
	bt_byte err_code;       ///< The result of the request.
                            ///< \li If the remote accepted the request \c err_code == AVDTP_ERROR_SUCCESS.
                            ///< \li Otherwise \c err_code == the error code returned by the remote party.
	bt_byte strm_handle;    ///< Stream handle.
} bt_avdtp_evt_start_stream_completed_t;

/**
* \brief  Parameter to AVDTP_EVT_CLOSE_STREAM_COMPLETED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::close_stream_completed -
*          when AVDTP received a response to a "close stream" request. 
*/
typedef struct _bt_avdtp_evt_close_stream_completed_t
{
	bt_byte err_code;       ///< The result of the request.
                            ///< \li If the remote accepted the request \c err_code == AVDTP_ERROR_SUCCESS.
                            ///< \li Otherwise \c err_code == the error code returned by the remote party.
	bt_byte strm_handle;    ///< Stream handle.
} bt_avdtp_evt_close_stream_completed_t;

/**
* \brief  Parameter to AVDTP_EVT_SUSPEND_STREAM_COMPLETED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::suspend_stream_completed -
*          when AVDTP received a response to a "suspend stream" request. 
*/
typedef struct _bt_avdtp_evt_suspend_stream_completed_t
{
	bt_byte err_code;       ///< The result of the request.
                            ///< \li If the remote accepted the request \c err_code == AVDTP_ERROR_SUCCESS.
                            ///< \li Otherwise \c err_code == the error code returned by the remote party.
	bt_byte strm_handle;    ///< Stream handle.
} bt_avdtp_evt_suspend_stream_completed_t;

/**
* \brief  Parameter to AVDTP_EVT_STREAM_SECURITY_CONTROL_COMPLETED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::security_control_completed -
*          when AVDTP received a response to a "exchange content protection control data" request. 
*/
typedef struct _bt_avdtp_evt_stream_security_control_completed_t
{
	bt_byte err_code;       ///< The result of the request.
                            ///< \li If the remote accepted the request \c err_code == AVDTP_ERROR_SUCCESS.
                            ///< \li Otherwise \c err_code == the error code returned by the remote party.
} bt_avdtp_evt_stream_security_control_completed_t;

/**
* \brief  Parameter to AVDTP_EVT_SET_STREAM_CONFIGURATION_REQUESTED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::set_stream_configuration_requested -
*          when AVDTP received a "set stream configuration" request. 
*/
typedef struct _bt_avdtp_evt_set_stream_configuration_requested_t
{
	bt_byte err_code;       ///< The result to be sent to the remote party.
	                        ///< \li If local device accepts the configuration requested by the remote device 
	                        ///< it should set \c err_code to AVDTP_ERROR_SUCCESS. Otherwise it should set \c err_code 
	                        ///< to one of the AVDTP_ERROR_ constants.
	bt_byte err_category;   ///< If local device cannot accept the request it should set \c err_category to the value of the forst Service Category that failed.
	bt_avdtp_sep_t* sep;    ///< Description of the local SEP.
	bt_byte int_sep_id;     ///< The ID of the remote SEP.
	bt_avdtp_sep_capabilities_t* config;    ///< Stream configuration requested by the remote party.
	bt_byte strm_handle;    ///< Stream handle.
} bt_avdtp_evt_set_stream_configuration_requested_t;

/**
* \brief  Parameter to AVDTP_EVT_RECONFIGURE_STREAM_REQUESTED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::reconfigure_stream_requested -
*          when AVDTP received a "change stream configuration" request. 
*/
typedef struct _bt_avdtp_evt_reconfigure_stream_requested_t
{
	bt_byte err_code;       ///< The result to be sent to the remote party.
                            ///< \li If local device accepts the configuration requested by the remote device 
                            ///< it should set \c err_code to AVDTP_ERROR_SUCCESS. Otherwise it should set \c err_code 
                            ///< to one of the AVDTP_ERROR_ constants.
	bt_byte err_category;   ///< If local device cannot accept the request it should set \c err_category to the value of the forst Service Category that failed.
	bt_avdtp_sep_t* sep;    ///< Description of the local SEP.
	bt_avdtp_sep_capabilities_t* config;    ///< Stream configuration requested by the remote party.
	bt_byte strm_handle;    ///< Stream handle.
} bt_avdtp_evt_reconfigure_stream_requested_t;

/**
* \brief  Parameter to AVDTP_EVT_OPEN_STREAM_REQUESTED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::open_stream_requested -
*          when AVDTP received a "open stream" request. 
*/
typedef struct _bt_avdtp_evt_open_stream_requested_t
{
	bt_byte err_code;       ///< The result to be sent to the remote party.
                            ///< \li If local device accepts the configuration requested by the remote device 
                            ///< it should set \c err_code to AVDTP_ERROR_SUCCESS. Otherwise it should set \c err_code 
                            ///< to one of the AVDTP_ERROR_ constants.
	bt_byte strm_handle;    ///< The handle of a stream to open.
} bt_avdtp_evt_open_stream_requested_t;

/**
* \brief  Parameter to AVDTP_EVT_START_STREAM_REQUESTED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::start_stream_requested -
*          when AVDTP received a "start stream" request. 
*/
typedef struct _bt_avdtp_evt_start_stream_requested_t
{
	bt_byte err_code;       ///< The result to be sent to the remote party.
                            ///< \li If local device accepts the configuration requested by the remote device 
                            ///< it should set \c err_code to AVDTP_ERROR_SUCCESS. Otherwise it should set \c err_code 
                            ///< to one of the AVDTP_ERROR_ constants.
	bt_byte strm_handle;    ///< The handle of a stream to start.
} bt_avdtp_evt_start_stream_requested_t;

/**
* \brief  Parameter to AVDTP_EVT_SUSPEND_STREAM_REQUESTED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::suspend_stream_requested -
*          when AVDTP received a "suspend stream" request. 
*/
typedef struct _bt_avdtp_evt_suspend_stream_requested_t
{
	bt_byte err_code;       ///< The result to be sent to the remote party.
                            ///< \li If local device accepts the configuration requested by the remote device 
                            ///< it should set \c err_code to AVDTP_ERROR_SUCCESS. Otherwise it should set \c err_code 
                            ///< to one of the AVDTP_ERROR_ constants.
	bt_byte strm_handle;    ///< The handle of a stream to suspend.
} bt_avdtp_evt_suspend_stream_requested_t;

/**
* \brief  Parameter to AVDTP_EVT_CLOSE_STREAM_REQUESTED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::close_stream_requested -
*          when AVDTP received a "close stream" request. 
*/
typedef struct _bt_avdtp_evt_close_stream_requested_t
{
	bt_byte err_code;       ///< The result to be sent to the remote party.
                            ///< \li If local device accepts the configuration requested by the remote device 
                            ///< it should set \c err_code to AVDTP_ERROR_SUCCESS. Otherwise it should set \c err_code 
                            ///< to one of the AVDTP_ERROR_ constants.
	bt_byte strm_handle;    ///< The handle of a stream to close.
} bt_avdtp_evt_close_stream_requested_t;

/**
* \brief  Parameter to AVDTP_EVT_ABORT_STREAM_REQUESTED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::abort_stream_requested -
*          when AVDTP received a "abort stream" request. 
*/
typedef struct _bt_avdtp_evt_abort_stream_requested_t
{
	bt_byte err_code;       ///< The result to be sent to the remote party.
                            ///< \li If local device accepts the configuration requested by the remote device 
                            ///< it should set \c err_code to AVDTP_ERROR_SUCCESS. Otherwise it should set \c err_code 
                            ///< to one of the AVDTP_ERROR_ constants.
	bt_byte strm_handle;    ///< The handle of a stream to abort.
} bt_avdtp_evt_abort_stream_requested_t;

/**
* \brief  Parameter to AVDTP_EVT_DELAYREPORT_COMPLETED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::delay_report_completed -
*          when AVDTP received a response to a "delay report" request. 
*/
typedef struct _bt_avdtp_evt_delay_report_completed_t
{
	bt_byte err_code;    ///< The result of the request.
                         ///< \li If the remote accepted the request \c err_code == AVDTP_ERROR_SUCCESS.
                         ///< \li Otherwise \c err_code == the error code returned by the remote party.
} bt_avdtp_evt_delay_report_completed_t;

///////////////////////////////////////////////////////////////////////////////

/**
* \brief  Parameter to AVDTP_EVT_STREAM_CONFIGURED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::stream_configured -
*          to notify the AVDTP consumer that a stream configuration has been successfully completed. 
*/
typedef struct _bt_avdtp_evt_stream_configured_t
{
	bt_byte strm_handle;    ///< The handle of a stream that has been configured.
} bt_avdtp_evt_stream_configured_t;

/**
* \brief  Parameter to AVDTP_EVT_STREAM_RECONFIGURED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::stream_reconfigured -
*          to notify the AVDTP consumer that a stream configuration has been successfully changed. 
*/
typedef struct _bt_avdtp_evt_stream_reconfigured_t
{
	bt_byte strm_handle;    ///< The handle of a stream that has been reconfigured.
} bt_avdtp_evt_stream_reconfigured_t;

/**
* \brief  Parameter to AVDTP_EVT_STREAM_OPENED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::stream_opened -
*          to notify the AVDTP consumer that a stream has been successfully opened. 
*/
typedef struct _bt_avdtp_evt_stream_opened_t
{
	bt_byte strm_handle;    ///< The handle of a stream that has been opened.
} bt_avdtp_evt_stream_opened_t;

/**
* \brief  Parameter to AVDTP_EVT_STREAM_STARTED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::stream_started -
*          to notify the AVDTP consumer that a stream has been successfully started. 
*/
typedef struct _bt_avdtp_evt_stream_started_t
{
	bt_byte strm_handle;    ///< The handle of a stream that has been started.
} bt_avdtp_evt_stream_started_t;

/**
* \brief  Parameter to AVDTP_EVT_STREAM_SUSPENDED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::stream_suspended -
*          to notify the AVDTP consumer that a stream has been successfully suspended. 
*/
typedef struct _bt_avdtp_evt_stream_suspended_t
{
	bt_byte strm_handle;    ///< The handle of a stream that has been suspended.
} bt_avdtp_evt_stream_suspended_t;

/**
* \brief  Parameter to AVDTP_EVT_STREAM_CLOSED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::stream_closed -
*          to notify the AVDTP consumer that a stream has been successfully closed. 
*/
typedef struct _bt_avdtp_evt_stream_closed_t
{
	bt_byte strm_handle;    ///< The handle of a stream that has been closed.
} bt_avdtp_evt_stream_closed_t;

/**
* \brief  Parameter to AVDTP_EVT_STREAM_ABORTED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::stream_closed -
*          to notify the AVDTP consumer that a stream has been successfully aborted. 
*/
typedef struct _bt_avdtp_evt_stream_aborted_t
{
	bt_byte strm_handle;    ///< The handle of a stream that has been aborted.
} bt_avdtp_evt_stream_aborted_t;

///////////////////////////////////////////////////////////////////////////////

/**
* \brief  Parameter to AVDTP_EVT_MEDIA_PACKET_RECEIVED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::media_packet_received -
*          when ADVDTP received a media packet from the remote device.
*/
typedef struct _bt_avdtp_evt_media_packet_received_t
{
	bt_byte strm_handle;          ///< The handle of a stream which received a packet.
	bt_media_packet_t* packet;    ///< A pointer to a media packet buffer.
} bt_avdtp_evt_media_packet_received_t;

/**
* \brief  Parameter to AVDTP_EVT_MEDIA_PACKET_SENT event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::media_packet_sent -
*          when ADVDTP sent a media packet to the remote device.
*/
typedef struct _bt_avdtp_evt_media_packet_sent_t
{
	bt_byte strm_handle;          ///< The handle of a stream which received a packet.
	bt_media_packet_t* packet;    ///< A pointer to a media packet buffer.
} bt_avdtp_evt_media_packet_sent_t;

/**
* \brief  Parameter to AVDTP_EVT_MEDIA_PACKET_SEND_FAILED event
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the AVDTP application callback as 
*          a valid member of the bt_avdtp_event_t union - bt_avdtp_event_t::media_packet_send_failed -
*          when ADVDTP failed to send a media packet to the remote device.
*/
typedef struct _bt_avdtp_evt_media_packet_send_failed_t
{
	bt_byte strm_handle;          ///< The handle of a stream which received a packet.
	bt_media_packet_t* packet;    ///< A pointer to a media packet buffer.
} bt_avdtp_evt_media_packet_send_failed_t;

/**
* \brief  Parameter to an application callback.
* \ingroup avdtp
*
* \details This union is used to pass event specific data to the AVDTP consumer. Which member of the union 
*          points to a valid structure depends on the event reported to the consumer. In general, each event
*          has a corresponding member in the union.
*/
typedef union _bt_avdtp_event_u
{
	bt_avdtp_evt_ctrl_channel_connected_t               ctrl_channel_connected;               ///< Valid if event is AVDTP_EVT_CTRL_CHANNEL_CONNECTED
	bt_avdtp_evt_ctrl_channel_disconnected_t            ctrl_channel_disconnected;            ///< Valid if event is AVDTP_EVT_CTRL_CHANNEL_DISCONNECTED
	bt_avdtp_evt_discover_completed_t                   discover_completed;                   ///< Valid if event is AVDTP_EVT_DISCOVER_COMPLETED
	bt_avdtp_evt_sep_info_received_t                    sep_info_received;                    ///< Valid if event is AVDTP_EVT_SEP_INFO_RECEIVED
	bt_avdtp_evt_get_sep_capabilities_completed_t       get_sep_capabilities_completed;       ///< Valid if event is AVDTP_EVT_GET_SEP_CAPABILITIES_COMPLETED
	bt_avdtp_evt_sep_capabilities_received_t            sep_capabilities_received;            ///< Valid if event is AVDTP_EVT_SEP_CAPABILITIES_RECEIVED
	bt_avdtp_evt_set_stream_configuration_completed_t   set_stream_configuration_completed;   ///< Valid if event is AVDTP_EVT_SET_STREAM_CONFIGURATION_COMPLETED
	bt_avdtp_evt_get_stream_configuration_completed_t   get_stream_configuration_completed;   ///< Valid if event is AVDTP_EVT_GET_STREAM_CONFIGURATION_COMPLETED
	bt_avdtp_evt_stream_reconfigure_completed_t         stream_reconfigure_completed;         ///< Valid if event is AVDTP_EVT_STREAM_RECONFIGURE_COMPLETED
	bt_avdtp_evt_open_stream_completed_t                open_stream_completed;                ///< Valid if event is AVDTP_EVT_OPEN_STREAM_COMPLETED
	bt_avdtp_evt_start_stream_completed_t               start_stream_completed;               ///< Valid if event is AVDTP_EVT_START_STREAM_COMPLETED
	bt_avdtp_evt_close_stream_completed_t               close_stream_completed;               ///< Valid if event is AVDTP_EVT_CLOSE_STREAM_COMPLETED
	bt_avdtp_evt_suspend_stream_completed_t             suspend_stream_completed;             ///< Valid if event is AVDTP_EVT_SUSPEND_STREAM_COMPLETED
	bt_avdtp_evt_stream_security_control_completed_t    security_control_completed;           ///< Valid if event is AVDTP_EVT_STREAM_SECURITY_CONTROL_COMPLETED
	bt_avdtp_evt_set_stream_configuration_requested_t   set_stream_configuration_requested;   ///< Valid if event is AVDTP_EVT_SET_STREAM_CONFIGURATION_REQUESTED
	bt_avdtp_evt_reconfigure_stream_requested_t         reconfigure_stream_requested;         ///< Valid if event is AVDTP_EVT_RECONFIGURE_STREAM_REQUESTED
	bt_avdtp_evt_open_stream_requested_t                open_stream_requested;                ///< Valid if event is AVDTP_EVT_OPEN_STREAM_REQUESTED
	bt_avdtp_evt_start_stream_requested_t               start_stream_requested;               ///< Valid if event is AVDTP_EVT_START_STREAM_REQUESTED
	bt_avdtp_evt_suspend_stream_requested_t             suspend_stream_requested;             ///< Valid if event is AVDTP_EVT_SUSPEND_STREAM_REQUESTED
	bt_avdtp_evt_close_stream_requested_t               close_stream_requested;               ///< Valid if event is AVDTP_EVT_CLOSE_STREAM_REQUESTED
	bt_avdtp_evt_abort_stream_requested_t               abort_stream_requested;               ///< Valid if event is AVDTP_EVT_ABORT_STREAM_REQUESTED
	bt_avdtp_evt_delay_report_completed_t               delay_report_completed;               ///< Valid if event is AVDTP_EVT_DELAYREPORT_COMPLETED

	/////////////////////////////////////////////////////////////////////////////

	bt_avdtp_evt_stream_configured_t                    stream_configured;                    ///< Valid if event is AVDTP_EVT_STREAM_CONFIGURED
	bt_avdtp_evt_stream_reconfigured_t                  stream_reconfigured;                  ///< Valid if event is AVDTP_EVT_STREAM_RECONFIGURED
	bt_avdtp_evt_stream_opened_t                        stream_opened;                        ///< Valid if event is AVDTP_EVT_STREAM_OPENED
	bt_avdtp_evt_stream_started_t                       stream_started;                       ///< Valid if event is AVDTP_EVT_STREAM_STARTED
	bt_avdtp_evt_stream_suspended_t                     stream_suspended;                     ///< Valid if event is AVDTP_EVT_STREAM_SUSPENDED
	bt_avdtp_evt_stream_closed_t                        stream_closed;                        ///< Valid if event is AVDTP_EVT_STREAM_CLOSED
	bt_avdtp_evt_stream_aborted_t                       stream_aborted;                       ///< Valid if event is AVDTP_EVT_STREAM_ABORTED

	/////////////////////////////////////////////////////////////////////////////

	bt_avdtp_evt_media_packet_received_t                media_packet_received;                ///< Valid if event is AVDTP_EVT_MEDIA_PACKET_RECEIVED
	bt_avdtp_evt_media_packet_sent_t                    media_packet_sent;                    ///< Valid if event is AVDTP_EVT_MEDIA_PACKET_SENT
	bt_avdtp_evt_media_packet_send_failed_t             media_packet_send_failed;             ///< Valid if event is AVDTP_EVT_MEDIA_PACKET_SEND_FAILED

} bt_avdtp_event_t;

/**
* \brief  Transport channel description
* \ingroup avdtp
*
* \details This structure is used to hold information about transport channels available on a local device.
*/
typedef struct _bt_avdtp_transport_channel_t
{
	struct _bt_avdtp_transport_channel_t* next_channel; ///< Pointer to next channel.

	bt_byte id;                                         ///< ID of the channel.
	bt_byte type;                                       ///< Type of the channel. This can be one of the following values:
                                                        ///< \li AVDTP_TRANSPORT_CHANNEL_TYPE_DEDICATED
                                                        ///< \li AVDTP_TRANSPORT_CHANNEL_TYPE_SHARED
                                                        ///< \note Shared channles (i.e. multiplexing) is currently not supported by dotstack
	bt_l2cap_channel_t* l2cap_channel;                  ///< L2CAp channel used to transfer this AVDTP channel's data.
	bt_byte ref_count;                                  ///< Channel's reference count. This value is intended for use with shared channels. 
                                                        ///< When ref count reaches 0 the channel is closed.
                                                        ///< \note Shared channles (i.e. multiplexing) is currently not supported by dotstack
	bt_byte connect_ref_count;                          ///< This value is currently not used.

} bt_avdtp_transport_channel_t;

/**
* \brief  Transport session description
* \ingroup avdtp
*
* \details This structure is used to hold information about transport sessions available on a local device.
*/
typedef struct _bt_avdtp_transport_session_t
{
	bt_byte id;                                         ///< ID of the transport session.
	bt_byte type;                                       ///< Type of the transport session. This can be one of the following values:
                                                        ///< \li AVDTP_TRANSPORT_SESSION_TYPE_MEDIA
                                                        ///< \li AVDTP_TRANSPORT_SESSION_TYPE_REPORTING
                                                        ///< \li AVDTP_TRANSPORT_SESSION_TYPE_RECOVERY
	bt_avdtp_transport_channel_t* transport_channel;    ///< Transport channel used for this transport session.

} bt_avdtp_transport_session_t;

struct _bt_avdtp_sep_t;

/**
* \brief  Stream description
* \ingroup avdtp
*
* \details This structure is used to hold information about streams available on a local device.
*/
typedef struct _bt_avdtp_stream_t
{
	struct _bt_avdtp_stream_t* next_stream;             ///< Pointer to next stream.

	bt_byte handle;                                     ///< Stream handle. This values is allocated by dotstack and is used to manipulate the stream by the AVDTP consumer.
	bt_byte state;                                      ///< State of the stream. This value can be one of the following values:
                                                        ///< \li AVDTP_STREAM_STATE_IDLE
                                                        ///< \li AVDTP_STREAM_OPENING_TRANSPORT_CHANNELS
                                                        ///< \li AVDTP_STREAM_CLOSING_TRANSPORT_CHANNELS
                                                        ///< \li AVDTP_STREAM_STATE_CONFIGURED
                                                        ///< \li AVDTP_STREAM_STATE_OPEN
                                                        ///< \li AVDTP_STREAM_STATE_STREAMING
                                                        ///< \li AVDTP_STREAM_STATE_CLOSING
                                                        ///< \li AVDTP_STREAM_STATE_ABORTING
	bt_byte flags;                                      ///< Additional stream state. This value can be one of the following values:
                                                        ///< \li AVDTP_STREAM_FLAG_LISTENING
	struct _bt_avdtp_sep_t* sep;                        ///< Local SEP this stream is connected to.
	bt_avdtp_sep_capabilities_t* config;                ///< Current SEP configuration.
	bt_byte remote_seid;                                ///< ID of the remote SEP.
	bt_bdaddr_t remote_addr;                            ///< BT address of the remote device.
	bt_avdtp_transport_session_t sessions[AVDTP_MAX_STREAM_TRANSPORT_SESSION];   ///< List of transport session available/active on this stream.
                                                                                 ///< \note There can be up to 3 (AVDTP_MAX_STREAM_TRANSPORT_SESSION == 3) transport sessions on a stream - media, reporting and recovery.
                                                                                 ///<       dotstack supports only media sessions so the other two are never used.
	struct _bt_avdtp_mgr_t* mgr;                        ///< AVDTP manager this stream belongs to.
	bt_byte cur_channel_index;                          ///< This value is currenlty not used.
	bt_byte* listen_sep_list;                           ///< A list of SEPs this channel this channel is listening on (i.e. can accept incoming connection requests).
	bt_byte listen_sep_count;                           ///< The number of SEPs in \c listen_sep_list.

	bt_queue_element_t* media_rx_queue;                 ///< A list of media packet buffer for receiving incoming packets.
	bt_queue_element_t* media_tx_queue;                 ///< A list of media packet buffer to be sent to a remote device.
} bt_avdtp_stream_t;


struct _bt_avdtp_mgr_t;
struct _bt_avdtp_codec_t;
typedef union _bt_avdtp_codec_op_param_u bt_avdtp_codec_op_param_t;

/**
* \brief  Codec handler.
* \ingroup avdtp
*
* \details AVDTP in theory can support any type of codec. Each codec uses its own format for exchanging capabilities and configuration information. 
*          In order to make out implementation do not care about these formats we use a simple way of telling AVDTP how to parse and serialize 
*          codec's configuration. The consumer of AVDTP (e.g. A2DP) for each codec it wishes to support has to register a callback 
*          function (one per codec type). That callback has to perform two function. The first one is to read the configuration received from 
*          the remote device and store it in a structure defined by the consumer. The second one is to serialize the data from a structure 
*          to a format (in case of standard A2DP codecs the format is defined in A2DP specification, vendor specific codecs can define
*          their own formats) suitable for sending as a part of a AVDTP request.\n
*          This typedef defines the interace for the callback function.
*
* \param codec A pointer to a structure that describes a codec.
* \param opcode The code of an operation to execute.
*               The \c opcode can be one of the following values:
*                 @arg AVDTP_CODEC_OPCODE_PARSE_CONFIG:      The handler has to parse configuration received from the remote device
*                                                            and store it in a structure defined by the consumer.
*                 @arg AVDTP_CODEC_OPCODE_SERIALIZE_CONFIG:  The handler has to serialize the data from a consumer defined structure 
*                                                            to a format suitable for sending as a part of a AVDTP request.
* \param op_param A pointer to the operation's specific parameters. Parameters are passed as a pointer to the bt_avdtp_codec_op_param_t union.
*                 Which member of the union points to a valid structure depends on the value of the \c opcode:
*                   @arg bt_avdtp_codec_op_param_t::parse: Valid if \c opcode == AVDTP_CODEC_OPCODE_PARSE_CONFIG.
*                   @arg bt_avdtp_codec_op_param_t::serialize: Valid if \c opcode == AVDTP_CODEC_OPCODE_SERIALIZE_CONFIG.
* \param mgr AVDTP manager.
*
*/
typedef bt_byte (*bt_avdtp_codec_handler_fp)(struct _bt_avdtp_codec_t* codec, bt_byte opcode, bt_avdtp_codec_op_param_t* op_param, struct _bt_avdtp_mgr_t* mgr);

/**
* \brief  Codec handler description.
* \ingroup avdtp
*
* \details This structure is used to register a codec handler for parsing/serializing codec capabilities and configuration. 
*          See description of the ::bt_avdtp_register_codec for more details.
*          
*/
typedef struct _bt_avdtp_codec_t
{
	struct _bt_avdtp_codec_t* next_codec;

	bt_byte codec_type;                         ///< Codec type.
	                                            ///< The \c codec_type can be one of the following values:
	                                            ///< @arg AVDTP_CODEC_TYPE_SBC:           SBC
	                                            ///< @arg AVDTP_CODEC_TYPE_MPEG1_2_AUDIO: MPEG-1,2 (used in MP3 files)
	                                            ///< @arg AVDTP_CODEC_TYPE_MPEG2_4_AAC:   MPEG-2,4 AAC (used in Apple products)
	                                            ///< @arg AVDTP_CODEC_TYPE_ATRAC:         ATRAC (used in Sony products)
	                                            ///< @arg AVDTP_CODEC_TYPE_NON_A2DP:      Non-A2DP Codec
	bt_avdtp_codec_handler_fp codec_handler;    ///< A pointer to a codec handler.
} bt_avdtp_codec_t;

/**
* \brief  Parameter to AVDTP_CODEC_OPCODE_PARSE_CONFIG operation.
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the codec handler as 
*          a valid member of the bt_avdtp_codec_op_param_t union - bt_avdtp_codec_op_param_t::parse -
*          when ADVDTP needs to parse codec's capabilities/configuration received from the remote device.
*/
typedef struct _bt_avdtp_codec_op_parse_config_t
{
	void* codec_config;               ///< A pointer to a structure defined by the AVDTP consumer where codec's configuration 
	                                  ///< will be stored by the handler. The format of the structure is totally up to
                                      ///< the AVDTP consumer. The dotstack defines such structures for SBC, MPEG-1,2 and MPEG-2,4 AAC:
	                                  ///<   @arg SBC: bt_a2dp_sbc_config_t (defined in a2dp_sbc_codec.h)
                                      ///<   @arg MPEG-1,2: bt_a2dp_mpeg_config_t (defined in a2dp_mpeg_codec.h)
		                              ///<   @arg MPEG-2,4 AAC: bt_a2dp_aac_config_t (defined in a2dp_aac_codec.h)
	bt_byte codec_config_max_size;    ///< The maximum size of a buffer pointed to by \c codec_config field.
	bt_byte* buffer;                  ///< A pointer to a buffer holding codec's configuration in OTA format.
	bt_int buffer_len;                ///< The length of the \c buffer.
	bt_int offset;                    ///< The \c buffer points to a complete packet received from the remoted device. The \c offset
	                                  ///< points to a location in the \c buffer where codec's configuration starts.
} bt_avdtp_codec_op_parse_config_t;

/**
* \brief  Parameter to AVDTP_CODEC_OPCODE_SERIALIZE_CONFIG operation.
* \ingroup avdtp
*
* \details A pointer to this structure is passed to the codec handler as 
*          a valid member of the bt_avdtp_codec_op_param_t union - bt_avdtp_codec_op_param_t::serialize -
*          when ADVDTP needs to serialize codec's capabilities/configuration for sending to the remote device.
*/
typedef struct _bt_avdtp_codec_op_serialize_config_t
{
	void* codec_config;               ///< A pointer to a structure defined by the AVDTP consumer where codec's configuration 
                                      ///< will be read from by the handler. The format of the structure is totally up to
                                      ///< the AVDTP consumer. The dotstack defines such structures for SBC, MPEG-1,2 and MPEG-2,4 AAC:
                                      ///<   @arg SBC: bt_a2dp_sbc_config_t (defined in a2dp_sbc_codec.h)
                                      ///<   @arg MPEG-1,2: bt_a2dp_mpeg_config_t (defined in a2dp_mpeg_codec.h)
                                      ///<   @arg MPEG-2,4 AAC: bt_a2dp_aac_config_t (defined in a2dp_aac_codec.h)
	bt_byte* buffer;                  ///< A pointer to a buffer where the handler has to write codec's configuration in OTA format.
	bt_int buffer_len;                ///< The length of the \c buffer.
	bt_int offset;                    ///< The \c buffer points to a complete packet that will be sent to the remote device. The \c offset
	                                  ///< points to a location in the \c buffer where codec's configuration has to be written.
} bt_avdtp_codec_op_serialize_config_t;

// There is currently no use for this structure.
typedef struct _bt_avdtp_codec_op_parse_packet_t
{
	void* packet_info;
	bt_media_packet_t* packet;
} bt_avdtp_codec_op_parse_packet_t;

/**
* \brief  Parameter to a codec handler.
* \ingroup avdtp
*
* \details This union is used to pass operation specific data to a codec handler. Which member of the union 
*          points to a valid structure depends on the operation.
*/
union _bt_avdtp_codec_op_param_u
{
	bt_avdtp_codec_op_parse_config_t parse;            ///< Valid if operation is AVDTP_CODEC_OPCODE_PARSE_CONFIG.
	bt_avdtp_codec_op_serialize_config_t serialize;    ///< Valid if operation is AVDTP_CODEC_OPCODE_SERIALIZE_CONFIG.
	bt_avdtp_codec_op_parse_packet_t parse_packet;     ///< This member is currently not used.
};

// There is currently no use for this structure.
typedef struct _bt_avdtp_codec_op_encode_t
{
	void* codec_config;
	bt_byte* in_buffer;
	bt_int in_len;
	bt_byte* out_buffer;
	bt_int out_len;
} bt_avdtp_codec_op_encode_t;

// There is currently no use for this structure.
typedef struct _bt_avdtp_codec_op_decode_t
{
	void* codec_config;
	bt_byte* in_buffer;
	bt_int in_len;
	bt_byte* out_buffer;
	bt_int out_len;
} bt_avdtp_codec_op_decode_t;

/**
 * \brief AVDTP application callback.
 * \ingroup avdtp
 *
 * \details In order to be notified of various events a consumer of the AVDTP layer has to register a callback function. 
 *          The stack will call that function whenever a new event has been generated.
 *
 * \param mgr AVDTP manager.
 *
 * \param evt AVDTP event.
 *          The event can be one of the following values:
 *            @arg AVDTP_EVT_CTRL_CHANNEL_CONNECTED:             Control channel connected.
 *            @arg AVDTP_EVT_CTRL_CHANNEL_DISCONNECTED:          Control channel disconnected.
 *            @arg AVDTP_EVT_CTRL_CONNECTION_FAILED:             Control channel connection failed (generated only if control connection has been initiated by the local device).
 *            @arg AVDTP_EVT_DISCOVER_COMPLETED:                 Local device completed discovering remote SEPs.
 *            @arg AVDTP_EVT_GET_SEP_CAPABILITIES_COMPLETED:     Local device received a response to Get SEP capabilities operation.
 *            @arg AVDTP_EVT_SET_STREAM_CONFIGURATION_COMPLETED: Local device received a response to Set stream configuration operation.
 *            @arg AVDTP_EVT_GET_STREAM_CONFIGURATION_COMPLETED: Local device received a response to Get stream configuration operation.
 *            @arg AVDTP_EVT_STREAM_RECONFIGURE_COMPLETED:       Local device received a response to Reconfigure stream operation.
 *            @arg AVDTP_EVT_OPEN_STREAM_COMPLETED:              Local device received a response to Open stream operation.
 *            @arg AVDTP_EVT_START_STREAM_COMPLETED:             Local device received a response to Start stream operation.
 *            @arg AVDTP_EVT_CLOSE_STREAM_COMPLETED:             Local device received a response to Close stream operation.
 *            @arg AVDTP_EVT_SUSPEND_STREAM_COMPLETED:           Local device received a response to Suspend stream operation.
 *            @arg AVDTP_EVT_STREAM_SECURITY_CONTROL_COMPLETED:  Local device received a response to Stream security control operation.
 *            @arg AVDTP_EVT_ABORT_STREAM_COMPLETED:             Local device received a response to Abort stream operation.
 *            @arg AVDTP_EVT_SEP_INFO_RECEIVED:                  SEP information received.
 *            @arg AVDTP_EVT_SEP_CAPABILITIES_RECEIVED:          SEP capabilities received.
 *            @arg AVDTP_EVT_STREAM_CONFIGURATION_RECEIVED:      Stream configuration received.
 *
 *            @arg AVDTP_EVT_SET_STREAM_CONFIGURATION_REQUESTED: Remote device requested stream configuration.
 *            @arg AVDTP_EVT_OPEN_STREAM_REQUESTED:              Remote device requested to open a stream.
 *            @arg AVDTP_EVT_START_STREAM_REQUESTED:             Remote device requested to start a stream.
 *            @arg AVDTP_EVT_CLOSE_STREAM_REQUESTED:             Remote device requested to close a stream.
 *            @arg AVDTP_EVT_SUSPEND_STREAM_REQUESTED:           Remote device requested to suspend a stream.
 *            @arg AVDTP_EVT_ABORT_STREAM_REQUESTED:             Remote device requested to abort a stream.
 *            @arg AVDTP_EVT_RECONFIGURE_STREAM_REQUESTED:       Remote device requested to reconfigure a stream.
 *            @arg AVDTP_EVT_MEDIA_PACKET_RECEIVED:              Remote device sent a media packet.
 *            @arg AVDTP_EVT_STREAM_CONFIGURED:                  A stream has been configured (This event is generated right after AVDTP_EVT_SET_STREAM_CONFIGURATION_REQUESTED if the local devices accepted the request).
 *            @arg AVDTP_EVT_STREAM_RECONFIGURED:                A stream has been re-configured (This event is generated right after AVDTP_EVT_RECONFIGURE_STREAM_REQUESTED if the local devices accepted the request).
 *            @arg AVDTP_EVT_STREAM_OPENED:                      A stream has been opened (This event is generated as a result of local or remote stream opening request).
 *            @arg AVDTP_EVT_STREAM_STARTED:                     A stream has been started (This event is generated right after AVDTP_EVT_START_STREAM_REQUESTED if the local devices accepted the request).
 *            @arg AVDTP_EVT_STREAM_CLOSED:                      A stream has been close (This event is generated right after AVDTP_EVT_CLOSE_STREAM_REQUESTED if the local devices accepted the request).
 *            @arg AVDTP_EVT_STREAM_SUSPENDED:                   A stream has been suspended (This event is generated right after AVDTP_EVT_SUSPEND_STREAM_REQUESTED if the local devices accepted the request).
 *            @arg AVDTP_EVT_STREAM_ABORTED:                     A stream has been aborted (This event is generated right after AVDTP_EVT_SUSPEND_STREAM_REQUESTED 
 *                                                               if the local devices accepted the request. 
 *                                                               It is also generated if connection between devices has been terminated by means other than
 *                                                               AVDTP signaling, e.g. devices going out of rage).
 *            @arg AVDTP_EVT_MEDIA_PACKET_SENT:                  The local device has successfully sent a media packet to the remote device.
 *            @arg AVDTP_EVT_MEDIA_PACKET_SEND_FAILED:           The local device was not able to send a media packet to the remote device.
 *
 * \param evt_param Event parameter.
 *          Which member of the bt_avdtp_event_t union is valid depends on the event:
 *            @arg AVDTP_EVT_CTRL_CHANNEL_CONNECTED:             \c bt_avdtp_evt_ctrl_channel_connected_t               ctrl_channel_connected
 *            @arg AVDTP_EVT_CTRL_CHANNEL_DISCONNECTED:          \c bt_avdtp_evt_ctrl_channel_disconnected_t            ctrl_channel_disconnected
 *            @arg AVDTP_EVT_CTRL_CONNECTION_FAILED:             \c NULL
 *            @arg AVDTP_EVT_DISCOVER_COMPLETED:                 \c bt_avdtp_evt_discover_completed_t                   discover_completed;
 *            @arg AVDTP_EVT_GET_SEP_CAPABILITIES_COMPLETED:     \c bt_avdtp_evt_get_sep_capabilities_completed_t       get_sep_capabilities_completed
 *            @arg AVDTP_EVT_SET_STREAM_CONFIGURATION_COMPLETED: \c bt_avdtp_evt_set_stream_configuration_completed_t   set_stream_configuration_completed
 *            @arg AVDTP_EVT_GET_STREAM_CONFIGURATION_COMPLETED: \c bt_avdtp_evt_get_stream_configuration_completed_t   get_stream_configuration_completed
 *            @arg AVDTP_EVT_STREAM_RECONFIGURE_COMPLETED:       \c bt_avdtp_evt_stream_reconfigure_completed_t         stream_reconfigure_completed
 *            @arg AVDTP_EVT_OPEN_STREAM_COMPLETED:              \c bt_avdtp_evt_open_stream_completed_t                open_stream_completed
 *            @arg AVDTP_EVT_START_STREAM_COMPLETED:             \c bt_avdtp_evt_start_stream_completed_t               start_stream_completed
 *            @arg AVDTP_EVT_CLOSE_STREAM_COMPLETED:             \c bt_avdtp_evt_close_stream_completed_t               close_stream_completed
 *            @arg AVDTP_EVT_SUSPEND_STREAM_COMPLETED:           \c bt_avdtp_evt_suspend_stream_completed_t             suspend_stream_completed
 *            @arg AVDTP_EVT_STREAM_SECURITY_CONTROL_COMPLETED:  \c bt_avdtp_evt_stream_security_control_completed_t    security_control_completed
 *            @arg AVDTP_EVT_ABORT_STREAM_COMPLETED:             \c bt_avdtp_evt_abort_stream_requested_t               abort_stream_requested
 *            @arg AVDTP_EVT_SEP_INFO_RECEIVED:                  \c bt_avdtp_evt_sep_info_received_t                    sep_info_received
 *            @arg AVDTP_EVT_SEP_CAPABILITIES_RECEIVED:          \c bt_avdtp_evt_sep_capabilities_received_t            sep_capabilities_received
 *            @arg AVDTP_EVT_STREAM_CONFIGURATION_RECEIVED:      \c bt_avdtp_evt_sep_capabilities_received_t            sep_capabilities_received
 *
 *            @arg AVDTP_EVT_SET_STREAM_CONFIGURATION_REQUESTED: \c bt_avdtp_evt_set_stream_configuration_requested_t   set_stream_configuration_requested
 *            @arg AVDTP_EVT_OPEN_STREAM_REQUESTED:              \c bt_avdtp_evt_open_stream_requested_t                open_stream_requested
 *            @arg AVDTP_EVT_START_STREAM_REQUESTED:             \c bt_avdtp_evt_start_stream_requested_t               start_stream_requested
 *            @arg AVDTP_EVT_CLOSE_STREAM_REQUESTED:             \c bt_avdtp_evt_close_stream_requested_t               close_stream_requested
 *            @arg AVDTP_EVT_SUSPEND_STREAM_REQUESTED:           \c bt_avdtp_evt_suspend_stream_requested_t             suspend_stream_requested
 *            @arg AVDTP_EVT_ABORT_STREAM_REQUESTED:             \c bt_avdtp_evt_abort_stream_requested_t               abort_stream_requested
 *            @arg AVDTP_EVT_RECONFIGURE_STREAM_REQUESTED:       \c bt_avdtp_evt_reconfigure_stream_requested_t         reconfigure_stream_requested
 *            @arg AVDTP_EVT_MEDIA_PACKET_RECEIVED:              \c bt_avdtp_evt_media_packet_received_t                media_packet_received
 *            @arg AVDTP_EVT_STREAM_CONFIGURED:                  \c bt_avdtp_evt_stream_configured_t                    stream_configured
 *            @arg AVDTP_EVT_STREAM_RECONFIGURED:                \c bt_avdtp_evt_stream_reconfigured_t                  stream_reconfigured
 *            @arg AVDTP_EVT_STREAM_OPENED:                      \c bt_avdtp_evt_stream_opened_t                        stream_opened
 *            @arg AVDTP_EVT_STREAM_STARTED:                     \c bt_avdtp_evt_stream_started_t                       stream_started
 *            @arg AVDTP_EVT_STREAM_CLOSED:                      \c bt_avdtp_evt_stream_closed_t                        stream_closed
 *            @arg AVDTP_EVT_STREAM_SUSPENDED:                   \c bt_avdtp_evt_stream_suspended_t                     stream_suspended
 *            @arg AVDTP_EVT_STREAM_ABORTED:                     \c bt_avdtp_evt_stream_aborted_t                       stream_aborted
 *            @arg AVDTP_EVT_MEDIA_PACKET_SENT:                  \c bt_avdtp_evt_media_packet_sent_t                    media_packet_sent
 *            @arg AVDTP_EVT_MEDIA_PACKET_SEND_FAILED:           \c bt_avdtp_evt_media_packet_send_failed_t             media_packet_send_failed
 *
 * \param callback_param A pointer to an arbitrary data set by a call to bt_avdtp_register_callback.
 * 
 */
typedef void (*bt_avdtp_mgr_callback_fp)(struct _bt_avdtp_mgr_t* mgr, bt_byte evt, bt_avdtp_event_t* evt_param, void* callback_param);

/**
 * \brief AVDTP manager.
 * \ingroup avdtp
 *
 * \details A structure that glues all pieces together. There is only one instance of this structure allocated by dotstack.
 *          A pointer to the instance can be get with \c bt_avdtp_get_mgr().
 *          
 */
typedef struct _bt_avdtp_mgr_t
{
	bt_byte state;                                     ///< Manager state. This value can be one of the following values:
                                                       ///< \li AVDTP_MANAGER_STATE_IDLE
                                                       ///< \li AVDTP_MANAGER_STATE_CONNECTING

	bt_byte flags;                                     ///< Additional manager state. This value can be a combination of the following values:
                                                       ///< \li AVDTP_MANAGER_FLAG_SENDING_MEDIA_PACKET

	bt_avdtp_sep_t* seps;                              ///< Pointer to a buffer of SEPs available for allocating with \c bt_avdtp_register_sep.
	bt_byte next_sep_id;                               ///< Holds ID of the next SEP to be allocated. Every time \c bt_avdtp_register_sep is called this value is increased by 1.

	bt_avdtp_stream_t* streams;                        ///< Pointer to a buffer of streams available for allocating with \c bt_avdtp_create_stream.
	bt_byte next_stream_handle;                        ///< Holds ID of the next stream to be allocated. Every time \c bt_avdtp_create_stream is called this value is increased by 1.

	bt_avdtp_control_channel_t* control_channels;      ///< Pointer to a buffer of available control channles.
	bt_avdtp_transport_channel_t* transport_channels;  ///< Pointer to a buffer of available transport channles.

	bt_avdtp_codec_t* codecs;                          ///< A list of supported codecs.

	bt_avdtp_mgr_callback_fp callback;                 ///< Pointer to a function which a AVDTP consumer must register in order to be notified of various events.
	void* callback_param;                              ///< Pointer to arbitrary data to be passed to the \c callback.

	bt_avdtp_control_cmd_t* pending_command;           ///< If local device wants to send a command to a remote device but control channel does not exists this member holds a pointer to the command until the channel is created.

	bt_avdtp_stream_t* opening_strm;                   ///< Holds a pointer to a stream being opened by a remote device. 
} bt_avdtp_mgr_t;


/**
 * \brief Return a pointer to an instance of the AVDTP manager.
 * \ingroup avdtp
 *
 * \details This function returns a pointer to an instance of the AVDTP manager. There is only one instance of the manager allocated by the stack. 
 *          The pointer is passed as the first parameter to all AVDTP functions.
 */
bt_avdtp_mgr_t* bt_avdtp_get_mgr(void);

/**
 * \brief Initialize the AVDTP layer.
 * \ingroup avdtp
 *
 * \details This function initializes the AVDTP layer of the stack. It must be called prior to any other
 * AVDTP function can be called.
 */
void bt_avdtp_init(void);

/**
 * \brief Start the AVDTP layer.
 * \ingroup avdtp
 *
 * \details This function makes the AVDTP layer ready to accept connection requests from remote device. 
 *          To make an outgoing connection calling this function is not required.
 *
 * \param mgr AVDTP manager.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
*/
bt_bool bt_avdtp_start(bt_avdtp_mgr_t* mgr);

/**
 * \brief Register a AVDTP application callback.
 * \ingroup avdtp
 *
 * \details In order to be notified of various events a consumer of the AVDTP layer has to register a callback function. 
 *          The stack will call this function whenever a new event has been generated passing the code of the event as the second parameter.
 *          The event can be one of the following values:
 * 
 *    @arg AVDTP_EVT_CTRL_CHANNEL_CONNECTED:             Control channel connected.
 *    @arg AVDTP_EVT_CTRL_CHANNEL_DISCONNECTED:          Control channel disconnected.
 *    @arg AVDTP_EVT_CTRL_CONNECTION_FAILED:             Control channel connection failed (generated only if control connection has been initiated by the local device).
 *    @arg AVDTP_EVT_DISCOVER_COMPLETED:                 Local device completed discovering remote SEPs.
 *    @arg AVDTP_EVT_GET_SEP_CAPABILITIES_COMPLETED:     Local device received a response to Get SEP capabilities operation.
 *    @arg AVDTP_EVT_SET_STREAM_CONFIGURATION_COMPLETED: Local device received a response to Set stream configuration operation.
 *    @arg AVDTP_EVT_GET_STREAM_CONFIGURATION_COMPLETED: Local device received a response to Get stream configuration operation.
 *    @arg AVDTP_EVT_STREAM_RECONFIGURE_COMPLETED:       Local device received a response to Reconfigure stream operation.
 *    @arg AVDTP_EVT_OPEN_STREAM_COMPLETED:              Local device received a response to Open stream operation.
 *    @arg AVDTP_EVT_START_STREAM_COMPLETED:             Local device received a response to Start stream operation.
 *    @arg AVDTP_EVT_CLOSE_STREAM_COMPLETED:             Local device received a response to Close stream operation.
 *    @arg AVDTP_EVT_SUSPEND_STREAM_COMPLETED:           Local device received a response to Suspend stream operation.
 *    @arg AVDTP_EVT_STREAM_SECURITY_CONTROL_COMPLETED:  Local device received a response to Stream security control operation.
 *    @arg AVDTP_EVT_ABORT_STREAM_COMPLETED:             Local device received a response to Abort stream operation.
 *    @arg AVDTP_EVT_SEP_INFO_RECEIVED:                  SEP information received.
 *    @arg AVDTP_EVT_SEP_CAPABILITIES_RECEIVED:          SEP capabilities received.
 *    @arg AVDTP_EVT_STREAM_CONFIGURATION_RECEIVED:      Stream configuration received.
 *
 *    @arg AVDTP_EVT_SET_STREAM_CONFIGURATION_REQUESTED: Remote device requested stream configuration.
 *    @arg AVDTP_EVT_OPEN_STREAM_REQUESTED:              Remote device requested to open a stream.
 *    @arg AVDTP_EVT_START_STREAM_REQUESTED:             Remote device requested to start a stream.
 *    @arg AVDTP_EVT_CLOSE_STREAM_REQUESTED:             Remote device requested to close a stream.
 *    @arg AVDTP_EVT_SUSPEND_STREAM_REQUESTED:           Remote device requested to suspend a stream.
 *    @arg AVDTP_EVT_ABORT_STREAM_REQUESTED:             Remote device requested to abort a stream.
 *    @arg AVDTP_EVT_RECONFIGURE_STREAM_REQUESTED:       Remote device requested to reconfigure a stream.
 *    @arg AVDTP_EVT_MEDIA_PACKET_RECEIVED:              Remote device sent a media packet.
 *    @arg AVDTP_EVT_STREAM_CONFIGURED:                  A stream has been configured (This event is generated right after AVDTP_EVT_SET_STREAM_CONFIGURATION_REQUESTED if the local devices accepted the request).
 *    @arg AVDTP_EVT_STREAM_RECONFIGURED:                A stream has been re-configured (This event is generated right after AVDTP_EVT_RECONFIGURE_STREAM_REQUESTED if the local devices accepted the request).
 *    @arg AVDTP_EVT_STREAM_OPENED:                      A stream has been opened (This event is generated as a result of local or remote stream opening request).
 *    @arg AVDTP_EVT_STREAM_STARTED:                     A stream has been started (This event is generated right after AVDTP_EVT_START_STREAM_REQUESTED if the local devices accepted the request).
 *    @arg AVDTP_EVT_STREAM_CLOSED:                      A stream has been close (This event is generated right after AVDTP_EVT_CLOSE_STREAM_REQUESTED if the local devices accepted the request).
 *    @arg AVDTP_EVT_STREAM_SUSPENDED:                   A stream has been suspended (This event is generated right after AVDTP_EVT_SUSPEND_STREAM_REQUESTED if the local devices accepted the request).
 *    @arg AVDTP_EVT_STREAM_ABORTED:                     A stream has been aborted (This event is generated right after AVDTP_EVT_SUSPEND_STREAM_REQUESTED 
 *                                                       if the local devices accepted the request. 
 *                                                       It is also generated if connection between devices has been terminated by means other than
 *                                                       AVDTP signaling, e.g. devices going out of rage).
 *    @arg AVDTP_EVT_MEDIA_PACKET_SENT:                  The local device has successfully sent a media packet to the remote device.
 *    @arg AVDTP_EVT_MEDIA_PACKET_SEND_FAILED:           The local device was not able to send a media packet to the remote device.
 *
 * \param mgr AVDTP manager.
 * \param callback The callback function that will be called when the AVDTP generates an event.
 * \param callback_param A pointer to arbitrary data to be passed to the \c callback callback.
 * 
 */
void bt_avdtp_register_callback(bt_avdtp_mgr_t* mgr, bt_avdtp_mgr_callback_fp callback, void* callback_param);

/**
 * \brief Register a SEP with the local AVDTP manager.
 * \ingroup avdtp
 *
 * \details This function is used to make a list of SEPs supported by the local ADVTP entity. 
 *
 * \param mgr AVDTP manager.
 * \param type The type of a SEP.
 *        The type can be one of the following values:
 *            @arg AVDTP_SEP_TYPE_SOURCE: The SEP is a source.
 *            @arg AVDTP_SEP_TYPE_SINK:   The SEP is a sink.
 * \param caps The capabilities of a SEP.
 * 
 * \return
 *        \li \c ID of a SEP if the function succeeds.
 *        \li \c FALSE otherwise.
*/
bt_byte bt_avdtp_register_sep(bt_avdtp_mgr_t* mgr, bt_byte type, const bt_avdtp_sep_capabilities_t* caps);

/**
 * \brief Get a SEP info by its ID.
 * \ingroup avdtp
 *
 * \details This function returns a pointer to bt_avdtp_sep_t structure that describes a SEP previously registered with bt_avdtp_register_sep.
 *
 * \param mgr AVDTP manager.
 * \param sep_id The ID of a SEP.
 * 
 * \return
 *        \li \c Pointer to bt_avdtp_sep_t if the SEP is in the list of registered SEPs.
 *        \li \c NULL otherwise.
*/
bt_avdtp_sep_t* bt_avdtp_get_sep(bt_avdtp_mgr_t* mgr, bt_byte sep_id);

/**
 * \brief Connect to a remote device.
 * \ingroup avdtp
 *
 * \details This function opens a control channel connection to a remote device specified by the \p remote_addr. If connection
 *          cannot be initiated for some reason, for example, there is not enough resources, it returns FALSE and not events are generated. Otherwise
 *          the result of an attempt to connect to the remote device is reported via the AVDTP callback. 
 *          The events generated will either be AVDTP_EVT_CTRL_CHANNEL_CONNECTED or AVDTP_EVT_CTRL_CHANNEL_CONNECTION_FAILED.
 *
 * \param mgr AVDTP manager.
 * \param remote_addr The address of a remote device.
 * 
 * \return
 *        \li \c TRUE if connection establishment has been started.
 *        \li \c FALSE otherwise.
*/
#define bt_avdtp_connect(mgr, remote_addr)	_bt_avdtp_open_control_channel_ex(mgr, remote_addr, HCI_CONFIG_ENABLE_AUTHENTICATION | HCI_CONFIG_ENABLE_ENCRYPTION)

/**
 * \brief Connect to a remote device.
 * \ingroup avdtp
 *
 * \details This function opens a control channel connection to a remote device specified by the \p remote_addr. If connection
 *          cannot be initiated for some reason, for example, there is not enough resources, it returns FALSE and not events are generated. Otherwise
 *          the result of an attempt to connect to the remote device is reported via the AVDTP callback. 
 *          The events generated will either be AVDTP_EVT_CTRL_CHANNEL_CONNECTED or AVDTP_EVT_CTRL_CHANNEL_CONNECTION_FAILED.
 *
 * \param mgr AVDTP manager.
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
#define bt_avdtp_connect_ex(mgr, remote_addr, acl_config)	_bt_avdtp_open_control_channel_ex(mgr, remote_addr, acl_config)

/**
 * \brief Disconnect from a remote device.
 * \ingroup avdtp
 *
 * \details This function closes a control and transport channels on all streams associated with the remote device specified by the \p remote_addr. 
 *          As a result of this operation the following events will be generated:
 *            @arg AVDTP_EVT_MEDIA_PACKET_RECEIVED: if a stream's receive queue is not empty this event is generated for each buffer with bt_media_packet_t::data_len set to 0
 *            @arg AVDTP_EVT_MEDIA_PACKET_SENT:     if a stream's send queue is not empty this event is generated for each buffer with bt_media_packet_t::data_len set to 0
 *            @arg AVDTP_EVT_STREAM_CLOSED:         this event is generate if a stream is in "closing" state as a result of a request from the remote device or 
 *                                             bt_avdtp_close_stream call before bt_avdtp_disconnect call
 *            @arg AVDTP_EVT_STREAM_ABORTED:        this event is generated if a stream is in "active" state at the time of bt_avdtp_disconnect call.
 *
 * \param mgr AVDTP manager.
 * \param remote_addr The address of a remote device.
 * 
 * \return
 *        \li \c TRUE if disconnection has been started.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_avdtp_disconnect(bt_avdtp_mgr_t* mgr, bt_bdaddr_t* remote_addr);

/**
 * \brief Discover SEPs on a remote device.
 * \ingroup avdtp
 *
 * \details This function asks the remote device to send a list of all available SEPs.
 *          As a result of this operation the following events will be generated:
 *            @arg AVDTP_EVT_SEP_INFO_RECEIVED:   this event is generated for every SEP received from the remote device. the \p evt_param.sep_info_received contains SEP information.
 *            @arg AVDTP_EVT_DISCOVER_COMPLETED:  this event is generated after last AVDTP_EVT_SEP_INFO_RECEIVED if the remote accepted the request and the \p evt_param.discover_completed.err_code == AVDTP_ERROR_SUCCESS.
 *                                           if the remote rejected the request the \p evt_param.discover_completed.err_code == the error code sent by the remote.
 *
 * \param mgr AVDTP manager.
 * \param remote_addr The address of a remote device.
 * 
 * \return
 *        \li \c TRUE if discover request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_avdtp_discover(bt_avdtp_mgr_t* mgr, bt_bdaddr_t* remote_addr);

/**
 * \brief Get remote SEP capabilities.
 * \ingroup avdtp
 *
 * \details This function asks the remote device to send capabilities of a SEP specified by the \p seid_acp.
 *          As a result of this operation the following events will be generated:
 *            @arg AVDTP_EVT_SEP_CAPABILITIES_RECEIVED:       this event is generated if the remote device accepted the request. the \p evt_param.sep_capabilities_received contains SEP capabilities.
 *            @arg AVDTP_EVT_GET_SEP_CAPABILITIES_COMPLETED:  this event is generated right after AVDTP_EVT_SEP_CAPABILITIES_RECEIVED 
 *                                                       if the remote accepted the request the \p evt_param.get_sep_capabilities_completed.err_code == AVDTP_ERROR_SUCCESS.
 *                                                       if the remote rejected the request the \p evt_param.get_sep_capabilities_completed.err_code == the error code sent by the remote.
 *
 * \param mgr AVDTP manager.
 * \param remote_addr The address of a remote device.
 * \param seid_acp The ID of a remote SEP.
 * 
 * \return
 *        \li \c TRUE if discover request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_avdtp_get_capabilities(bt_avdtp_mgr_t* mgr, bt_bdaddr_t* remote_addr, bt_byte seid_acp);

/**
 * \brief Get remote SEP capabilities.
 * \ingroup avdtp
 *
 * \details This function asks the remote device to send capabilities of a SEP specified by the \p seid_acp.
 *          As a result of this operation the following events will be generated:
 *            @arg AVDTP_EVT_SEP_CAPABILITIES_RECEIVED:       this event is generated if the remote device accepted the request. the \p evt_param.sep_capabilities_received contains SEP capabilities.
 *            @arg AVDTP_EVT_GET_SEP_CAPABILITIES_COMPLETED:  this event is generated right after AVDTP_EVT_SEP_CAPABILITIES_RECEIVED 
 *                                                       if the remote accepted the request the \p evt_param.get_sep_capabilities_completed.err_code == AVDTP_ERROR_SUCCESS.
 *                                                       if the remote rejected the request the \p evt_param.get_sep_capabilities_completed.err_code == the error code sent by the remote.
 *
 * \param mgr AVDTP manager.
 * \param remote_addr The address of a remote device.
 * \param seid_acp The ID of a remote SEP.
 * 
 * \return
 *        \li \c TRUE if discover request has been sent.
 *        \li \c FALSE otherwise. No events will be generated.
*/
bt_bool bt_avdtp_get_all_capabilities(bt_avdtp_mgr_t* mgr, bt_bdaddr_t* remote_addr, bt_byte seid_acp);

/**
 * \brief Create a stream.
 * \ingroup avdtp
 *
 * \details This function allocates memory for storing stream's data and assigns a stream handle. 
 *          The stream handle is used to manipulate the stream - open, close, configure, suspend, abort.
 *
 * \param mgr AVDTP manager.
 * 
 * \return
 *        \li \c Stream handle if the function succeeds.
 *        \li \c 0 otherwise.
*/
bt_byte bt_avdtp_create_stream(bt_avdtp_mgr_t* mgr);

/**
 * \brief Destroy a stream.
 * \ingroup avdtp
 *
 * \details This function frees memory used by the stream. The stream has to exist and be in the "idle" state for this function to succeed. 
 *          I.e. the stream has to be closed or aborted before this function can be called.
 *
 * \param mgr AVDTP manager.
 * \param strm_handle Stream handle.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
*/
bt_bool bt_avdtp_destroy_stream(bt_avdtp_mgr_t* mgr, bt_byte strm_handle);

/**
 * \brief Listen for incoming connections.
 * \ingroup avdtp
 *
 * \details This function tells a stream that it can use a particular SEP to accept incoming requests to open it. The SEP can be associated
 *          with multiple streams but used with only one. The stream has to be closed before the SEP can be used with another stream.
 *          For outgoing connections this is not needed. Any SEP can be used with any stream given that the SEP is not already in use by
 *          another stream.
 *
 * \param mgr AVDTP manager.
 * \param strm_handle Stream handle.
 * \param sep_id Local SEP ID.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
*/
bt_bool bt_avdtp_listen(bt_avdtp_mgr_t* mgr, bt_byte strm_handle, bt_byte sep_id);

/**
 * \brief Cancel listening for incoming connections.
 * \ingroup avdtp
 *
 * \details This function removes a SEP from a list of SEPS which a stream can use for incoming requests.
 *
 * \param mgr AVDTP manager.
 * \param strm_handle Stream handle.
 * \param sep_id Local SEP ID.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise.
*/
void bt_avdtp_cancel_listen(bt_avdtp_mgr_t* mgr, bt_byte strm_handle, bt_byte sep_id);

/**
 * \brief Set stream configuration.
 * \ingroup avdtp
 *
 * \details This function tries to configure a stream before opening it.
 *          As a result of this operation the AVDTP_EVT_SET_STREAM_CONFIGURATION_COMPLETED event will be generated. 
 *          If configuration was a success the \p evt_param.set_stream_configuration_completed.err_code == AVDTP_ERROR_SUCCESS.
 *          Otherwise the \p evt_param.set_stream_configuration_completed.err_code == the error code sent by the remote and 
 *          \p evt_param.set_stream_configuration_completed.svc_category == the value of the first Service Category to fail.
 *
 * \param mgr AVDTP manager.
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
bt_bool bt_avdtp_set_configuration(
	bt_avdtp_mgr_t* mgr, bt_byte strm_handle,
	bt_bdaddr_t* remote_addr, bt_byte seid_int, bt_byte seid_acp,
	const bt_avdtp_sep_capabilities_t* caps);

/**
 * \brief Get stream configuration.
 * \ingroup avdtp
 *
 * \details This function requests stream configuration from a remote device.
 *          As a result of this operation the following events will be generated:
 *              @arg AVDTP_EVT_STREAM_CONFIGURATION_RECEIVED:      this event is generated if the remote accepted the request. 
 *                                                            the \p ebt_para.sep_capabilities_received.caps will contain current stream configuration.
 *              @arg AVDTP_EVT_GET_STREAM_CONFIGURATION_COMPLETED: If the remote accepted the request the \p evt_param.get_stream_configuration_completed.err_code == AVDTP_ERROR_SUCCESS.
 *                                                            if the remote rejected the request the \p evt_param.get_stream_configuration_completed.err_code == the error code sent by the remote.
 *
 * \param mgr AVDTP manager.
 * \param strm_handle Stream handle.
 * 
 * \return
 *        \li \c TRUE if the function succeeds, i.e. the actual request has been sent to the remote party.
 *        \li \c FALSE otherwise. No events will be generated.
 */
bt_bool bt_avdtp_get_configuration(bt_avdtp_mgr_t* mgr, bt_byte strm_handle);

/**
 * \brief Reconfigure stream.
 * \ingroup avdtp
 *
 * \details This function tries to change the stream's configuration. For this function to succeed the stream has to be open.
 *          As a result of this operation the AVDTP_EVT_STREAM_RECONFIGURE_COMPLETED event will be generated. 
 *          If reconfiguration was a success the \p evt_param.stream_reconfigure_completed.err_code == AVDTP_ERROR_SUCCESS.
 *          Otherwise the \p evt_param.stream_reconfigure_completed.err_code == the error code sent by the remote.
 *
 * \param mgr AVDTP manager.
 * \param strm_handle Stream handle.
 * \param caps New stream configuration.
 * 
 * \return
 *        \li \c TRUE if the function succeeds, i.e. the actual request has been sent to the remote party.
 *        \li \c FALSE otherwise. No events will be generated.
 */
bt_bool bt_avdtp_reconfigure_stream(
	bt_avdtp_mgr_t* mgr, bt_byte strm_handle,
	bt_avdtp_sep_capabilities_t* caps);

/**
 * \brief Get local stream state.
 * \ingroup avdtp
 *
 * \details This function returns local state of a stream specified by the \p strm_handle. No request is sent to the remote party.
 *
 * \param mgr AVDTP manager.
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
bt_byte bt_avdtp_get_stream_state(bt_avdtp_mgr_t* mgr, bt_byte strm_handle);

/**
 * \brief Get stream's local SEP ID.
 * \ingroup avdtp
 *
 * \details This function returns the ID of the local SEP associated with the stream.
 *
 * \param mgr AVDTP manager.
 * \param strm_handle Stream handle.
 * 
 * \return \li The ID of the local SEP if strm_handle specifies a valid stream. 
 *         \li 0 otherwise.
 */
bt_byte bt_avdtp_get_stream_local_sep_id(bt_avdtp_mgr_t* mgr, bt_byte strm_handle);

/**
 * \brief Get stream's remote SEP ID.
 * \ingroup avdtp
 *
 * \details This function returns the ID of the remote SEP associated with the stream.
 *
 * \param mgr AVDTP manager.
 * \param strm_handle Stream handle.
 * 
 * \return \li The ID of the remote SEP if strm_handle specifies a valid stream. 
 *         \li 0 otherwise.
 */
bt_byte bt_avdtp_get_stream_remote_sep_id(bt_avdtp_mgr_t* mgr, bt_byte strm_handle);

/**
 * \brief Get stream's remote BT address.
 * \ingroup avdtp
 *
 * \details This function returns the address of the remote device associated with the stream.
 *
 * \param mgr AVDTP manager.
 * \param strm_handle Stream handle.
 * 
 * \return \li The address of the remote device if strm_handle specifies a valid stream. 
 *         \li NULL otherwise.
 */
bt_bdaddr_t* bt_avdtp_get_stream_remote_address(bt_avdtp_mgr_t* mgr, bt_byte strm_handle);

/**
 * \brief Get the type of the codec currently used with the stream.
 * \ingroup avdtp
 *
 * \details This function returns the type of the codec currently used with the stream.
 *
 * \param mgr AVDTP manager.
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
bt_byte bt_avdtp_get_stream_codec_type(bt_avdtp_mgr_t* mgr, bt_byte strm_handle);

/**
 * \brief Get the configuration of the codec currently used with the stream.
 * \ingroup avdtp
 *
 * \details This function returns a pointer to a structure that contains configuration of the codec currently used with the stream.
 *          The structure returned depends on the codec. The dotstack defines structures only for SBC, MPEG-1,2 and MPEG-2,4 AAC codecs:
 *              @arg SBC: bt_a2dp_sbc_config_t (defined in a2dp_sbc_codec.h)
 *              @arg MPEG-1,2: bt_a2dp_mpeg_config_t (defined in a2dp_mpeg_codec.h)
 *              @arg MPEG-2,4 AAC: bt_a2dp_aac_config_t (defined in a2dp_aac_codec.h)
 *
 * \param mgr AVDTP manager.
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
void* bt_avdtp_get_stream_codec_config(bt_avdtp_mgr_t* mgr, bt_byte strm_handle);

/**
 * \brief Get stream's configuration.
 * \ingroup avdtp
 *
 * \details This function returns a pointer to a structure holding the current configuration of stream.
 *
 * \param mgr AVDTP manager.
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
bt_avdtp_sep_capabilities_t* bt_avdtp_get_stream_config(bt_avdtp_mgr_t* mgr, bt_byte strm_handle);

/**
 * \brief Open a stream.
 * \ingroup avdtp
 *
 * \details This function tries to open a stream by sending a request to the remote party. The stream has to be already 
 *          configured with a bt_avdtp_set_configuration call.
 *          As a result of this operation the AVDTP_EVT_OPEN_STREAM_COMPLETED event will be generated.
 *          If the stream has been open the \p evt_param.open_stream_completed.err_code == AVDTP_ERROR_SUCCESS.
 *          Otherwise, if the remote device for any reason cannot or does not wish to open the stream,
 *          the \p evt_param.open_stream_completed.err_code == the error code sent by the remote.
 *
 * \param mgr AVDTP manager.
 * \param strm_handle Stream handle.
 * 
 * \return
 *        \li \c TRUE if the function succeeds, i.e. the actual request has been sent to the remote party.
 *        \li \c FALSE otherwise. No events will be generated.
 */
bt_bool bt_avdtp_open_stream(bt_avdtp_mgr_t* mgr, bt_byte strm_handle);

/**
 * \brief Start a stream.
 * \ingroup avdtp
 *
 * \details This function tries to start a stream by sending a request to the remote party. The stream has to be in 
 *          AVDTP_STREAM_STATE_OPEN state. The stream goes to this state as a result of successful configuration or suspension (both
 *          can be initiated by either party).
 *          As a result of this operation the AVDTP_EVT_START_STREAM_COMPLETED event will be generated.
 *          If the stream has been open the \p evt_param.start_stream_requested.err_code == AVDTP_ERROR_SUCCESS.
 *          Otherwise, if the remote device for any reason cannot or does not wish to start the stream,
 *          the \p evt_param.start_stream_requested.err_code == the error code sent by the remote.
 *
 * \param mgr AVDTP manager.
 * \param strm_handle Stream handle.
 * 
 * \return
 *        \li \c TRUE if the function succeeds, i.e. the actual request has been sent to the remote party.
 *        \li \c FALSE otherwise. No events will be generated.
 */
bt_bool bt_avdtp_start_stream(bt_avdtp_mgr_t* mgr, bt_byte strm_handle);

/**
 * \brief Close a stream.
 * \ingroup avdtp
 *
 * \details This function tries to close a stream by sending a request to the remote party. The stream has to be in 
 *          AVDTP_STREAM_STATE_OPEN or AVDTP_STREAM_STATE_STREAMING state.
 *          As a result of this operation the AVDTP_EVT_CLOSE_STREAM_COMPLETED event will be generated.
 *          If the stream has been closed the \p evt_param.bt_avdtp_evt_close_stream_completed_t.err_code == AVDTP_ERROR_SUCCESS.
 *          Otherwise, if the remote device for any reason cannot or does not wish to close the stream,
 *          the \p evt_param.bt_avdtp_evt_close_stream_completed_t.err_code == the error code sent by the remote.
 *
 * \param mgr AVDTP manager.
 * \param strm_handle Stream handle.
 * 
 * \return
 *        \li \c TRUE if the function succeeds, i.e. the actual request has been sent to the remote party.
 *        \li \c FALSE otherwise. No events will be generated.
 */
bt_bool bt_avdtp_close_stream(bt_avdtp_mgr_t* mgr, bt_byte strm_handle);

/**
 * \brief Suspend a stream.
 * \ingroup avdtp
 *
 * \details This function tries to suspend a stream by sending a request to the remote party. The stream has to be in 
 *          AVDTP_STREAM_STATE_STREAMING state.
 *          As a result of this operation the AVDTP_EVT_SUSPEND_STREAM_COMPLETED event will be generated.
 *          If the stream has been suspended the \p evt_param.bt_avdtp_evt_suspend_stream_requested_t.err_code == AVDTP_ERROR_SUCCESS.
 *          Otherwise, if the remote device for any reason cannot or does not wish to suspend the stream,
 *          the \p evt_param.bt_avdtp_evt_suspend_stream_requested_t.err_code == the error code sent by the remote.
 *
 * \param mgr AVDTP manager.
 * \param strm_handle Stream handle.
 * 
 * \return
 *        \li \c TRUE if the function succeeds, i.e. the actual request has been sent to the remote party.
 *        \li \c FALSE otherwise. No events will be generated.
 */
bt_bool bt_avdtp_suspend_stream(bt_avdtp_mgr_t* mgr, bt_byte strm_handle);

/**
 * \brief Suspend a stream.
 * \ingroup avdtp
 *
 * \details This function tries to suspend a stream by sending a request to the remote party. The stream can be in any state state except AVDTP_STREAM_STATE_IDLE.
 *          As a result of this operation the AVDTP_EVT_ABORT_STREAM_COMPLETED event will be generated.
 *          This operation cannot be rejected. The \p evt_param.abort_stream_requested.err_code is always == AVDTP_ERROR_SUCCESS.
 *
 * \param mgr AVDTP manager.
 * \param strm_handle Stream handle.
 * 
 * \return
 *        \li \c TRUE if the function succeeds, i.e. the actual request has been sent to the remote party.
 *        \li \c FALSE otherwise. No events will be generated.
 */
bt_bool bt_avdtp_abort_stream(bt_avdtp_mgr_t* mgr, bt_byte strm_handle);

/**
 * \brief Exchange content protection control data.
 * \ingroup avdtp
 *
 * \details This function tries to establish content protection by sending a request to the remote party. 
 *          The stream can be in any state state except AVDTP_STREAM_STATE_IDLE, AVDTP_STREAM_STATE_CLOSING, AVDTP_STREAM_STATE_ABORTING.
 *          As a result of this operation the AVDTP_EVT_STREAM_SECURITY_CONTROL_COMPLETED event will be generated.
 *          If the stream's content protection data has been accepted by the remote party 
 *          the \p evt_param.security_control_completed.err_code == AVDTP_ERROR_SUCCESS.
 *          Otherwise the \p evt_param.security_control_completed.err_code == the error code sent by the remote.
 *
 * \note    The dotstack does not support content protection. Although the request can be sent it will not affect the operation of the AVDTP
 *          in any way.
 *
 * \param mgr AVDTP manager.
 * \param strm_handle Stream handle.
 * 
 * \return
 *        \li \c TRUE if the function succeeds, i.e. the actual request has been sent to the remote party.
 *        \li \c FALSE otherwise. No events will be generated.
 */
bt_bool bt_avdtp_security_control(bt_avdtp_mgr_t* mgr, bt_byte strm_handle, bt_byte* sc_data, bt_byte sc_data_len);

/**
 * \brief Report delay value of a Sink to a Source.
 * \ingroup avdtp
 *
 * \details This function sends the delay value of a Sink to a Source. 
 *          This enables synchronous playback of audio and video. Delay reports are always sent from the Sink to the Source.
 *          If the Sink's delay report has been accepted by the Source
 *          the \p evt_param.delay_report_completed.err_code == AVDTP_ERROR_SUCCESS.
 *          Otherwise the \p evt_param.delay_report_completed.err_code == the error code sent by the Source.
 *
 *
 * \param mgr AVDTP manager.
 * \param strm_handle Stream handle.
 * \param delay The delay value in 1/10 milliseconds.
 * 
 * \return
 *        \li \c TRUE if the function succeeds, i.e. the actual request has been sent to the remote party.
 *        \li \c FALSE otherwise. No events will be generated.
 */
bt_bool bt_avdtp_report_delay(bt_avdtp_mgr_t* mgr, bt_byte strm_handle, bt_uint delay);

/**
 * \brief Register a codec
 * \ingroup avdtp
 *
 * \details AVDTP in theory can support any type of codec. Each codec uses its own format for exchanging capabilities and configuration information. 
 *          In order to make out implementation do not care about these formats we use a simple way of telling AVDTP how to parse and serialize 
 *          codec's configuration. The consumer of AVDTP (e.g. A2DP) for each codec it wishes to support has to register a callback 
 *          function (one per codec type). That callback has to perform two function. The first one is to read the configuration received from 
 *          the remote device and store it in a structure defined by the consumer. The second one is to serialize the data from a structure 
 *          to a format (in case of standard A2DP codecs the format is defined in A2DP specification, vendor specific codecs can define
 *          their own formats) suitable for sending as a part of a AVDTP request.
 *          This function adds a codec's callback function to an internal list.
 *
 *
 * \param mgr AVDTP manager.
 * \param codec Pointer to a structure specifying codec type and a callback.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise. The function fails if there already is a callback for a codec type specified in the \p codec parameter.
 */
bt_bool bt_avdtp_register_codec(bt_avdtp_mgr_t* mgr, bt_avdtp_codec_t* codec);

/**
 * \brief Unregister a codec
 * \ingroup avdtp
 *
 * \details AVDTP in theory can support any type of codec. Each codec uses its own format for exchanging capabilities and configuration information. 
 *          In order to make out implementation do not care about these formats we use a simple way of telling AVDTP how to parse and serialize 
 *          codec's configuration. The consumer of AVDTP (e.g. A2DP) for each codec it wishes to support has to register a callback 
 *          function (one per codec type). That callback has to perform two function. The first one is to read the configuration received from 
 *          the remote device and store it in a structure defined by the consumer. The second one is to serialize the data from a structure 
 *          to a format (in case of standard A2DP codecs the format is defined in A2DP specification, vendor specific codecs can define
 *          their own formats) suitable for sending as a part of a AVDTP request.
 *          This function removes a codec's callback function from an internal list.
 *
 *
 * \param mgr AVDTP manager.
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
 *        \li has not been previously registered with bt_avdtp_register_codec.
 */
bt_bool bt_avdtp_unregister_codec(bt_avdtp_mgr_t* mgr, bt_byte codec_type);

/**
 * \brief Find a codec
 * \ingroup avdtp
 *
 * \details AVDTP in theory can support any type of codec. Each codec uses its own format for exchanging capabilities and configuration information. 
 *          In order to make out implementation do not care about these formats we use a simple way of telling AVDTP how to parse and serialize 
 *          codec's configuration. The consumer of AVDTP (e.g. A2DP) for each codec it wishes to support has to register a callback 
 *          function (one per codec type). That callback has to perform two function. The first one is to read the configuration received from 
 *          the remote device and store it in a structure defined by the consumer. The second one is to serialize the data from a structure 
 *          to a format (in case of standard A2DP codecs the format is defined in A2DP specification, vendor specific codecs can define
 *          their own formats) suitable for sending as a part of a AVDTP request.
 *          This function returns a pointer to a structure that holds a pointer to a codec's callback function.
 *
 *
 * \param mgr AVDTP manager.
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
 *        \li has not been previously registered with bt_avdtp_register_codec.
 */
bt_avdtp_codec_t* bt_avdtp_find_codec(bt_avdtp_mgr_t* mgr, bt_byte codec_type);

/**
 * \brief Add a media packet buffer to a receive queue
 * \ingroup avdtp
 *
 * \details The consumer of AVDTP is responsible for allocating and supplying AVDTP with buffers used to store received packets. 
 *          AVDTP itself only has a queue for storing pointers to buffers supplied by the consumer. When a packet comes in AVDTP finds
 *          the first buffer large enough to hold the received packet, copies the packet to the buffer and generates 
 *          a AVDTP_EVT_MEDIA_PACKET_RECEIVED event. The consumer then has to process the data in the buffer and return it back to the queue.
 *          If there is no buffers in the queue or none of the buffers is large enough the received packets is dropped. 
 *          Each buffer has a field (data_len) that holds the length of the received buffer. This field is never 0 if the buffer contains
 *          a packet. If a channel closed regardless of what has caused that and there are still buffers in the queue AVDTP generates
 *          a AVDTP_EVT_MEDIA_PACKET_RECEIVED event for each buffer and sets the data_len to 0. This is to inform the AVDTP consumer that
 *          the buffer has not been used and can be, for example, deallocated.
 *          This function adds a buffer to the receive queue.
 *
 *
 * \param mgr AVDTP manager.
 * \param strm_handle Stream handle.
 * \param buffer Pointer to a structure that holds the buffer and its parameters.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise. The function fails only if a stream specified by the \p strm_handle parameter 
 *        \li does not exist. The stream can be in any state to call this function.
 */
bt_bool bt_avdtp_add_media_rx_buffer(bt_avdtp_mgr_t* mgr, bt_byte strm_handle, bt_media_packet_t* buffer);

/**
 * \brief Remove a media packet buffer from a receive queue
 * \ingroup avdtp
 *
 * \details The consumer of AVDTP is responsible for allocating and supplying AVDTP with buffers used to store received packets. 
 *          AVDTP itself only has a queue for storing pointers to buffers supplied by the consumer. When a packet comes in AVDTP finds
 *          the first buffer large enough to hold the received packet, copies the packet to the buffer and generates 
 *          a AVDTP_EVT_MEDIA_PACKET_RECEIVED event. The consumer then has to process the data in the buffer and return it back to the queue.
 *          If there is no buffers in the queue or none of the buffers is large enough the received packets is dropped. 
 *          Each buffer has a field (data_len) that holds the length of the received buffer. This field is never 0 if the buffer contains
 *          a packet. If a channel closed regardless of what has caused that and there are still buffers in the queue AVDTP generates
 *          a AVDTP_EVT_MEDIA_PACKET_RECEIVED event for each buffer and sets the data_len to 0. This is to inform the AVDTP consumer that
 *          the buffer has not been used and can be, for example, deallocated.
 *          This function removes a buffer from the receive queue.
 *
 *
 * \param mgr AVDTP manager.
 * \param strm_handle Stream handle.
 * \param buffer Pointer to a structure that holds the buffer and its parameters.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise. The function fails only if a stream specified by the \p strm_handle parameter 
 *        \li does not exist. The stream can be in any state to call this function.
 */
bt_bool bt_avdtp_remove_media_rx_buffer(bt_avdtp_mgr_t* mgr, bt_byte strm_handle, bt_media_packet_t* buffer);

/**
 * \brief Add a media packet buffer to a send queue
 * \ingroup avdtp
 *
 * \details When the consumer of AVDTP wants to send a packet to a remote device it calls this function. The function adds the packet to a queue
 *          and tells AVDTP that it has something to send. The packet will be send as soon as the stream goes to AVDTP_STREAM_STATE_STREAMING
 *          state. When the packet has been successfully sent a AVDTP_EVT_MEDIA_PACKET_SENT is generated. Otherwise a AVDTP_EVT_MEDIA_PACKET_SEND_FAILED
 *          is generated. Regardless of the event generated the consumer can re-use the buffer as AVDTP has removed it from the queue and gave up control
 *          over it. As in the case of received buffers, if a channel closed regardless of what has caused that and there are still buffers in the queue AVDTP generates
 *          a AVDTP_EVT_MEDIA_PACKET_SENT event for each buffer and sets the data_len field to 0. This is to inform the AVDTP consumer that
 *          the buffer has not been used and can be, for example, deallocated.
 *
 *
 * \param mgr AVDTP manager.
 * \param strm_handle Stream handle.
 * \param buffer Pointer to a structure that holds the buffer and its parameters.
 * 
 * \return
 *        \li \c TRUE if the function succeeds.
 *        \li \c FALSE otherwise. The function fails only if a stream specified by the \p strm_handle parameter 
 *        \li does not exist. The stream can be in any state to call this function.
 */
bt_bool bt_avdtp_add_media_tx_buffer(bt_avdtp_mgr_t* mgr, bt_byte strm_handle, bt_media_packet_t* buffer);

/**
 * \brief Remove a media packet buffer from a send queue
 * \ingroup avdtp
 *
 * \details When the consumer of AVDTP wants to send a packet to a remote device it calls bt_avdtp_add_media_tx_buffer function. 
 *          The function adds the packet to a queue and tells AVDTP that it has something to send. The packet will be send as soon as the stream goes 
 *          to AVDTP_STREAM_STATE_STREAMING state. The consumer has a chance to remove a packet from the queue before it has been sent to a remote device
 *          by calling bt_avdtp_remove_media_tx_buffer.
 *
 *
 * \param mgr AVDTP manager.
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
bt_bool bt_avdtp_remove_media_tx_buffer(bt_avdtp_mgr_t* mgr, bt_byte strm_handle, bt_media_packet_t* buffer);

/**
 * \brief Get HCI connection for a stream
 * \ingroup avdtp
 *
 * \details This function returns a pointer to a structure that describes an HCI connection a stream is open on. 
 *          The return value can be used to call various function from the HCI layer. For example, if an app
 *          wants to force disconnection from a remote device it can call bt_hci_disconnect.
 *
 * \param mgr AVDTP manager.
 * \param strm_handle Stream handle.
 * 
 * \return
 *        \li \c Pointer to a structure that describes an HCI connection if the function succeeds.
 *        \li \c NULL otherwise. The function fails only if a stream specified by the \p strm_handle parameter 
 *        \li does not exist or there is no HCI connection between local and remote devices associated with the stream.
 *
 * \note This function has not been implemented.
 */
bt_hci_conn_state_t* bt_avdtp_get_hci_connection(bt_avdtp_mgr_t* mgr, bt_byte strm_handle);

bt_l2cap_channel_t* bt_avdtp_get_l2cap_channel(bt_avdtp_mgr_t* mgr, bt_byte strm_handle, bt_byte transport_session);

#ifdef __cplusplus
}
#endif


#include "cdbt/avdtp/avdtp_private.h"

#endif // __AVDTP_H
