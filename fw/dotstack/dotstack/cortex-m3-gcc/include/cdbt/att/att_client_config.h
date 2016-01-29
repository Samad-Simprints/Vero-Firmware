/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/


#ifndef __ATT_CLIENT_CONFIG_H
#define __ATT_CLIENT_CONFIG_H

/**
 * \defgroup att_client_config Configuration
 * \ingroup att_client
 *
 * This module describes parameters used to configure ATT client layer.
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
	#define ATT_CLIENT_MAX_SESSIONS      ...

	#include "cdbt/bt/bt_oem_config.h"

    \endcode
 *
*/

#include "cdbt/att/att_client.h"

#ifdef _DEBUG

#define ATT_CLIENT_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\
	const bt_uint _ram_size_att_client_buffers =	\
	sizeof(_att_client_sessions) +	\
	sizeof(_att_max_client_sessions);

#else
#define ATT_CLIENT_ALLOCATE_BUFFERS_RAM_SIZE_VAR
#endif

#ifndef ATT_CLIENT_MAX_SESSIONS
#error "ATT_CLIENT_MAX_SESSIONS is not defined"

/**
* \brief  Maximum number of ATT sessions
* \ingroup att_client_config
*
* \details This parameter defines the maximum number of connections a client can have with servers.
*          The minimum value is 1. The maximum value is 7.
*/
#define ATT_CLIENT_MAX_SESSIONS
#endif

#ifdef BT_INCLUDE_ATT
	#define ATT_DECLARE_BRIDGE	\
		bt_att_bridge_t  _att_bridge_mem[ATT_CLIENT_MAX_SESSIONS];	\
		bt_att_bridge_t* _att_bridge = _att_bridge_mem;
#else
	#define ATT_DECLARE_BRIDGE	\
		bt_att_bridge_t* _att_bridge = NULL;
#endif

#ifdef BT_INCLUDE_SM
	#define ATT_CLIENT_DECL_SM_FUNCTIONS	\
		void (*_bt_gatt_client_register_sm_listener_fp)(bt_att_client_session_t* session, bt_att_client_err_handler_fp error_handler, bt_uint handle, bt_byte errcode) = &_bt_gatt_client_register_sm_listener;	\
		void (*_bt_gatt_client_unregister_sm_listener_fp)(bt_att_client_session_t* session) = &_bt_gatt_client_unregister_sm_listener;
#else
	#define ATT_CLIENT_DECL_SM_FUNCTIONS	\
		void (*_bt_gatt_client_register_sm_listener_fp)(bt_att_client_session_t* session, bt_att_client_err_handler_fp error_handler, bt_uint handle, bt_byte errcode) = NULL;	\
		void (*_bt_gatt_client_unregister_sm_listener_fp)(bt_att_client_session_t* session) = NULL;
#endif


#define ATT_CLIENT_ALLOCATE_BUFFERS_VARS()	\
	bt_att_client_session_t            _att_client_sessions[ATT_CLIENT_MAX_SESSIONS];	\
	const bt_byte                      _att_max_client_sessions = ATT_CLIENT_MAX_SESSIONS; \
	ATT_DECLARE_BRIDGE	\
	\
	ATT_CLIENT_DECL_SM_FUNCTIONS	\
	\
	ATT_CLIENT_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\

#define ATT_CLIENT_ALLOCATE_BUFFERS()	\
	ATT_CLIENT_ALLOCATE_BUFFERS_VARS()	\
	typedef int ATT_CLIENT_BUFFERS_ALLOCATED

#endif // __ATT_CLIENT_CONFIG_H
