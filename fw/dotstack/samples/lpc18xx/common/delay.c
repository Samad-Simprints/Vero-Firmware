/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#include "config.h"
#include "delay.h"

#if defined(__GNUC__)
__attribute__((naked)) void delayMicroseconds(unsigned n)
{
	__asm volatile ("1: subs r0, r0, #1");
	__asm volatile (" bne 1b");
	__asm volatile (" bx lr");
}
#elif defined( __ICCARM__)

#pragma optimize=no_inline

/*__task*/ void delayMicroseconds(unsigned n)
{
	asm volatile (
		"delayloop: \n"
			"subs r0, r0, #1\n"
			"bne.w delayloop\n");
//			"bx lr");
}

#elif defined(__CC_ARM)

__asm void delayMicroseconds(unsigned n)
{
delayloop	subs r0, r0, #1
			bne delayloop
			bx lr
}

#endif
