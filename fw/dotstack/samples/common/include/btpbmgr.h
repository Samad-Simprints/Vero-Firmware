/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __BTPBMGR_H_INCLUDED__
#define __BTPBMGR_H_INCLUDED__

#include "cdbt/bt/bt_std.h"
#include "btapp_config.h"

//-----------------------------------------------------------------------------
//   Public interface.
//-----------------------------------------------------------------------------

#define BTPBMGR_MAX_VALUE_LEN         31
#define BTPBMGR_MAX_ENTRIES           100
#define BTPBMGR_MAX_BOOKS             4

typedef struct _btpbmgr_PhoneBookEntry
{
	bt_byte number[BTPBMGR_MAX_VALUE_LEN + 1];
	bt_byte name[BTPBMGR_MAX_VALUE_LEN + 1];
	bt_byte callType[BTPBMGR_MAX_VALUE_LEN + 1];
} btpbmgr_PhoneBookEntry;

typedef struct _btpbmgr_PhoneBook
{
	bt_uint                 signature;
	bt_uint                 numOfEntries;
	btpbmgr_PhoneBookEntry  entries[1];
} btpbmgr_PhoneBook;

typedef void (*btpbmgr_Callback)(void);

void btpbmgr_init(void);

void btpbmgr_start(btpbmgr_Callback callback);

void btpbmgr_getPhoneBookHeader(bt_byte index, btpbmgr_PhoneBook** pb, btpbmgr_Callback callback);

void btpbmgr_getPhoneBook(bt_byte index, btpbmgr_PhoneBook** pb, btpbmgr_Callback callback);

void btpbmgr_savePhoneBookStart(bt_byte index, btpbmgr_Callback callback);

void btpbmgr_addPhoneBookEntry(bt_byte index, btpbmgr_PhoneBookEntry* entry, btpbmgr_Callback callback);

void btpbmgr_savePhoneBookEnd(bt_byte index, btpbmgr_Callback callback);

void btpbmgr_clearAllPhoneBooks(btpbmgr_Callback callback);

void btpbmgr_clearPhoneBooks(bt_byte start, bt_byte end, btpbmgr_Callback callback);

//-----------------------------------------------------------------------------
//   Platform Abstraction Layer interface.
//-----------------------------------------------------------------------------

typedef void (*btpbmgr_pal_PersistentDataCallback)(void);

void btpbmgr_pal_initStorage(void);

void btpbmgr_pal_getPhoneBookHeader(bt_byte index, btpbmgr_PhoneBook** pb, btpbmgr_pal_PersistentDataCallback callback);

void btpbmgr_pal_getPhoneBook(bt_byte index, btpbmgr_PhoneBook** pb, btpbmgr_pal_PersistentDataCallback callback);

void btpbmgr_pal_addPhoneBookEntry(bt_byte index, btpbmgr_PhoneBookEntry* entry, btpbmgr_pal_PersistentDataCallback callback);

void btpbmgr_pal_savePhoneBookHeader(bt_byte index, btpbmgr_pal_PersistentDataCallback callback);

void btpbmgr_pal_clearAllPhoneBooks(btpbmgr_pal_PersistentDataCallback callback);

void btpbmgr_pal_clearPhoneBooks(bt_byte start, bt_byte end, btpbmgr_pal_PersistentDataCallback callback);

#endif // __BTPBMGR_H_INCLUDED__
