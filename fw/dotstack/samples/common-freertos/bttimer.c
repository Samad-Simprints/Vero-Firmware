/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#include "FreeRTOS.h"
#include "task.h"
#include <assert.h>
#include <string.h>

#include "bttimer.h"
#include "bttask.h"
#include "btapp.h"

#include "cdbt/bt/bt_std.h"
#include "cdbt/bt/bt_timer.h"


#define TICK_PERIOD_MS   (1000 / configTICK_RATE_HZ)    // timer tick period in ms

#define TOTAL_TIMERS  (BT_TIMER_MAX + BTTIMER_MAX_TIMERS + BTAPP_MAX_TIMERS)


typedef struct _TimerData
{
	bt_ulong              duration;
	bt_ulong              startTime;
	bt_timer_callback_fp  callback;
} TimerData;


static volatile bt_ulong  mTicks;
static volatile bt_ulong  mTimerCounter;
static TimerData          mTimers[TOTAL_TIMERS];
static bt_bool            mProcessingTimers;


static void setTimer(bt_uint timerId, bt_ulong milliseconds, bttimer_TimerCallback callback);
static void clearTimer(bt_uint timerId);
static void updateTimerCounter(bt_ulong currentTicks);


void bttask_pal_initTimer(void)
{
	memset(mTimers, 0, sizeof(mTimers));
	mTimerCounter = 0;
	mProcessingTimers = BT_FALSE;
}


void bttask_pal_handleTimerSignal(void)
{
	int i;
	bt_ulong ticks;
	
	mProcessingTimers = BT_TRUE;

	taskENTER_CRITICAL();
	ticks = mTicks;
	taskEXIT_CRITICAL();
	
	for (i = 0; i < TOTAL_TIMERS; i++)
	{
		if (mTimers[i].duration != 0 &&
			(ticks - mTimers[i].startTime) >= mTimers[i].duration)
		{
			bt_timer_callback_fp callback = mTimers[i].callback;
			mTimers[i].duration = 0;
			mTimers[i].callback = NULL;
			(*callback)();
		}
	}
	
	updateTimerCounter(ticks);

	mProcessingTimers = BT_FALSE;
}

void bttimer_setTimer(bt_uint timerId, bt_ulong milliseconds, bttimer_TimerCallback callback)
{
	setTimer(timerId + BT_TIMER_MAX, milliseconds, callback);
}


void bttimer_clearTimer(bt_uint timerId)
{
	clearTimer(timerId + BT_TIMER_MAX);
}

bt_ulong btapp_getCurrentTime(void)
{
	return mTicks * TICK_PERIOD_MS;
}

void btapp_setTimer(btapp_TimerId timerId, bt_ulong milliseconds, btapp_TimerCallback callback)
{
	setTimer((int)timerId + (int)BT_TIMER_MAX + BTTIMER_MAX_TIMERS, milliseconds, (bttimer_TimerCallback)callback);
}


void btapp_clearTimer(btapp_TimerId timerId)
{
	clearTimer((int)timerId + (int)BT_TIMER_MAX + BTTIMER_MAX_TIMERS);
}


//
// This function is called by dotstack to set a timer
//
void bt_oem_timer_set(bt_uint timerId, bt_ulong milliseconds, bt_timer_callback_fp callback)
{
	setTimer(timerId, milliseconds, (bttimer_TimerCallback)callback);
}


//
// This function is called by dotstack to clear a timer
//
void bt_oem_timer_clear(bt_uint timerId)
{
	clearTimer(timerId);
}


void bttimer_onSystemTick(void)
{
	mTicks++;
	
	if (mTimerCounter)
	{
		if (--mTimerCounter == 0)
		{
			bttask_setSignalFromTickHook(BTTASK_SIG_TIMER);
		}
	}
}


static void setTimer(bt_uint timerId, bt_ulong milliseconds, bttimer_TimerCallback callback)
{
	bt_ulong duration;
	bt_ulong tmpTimerCounter;
	bt_ulong tmpTicks;
	
	assert(milliseconds != 0);
	assert(callback != NULL);
	
	duration = (milliseconds + TICK_PERIOD_MS - 1) / TICK_PERIOD_MS; // number of ticks
	if (duration == 0) duration = 1; // set to at least 1 tick
	
	taskENTER_CRITICAL();
	tmpTicks = mTicks;
	tmpTimerCounter = mTimerCounter;
	if (tmpTimerCounter == 0 || tmpTimerCounter > duration)
	{
		mTimerCounter = duration;
	}
	taskEXIT_CRITICAL();
	
	mTimers[timerId].startTime = tmpTicks;
	mTimers[timerId].duration = duration;
	mTimers[timerId].callback = callback;
	
	if (!mProcessingTimers)
	{
		updateTimerCounter(tmpTicks);
	}
}


static void clearTimer(bt_uint timerId)
{
	mTimers[timerId].duration = 0;
	mTimers[timerId].callback = NULL;
	
	if (!mProcessingTimers)
	{
		bt_ulong ticks;
		
		taskENTER_CRITICAL();
		ticks = mTicks;
		taskEXIT_CRITICAL();
		
		updateTimerCounter(ticks);
	}
}


static void updateTimerCounter(bt_ulong currentTicks)
{
	int i;
	bt_ulong tmp;
	bt_ulong timerCounter = 0xFFFFFFFF;

	for (i = 0; i < TOTAL_TIMERS; i++)
	{
		if (mTimers[i].duration != 0)
		{
			tmp = (mTimers[i].startTime + mTimers[i].duration) - currentTicks;
			if (tmp != 0 && tmp < timerCounter)
			{
				timerCounter = tmp;
			}
		}
	}
	
	if (timerCounter != 0)
	{
		taskENTER_CRITICAL();
		mTimerCounter = timerCounter;
		taskEXIT_CRITICAL();
	}
}
