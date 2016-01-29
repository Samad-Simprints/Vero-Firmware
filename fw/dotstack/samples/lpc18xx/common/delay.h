/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __DELAY_H_INCLUDED__
#define __DELAY_H_INCLUDED__

// The multiplier is calculated with the following formula:
// m = 0.000001 / (1 / SystemCoreClock * 3)
//
// m = 0.000001 / (1 / 100000000 * 3) = 56
#define DELAY_US(us)	delayMicroseconds(us * 33)

#if defined(__GNUC__)
__attribute__((naked)) void delayMicroseconds(unsigned n);
#elif defined( __ICCARM__)
/*__task*/ void delayMicroseconds(unsigned n);
#elif defined(__CC_ARM)
__asm void delayMicroseconds(unsigned n);
#endif

#endif // __DELAY_H_INCLUDED__
