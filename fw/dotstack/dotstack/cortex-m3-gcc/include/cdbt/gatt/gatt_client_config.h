/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/


#ifndef __GATT_CLIENT_CONFIG_H
#define __GATT_CLIENT_CONFIG_H

/**
 * \defgroup gatt_client_config Configuration
 * \ingroup gatt_client
 *
 * This module describes parameters used to configure GATT client.
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
	// for each used module. E.g., to use and configure GATT,
	// the following values must be defined:
	
	#define BT_INCLUDE_ATT               // tells dotstack to compile in ATT support
	#define ATT_CLIENT_MAX_SESSIONS      ...

	#define BT_INCLUDE_GATT_CLIENT       // tells dotstack to compile in GATT support

	// By default all GATT client functions can be used by the application. In most cases only a subset of the API will be used. 
	// To to save quite significant amount of code space unused functions can be disabled. To disable a function the correspondign symbol
	// must be defined in this configuration file. For example do disable bt_gatt_client_read_by_char_uuid the GATT_NO_READ_BY_CHAR_UUID
	// must be defined:
	//   #define GATT_NO_READ_BY_CHAR_UUID

	#include "cdbt/bt/bt_oem_config.h"

    \endcode

	\li GATT_NO_DISCOVER_ALL_SERVICES - Disable bt_gatt_client_discover_all_services_ex()
	\li GATT_NO_DISCOVER_BY_SERVICE_UUID - Disable bt_gatt_client_discover_by_service_uuid()
	\li GATT_NO_FIND_INCLUDED_SERVICES - Disable bt_gatt_client_find_included_services_ex()
	\li GATT_NO_DISCOVER_ALL_CHARS - Disable bt_gatt_client_discover_all_chars_ex()
	\li GATT_NO_DISCOVER_CHAR_BY_UUID - Disable bt_gatt_client_discover_char_by_uuid()
	\li GATT_NO_READ_CHAR_VALUE - Disable bt_gatt_client_read_char_value()
	\li GATT_NO_READ_BY_CHAR_UUID - Disable bt_gatt_client_read_by_char_uuid()
	\li GATT_NO_READ_MULTIPLE_CHAR_VALUES - Disable bt_gatt_client_read_multiple_char_values()
	\li GATT_NO_READ_CHAR_DESCRIPTOR - Disable bt_gatt_client_read_char_descriptor()
	\li GATT_NO_READ_CHAR_LONG_VALUE - Disable bt_gatt_client_read_char_long_value()
	\li GATT_NO_READ_CHAR_LONG_DESCRIPTOR - Disable bt_gatt_client_read_char_long_descriptor()
	\li GATT_NO_WRITE_CHAR_VALUE - Disable bt_gatt_client_write_char_value()
	\li GATT_NO_WRITE_CHAR_DESCRIPTOR - Disable bt_gatt_client_write_char_descriptor()
	\li GATT_NO_WRITE_CHAR_LONG_VALUE - Disable bt_gatt_client_write_char_long_value()
	\li GATT_NO_WRITE_CHAR_LONG_DESCRIPTOR - Disable bt_gatt_client_write_char_long_descriptor()
	\li GATT_NO_DISCOVER_CHAR_DESCRIPTORS - Disable bt_gatt_client_discover_char_descriptors()
 *
*/

#include "cdbt/gatt/gatt_client.h"

#ifdef _DEBUG

#define GATT_CLIENT_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\
	const bt_uint _ram_size_gatt_client_buffers =	\
	sizeof(_gatt_client_sessions) +	\
	sizeof(_gatt_client_ancs_cmd_buffer) +	\
	sizeof(_gatt_client_ancs_cmd_buffer_len) +	\
	sizeof(_gatt_client_ancs_operation_max_queue_len) +	\
	sizeof(_gatt_client_ancs_operation_queue) +	\
	sizeof(_gatt_client_ancs_object_id_max_len) +	\
	sizeof(_gatt_client_ancs_object_id_buffer);

#else
#define GATT_CLIENT_ALLOCATE_BUFFERS_RAM_SIZE_VAR
#endif


#ifndef GATT_NO_DISCOVER_ALL_SERVICES
	#define GATT_DECL_DISCOVER_ALL_SERVICES	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_rbgt_response_handler_fp = &_bt_gatt_client_read_by_group_type_response_handler;
#else
	#define GATT_DECL_DISCOVER_ALL_SERVICES	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_rbgt_response_handler_fp = &_bt_gatt_client_default_response_handler;
#endif

#ifndef GATT_NO_DISCOVER_BY_SERVICE_UUID
	#define GATT_DECL_DISCOVER_BY_SERVICE_UUID	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_fbtv_response_handler_fp = &_bt_gatt_client_find_by_type_value_response_handler;
#else
	#define GATT_DECL_DISCOVER_BY_SERVICE_UUID	\
		const bt_gatt_client_response_handler_fp  _bt_gatt_client_fbtv_response_handler_fp = &_bt_gatt_client_default_response_handler;
#endif

#ifndef GATT_NO_FIND_INCLUDED_SERVICES
	#define GATT_DECL_FIND_INCLUDED_SERVICES	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_fi_rbt_response_handler_fp = &_bt_gatt_client_find_included_read_by_type_response_handler;
#else
	#define GATT_DECL_FIND_INCLUDED_SERVICES	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_fi_rbt_response_handler_fp = &_bt_gatt_client_default_response_handler;
#endif

#if defined(GATT_NO_DISCOVER_ALL_CHARS) && defined(GATT_NO_DISCOVER_CHAR_BY_UUID)
	#define GATT_DECL_DISCOVER_CHARS	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_dc_rbt_response_handler_fp = &_bt_gatt_client_default_response_handler;
#else
	#define GATT_DECL_DISCOVER_CHARS	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_dc_rbt_response_handler_fp = &_bt_gatt_client_discover_chars_read_by_type_response_handler;
#endif

#ifndef GATT_NO_READ_CHAR_VALUE
	#define GATT_DECL_READ_CHAR_VALUE	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_rcv_r_response_handler_fp = &_bt_gatt_client_read_char_value_read_response_handler;
#else
	#define GATT_DECL_READ_CHAR_VALUE	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_rcv_r_response_handler_fp = &_bt_gatt_client_default_response_handler;
#endif

#ifndef GATT_NO_READ_BY_CHAR_UUID
#define GATT_DECL_READ_BY_CHAR_UUID	\
	const bt_gatt_client_response_handler_fp _bt_gatt_client_rucu_rbt_response_handler_fp = &_bt_gatt_client_read_by_char_uuid_read_by_type_response_handler;
#else
#define GATT_DECL_READ_BY_CHAR_UUID	\
	const bt_gatt_client_response_handler_fp _bt_gatt_client_rucu_rbt_response_handler_fp = &_bt_gatt_client_default_response_handler;
#endif

#ifndef GATT_NO_READ_MULTIPLE_CHAR_VALUES
#define GATT_DECL_READ_MULTIPLE_CHAR_VALUES	\
	const bt_gatt_client_response_handler_fp _bt_gatt_client_rm_response_handler_fp = &_bt_gatt_client_read_multiple_response_handler;
#else
#define GATT_DECL_READ_MULTIPLE_CHAR_VALUES	\
	const bt_gatt_client_response_handler_fp _bt_gatt_client_rm_response_handler_fp = &_bt_gatt_client_default_response_handler;
#endif

#ifndef GATT_NO_READ_CHAR_DESCRIPTOR
	#define GATT_DECL_READ_CHAR_DESCRIPTOR	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_rcd_r_response_handler_fp = &_bt_gatt_client_read_char_descriptor_read_response_handler;
#else
	#define GATT_DECL_READ_CHAR_DESCRIPTOR	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_rcd_r_response_handler_fp = &_bt_gatt_client_default_response_handler;
#endif

#ifndef GATT_NO_READ_CHAR_LONG_VALUE
	#define GATT_DECL_READ_CHAR_LONG_VALUE	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_rcv_rb_response_handler_fp = &_bt_gatt_client_read_char_value_read_blob_response_handler;
#else
	#define GATT_DECL_READ_CHAR_LONG_VALUE	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_rcv_rb_response_handler_fp = &_bt_gatt_client_default_response_handler;
#endif

#ifndef GATT_NO_READ_CHAR_LONG_DESCRIPTOR
	#define GATT_DECL_READ_CHAR_LONG_DESCRIPTOR	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_rcd_rb_response_handler_fp = &_bt_gatt_client_read_char_descriptor_read_blob_response_handler;
#else
	#define GATT_DECL_READ_CHAR_LONG_DESCRIPTOR	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_rcd_rb_response_handler_fp = &_bt_gatt_client_default_response_handler;
#endif

#if defined(GATT_NO_WRITE_CHAR_VALUE) && defined(GATT_NO_WRITE_CHAR_DESCRIPTOR)
	#define GATT_DECL_WRITE_CHAR_VALUE_DESCRIPTOR	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_w_response_handler_fp = &_bt_gatt_client_default_response_handler;
#else
	#define GATT_DECL_WRITE_CHAR_VALUE_DESCRIPTOR	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_w_response_handler_fp = &_bt_gatt_client_write_response_handler;
#endif

#ifndef GATT_NO_WRITE_CHAR_LONG_VALUE
	#define GATT_DECL_WRITE_CHAR_LONG_VALUE 	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_wcv_pw_response_handler_fp = &_bt_gatt_client_write_char_value_prepare_write_response_handler;	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_wcv_ew_response_handler_fp = &_bt_gatt_client_write_char_value_execute_write_response_handler;
#else
	#define GATT_DECL_WRITE_CHAR_LONG_VALUE	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_wcv_pw_response_handler_fp = &_bt_gatt_client_default_response_handler;	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_wcv_ew_response_handler_fp = &_bt_gatt_client_default_response_handler;
#endif

#ifndef GATT_NO_WRITE_CHAR_LONG_DESCRIPTOR
	#define GATT_DECL_WRITE_CHAR_LONG_DESCRIPTOR	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_wcd_pw_response_handler_fp = &_bt_gatt_client_write_char_descriptor_prepare_write_response_handler;	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_wcd_ew_response_handler_fp = &_bt_gatt_client_write_char_descriptor_execute_write_response_handler;
#else
	#define GATT_DECL_WRITE_CHAR_LONG_DESCRIPTOR	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_wcd_pw_response_handler_fp = &_bt_gatt_client_default_response_handler;	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_wcd_ew_response_handler_fp = &_bt_gatt_client_default_response_handler;
#endif

#ifndef GATT_NO_DISCOVER_CHAR_DESCRIPTORS
	#define GATT_DECL_DISCOVER_CHAR_DESCRIPTORS	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_fi_response_handler_fp = &_bt_gatt_client_find_info_response_handler;
#else
	#define GATT_DECL_DISCOVER_CHAR_DESCRIPTORS	\
		const bt_gatt_client_response_handler_fp _bt_gatt_client_fi_response_handler_fp = &_bt_gatt_client_default_response_handler;
#endif

#ifndef GATT_CLIENT_ANCS_CMD_BUFFER_LEN
	#define GATT_CLIENT_ANCS_CMD_BUFFER_LEN             32
#endif

#ifndef GATT_ANCS_CLIENT_OPERATION_MAX_QUEUE_LEN
	#define GATT_ANCS_CLIENT_OPERATION_MAX_QUEUE_LEN    20
#endif

#ifndef GATT_ANCS_CLIENT_OBJECT_ID_MAX_LEN
	#define GATT_ANCS_CLIENT_OBJECT_ID_MAX_LEN             32
#endif


#define GATT_CLIENT_ALLOCATE_BUFFERS_VARS()	\
	bt_gatt_client_session_t            _gatt_client_sessions[ATT_CLIENT_MAX_SESSIONS];	\
	bt_byte                             _gatt_client_ancs_cmd_buffer[GATT_CLIENT_ANCS_CMD_BUFFER_LEN];	\
	const bt_uint                       _gatt_client_ancs_cmd_buffer_len = GATT_CLIENT_ANCS_CMD_BUFFER_LEN;	\
	const bt_uint                       _gatt_client_ancs_operation_max_queue_len = GATT_ANCS_CLIENT_OPERATION_MAX_QUEUE_LEN;	\
	const bt_uint                       _gatt_client_ancs_object_id_max_len = GATT_ANCS_CLIENT_OBJECT_ID_MAX_LEN;	\
	bt_gatt_client_ancs_operation_t     _gatt_client_ancs_operation_queue[(GATT_ANCS_CLIENT_OPERATION_MAX_QUEUE_LEN) * (ATT_CLIENT_MAX_SESSIONS)];	\
	bt_byte                             _gatt_client_ancs_object_id_buffer[(GATT_ANCS_CLIENT_OBJECT_ID_MAX_LEN) * (GATT_ANCS_CLIENT_OPERATION_MAX_QUEUE_LEN) * (ATT_CLIENT_MAX_SESSIONS)];	\
	GATT_DECL_DISCOVER_ALL_SERVICES	\
	GATT_DECL_DISCOVER_BY_SERVICE_UUID	\
	GATT_DECL_FIND_INCLUDED_SERVICES	\
	GATT_DECL_DISCOVER_CHARS	\
	GATT_DECL_READ_CHAR_VALUE	\
	GATT_DECL_READ_BY_CHAR_UUID	\
	GATT_DECL_READ_MULTIPLE_CHAR_VALUES	\
	GATT_DECL_READ_CHAR_DESCRIPTOR	\
	GATT_DECL_READ_CHAR_LONG_VALUE	\
	GATT_DECL_READ_CHAR_LONG_DESCRIPTOR	\
	GATT_DECL_WRITE_CHAR_VALUE_DESCRIPTOR	\
	GATT_DECL_WRITE_CHAR_LONG_VALUE	\
	GATT_DECL_WRITE_CHAR_LONG_DESCRIPTOR	\
	GATT_DECL_DISCOVER_CHAR_DESCRIPTORS	\
	\
	GATT_CLIENT_ALLOCATE_BUFFERS_RAM_SIZE_VAR	\

#define GATT_INIT_HANDLERS_FUNCTION()	\
	void _bt_gatt_client_init_handlers(void)	\
	{	\
	} \

#define GATT_CLIENT_ALLOCATE_BUFFERS()	\
	GATT_CLIENT_ALLOCATE_BUFFERS_VARS()	\
	GATT_INIT_HANDLERS_FUNCTION()	\
	typedef int GATT_CLIENT_BUFFERS_ALLOCATED

#endif // __GATT_CLIENT_CONFIG_H
