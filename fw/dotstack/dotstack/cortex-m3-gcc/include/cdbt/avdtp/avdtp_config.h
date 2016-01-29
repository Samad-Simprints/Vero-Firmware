/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/


#ifndef __AVDTP_CONFIG_H
#define __AVDTP_CONFIG_H

#include "cdbt/avdtp/avdtp.h"

/**
 * \defgroup avdtp_config Configuration
 * \ingroup avdtp
 *
 * This module describes parameters used to configure AVDTP layer.
 *
 * dotstack is customized using a configuration file. The configuration file tailors the dotstack to the application being built. It has to have the structure shown below.
 * 
    \code
	#include "cdbt/bt/bt_std.h"

	// HCI, L2CAP and SDP must always be present

	// HCI configuration parameters
	#define HCI_MAX_CMD_BUFFERS            ...
	#define HCI_MAX_DATA_BUFFERS           ...
	#define HCI_MAX_HCI_CONNECTIONS        ...
	#define HCI_RX_BUFFER_LEN              ...
	#define HCI_TX_BUFFER_LEN              ...
	#define HCI_L2CAP_BUFFER_LEN           ...
	#define HCI_MAX_CMD_PARAM_LEN          ...

	// L2CAP configuration parameters
	#define L2CAP_MAX_CMD_BUFFERS          ...
	#define L2CAP_MAX_FRAME_BUFFERS        ...
	#define L2CAP_MAX_PSMS                 ...
	#define L2CAP_MAX_CHANNELS             ...

	// SDP configuration parameters
	#define SDP_MAX_SEARCH_RESULT_LEN	   ...
	#define SDP_MAX_ATTRIBUTE_RESULT_LEN   ...

	// Depending on protocols and profiles used below goes configuration parameters
	// for each used module. E.g., to use and configure AVDTP & A2DP (this one does not need configuration),
	// the following values must be defined:
	
	#define BT_INCLUDE_AVDTP               // tells dotstack to compile in AVDTP support
	#define AVDTP_MAX_SEP                  ...
	#define AVDTP_MAX_STREAMS              ...
	#define AVDTP_MAX_REMOTE_DEVICES       ...
	#define AVDTP_MAX_CMD_BUFFERS          ...
	#define AVDTP_MAX_TRANSPORT_CHANNELS   ...
	#define AVDTP_MAX_TX_BUFFER_LEN        ...
	#define AVDTP_MAX_CMD_PARAM_LEN        ...
	#define AVDTP_CODEC_CONFIG_BUFFER_LEN  ...

	#include "cdbt/bt/bt_oem_config.h"

    \endcode
 *
*/

#ifdef _DEBUG

#define AVDTP_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\
	const bt_uint _ram_size_avdtp_buffers =	\
		sizeof(_avdtp_seps) +	\
		sizeof(_avdtp_max_seps) +	\
		sizeof(_avdtp_streams) +	\
		sizeof(_avdtp_max_streams) +	\
		sizeof(_avdtp_control_channels) +	\
		sizeof(_avdtp_max_control_channels) +	\
		sizeof(_avdtp_cmd_buffer_headers) +	\
		sizeof(_avdtp_cmd_buffers) +	\
		sizeof(_avdtp_max_cmd_buffers) +	\
		sizeof(_avdtp_cmd_param_buffers) +	\
		sizeof(_avdtp_max_cmd_param_len) +	\
		sizeof(_avdtp_transport_channels) +	\
		sizeof(_avdtp_max_transport_channels) +	\
		sizeof(_avdtp_max_tx_buffer_len) + \
		sizeof(_avdtp_tx_buffers) + \
		sizeof(_avdtp_codec_cfg_buffers) + \
		sizeof(_avdtp_max_codec_config_buffer_len) +	\
		sizeof(_avdtp_sep_cfg_buffer_headers) +	\
		sizeof(_avdtp_sep_cfg_buffers) +	\
		sizeof(_avdtp_rcv_sep_caps) +	\
		sizeof(_avdtp_rcv_sep_codec_cfg_buffer) +	\
		sizeof(_avdtp_listen_sep_buffers) +	\
		sizeof(_avdtp_l2cap_media_packet_buffer);

#else
	#define AVDTP_ALLOCATE_BUFFERS_RAM_SIZE_VAR
#endif

#ifndef AVDTP_MAX_SEP
#error "AVDTP_MAX_SEP is not defined"

/**
* \brief  Maximum number of SEPs that can be exposed by a local device
* \ingroup avdtp_config
*
* \details This parameter defines the number of SEPs an application can expose to remote devices.
*          The minimum value is 1. The maximum value is 255.
*/
#define AVDTP_MAX_SEP

#endif

#ifndef AVDTP_MAX_STREAMS
#error "AVDTP_MAX_STREAMS is not defined"

/**
* \brief  Maximum number of streams that can be exposed by a local device
* \ingroup avdtp_config
*
* \details This parameter defines the number of streams an application can open between local and remote devices.
*          This value can be different from AVDTP_MAX_SEP but should not exceed it. Since each SEP can only be used once 
*          the local device can only have as much streams as there are SEPs. If AVDTP_MAX_STREAMS > AVDTP_MAX_SEP all memory
*          reserved for streams in excess of AVDTP_MAX_SEP will be wasted. 
*          The minimum value is 1. The maximum value is 255.
*/
#define AVDTP_MAX_STREAMS
#endif

#if defined(AVDTP_WITH_REPORTING) && defined(AVDTP_WITH_RECOVERY)
	#if L2CAP_MAX_CHANNELS < 2 + AVDTP_MAX_STREAMS * 4
	#error "L2CAP_MAX_CHANNELS mus be at least 2 + AVDTP_MAX_STREAMS * 4"
	#endif
#elif defined(AVDTP_WITH_REPORTING) || defined(AVDTP_WITH_RECOVERY)
	#if L2CAP_MAX_CHANNELS < 2 + AVDTP_MAX_STREAMS * 3
	#error "L2CAP_MAX_CHANNELS mus be at least 2 + AVDTP_MAX_STREAMS * 3"
	#endif
#else
	#if L2CAP_MAX_CHANNELS < 2 + AVDTP_MAX_STREAMS * 2
	#error "L2CAP_MAX_CHANNELS mus be at least 2 + AVDTP_MAX_STREAMS * 2"
	#endif
#endif


#ifndef AVDTP_MAX_REMOTE_DEVICES
#error "AVDTP_MAX_REMOTE_DEVICES is not defined"

/**
* \brief  Maximum number of remote devices a local device can be connected to
* \ingroup avdtp_config
*
* \details This parameter defines the number of remote devices a local device can have simultaneous connections to (i.e. control channels).
*          This value should not exceed AVDTP_MAX_STREAMS. For each remote device AVDTP creates one control channel regardless 
*          of the number of streams between the local and the remote devices. Assuming that the local devices wants to have only one channel 
*          with each remote device and if AVDTP_MAX_REMOTE_DEVICES > AVDTP_MAX_STREAMS all memory
*          reserved for devices in excess of AVDTP_MAX_STREAMS will be wasted. 
*          The minimum value is 1. The maximum value is 255.
*/
#define AVDTP_MAX_REMOTE_DEVICES
#endif

#ifndef AVDTP_MAX_CMD_BUFFERS
#error "AVDTP_MAX_CMD_BUFFERS is not defined"

/**
* \brief  Maximum number of command buffers.
* \ingroup avdtp_config
*
* \details This parameter defines the number of buffers reserved for sending commands to a remote device over its control channel. Each channel
*          uses its own buffers so the total number of buffers is AVDTP_MAX_REMOTE_DEVICES * AVDTP_MAX_CMD_BUFFERS.
*          The minimum value is 1. The maximum value is 255. 2 is usually sufficient.
*/
#define AVDTP_MAX_CMD_BUFFERS
#endif

#ifndef AVDTP_MAX_TRANSPORT_CHANNELS
#error "AVDTP_MAX_TRANSPORT_CHANNELS is not defined"

/**
* \brief  Maximum number of transport channels.
* \ingroup avdtp_config
*
* \details Depending on the SEP capabilities (multiplexing, recovery, reporting) each stream may need up to 3 transport channels. 
*          E.g., if multiplexing is not supported and recovery and reporting are supported a stream will use 3 transport channels - 1 for media transport session,
*          1 for recovery transport session and 1 for reporting transport session. If multiplexing is not supported and only reporting is supported the stream 
*          will use 2 transport channels - 1 for media transport session and 1 for reporting transport session. If multiplexing is supported the stream will need only 1 
*          transport channel for all supported transport session. With multiplexing even different streams can share the same transport channel.
*          dotstack currently does not support multiplexing, recovery and reporting. So each stream needs its own transport channel for it media transport session.
*          Hence, AVDTP_MAX_TRANSPORT_CHANNELS must be equal to AVDTP_MAX_STREAMS
*/
#define AVDTP_MAX_TRANSPORT_CHANNELS
#endif

#ifndef AVDTP_MAX_TX_BUFFER_LEN
#error "AVDTP_MAX_TX_BUFFER_LEN is not defined"

/**
* \brief  Size of the transmit buffer.
* \ingroup avdtp_config
*
* \details This parameter defines the size of the buffer used to send AVDTP control commands to L2CAP layer. Each control channel
*          has it own buffer so the total amount of memory allocated for these buffers is AVDTP_MAX_TX_BUFFER_LEN) * AVDTP_MAX_REMOTE_DEVICES.
*          The minimum value is 1. The maximum value is 255. The value of 32 is usually sufficient.
*/
#define AVDTP_MAX_TX_BUFFER_LEN
#endif

#ifndef AVDTP_MAX_CMD_PARAM_LEN
#error "AVDTP_MAX_CMD_PARAM_LEN is not defined"

/**
* \brief  Maximum length of control command parameters
* \ingroup avdtp_config
*
* \details This parameter defines the maximum length of all command parameters. The value should not exceed AVDTP_MAX_TX_BUFFER_LEN - 2 (command header).
*/
#define AVDTP_MAX_CMD_PARAM_LEN
#endif

#ifndef AVDTP_CODEC_CONFIG_BUFFER_LEN
#error "AVDTP_CODEC_CONFIG_BUFFER_LEN is not defined"

/**
* \brief  Size of the buffer used to store codec specific configuration.
* \ingroup avdtp_config
*
* \details Each codec uses unique configuration which can take different amount of memory. 
*          This parameter defines the size of the buffer for storing codec's configuration.
*          The value of 16 is sufficient for SBC, AAC and MPEG1,2. If vendor specific codec 
*          is to be used this value may need to increased.
*/
#define AVDTP_CODEC_CONFIG_BUFFER_LEN
#endif

#define AVDTP_ALLOCATE_BUFFERS_VARS()	\
	bt_avdtp_sep_t                     _avdtp_seps[AVDTP_MAX_SEP];	\
	const bt_byte                      _avdtp_max_seps = AVDTP_MAX_SEP; \
	bt_avdtp_stream_t                  _avdtp_streams[AVDTP_MAX_STREAMS];	\
	const bt_byte                      _avdtp_max_streams = AVDTP_MAX_STREAMS; \
	bt_avdtp_control_channel_t         _avdtp_control_channels[AVDTP_MAX_REMOTE_DEVICES];	\
	const bt_byte                      _avdtp_max_control_channels = AVDTP_MAX_REMOTE_DEVICES; \
	const bt_byte                      _avdtp_max_tx_buffer_len = AVDTP_MAX_TX_BUFFER_LEN; \
	bt_byte                            _avdtp_tx_buffers[(AVDTP_MAX_TX_BUFFER_LEN) * (AVDTP_MAX_REMOTE_DEVICES)]; \
	bt_avdtp_transport_channel_t       _avdtp_transport_channels[AVDTP_MAX_TRANSPORT_CHANNELS];	\
	const bt_byte                      _avdtp_max_transport_channels = AVDTP_MAX_TRANSPORT_CHANNELS; \
	bt_buffer_header_t                 _avdtp_cmd_buffer_headers[(AVDTP_MAX_CMD_BUFFERS) * (AVDTP_MAX_REMOTE_DEVICES)];	\
	bt_avdtp_control_cmd_t             _avdtp_cmd_buffers[(AVDTP_MAX_CMD_BUFFERS) * (AVDTP_MAX_REMOTE_DEVICES)];	\
	const bt_byte                      _avdtp_max_cmd_buffers = AVDTP_MAX_CMD_BUFFERS; \
	bt_byte                            _avdtp_cmd_param_buffers[(AVDTP_MAX_CMD_BUFFERS) * (AVDTP_MAX_REMOTE_DEVICES) * (AVDTP_MAX_CMD_PARAM_LEN)];	\
	const bt_uint                      _avdtp_max_cmd_param_len = AVDTP_MAX_CMD_PARAM_LEN;	\
	bt_byte                            _avdtp_codec_cfg_buffers[(AVDTP_MAX_STREAMS) * 2 * (AVDTP_CODEC_CONFIG_BUFFER_LEN)];	\
	const bt_byte                      _avdtp_max_codec_config_buffer_len = AVDTP_CODEC_CONFIG_BUFFER_LEN;	\
	bt_buffer_header_t                 _avdtp_sep_cfg_buffer_headers[(AVDTP_MAX_STREAMS) * 2];	\
	bt_avdtp_sep_capabilities_t        _avdtp_sep_cfg_buffers[(AVDTP_MAX_STREAMS) * 2];	\
	bt_avdtp_sep_capabilities_t        _avdtp_rcv_sep_caps;	\
	bt_byte                            _avdtp_rcv_sep_codec_cfg_buffer[(AVDTP_CODEC_CONFIG_BUFFER_LEN)];	\
	bt_byte                            _avdtp_listen_sep_buffers[(AVDTP_MAX_STREAMS) * (AVDTP_MAX_SEP)];	\
	bt_byte                            _avdtp_l2cap_media_packet_buffer[HCI_L2CAP_BUFFER_LEN - L2CAP_HEADER_LEN];	\
	\
	AVDTP_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\

#define AVDTP_ALLOCATE_BUFFERS_FUNCTION()	\
	void _bt_avdtp_allocate_buffers(void)	\
	{	\
	} \

#define AVDTP_ALLOCATE_BUFFERS()	\
		AVDTP_ALLOCATE_BUFFERS_VARS()	\
		AVDTP_ALLOCATE_BUFFERS_FUNCTION()	\
		typedef int AVDTP_BUFFERS_ALLOCATED

#endif // __AVDTP_CONFIG_H
