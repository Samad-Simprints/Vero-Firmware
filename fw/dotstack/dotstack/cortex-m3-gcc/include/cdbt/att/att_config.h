/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/


#ifndef __ATT_CONFIG_H
#define __ATT_CONFIG_H

/**
 * \defgroup att_config Configuration
 * \ingroup att_server
 *
 * This module describes parameters used to configure ATT server layer.
 *
 * dotstack is customized using a configuration file. The configuration file tailors the dotstack to the application being built. It has to have the structure shown below.
 * 
    \code
	#include "cdbt/bt/bt_std.h"

	// HCI and L2CAP must always be present
	// SDP is required only if stack is running in dual mode. This is the default mode.
	// To run the stack in single mode (i.e. only BLE is supported) a BT_BLE_SINGLE_MODE symbol
	// must be defined:
	// #define BT_BLE_SINGLE_MODE

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
	// for each used module. E.g., to use and configure ATT,
	// the following values must be defined:
	
	#define BT_INCLUDE_ATT               // tells dotstack to compile in ATT support
	#define ATT_MAX_CLIENTS                        ...
	#define ATT_MAX_FOUND_ATTRIBUTES               ...
	#define ATT_MAX_QUEUED_WRITE_BUFFER_SIZE       ...

	#include "cdbt/bt/bt_oem_config.h"

    \endcode
 *
*/

#include "cdbt/att/att.h"

#ifdef _DEBUG

#define ATT_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\
	const bt_uint _ram_size_att_buffers =	\
		sizeof(_att_sessions) +	\
		sizeof(_att_max_sessions) +	\
		sizeof(_att_max_found_attributes) +	\
		sizeof(_att_found_attributes) +	\
		sizeof(_att_max_max_queued_write_buffer_size) +	\
		sizeof(_att_queued_write_buffers) +	\
		_att_max_max_queued_write_buffer_size * (ATT_MAX_CLIENTS) +	\
		ATT_SIZEOF_QUEUED_WRITE_BUFFERS;

#else
	#define ATT_ALLOCATE_BUFFERS_RAM_SIZE_VAR
#endif

#ifndef ATT_MAX_CLIENTS
#error "ATT_MAX_CLIENTS is not defined"

/**
* \brief  Maximum number of clients
* \ingroup att_config
*
* \details This parameter defines the maximum number of clients that can be simultaneously connected to the server.
*          If ATT is used only over LE this number must be set to 1. Any other numbers will be a waste of RAM because
*          an LE slave (server) can have only one connection to a master (client) at any given moment.
*/
#define ATT_MAX_CLIENTS
#endif

#ifndef ATT_MAX_FOUND_ATTRIBUTES
#error "ATT_MAX_FOUND_ATTRIBUTES is not defined"

/**
* \brief  Maximum number of attributes that can be returned in one multi-attribute response
* \ingroup att_config
*
* \details This parameter defines the maximum number of attributes that can be returned in one multi-attribute response.
*          The minimum value is 1. The maximum value is 255. This number must be set to as large value as possible (given 
*          there is enough RAM) to minimize the number of request needed by the client to find all requested attributes.
*/
#define ATT_MAX_FOUND_ATTRIBUTES
#endif

#ifdef ATT_MAX_QUEUED_WRITE_BUFFER_SIZE
#define ATT_SIZEOF_QUEUED_WRITE_BUFFERS	\
	sizeof(_att_allocate_queued_write_buffers)
#define ATT_DECLARE_QUEUED_WRITE_BUFFERS	\
	bt_byte _att_allocate_queued_write_buffers[(ATT_MAX_QUEUED_WRITE_BUFFER_SIZE) * (ATT_MAX_CLIENTS)];	\
	bt_byte* _att_queued_write_buffers = _att_allocate_queued_write_buffers;

#else
#define ATT_SIZEOF_QUEUED_WRITE_BUFFERS		0

/**
* \brief  Size of the buffer for storing queued writes
* \ingroup att_config
*
* \details This parameter defines the Size of the buffer for storing queued writes.
*          The minimum value is 0. The maximum value is 65535. If this is set to 0 queued writes
*          will not be supported.
*/
#define ATT_MAX_QUEUED_WRITE_BUFFER_SIZE	0

#define ATT_DECLARE_QUEUED_WRITE_BUFFERS	\
	bt_byte* _att_queued_write_buffers = NULL;

#endif

#define ATT_ALLOCATE_BUFFERS_VARS()	\
	bt_att_session_t                   _att_sessions[ATT_MAX_CLIENTS];	\
	const bt_byte                      _att_max_sessions = ATT_MAX_CLIENTS; \
	const bt_byte                      _att_max_found_attributes = ATT_MAX_FOUND_ATTRIBUTES;	\
	bt_att_found_attribyte_t           _att_found_attributes[(ATT_MAX_CLIENTS) * (ATT_MAX_FOUND_ATTRIBUTES)];	\
	const bt_uint                      _att_max_max_queued_write_buffer_size = ATT_MAX_QUEUED_WRITE_BUFFER_SIZE;	\
	ATT_DECLARE_QUEUED_WRITE_BUFFERS	\
	\
	ATT_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\

#define ATT_ALLOCATE_BUFFERS()	\
		ATT_ALLOCATE_BUFFERS_VARS()	\
		typedef int ATT_BUFFERS_ALLOCATED

#endif // __ATT_CONFIG_H
