/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#include "btlog.h"

#include "cdbt/bt/bt_std.h"
#include "cdbt/bt/bt_system.h"
#include "cdbt/bt/bt_log.h"

#include <stdarg.h>

#ifdef ENABLE_LOG

#define BTLOG_BUFFER_SIZE_MASK    (BTLOG_BUFFER_SIZE - 1)

// Circular buffer for storing log entries until they can be transmitted.
static char             mLogBuffer[BTLOG_BUFFER_SIZE];
static volatile int     mLogHead;
static int              mLogTail;
static char             mNumBuffer[37];

// Flag showing that transmission is in progress.
static volatile char    mTransmitting;

// Log overflow indicator
static char             mOverflow;

// Buffer for composing log messages.
static char mBuffer[BTLOG_MAX_MESSAGE_LEN];

// Buffer for composing log messages coming from the core library
// through bt_oem_log_write()
static char mOemBuffer[BTLOG_OEM_BUFFER_SIZE];
static bt_int mOemIndex;


static void writeToLogBuffer(const char* msg);


void btlog_init(void)
{
	mLogHead = mLogTail = 0;
	mTransmitting = 0;
	mOverflow = 0;
	mOemIndex = 0;
	
	btlog_pal_init();
}


void btlog_start(void)
{
	btlog_pal_start();
}


void btlog_writeEntry(const char* tag, const char* message)
{
	int index = 0;
	int size = BTLOG_MAX_MESSAGE_LEN;
	
	btlog_pal_lockBuffer();
	{
		if (tag != NULL)
		{
			index = 0;
			while (*tag && index < 10) // limit tag to 10 characters
			{
				mBuffer[index++] = *tag++;
			}
			mBuffer[index++] = ':';
			size -= index;
		}
		
		size -= 3; // reserve space for end of line
		while (*message && index < size)
		{
			mBuffer[index++] = *message++;
		}
		mBuffer[index++] = '\r';
		mBuffer[index++] = '\n';
		mBuffer[index++] = 0;
	
		writeToLogBuffer(mBuffer);
	}
	btlog_pal_unlockBuffer();
}


void btlog_formatEntry(const char* tag, const char* format, ...)
{
	int index = 0;
	int size = BTLOG_MAX_MESSAGE_LEN;
	char c;
	va_list ap;
	
	va_start(ap, format);
	
	btlog_pal_lockBuffer();
	{
		if (tag != NULL)
		{
			index = 0;
			while (*tag && index < 10) // limit tag to 10 characters
			{
				mBuffer[index++] = *tag++;
			}
			mBuffer[index++] = ':';
			size -= index;
		}
		
		size -= 3; // reserve space for end of line
		while (*format && index < size)
		{
			c = *format++;
			if (c == '\\')
			{
				c = *format;
				if (c != 0)
				{
					format++;
					mBuffer[index++] = c;
				}
			}
			else if (c == '%')
			{
				c = *format;
				if (c != 0)
				{
					int i = 0;
					
					format++;
					if (c == 'd')
					{
						int val, r;
						val =  va_arg(ap, int);
						do
						{
							r = val % 10;
							val /= 10;
							mNumBuffer[i++] = r + '0';
						}
						while (val);
					}
					else if (c == 'x')
					{
						unsigned int val, r;
						val =  va_arg(ap, unsigned int);
						do
						{
							r = val % 16;
							val /= 16;
							if (r < 10)
								mNumBuffer[i++] = r + '0';
							else
								mNumBuffer[i++] = r + 'A' - 10;
						}
						while (val);
					}
					else if (c == 'b')
					{
						bt_bdaddr_t* bdaddr;
						
						bdaddr = va_arg(ap, bt_bdaddr_t*);

						mNumBuffer[i++] = *_ulong2str(bdaddr->bd_addr_l & 0x0000000F);
						mNumBuffer[i++] = *_ulong2str((bdaddr->bd_addr_l & 0x000000F0) >> 4);
						mNumBuffer[i++] = ':';
						mNumBuffer[i++] = *_ulong2str((bdaddr->bd_addr_l & 0x00000F00) >> 8);
						mNumBuffer[i++] = *_ulong2str((bdaddr->bd_addr_l & 0x0000F000) >> 12);
						mNumBuffer[i++] = ':';
						mNumBuffer[i++] = *_ulong2str((bdaddr->bd_addr_l & 0x000F0000) >> 16);
						mNumBuffer[i++] = *_ulong2str((bdaddr->bd_addr_l & 0x00F00000) >> 20);
						mNumBuffer[i++] = ':';
						mNumBuffer[i++] = *_ulong2str((bdaddr->bd_addr_l & 0x0F000000) >> 24);
						mNumBuffer[i++] = *_ulong2str((bdaddr->bd_addr_l & 0xF0000000) >> 28);
						mNumBuffer[i++] = ':';
						mNumBuffer[i++] = *_ulong2str(bdaddr->bd_addr_m & 0x000F);
						mNumBuffer[i++] = *_ulong2str((bdaddr->bd_addr_m & 0x00F0) >> 4);
						mNumBuffer[i++] = ':';
						mNumBuffer[i++] = *_ulong2str((bdaddr->bd_addr_m & 0x0F00) >> 8);
						mNumBuffer[i++] = *_ulong2str((bdaddr->bd_addr_m & 0xF000) >> 12);
					}
					else if (c == 'u')
					{
						bt_uuid_t* uuid;
						
						uuid = va_arg(ap, bt_uuid_t*);
						
						mNumBuffer[i++] = *_ulong2str((uuid->uuid0 & 0x0000000F));
						mNumBuffer[i++] = *_ulong2str((uuid->uuid0 & 0x000000F0) >> 4);
						mNumBuffer[i++] = *_ulong2str((uuid->uuid0 & 0x00000F00) >> 8);
						mNumBuffer[i++] = *_ulong2str((uuid->uuid0 & 0x0000F000) >> 12);
						mNumBuffer[i++] = *_ulong2str((uuid->uuid0 & 0x000F0000) >> 16);
						mNumBuffer[i++] = *_ulong2str((uuid->uuid0 & 0x00F00000) >> 20);
						mNumBuffer[i++] = *_ulong2str((uuid->uuid0 & 0x0F000000) >> 24);
						mNumBuffer[i++] = *_ulong2str((uuid->uuid0 & 0xF0000000) >> 28);
						mNumBuffer[i++] = *_ulong2str((uuid->uuid1 & 0x0000000F));
						mNumBuffer[i++] = *_ulong2str((uuid->uuid1 & 0x000000F0) >> 4);
						mNumBuffer[i++] = *_ulong2str((uuid->uuid1 & 0x00000F00) >> 8);
						mNumBuffer[i++] = *_ulong2str((uuid->uuid1 & 0x0000F000) >> 12);
						mNumBuffer[i++] = '-';
						mNumBuffer[i++] = *_ulong2str((uuid->uuid1 & 0x000F0000) >> 16);
						mNumBuffer[i++] = *_ulong2str((uuid->uuid1 & 0x00F00000) >> 20);
						mNumBuffer[i++] = *_ulong2str((uuid->uuid1 & 0x0F000000) >> 24);
						mNumBuffer[i++] = *_ulong2str((uuid->uuid1 & 0xF0000000) >> 28);
						mNumBuffer[i++] = '-';
						mNumBuffer[i++] = *_ulong2str((uuid->uuid2 & 0x0000000F));
						mNumBuffer[i++] = *_ulong2str((uuid->uuid2 & 0x000000F0) >> 4);
						mNumBuffer[i++] = *_ulong2str((uuid->uuid2 & 0x00000F00) >> 8);
						mNumBuffer[i++] = *_ulong2str((uuid->uuid2 & 0x0000F000) >> 12);
						mNumBuffer[i++] = '-';
						mNumBuffer[i++] = *_ulong2str((uuid->uuid2 & 0x000F0000) >> 16);
						mNumBuffer[i++] = *_ulong2str((uuid->uuid2 & 0x00F00000) >> 20);
						mNumBuffer[i++] = *_ulong2str((uuid->uuid2 & 0x0F000000) >> 24);
						mNumBuffer[i++] = *_ulong2str((uuid->uuid2 & 0xF0000000) >> 28);
						mNumBuffer[i++] = '-';
						mNumBuffer[i++] = *_ulong2str((uuid->uuid3 & 0x0000000F));
						mNumBuffer[i++] = *_ulong2str((uuid->uuid3 & 0x000000F0) >> 4);
						mNumBuffer[i++] = *_ulong2str((uuid->uuid3 & 0x00000F00) >> 8);
						mNumBuffer[i++] = *_ulong2str((uuid->uuid3 & 0x0000F000) >> 12);
						mNumBuffer[i++] = *_ulong2str((uuid->uuid3 & 0x000F0000) >> 16);
						mNumBuffer[i++] = *_ulong2str((uuid->uuid3 & 0x00F00000) >> 20);
						mNumBuffer[i++] = *_ulong2str((uuid->uuid3 & 0x0F000000) >> 24);
						mNumBuffer[i++] = *_ulong2str((uuid->uuid3 & 0xF0000000) >> 28);
						
					}
					else if (c == 's')
					{
						char* s;
						
						s = va_arg(ap, char*);
						
						while (index < size && *s)
						{
							mBuffer[index++] = *s++;
						}
					}
					
					while (index < size && i > 0)
					{
						mBuffer[index++] = mNumBuffer[--i];
					}
				}
			}
			else
			{
				mBuffer[index++] = c;
			}
		}
		mBuffer[index++] = '\r';
		mBuffer[index++] = '\n';
		mBuffer[index++] = 0;
	
		writeToLogBuffer(mBuffer);
	}
	btlog_pal_unlockBuffer();
				
	va_end(ap);
}


void bt_oem_log_write(const char* msg)
{
	char c;
	
	while ((c = *msg++) != 0)
	{
		if (mOemIndex == (BTLOG_OEM_BUFFER_SIZE - 3))
		{
			mOemBuffer[mOemIndex++] = '\r';
			mOemBuffer[mOemIndex++] = '\n';
			mOemBuffer[mOemIndex++] = 0;
			
			btlog_pal_lockBuffer();
			writeToLogBuffer(mOemBuffer);
			btlog_pal_unlockBuffer();
			mOemIndex = 0;
		}
		
		mOemBuffer[mOemIndex++] = c;
		
		if (c == '\n')
		{
			mOemBuffer[mOemIndex++] = 0;
			
			btlog_pal_lockBuffer();
			writeToLogBuffer(mOemBuffer);
			btlog_pal_unlockBuffer();
			mOemIndex = 0;
		}
	}
}


static void writeToLogBuffer(const char* msg)
{
	int avail;
	int len = strlen(msg);
	
	btlog_pal_lockTransmission();
	{
		// Get available space in the buffer
		avail = BTLOG_BUFFER_SIZE - 1 - ((mLogTail - mLogHead) & BTLOG_BUFFER_SIZE_MASK);
	}
	btlog_pal_unlockTransmission();
	
	avail -= 3; // reserve space for the overflow indicator
	
	if (len > avail)
	{
		if (mOverflow)
			return;
		mOverflow = 1;
		msg = "*\r\n";  // overflow indicator
		len = 3;
	}
	else
	{
		mOverflow = 0;
	}

	if (len > 0)
	{
		if ((BTLOG_BUFFER_SIZE - mLogTail) >= len)
		{
			// Whole message can be written at once.
			memcpy(mLogBuffer + mLogTail, msg, len);
		}
		else
		{
			// Part of the message is written to the end of the buffer
			// and next part to the beginning.
			
			// Free bytes available from the tail pointer to the end of the buffer
			int avail2 = BTLOG_BUFFER_SIZE - mLogTail;
			memcpy(mLogBuffer + mLogTail, msg, avail2);
			memcpy(mLogBuffer, msg + avail2, len - avail2);
		}
		
		btlog_pal_lockTransmission();
		{
			mLogTail = (mLogTail + len) & BTLOG_BUFFER_SIZE_MASK;
			if (!mTransmitting)
			{
				mTransmitting = 1;
				btlog_pal_transmit();
			}
		}
		btlog_pal_unlockTransmission();
	}
}


char* btlog_getTransmitData(int* transmitLen)
{
	int len;
	
	// Calculate how many bytes can be transmitted
	len = mLogTail - mLogHead;
	if (len < 0)
		len = BTLOG_BUFFER_SIZE - mLogHead;
	
	*transmitLen = len;
	
	if (len == 0)
		return NULL;
	else
		return &mLogBuffer[mLogHead];
}


void btlog_onDataTransmitted(int transmitLen)
{
	if (transmitLen == 0)
	{
		mTransmitting = 0;
	}
	else
	{
		mLogHead = (mLogHead + transmitLen) & BTLOG_BUFFER_SIZE_MASK;
	}
}

#else // ENABLE_LOG

void bt_oem_log_write(const char* msg)
{
}

#endif
