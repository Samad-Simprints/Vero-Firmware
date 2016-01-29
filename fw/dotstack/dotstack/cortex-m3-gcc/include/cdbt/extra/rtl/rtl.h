/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __BTX_RTL_H_INCLUDED
#define __BTX_RTL_H_INCLUDED

#include "cdbt/bt/bt_std.h"
#include "cdbt/bt/bt_signal.h"
#include "cdbt/hci/hci.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RTL_MAX_PATCH_PACKET_LEN                 252

#define RTL_CONFIG_SIGNATURE                     0x8723AB55

#define RTL_CONFIG_REG_BAUD_RATE                 0x000c
#define RTL_CONFIG_REG_HOST_INTERFACE            0x0014
#define RTL_CONFIG_REG_UART_CONFIG               0x0018
#define RTL_CONFIG_REG_BDADDR                    0x003C
#define RTL_CONFIG_REG_RTS_PIN_FUNCTION          0x01E3

#define RTL_CONFIG_RTS_PIN_FUNCTION_RTS          0
#define RTL_CONFIG_RTS_PIN_FUNCTION_32K_CLOCK    1

#define RTL_CONFIG_HOST_INTERFACE_H4             0
#define RTL_CONFIG_HOST_INTERFACE_H5             1

#define RTL_CONFIG_UART_CONFIG_ENABLE_PARITY     1
#define RTL_CONFIG_UART_CONFIG_PARITY_EVEN       2
#define RTL_CONFIG_UART_CONFIG_HW_FLOW_CONTROL   4

// Macros for defining configuration file.
#define SET_CONFIG_VALUE_UINT8(reg_offset, value)	\
	(uint8_t)(((uint16_t)reg_offset) & 0xFF), (uint8_t)((((uint16_t)reg_offset) >> 8) & 0xFF),	\
	1,	\
	(uint8_t)(value)

#define SET_CONFIG_VALUE_UINT32(reg_offset, value)	\
	(uint8_t)(((uint16_t)reg_offset) & 0xFF), (uint8_t)((((uint16_t)reg_offset) >> 8) & 0xFF), \
	4,	\
	(uint8_t)(((uint32_t)(value)) & 0xFF), (uint8_t)((((uint16_t)(value)) >> 8) & 0xFF), (uint8_t)((((uint16_t)(value)) >> 16) & 0xFF), (uint8_t)((((uint16_t)(value)) >> 24) & 0xFF)

#define SET_CONFIG_VALUE_BDADDR(reg_offset, m, l)	\
	(uint8_t)(((uint16_t)reg_offset) & 0xFF), (uint8_t)((((uint16_t)reg_offset) >> 8) & 0xFF), \
	6,	\
	(uint8_t)(((uint32_t)l) & 0xFF), (uint8_t)((((uint32_t)l) >> 8) & 0xFF), (uint8_t)((((uint32_t)l) >> 16) & 0xFF), (uint8_t)((((uint32_t)l) >> 24) & 0xFF),	\
	(uint8_t)(((uint32_t)m) & 0xFF), (uint8_t)((((uint32_t)m) >> 8) & 0xFF)

typedef struct _btx_rtl_upload_patch_and_config_buffer_s btx_rtl_upload_patch_and_config_buffer_t;

typedef void(*btx_rtl_upload_patch_and_config_callback_fp)(bt_bool success, btx_rtl_upload_patch_and_config_buffer_t* buffer);

typedef struct _btx_rtl_patch_s
{
	const bt_byte* data;
	bt_int len;
} btx_rtl_patch_t;

struct _btx_rtl_upload_patch_and_config_buffer_s
{
	bt_byte phase;
	const btx_rtl_patch_t* patch;
	const bt_byte* config;
	bt_int config_len;
	bt_byte current_packet;
	bt_int data_remaining_len;
	bt_byte last_packet;
	btx_rtl_upload_patch_and_config_callback_fp callback;
	void* callback_param;
};

typedef struct _btx_rtl_8761a_uart_baud_rate_configuration_s
{
	bt_ulong baud_rate;
	bt_ulong baud_setting;
} btx_rtl_8761a_uart_baud_rate_configuration_t;

void btx_rtl_upload_patch_and_config(
	const btx_rtl_patch_t* patch,
	const bt_byte* data,
	btx_rtl_upload_patch_and_config_buffer_t* buffer,
	btx_rtl_upload_patch_and_config_callback_fp callback,
	void* callback_param);

bt_bool btx_rtl_set_uart_baud_rate(bt_ulong baud_rate, bt_hci_cmd_callback_fp callback);

const btx_rtl_patch_t* btx_rtl_get_patch(void);

#ifdef __cplusplus
}
#endif

#endif // __BTX_RTL_H_INCLUDED
