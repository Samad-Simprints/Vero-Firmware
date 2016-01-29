/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#include "cdbt/bt/bt_std.h"
#include "cdbt/bt/bt_system.h"

#include <stdarg.h>

static char mNumBuffer[37];

void btsprintf(char* buffer, int bufferSize, const char* format, ...)
{
	int index = 0;
	int size = bufferSize;
	char c;
	va_list ap;
	
	va_start(ap, format);
	
	size -= 1;; // reserve space for end of line
	while (*format && index < size)
	{
		c = *format++;
		if (c == '\\')
		{
			c = *format;
			if (c != 0)
			{
				format++;
				buffer[index++] = c;
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
						buffer[index++] = *s++;
					}
				}
				
				while (index < size && i > 0)
				{
					buffer[index++] = mNumBuffer[--i];
				}
			}
		}
		else
		{
			buffer[index++] = c;
		}
	}
	
	buffer[index++] = 0;

	va_end(ap);
}
