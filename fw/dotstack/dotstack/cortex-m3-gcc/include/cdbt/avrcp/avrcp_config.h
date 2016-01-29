/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/


#ifndef __AVRCP_CONFIG_H
#define __AVRCP_CONFIG_H

#include "cdbt/avrcp/avrcp.h"

/**
 * \defgroup avrcp_config Configuration
 * \ingroup avrcp
 *
 * This module describes parameters used to configure AVRCP layer.
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

	#define BT_INCLUDE_AVRCP               // tells dotstack to compile in AVRCP support
	#define AVRCP_MAX_CHANNELS             ...
	#define AVRCP_MAX_CMD_BUFFERS          ...
	#define AVRCP_MAX_CMD_PARAM_LEN        ...
	#define AVRCP_MAX_SEARCH_RESULTS       ...
	#define AVRCP_MAX_DEVICE_NAME_LEN      ...
	#define AVRCP_CMD_TIMEOUT              ...

	#include "cdbt/bt/bt_oem_config.h"

    \endcode
 *
*/

#ifdef _DEBUG

#define AVRCP_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\
	const bt_uint _ram_size_avrcp_buffers =	\
		sizeof(_avrcp_channels) +	\
		sizeof(_avrcp_max_channels) +	\
		sizeof(_avrcp_cmd_buffer_headers) +	\
		sizeof(_avrcp_cmd_buffers) +	\
		sizeof(_avrcp_max_cmd_buffers) +	\
		sizeof(_avrcp_cmd_param_buffers) +	\
		sizeof(_avrcp_max_search_results) +	\
		sizeof(_avrcp_devices_buffer) +	\
		sizeof(_avrcp_max_device_name_len) +	\
		sizeof(_avrcp_device_name_buffers) +	\
		sizeof(_avrcp_max_cmd_param_len) +	\
		sizeof(_avrcp_cmd_timeout) +	\
		sizeof(bt_av_element_attribute_t) * AVRCP_MAX_ELEMENT_ATTRIBUTES +	\
		sizeof(bt_ulong) * 3 +	\
		sizeof(bt_avrcp_mgr_t);

#else
	#define AVRCP_ALLOCATE_BUFFERS_RAM_SIZE_VAR
#endif


#ifndef AVRCP_MAX_CHANNELS
#error "AVRCP_MAX_CHANNELS is not defined"

/**
* \brief  Maximum number of remote devices a local device can be connected to
* \ingroup avrcp_config
*
* \details This parameter defines the number of remote devices a local device can have simultaneous connections to (i.e. channels).
*          This value should not exceed AVCTP_MAX_CHANNELS.
*/
#define AVRCP_MAX_CHANNELS
#endif

#ifndef AVRCP_MAX_CMD_BUFFERS
/**
* \brief  Maximum number of command buffers.
* \ingroup avrcp_config
*
* \details This parameter defines the number of buffers reserved for sending commands to a remote device over its control channel. Each channel
*          uses its own buffers so the total number of buffers is AVRCP_MAX_CHANNELS * AVRCP_MAX_CMD_BUFFERS.
*          The minimum value is 1. The maximum value is 255. If not define one buffer for each channel is reserved.
*/
#define AVRCP_MAX_CMD_BUFFERS	1
#endif

#ifndef AVRCP_MAX_CMD_PARAM_LEN
/**
* \brief  Maximum length of command parameters
* \ingroup avrcp_config
*
* \details This parameter defines the maximum length of all command parameters.
*          If not defined the default value of 512 is used.
*/
#define AVRCP_MAX_CMD_PARAM_LEN	512
#endif


#ifndef AVRCP_MAX_SEARCH_RESULTS
/**
* \brief  Maximum number of devices to find
* \ingroup avrcp_config
*
* \details This parameter defines the maximum number of devices bt_avrcp_find_targets can find.
*          If not defined the default value of 7 is used.
*/
#define AVRCP_MAX_SEARCH_RESULTS        7
#endif

#ifndef AVRCP_MAX_DEVICE_NAME_LEN
/**
* \brief  Maximum length of device name
* \ingroup avrcp_config
*
* \details This parameter defines the size of the buffer used to store device's name while searching for
*          nearby targets with bt_avrcp_find_targets. If the name of the device is longer than AVRCP_MAX_DEVICE_NAME_LEN
*          it is truncated to AVRCP_MAX_DEVICE_NAME_LEN.
*          If not defined the default value of 20 is used.
*/
#define AVRCP_MAX_DEVICE_NAME_LEN       20
#endif

#ifndef AVRCP_CMD_TIMEOUT
/**
* \brief  Command timeout
* \ingroup avrcp_config
*
* \details This parameter defines the amount of time in milliseconds AVRCP waits for a response to a request.
*          If not defined the default value of 10000 (10 secconds) is used.
*/
#define AVRCP_CMD_TIMEOUT  10000
#endif

#if !defined(AVRCP_ENABLE_CONTROLLER) && !defined(AVRCP_ENABLE_TARGET)
#error "AVRCP_ENABLE_CONTROLLER or AVRCP_ENABLE_TARGET must be defined"
#endif

#include "avrcp_config_event_handlers.h"

#define AVRCP_ALLOCATE_BUFFERS_VARS()	\
	bt_avrcp_channel_t  _avrcp_channels[AVRCP_MAX_CHANNELS];	\
	const bt_byte       _avrcp_max_channels = AVRCP_MAX_CHANNELS; \
	\
	bt_buffer_header_t  _avrcp_cmd_buffer_headers[(AVRCP_MAX_CMD_BUFFERS) * (AVRCP_MAX_CHANNELS)];	\
	bt_av_command_t     _avrcp_cmd_buffers[(AVRCP_MAX_CMD_BUFFERS) * (AVRCP_MAX_CHANNELS)];	\
	const bt_byte       _avrcp_max_cmd_buffers = AVRCP_MAX_CMD_BUFFERS; \
	bt_byte             _avrcp_cmd_param_buffers[(AVRCP_MAX_CMD_BUFFERS) * (AVRCP_MAX_CMD_PARAM_LEN) * (AVRCP_MAX_CHANNELS)];	\
	const bt_int        _avrcp_max_cmd_param_len = AVRCP_MAX_CMD_PARAM_LEN;	\
	const bt_byte       _avrcp_max_search_results = AVRCP_MAX_SEARCH_RESULTS;	\
	bt_avrcp_device_t   _avrcp_devices_buffer[AVRCP_MAX_SEARCH_RESULTS];	\
	const bt_byte       _avrcp_max_device_name_len = AVRCP_MAX_DEVICE_NAME_LEN;	\
	bt_byte             _avrcp_device_name_buffers[(AVRCP_MAX_SEARCH_RESULTS) * (AVRCP_MAX_DEVICE_NAME_LEN + 1)];	\
	const bt_byte       _avrcp_cmd_timeout = (AVRCP_CMD_TIMEOUT) / 100;	\
	\
	void (*_bt_avrcp_response_sent_handler)(bt_avrcp_mgr_t* mgr, bt_avctp_evt_response_sent_t* evt_param) = AVRCP_RESPONSE_SENT_HANDLER;	\
	void (*_bt_avrcp_command_handler)(bt_avrcp_mgr_t* mgr, bt_avctp_evt_command_received_t* evt_param) = AVRCP_COMMAND_HANDLER;	\
	void (*_bt_avrcp_command_sent_handler)(bt_avrcp_mgr_t* mgr, bt_avctp_evt_command_sent_t* evt_param) = AVRCP_COMMAND_SENT_HANDLER;	\
	void (*_bt_avrcp_response_handler)(bt_avrcp_mgr_t* mgr, bt_avctp_evt_response_received_t* evt_param) = AVRCP_RESPONSE_HANDLER;	\
	\
	AVRCP_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\

#define AVRCP_ALLOCATE_BUFFERS()	\
		AVRCP_ALLOCATE_BUFFERS_VARS()	\
		typedef int AVRCP_BUFFERS_ALLOCATED

#endif // __AVRCP_CONFIG_H
