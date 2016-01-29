/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __GATT_PRIVATE_H
#define __GATT_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

bt_bool _bt_gatt_find_char_descriptor(
	bt_uint value_handle, bt_uuid16 descriptor_type, 
	bt_att_attribute_t* descriptor_attr);

bt_bool _bt_gatt_find_char_value_attr(
	bt_uuid16 service_type, 
	bt_uuid16 service_id, 
	bt_uuid16 characteristic_id,
	bt_att_attribute_t* attr);

bt_bool _bt_gatt_find_char_value_attr2(
	bt_uint declaration_handle, 
	bt_uuid_t* characteristic_id,
	bt_att_attribute_t* attr);

bt_bool _bt_gatt_find_char_value_attr3(
	bt_uuid_t* service_type, 
	bt_uuid_t* service_id, 
	bt_uuid_t* characteristic_id,
	bt_att_attribute_t* attr);

bt_bool _bt_gatt_find_char_for_attr(
	bt_uint handle,
	bt_att_attribute_t* attr);

bt_bool _bt_gatt_find_service_for_attr(
	bt_uint handle,
	bt_att_attribute_t* attr);

void _bt_gatt_server_init(void);

void _bt_gatt_server_notify_listeners(bt_int evt, void* evt_param);

#ifdef __cplusplus
}
#endif

#endif // __GATT_PRIVATE_H
