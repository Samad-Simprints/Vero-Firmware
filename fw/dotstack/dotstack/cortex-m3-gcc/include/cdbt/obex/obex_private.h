/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*
* SEARAN LLC is the exclusive licensee and developer of dotstack with
* all its modifications and enhancements.
*
* Contains proprietary and confidential information of CandleDragon and
* may not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2009, 2010, 2011 CandleDragon. All Rights Reserved.
*******************************************************************************/

#ifndef __OBEX_PRIVATE_H
#define __OBEX_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

extern bt_obex_session   _obex_sessions[];
extern bt_byte           _obex_max_sessions;
extern bt_byte           _obex_rx_packet_buffers[];
extern bt_uint           _obex_max_packet_length;
extern bt_byte           _obex_rx_tmp_buffers[];
extern bt_byte           _obex_tx_packet_buffers[];

#ifdef _DEBUG
extern const bt_uint _ram_size_obex_buffers;
#endif

typedef void (*bt_obex_packet_handler_pf)(struct _bt_obex_session* session);

void bt_obex_init_sessions(void);
void bt_obex_free_session(bt_obex_session* psess);
bt_obex_session* bt_obex_find_session(bt_rfcomm_dlc_t* rfcomm_dlc);
bt_uint bt_calculate_packet_length(bt_byte opcode, bt_uint header_count, bt_obex_header* headers);
bt_bool bt_is_opcode_valid(bt_byte opcode);
void bt_obex_read_packet(bt_obex_session* session, bt_byte_p pdata, bt_int len, bt_obex_packet_handler_pf packet_handler);

#ifdef __cplusplus
}
#endif

#endif // __OBEX_PRIVATE_H
