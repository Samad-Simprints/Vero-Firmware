/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/


#ifndef __AVDTP_CONTROL_H
#define __AVDTP_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

#define AVDTP_CTRL_CHANNEL_STATE_DISCONNECTED    0
#define AVDTP_CTRL_CHANNEL_STATE_CONNECTED	     1
#define AVDTP_CTRL_CHANNEL_STATE_CONNECTING      2

#define AVDTP_CTLR_CHANNEL_EVT_NOTHING              0
#define AVDTP_CTLR_CHANNEL_EVT_CONNECTED            1
#define AVDTP_CTLR_CHANNEL_EVT_DISCONNECTED         2
#define AVDTP_CTLR_CHANNEL_EVT_CONNECTION_FAILED    3
#define AVDTP_CTRL_CHANNEL_EVT_DATA_RECEIVED        4

#define AVDTP_CTRL_PACKET_TYPE_SIGNLE      0
#define AVDTP_CTRL_PACKET_TYPE_START       0x04
#define AVDTP_CTRL_PACKET_TYPE_CONTINUE    0x08
#define AVDTP_CTRL_PACKET_TYPE_END         0x0c
#define AVDTP_CTRL_PACKET_TYPE_FLD         0x0c

#define AVDTP_CTRL_MESSAGE_TYPE_COMMAND             0
#define AVDTP_CTRL_MESSAGE_TYPE_GENERAL_REJECT      1
#define AVDTP_CTRL_MESSAGE_TYPE_ACCEPT              2
#define AVDTP_CTRL_MESSAGE_TYPE_REJECT              3
#define AVDTP_CTRL_MESSAGE_TYPE_FLD                 3

#define AVDTP_CMD_DISCOVER             1
#define AVDTP_CMD_GET_CAPABILITIES     2
#define AVDTP_CMD_SET_CONFIGURATION    3
#define AVDTP_CMD_GET_CONFIGURATION    4
#define AVDTP_CMD_RECONFIGURE          5
#define AVDTP_CMD_OPEN                 6
#define AVDTP_CMD_START                7
#define AVDTP_CMD_CLOSE                8
#define AVDTP_CMD_SUSPEND              9
#define AVDTP_CMD_ABORT                10
#define AVDTP_CMD_SECURITY_CONTROL     11
#define AVDTP_CMD_GET_ALL_CAPABILITIES     12
#define AVDTP_CMD_DELAYREPORT          13


typedef struct _bt_avdtp_control_cmd_t
{
	struct _bt_avdtp_control_cmd_t* next_cmd;

	bt_byte code;
	bt_byte type;
	bt_byte transaction_id;
	bt_byte* params;
	bt_uint params_len;
} bt_avdtp_control_cmd_t;

struct _bt_avdtp_mgr_t;

typedef struct _bt_avdtp_control_channel_t
{
	bt_byte state;
	bt_bdaddr_t remote_addr;
	bt_l2cap_channel_t* l2cap_channel;
	bt_byte* tx_buffer;

	bt_queue_element_t*    send_cq_head;
	bt_queue_element_t*    ack_cq_head;
	bt_byte next_transaction_id;

	bt_byte next_ts_id;
	bt_byte next_tc_id;

	struct _bt_avdtp_mgr_t* avdtp_mgr;

} bt_avdtp_control_channel_t;

typedef struct _bt_avdtp_ctrl_evt_data_received_t
{
	bt_byte* data;
	bt_int len;
} bt_avdtp_ctrl_evt_data_received_t;

bt_avdtp_control_channel_t* _bt_avdtp_get_control_channel(struct _bt_avdtp_mgr_t* mgr, bt_bdaddr_t* remote_addr);
bt_bool _bt_avdtp_open_control_channel_ex(struct _bt_avdtp_mgr_t* mgr, bt_bdaddr_t* remote_addr, bt_uint acl_config);

bt_bool _bt_avdtp_add_param_byte(bt_avdtp_control_cmd_t* command, bt_byte value);
bt_bool _bt_avdtp_add_param_uint(bt_avdtp_control_cmd_t* command, bt_uint value);
bt_bool _bt_avdtp_add_param_uintn(bt_avdtp_control_cmd_t* command, bt_uint value);

void _bt_avdtp_control_channel_event_handler(bt_avdtp_control_channel_t* channel, bt_byte evt, void* evt_param);

#ifdef __cplusplus
}
#endif

#endif // __AVDTP_CONTROL_H
