/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#include "FreeRTOS.h"
#include "task.h"

#include "btmgr.h"
#include "bttask.h"
#include "btport.h"
#include "bttimer.h"
#include "error.h"

#include "flash_iap.h"

#include <assert.h>

// This module uses the last sector of the MCU Flash memory (sector 29) to store
// Bluetooth persistent data. This method is for demonstration purpose only as
// the sector size (32 KB) is much larger than needed.

#define FLASH_BANK           FLASH_BANK_B_ID
// Note sector number specified in next 2 lines!
#define FLASH_SECTOR         10
#define FLASH_START_ADDR     FLASH_ADDR(FLASH_BANK_B, FLASH_SECTOR_10)
#define FLASH_WRITE_SIZE     512

// Storage size in FLASH_WRITE_SIZE-byte blocks
#define STORAGE_SIZE  ((sizeof(btmgr_PersistentData) + (FLASH_WRITE_SIZE-1)) / FLASH_WRITE_SIZE)

static uint8_t mRamCopy[STORAGE_SIZE * FLASH_WRITE_SIZE];
static char mSaving;
static btmgr_pal_WritePersistentDataCallback  mCallback;

static void saveToFlash(void);
static void timerCallback(void);

void bttask_pal_initStorage(void)
{
	// Read data from Flash memory
	memcpy(mRamCopy, (uint8_t*)FLASH_START_ADDR, sizeof(mRamCopy));
        memset(mRamCopy, 0, sizeof(mRamCopy)); //NST
}


void bttask_pal_startStorage(bttask_StartCallback callback)
{
	// There is nothing to do here in this implementation so call the
	// callback right away.
	callback();
}


btmgr_PersistentData* btmgr_pal_getPersistentData(void)
{
	return (btmgr_PersistentData*) mRamCopy;
}


void btmgr_pal_writePersistentData(btmgr_pal_WritePersistentDataCallback callback)
{
	assert(!mSaving);

	mCallback = callback;
	
	saveToFlash();
}


void bttask_pal_handleStorageSignal(void)
{
	// Not used in this implementation
}

static void saveToFlash(void)
{
	if (!mSaving)
	{
		mSaving = 2;
	
		// While Flash memory is being erased/written, program execution
		// from flash is not possible (the program will be suspended until
		// the erase/write cycle is complete.)
		// This presents a problem as interrupts from UART will not be processed
		// in time and commuincation with the BT controller will be lost.
		// To solve the problem we ask the btport module to use flow control to
		// tell the BT controller to stop sending data. Then we proceed with
		// erasing and writing Flash memory.
		btport_suspendRx();

		// We need to let some time to the BT controller to stop sending data.
		// Use application timer 0 to do that.
		// Actual writing to Flash will occur in the timer callback function.
		bttimer_setTimer(BTTIMER_STORAGE, 500, &timerCallback);
	}
}

static void timerCallback(void)
{
	int i;
	uint32_t flashAddress;

        // prepare IAP subsystem
        iap_init();

	// Prepare sector containing Bluetooth persistent data writing
	iap_write_prepare(FLASH_SECTOR, FLASH_SECTOR, FLASH_BANK);
	
	// Erase sector containing Bluetooth persistent data
	iap_erase(FLASH_SECTOR, FLASH_SECTOR, FLASH_BANK);
	
	// Write Bluetooth persistent data to flash
	flashAddress = FLASH_START_ADDR;
	for (i = 0; i < STORAGE_SIZE; i++, flashAddress += FLASH_WRITE_SIZE)
	{
		// Prepare sector containing Bluetooth persistent data writing
		iap_write_prepare(FLASH_SECTOR, FLASH_SECTOR, FLASH_BANK);
		
		// Write data
		iap_write((const char*)&mRamCopy[i * FLASH_WRITE_SIZE], (const char*)flashAddress, FLASH_WRITE_SIZE);
	}

	// Verify written data
	if (iap_compare((const char*)FLASH_START_ADDR, (const char*)mRamCopy, sizeof(mRamCopy)) != CMD_SUCCESS)
	{
		error_onFatalError();
	}

	// Resume receive operation in btport
	btport_resumeRx();
	
	mSaving = 0;
	mCallback();
}

