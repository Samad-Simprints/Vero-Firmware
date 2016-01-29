/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/


#ifndef __AVCTP_CONFIG_H
#define __AVCTP_CONFIG_H

#include "cdbt/avctp/avctp.h"

/**
 * \defgroup avctp_config Configuration
 * \ingroup avctp
 *
 * This module describes parameters used to configure AVCTP layer.
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
	// for each used module. E.g., to use and configure AVRCP,
	// the following values must be defined:
	
	#define BT_INCLUDE_AVCTP               // tells dotstack to compile in AVCTP support
	#define AVCTP_MAX_CHANNELS             ...
	#define AVCTP_MAX_TRANSPORT_CHANNELS   ...
	#define AVCTP_MAX_RX_MESSAGE_LEN       ...
	#define AVCTP_MAX_MESSAGE_BUFFERS      ...

	#include "cdbt/bt/bt_oem_config.h"

    \endcode
 *
*/

#ifdef _DEBUG

#define AVCTP_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\
	const bt_uint _ram_size_avctp_buffers =	\
		sizeof(_avctp_message_buffer_headers) +	\
		sizeof(_avctp_message_buffers) +	\
		sizeof(_avctp_transports) +	\
		sizeof(_avctp_max_transports) +	\
		sizeof(_avctp_channels) +	\
		sizeof(_avctp_max_channels) +	\
		sizeof(_avctp_max_message_buffers) +	\
		sizeof(_avctp_max_rx_message_len) +	\
		sizeof(_avctp_rx_buffers);

#else
	#define AVCTP_ALLOCATE_BUFFERS_RAM_SIZE_VAR
#endif

#ifndef AVCTP_MAX_CHANNELS
#error "AVCTP_MAX_CHANNELS is not defined"

/**
* \brief  Maximum number of AVCTP channels
* \ingroup avctp_config
*
* \details This parameter defines the maximum number of channels a local device can have with remote devices.
*/
#define AVCTP_MAX_CHANNELS
#endif

#ifndef AVCTP_MAX_TRANSPORT_CHANNELS
#error "AVCTP_MAX_TRANSPORT_CHANNELS is not defined"

/**
* \brief  Maximum number of AVCTP transports
* \ingroup avctp_config
*
* \details This parameter defines the maximum number of transports a local device can have with remote devices.
*          This value should not exceed AVCTP_MAX_CHANNELS.
*/
#define AVCTP_MAX_TRANSPORT_CHANNELS
#endif

#if AVCTP_MAX_TRANSPORT_CHANNELS > AVCTP_MAX_CHANNELS
#error "AVCTP_MAX_TRANSPORT_CHANNELS > AVCTP_MAX_CHANNELS is a waist of memory"
#endif

#if L2CAP_MAX_CHANNELS < 2 + AVCTP_MAX_TRANSPORT_CHANNELS
#error "L2CAP_MAX_CHANNELS mus be at least 2 + AVCTP_MAX_TRANSPORT_CHANNELS"
#endif

#ifndef AVCTP_MAX_RX_MESSAGE_LEN
#error "AVCTP_MAX_RX_MESSAGE_LEN is not defined"

/**
* \brief  Maximum size of received message
* \ingroup avctp_config
*
* \details This parameter defines the maximum size of a fragmented message (command or response) a local device can accept from a remote device.
*          If fragmented message are not expected this parameter can be set to 1.
*/
#endif

#ifndef AVCTP_MAX_MESSAGE_BUFFERS
#error "AVCTP_MAX_MESSAGE_BUFFERS is not defined"

/**
* \brief  Maximum number of message buffers
* \ingroup avctp_config
*
* \details This parameter defines the maximum number of buffer available for sending message.
*/
#endif

#define AVCTP_ALLOCATE_BUFFERS_VARS()	\
	bt_avctp_channel_t                 _avctp_channels[AVCTP_MAX_CHANNELS];	\
	const bt_byte                      _avctp_max_channels = AVCTP_MAX_CHANNELS; \
	bt_avctp_transport_t               _avctp_transports[AVCTP_MAX_TRANSPORT_CHANNELS];	\
	const bt_byte                      _avctp_max_transports = AVCTP_MAX_TRANSPORT_CHANNELS; \
	const bt_uint                      _avctp_max_rx_message_len = AVCTP_MAX_RX_MESSAGE_LEN; \
	bt_byte                            _avctp_rx_buffers[(AVCTP_MAX_RX_MESSAGE_LEN) * (AVCTP_MAX_TRANSPORT_CHANNELS)]; \
	bt_buffer_header_t                 _avctp_message_buffer_headers[(AVCTP_MAX_MESSAGE_BUFFERS) * (AVCTP_MAX_TRANSPORT_CHANNELS)];	\
	bt_avctp_message_t                 _avctp_message_buffers[(AVCTP_MAX_MESSAGE_BUFFERS) * (AVCTP_MAX_TRANSPORT_CHANNELS)];	\
	const bt_byte                      _avctp_max_message_buffers = AVCTP_MAX_MESSAGE_BUFFERS; \
	\
	AVCTP_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\

#define AVCTP_ALLOCATE_BUFFERS()	\
		AVCTP_ALLOCATE_BUFFERS_VARS()	\
		typedef int AVCTP_BUFFERS_ALLOCATED

#endif // __AVCTP_CONFIG_H
