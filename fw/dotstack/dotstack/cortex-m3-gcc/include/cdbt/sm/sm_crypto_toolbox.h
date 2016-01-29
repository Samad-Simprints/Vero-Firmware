/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __SM_CRYPTO_TOOLBOX_H
#define __SM_CRYPTO_TOOLBOX_H

#ifdef __cplusplus
extern "C" {
#endif

#define SM_ENCRYPTION_BLOCK_LEN    16

#define SM_CRYPTO_EVT_NOTHING         0
#define SM_CRYPTO_EVT_ENCRYPT         1
#define SM_CRYPTO_EVT_ADDRESS_HASH    2
#define SM_CRYPTO_EVT_CONFIRM         3
#define SM_CRYPTO_EVT_KEY             4
#define SM_CRYPTO_EVT_DIVERSIFIER     5
#define SM_CRYPTO_EVT_DIV_MASK        6
#define SM_CRYPTO_EVT_PRAND           7
#define SM_CRYPTO_EVT_RANDOM_ADDRESS  8

typedef struct _bt_sm_evt_header_s
{
	bt_byte status;
} bt_sm_evt_header_t;

typedef struct _bt_sm_evt_encrypt_s
{
	bt_sm_evt_header_t header;
	bt_byte* encrypted_data;
} bt_sm_encrypt_t;

typedef struct _bt_sm_evt_address_hash_s
{
	bt_sm_evt_header_t header;
	bt_byte* hash;
} bt_sm_evt_address_hash_t;

typedef struct _bt_sm_evt_confirm_s
{
	bt_sm_evt_header_t header;
	bt_byte* confirm;
} bt_sm_evt_confirm_t;

typedef struct _bt_sm_evt_key_s
{
	bt_sm_evt_header_t header;
	bt_byte* key;
} bt_sm_evt_key_t;

typedef struct _bt_sm_evt_diversifier_s
{
	bt_sm_evt_header_t header;
	bt_byte* diversifier;
} bt_sm_evt_diversifier_t;

typedef struct _bt_sm_evt_div_mask_s
{
	bt_sm_evt_header_t header;
	bt_uint div_mask;
} bt_sm_evt_div_mask_t;

typedef struct _bt_sm_evt_address_s
{
	bt_sm_evt_header_t header;
	bt_byte address_type;
	bt_bdaddr_t* address;
} bt_sm_evt_address_t;

typedef union _bt_sm_crypto_event_u
{
	bt_sm_evt_header_t header;
	bt_sm_encrypt_t encrypt;
	bt_sm_evt_address_hash_t address_hash;
	bt_sm_evt_confirm_t confirm;
	bt_sm_evt_key_t key;
	bt_sm_evt_diversifier_t diversifier;
	bt_sm_evt_div_mask_t div_mask;
	bt_sm_evt_address_t address;
} bt_sm_crypto_event_t;


typedef void (*bt_sm_crypto_callback_fp)(bt_byte evt, bt_sm_crypto_event_t* evt_param, void* callback_param);

bt_bool bt_sm_encrypt(
	const bt_byte* key, const bt_byte* data,
	bt_sm_crypto_callback_fp callback, void* callback_param);

bt_bool bt_sm_address_hash(
	const bt_byte* key, const bt_byte* r,
	bt_sm_crypto_callback_fp callback, void* callback_param);

bt_bool bt_sm_generate_confirmation(
	const bt_byte* key, const bt_byte* r, const bt_byte* pres, const bt_byte* preq,
	bt_byte iat, const bt_bdaddr_t* ia, bt_byte rat, const bt_bdaddr_t* ra, 
	bt_sm_crypto_callback_fp callback, void* callback_param);

bt_bool bt_sm_generate_key(
	const bt_byte* key, const bt_byte* r1, const bt_byte* r2,
	bt_sm_crypto_callback_fp callback, void* callback_param);

bt_bool bt_sm_generate_diversifier(
	const bt_byte* key, bt_uint d, bt_uint r,
	bt_sm_crypto_callback_fp callback, void* callback_param);

bt_bool bt_sm_generate_div_mask(
   const bt_byte* key, const bt_byte* r,
   bt_sm_crypto_callback_fp callback, void* callback_param);

bt_bool bt_sm_generate_random_address(
	bt_byte address_type, const bt_byte* irk,
	bt_sm_crypto_callback_fp callback, void* callback_param);

#ifdef __cplusplus
}
#endif

#endif // __SM_CRYPTO_TOOLBOX_H
