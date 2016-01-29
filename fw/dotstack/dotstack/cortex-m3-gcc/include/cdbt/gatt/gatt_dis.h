/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __GATT_DIS_H
#define __GATT_DIS_H

#define GATT_DIS_CLIENT_EVT_MANUFACTURER_NAME_RECEIVED        0
#define GATT_DIS_CLIENT_EVT_MODEL_NUMBER_RECEIVED             1
#define GATT_DIS_CLIENT_EVT_SERIAL_NUMBER_RECEIVED            2
#define GATT_DIS_CLIENT_EVT_HARDWARE_REVISION_RECEIVED        3
#define GATT_DIS_CLIENT_EVT_FIRMWARE_REVISION_RECEIVED        4
#define GATT_DIS_CLIENT_EVT_SOFTWARE_REVISION_RECEIVED        5
#define GATT_DIS_CLIENT_EVT_CERTIFICATION_RECEIVED            6
#define GATT_DIS_CLIENT_EVT_SYSTEM_ID_RECEIVED                7
#define GATT_DIS_CLIENT_EVT_PNP_ID_RECEIVED                   8

#define GATT_DIS_CLIENT_MANUFACTURER_NAME_INDEX               0
#define GATT_DIS_CLIENT_MODEL_NUMBER_INDEX                    1
#define GATT_DIS_CLIENT_SERIAL_NUMBER_INDEX                   2
#define GATT_DIS_CLIENT_HARDWARE_REVISION_INDEX               3
#define GATT_DIS_CLIENT_FIRMWARE_REVISION_INDEX               4
#define GATT_DIS_CLIENT_SOFTWARE_REVISION_INDEX               5
#define GATT_DIS_CLIENT_CERTIFICATION_INDEX                   6
#define GATT_DIS_CLIENT_SYSTEM_ID_INDEX                       7
#define GATT_DIS_CLIENT_PNP_ID_INDEX                          8
#define GATT_DIS_CLIENT_CHAR_COUNT                            (GATT_DIS_CLIENT_PNP_ID_INDEX + 1)

typedef struct _bt_gatt_client_dis_evt_manufacturer_name_received_s
{
	const bt_byte* manufacturer_name;
	bt_uint len;
} bt_gatt_client_dis_evt_manufacturer_name_received_t;

typedef struct _bt_gatt_client_dis_evt_model_number_received_s
{
	const bt_byte* model_number;
	bt_uint len;
} bt_gatt_client_dis_evt_model_number_received_t;

typedef struct _bt_gatt_client_dis_evt_serial_number_received_s
{
	const bt_byte* serial_number;
	bt_uint len;
} bt_gatt_client_dis_evt_serial_number_received_t;

typedef struct _bt_gatt_client_dis_evt_hardware_revision_received_s
{
	const bt_byte* hardware_revision;
	bt_uint len;
} bt_gatt_client_dis_evt_hardware_revision_received_t;

typedef struct _bt_gatt_client_dis_evt_firmware_revision_received_s
{
	const bt_byte* firmware_revision;
	bt_uint len;
} bt_gatt_client_dis_evt_firmware_revision_received_t;

typedef struct _bt_gatt_client_dis_evt_software_revision_received_s
{
	const bt_byte* software_revision;
	bt_uint len;
} bt_gatt_client_dis_evt_software_revision_received_t;

typedef struct _bt_gatt_client_dis_evt_system_id_received_s
{
	bt_byte  manufacturer_id_hi;
	bt_ulong manufacturer_id_lo;
	bt_ulong organizationally_unique_id;
} bt_gatt_client_dis_evt_system_id_received_t;

typedef struct _bt_gatt_client_dis_evt_certification_received_s
{
	const bt_byte* certification;
	bt_uint len;
} bt_gatt_client_dis_evt_certification_received_t;

typedef struct _bt_gatt_client_dis_evt_pnp_id_received_s
{
	bt_byte vendor_id_source;
	bt_uint vendor_id;
	bt_uint product_id;
	bt_uint product_version;
} bt_gatt_client_dis_evt_pnp_id_received_t;

typedef union _bt_gatt_client_dis_evt_u
{
	bt_gatt_client_dis_evt_manufacturer_name_received_t  manufacturer_name;     ///< Valid if event is GATT_DIS_CLIENT_EVT_MANUFACTURER_NAME_RECEIVED
	bt_gatt_client_dis_evt_model_number_received_t       model_number;          ///< Valid if event is GATT_DIS_CLIENT_EVT_MODEL_NUMBER_RECEIVED
	bt_gatt_client_dis_evt_serial_number_received_t      serial_number;         ///< Valid if event is GATT_DIS_CLIENT_EVT_SERIAL_NUMBER_RECEIVED
	bt_gatt_client_dis_evt_hardware_revision_received_t  hardware_revision;     ///< Valid if event is GATT_DIS_CLIENT_EVT_HARDWARE_REVISION_RECEIVED
	bt_gatt_client_dis_evt_firmware_revision_received_t  firmware_revision;     ///< Valid if event is GATT_DIS_CLIENT_EVT_FIRMWARE_REVISION_RECEIVED
	bt_gatt_client_dis_evt_software_revision_received_t  software_revision;     ///< Valid if event is GATT_DIS_CLIENT_EVT_SOFTWARE_REVISION_RECEIVED
	bt_gatt_client_dis_evt_system_id_received_t          system_id;             ///< Valid if event is GATT_DIS_CLIENT_EVT_SYSTEM_ID_RECEIVED
	bt_gatt_client_dis_evt_certification_received_t      certification;         ///< Valid if event is GATT_DIS_CLIENT_EVT_CERTIFICATION_RECEIVED
	bt_gatt_client_dis_evt_pnp_id_received_t             pnp_id;                ///< Valid if event is GATT_DIS_CLIENT_EVT_PNP_ID_RECEIVED

} bt_gatt_client_dis_evt_t;

typedef struct _bt_gatt_client_dis_s bt_gatt_client_dis_t;
typedef void (*bt_gatt_client_dis_callback_fp)(bt_gatt_client_dis_t* instance, bt_int evt, bt_gatt_client_dis_evt_t* evt_param, void* param);

typedef struct _bt_gatt_client_dis_byte_array_s
{
	bt_byte* value;
	bt_uint  len;
	bt_uint  max_len;
} bt_gatt_client_dis_byte_array_t;

typedef struct _bt_gatt_client_dis_system_id_s
{
	bt_byte  manufacturer_id_hi;
	bt_ulong manufacturer_id_lo;
	bt_ulong organizationally_unique_id;
} bt_gatt_client_dis_system_id_t;

typedef struct _bt_gatt_client_dis_pnp_id_s
{
	bt_byte vendor_id_source;
	bt_uint vendor_id;
	bt_uint product_id;
	bt_uint product_version;
} bt_gatt_client_dis_pnp_id_t;

struct _bt_gatt_client_dis_s
{
	bt_gatt_client_profile_header_t header;

	// DIS characteristic handles
	bt_uint handles[GATT_DIS_CLIENT_CHAR_COUNT];

	// DIS characteristic values
	bt_gatt_client_dis_byte_array_t values[GATT_DIS_CLIENT_CHAR_COUNT];
	bt_gatt_client_dis_system_id_t system_id;
	bt_gatt_client_dis_pnp_id_t pnp_id;

	bt_byte cur_char;
	bt_byte read_buffer[8];

	bt_gatt_client_dis_callback_fp callback;
	void* callback_param;
};

void bt_gatt_client_register_dis_finder(void);

void bt_gatt_client_dis_add_instance(
	bt_gatt_client_session_t* session, bt_gatt_client_dis_t* instance, 
	bt_byte* manufacturer_name_buffer,
	bt_uint  manufacturer_name_len,
	bt_byte* model_number_buffer,
	bt_uint  model_number_len,
	bt_byte* serial_number_buffer,
	bt_uint  serial_number_len,
	bt_byte* hardware_revision_buffer,
	bt_uint  hardware_revision_len,
	bt_byte* firmware_revision_buffer,
	bt_uint  firmware_revision_len,
	bt_byte* software_revision_buffer,
	bt_uint  software_revision_len,
	bt_byte* certification_buffer,
	bt_uint  certification_len,
	bt_gatt_client_dis_callback_fp callback, void* callback_param);

bt_gatt_client_dis_byte_array_t* bt_gatt_client_dis_get_manufacturer_name(bt_gatt_client_dis_t* instance);

bt_gatt_client_dis_byte_array_t* bt_gatt_client_dis_get_model_number(bt_gatt_client_dis_t* instance);

bt_gatt_client_dis_byte_array_t* bt_gatt_client_dis_get_serial_number(bt_gatt_client_dis_t* instance);

bt_gatt_client_dis_byte_array_t* bt_gatt_client_dis_get_hardware_revision(bt_gatt_client_dis_t* instance);

bt_gatt_client_dis_byte_array_t* bt_gatt_client_dis_get_firmware_revision(bt_gatt_client_dis_t* instance);

bt_gatt_client_dis_byte_array_t* bt_gatt_client_dis_get_software_revision(bt_gatt_client_dis_t* instance);

bt_gatt_client_dis_system_id_t* bt_gatt_client_dis_get_system_id(bt_gatt_client_dis_t* instance);

bt_gatt_client_dis_byte_array_t* bt_gatt_client_dis_get_certification(bt_gatt_client_dis_t* instance);

bt_gatt_client_dis_pnp_id_t* bt_gatt_client_dis_get_pnp_id(bt_gatt_client_dis_t* instance);


bt_bool bt_gatt_client_dis_read_manufacturer_name(bt_gatt_client_dis_t* instance);

bt_bool bt_gatt_client_dis_read_model_number(bt_gatt_client_dis_t* instance);

bt_bool bt_gatt_client_dis_read_serial_number(bt_gatt_client_dis_t* instance);

bt_bool bt_gatt_client_dis_read_hardware_revision(bt_gatt_client_dis_t* instance);

bt_bool bt_gatt_client_dis_read_firmware_revision(bt_gatt_client_dis_t* instance);

bt_bool bt_gatt_client_dis_read_software_revision(bt_gatt_client_dis_t* instance);

bt_bool bt_gatt_client_dis_read_system_id(bt_gatt_client_dis_t* instance);

bt_bool bt_gatt_client_dis_read_certification(bt_gatt_client_dis_t* instance);

bt_bool bt_gatt_client_dis_read_pnp_id(bt_gatt_client_dis_t* instance);

#endif // __GATT_DIS_H


