/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#include <string.h>

#include "btpbmgr.h"


#define BTLOG_TAG "BTPBMGR"
#include "btlog_define.h"

#define SIGNATURE     0x4827

#define FLAG_CHANGED  0x01
#define FLAG_WRITING  0x02

static btpbmgr_Callback   mCallback;
static bt_byte            mCurrentBook;
static btpbmgr_PhoneBook* mPbHeader;

static void writePersistentDataCallback(void);
static void readPersistentDataCallback(void);
static void clearAllPhoneBooksCallback(void);
static void savePhoneBookHeaderCallback(void);
static void getPbHeaderCallback(void);
static void checkPbHeaderIntegrityCallback(void);
static void clearPbHeaderCallback(void);


void btpbmgr_init(void)
{
	mCallback = 0;
}

void btpbmgr_start(btpbmgr_Callback callback)
{
	mCallback = callback;

	btpbmgr_pal_getPhoneBookHeader(0, &mPbHeader, &checkPbHeaderIntegrityCallback);
}

void btpbmgr_getPhoneBookHeader(bt_byte index, btpbmgr_PhoneBook** pb, btpbmgr_Callback callback)
{
	mCallback = callback;

	btpbmgr_pal_getPhoneBookHeader(index, pb, &readPersistentDataCallback);
}

void btpbmgr_getPhoneBook(bt_byte index, btpbmgr_PhoneBook** pb, btpbmgr_Callback callback)
{
	mCallback = callback;

	btpbmgr_pal_getPhoneBook(index, pb, &readPersistentDataCallback);
}

void btpbmgr_savePhoneBookStart(bt_byte index, btpbmgr_Callback callback)
{
	mCallback = callback;

	btpbmgr_pal_getPhoneBookHeader(index, &mPbHeader, &getPbHeaderCallback);
}

void btpbmgr_addPhoneBookEntry(bt_byte index, btpbmgr_PhoneBookEntry* entry, btpbmgr_Callback callback)
{
	mCallback = callback;
	btpbmgr_pal_addPhoneBookEntry(index, entry, &writePersistentDataCallback);
}

void btpbmgr_savePhoneBookEnd(bt_byte index, btpbmgr_Callback callback)
{
	mCallback = callback;
	btpbmgr_pal_savePhoneBookHeader(index, &writePersistentDataCallback);
}

void btpbmgr_clearAllPhoneBooks(btpbmgr_Callback callback)
{
	mCallback = callback;

	btpbmgr_pal_clearAllPhoneBooks(&writePersistentDataCallback);
}

void btpbmgr_clearPhoneBooks(bt_byte start, bt_byte end, btpbmgr_Callback callback)
{
	mCallback = callback;

	btpbmgr_pal_clearPhoneBooks(start, end, &writePersistentDataCallback);
}

static void writePersistentDataCallback(void)
{
	mCallback();
}

static void readPersistentDataCallback(void)
{
	mCallback();
}

static void clearAllPhoneBooksCallback(void)
{
	mCurrentBook = 0;
	btpbmgr_pal_getPhoneBookHeader(0, &mPbHeader, &clearPbHeaderCallback);
}

static void clearPbHeaderCallback(void)
{
	// Initialzie phone book header
	memset(mPbHeader, 0, sizeof(btpbmgr_PhoneBook));
	mPbHeader->signature = SIGNATURE;
	btpbmgr_pal_savePhoneBookHeader(mCurrentBook, &savePhoneBookHeaderCallback);
}

static void savePhoneBookHeaderCallback(void)
{
	if (++mCurrentBook < BTPBMGR_MAX_BOOKS)
	{
		btpbmgr_pal_getPhoneBookHeader(mCurrentBook, &mPbHeader, &clearPbHeaderCallback);
	}
	else
	{
		mCallback();
	}
}

static void checkPbHeaderIntegrityCallback(void)
{
	// Check if it is valid
	if (mPbHeader->signature != SIGNATURE)
	{
		btpbmgr_pal_clearAllPhoneBooks(&clearAllPhoneBooksCallback);
	}
	else
	{
		mCallback();
	}
}

static void getPbHeaderCallback(void)
{
	mPbHeader->signature = SIGNATURE;
	mPbHeader->numOfEntries = 0;

	mCallback();
}
