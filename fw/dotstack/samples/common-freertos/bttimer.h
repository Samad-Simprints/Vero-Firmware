/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __BTTIMER_H_INCLUDED__
#define __BTTIMER_H_INCLUDED__

#include "cdbt/bt/bt_std.h"

// Configuration file that defines timer IDs
#include "bttimer_config.h"
#if !defined(BTTIMER_MAX_TIMERS)
#error bttimer_config.h must define BTTIMER_MAX_TIMERS
#endif

typedef void (*bttimer_TimerCallback)(void);

void bttimer_init(void);
void bttimer_onSystemTick(void);

void bttimer_setTimer(bt_uint timerId, bt_ulong milliseconds, bttimer_TimerCallback callback);
void bttimer_clearTimer(bt_uint timerId);


#endif // __BTTIMER_H_INCLUDED__

