/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __BTX_TOSHIBA_H_INCLUDED
#define __BTX_TOSHIBA_H_INCLUDED

#include "cdbt/bt/bt_std.h"
#include "cdbt/bt/bt_signal.h"
#include "cdbt/hci/hci.h"

typedef void (*btx_toshiba_read_dbus_callback_fp)(bt_int status, bt_byte address, bt_uint value);

typedef void (*btx_toshiba_write_dbus_callback_fp)(bt_int status, bt_byte address);

void btx_toshiba_init(void);

bt_bool btx_toshiba_set_uart_baud_rate(bt_ulong baud_rate, bt_hci_cmd_callback_fp callback);

bt_bool btx_toshiba_set_deep_sleep(
	bt_byte clkreq, 
	bt_byte deep_sleep_notification, 
	bt_uint slow_clock_drift, 
	bt_uint slow_clock_jitter, 
	bt_uint wakeup_delay,  
	bt_hci_cmd_callback_fp callback);

bt_bool btx_toshiba_enable_power_control(bt_bool enable, bt_hci_cmd_callback_fp callback);

bt_bool btx_toshiba_dbus_read(bt_byte address, btx_toshiba_read_dbus_callback_fp callback);

bt_bool btx_toshiba_dbus_write(bt_byte address, bt_uint value, btx_toshiba_write_dbus_callback_fp callback);

#endif // __BTX_TOSHIBA_H_INCLUDED
