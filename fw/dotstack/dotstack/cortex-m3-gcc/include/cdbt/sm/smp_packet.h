/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __SMP_PACKET_H
#define __SMP_PACKET_H

#include "cdbt/utils/packet.h"

#ifdef __cplusplus
extern "C" {
#endif

	struct _bt_smp_session_t;
	typedef struct _bt_smp_packet_t
	{
		bt_packet_t header;
		bt_byte opcode;
		struct _bt_smp_session_t* session;

		union
		{
			struct  
			{
				bt_byte io_capability;
				bt_byte oob_data_flag;
				bt_byte auth_req;
				bt_byte max_key_size;
				bt_byte initiator_key_distribution;
				bt_byte responder_key_distribution;
			} pairing_request;

			struct  
			{
				bt_byte io_capability;
				bt_byte oob_data_flag;
				bt_byte auth_req;
				bt_byte max_key_size;
				bt_byte initiator_key_distribution;
				bt_byte responder_key_distribution;
			} pairing_response;

			struct  
			{
				bt_byte confirm_value[SMP_KEY_LEN];
			} pairing_confirm;

			struct  
			{
				bt_byte random_value[SMP_KEY_LEN];
			} pairing_random;

			struct
			{
				bt_byte reason;
			} pairing_failed;

			struct
			{
				bt_byte ltk[SMP_KEY_LEN];
			} encryption_info;

			struct
			{
				bt_uint ediv;
				bt_byte rand[SMP_RAND_LEN];
			} master_ident;

			struct
			{
				bt_byte irk[SMP_KEY_LEN]; // Identity Resolving Key
			} identity_info;

			struct
			{
				bt_byte addr_type;
				bt_bdaddr_t bdaddr;
			} identity_address_info;

			struct
			{
				bt_byte signature_key[SMP_KEY_LEN];
			} signing_info;

			struct
			{
				bt_byte auth_req;
			} sequrity_request;

		} data;

	} bt_smp_packet_t;

	bt_int _bt_smp_packet_assembler(bt_packet_t* packet, bt_byte* buffer, bt_int buffer_len);

	bt_int _bt_smp_assemble_pairing_request(bt_smp_packet_t* smp_packet, bt_byte* buffer, bt_int buffer_len);
	bt_int _bt_smp_assemble_pairing_response(bt_smp_packet_t* smp_packet, bt_byte* buffer, bt_int buffer_len);
	bt_int _bt_smp_assemble_pairing_confirm(bt_smp_packet_t* smp_packet, bt_byte* buffer, bt_int buffer_len);
	bt_int _bt_smp_assemble_pairing_random(bt_smp_packet_t* smp_packet, bt_byte* buffer, bt_int buffer_len);
	bt_int _bt_smp_assemble_pairing_failed(bt_smp_packet_t* smp_packet, bt_byte* buffer, bt_int buffer_len);
	bt_int _bt_smp_assemble_encryption_info(bt_smp_packet_t* smp_packet, bt_byte* buffer, bt_int buffer_len);
	bt_int _bt_smp_assemble_master_ident(bt_smp_packet_t* smp_packet, bt_byte* buffer, bt_int buffer_len);
	bt_int _bt_smp_assemble_identity_info(bt_smp_packet_t* smp_packet, bt_byte* buffer, bt_int buffer_len);
	bt_int _bt_smp_assemble_identity_address_info(bt_smp_packet_t* smp_packet, bt_byte* buffer, bt_int buffer_len);
	bt_int _bt_smp_assemble_signing_info(bt_smp_packet_t* smp_packet, bt_byte* buffer, bt_int buffer_len);
	bt_int _bt_smp_assemble_security_request(bt_smp_packet_t* smp_packet, bt_byte* buffer, bt_int buffer_len);

#ifdef __cplusplus
}
#endif

#endif // __SMP_PACKET_H
