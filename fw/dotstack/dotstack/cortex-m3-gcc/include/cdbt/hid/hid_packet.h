/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __HID_PACKET_H
#define __HID_PACKET_H

#include "cdbt/utils/packet.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _bt_l2cap_channel_t;

typedef struct _bt_hid_packet_t
{
	bt_packet_t header;

	bt_l2cap_channel_t* pch;
	bt_int report_data_len;
	bt_byte report_type;
	bt_byte report_id;
	bt_bool send_report_id;
	bt_byte tran_type;
	bt_hid_send_report_callback_fp send_cb;

} bt_hid_packet_t;

bt_int bt_hid_packet_assembler(bt_packet_t* packet, bt_byte* buffer, bt_int buffer_len);

#ifdef __cplusplus
}
#endif

#endif // __HID_PACKET_H
