/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/
#include "FreeRTOS.h"
#include "task.h"

#include <stdio.h>

#include "btapp.h"

#ifdef BTAPP_ENABLE_STATS

#include "stats.h"

#include "btlog_define.h"

#define STATS_UPDATE_FREQUENCY    2000 // ms
#define STATS_BUFFER_SIZE         100

typedef struct _stats_TaskStart
{
	xTaskHandle taskHandle;
	unsigned long runTimeCounter;
} stats_TaskStart;

unsigned long ulRunTimeStatsClock = 0;
unsigned long mMeasurementPeriodTotalStart = 0;
static stats_TaskStart* tasksMeasurementPeriodStart;
static unsigned portBASE_TYPE tasksMeasurementPeriodStartSize;

static void timerCallback(void);

void btstats_pal_init(void)
{
	tasksMeasurementPeriodStart = NULL;
	tasksMeasurementPeriodStartSize = 0;
	stats_pal_init();
}

void btstats_pal_start(void)
{
	stats_pal_start();

	btapp_setTimer(BTAPP_TIMER_STATS, STATS_UPDATE_FREQUENCY, &timerCallback);
}

void stats_updateClockFromISR(portBASE_TYPE *higherPriorityTaskWoken)
{
	(void)higherPriorityTaskWoken;

	ulRunTimeStatsClock++;
}

static void timerCallback(void)
{
	static signed char stats[STATS_BUFFER_SIZE];
	unsigned portBASE_TYPE arraySize = uxTaskGetNumberOfTasks();
	unsigned portBASE_TYPE x, y;
	unsigned long ulTotalTime;
	unsigned long taskStats;
	unsigned long taskStatsAsPercentage;

	/* Allocate an array index for each task. */
	xTaskStatusType* taskStatusArray = pvPortMalloc(arraySize * sizeof(xTaskStatusType));

	if( taskStatusArray != NULL )
	{
		/* Generate the (binary) data. */
		arraySize = uxTaskGetSystemState(taskStatusArray, arraySize, &ulTotalTime);

		/* Avoid divide by zero errors. */
		if( ulTotalTime > 0 )
		{
			ulTotalTime = ulRunTimeStatsClock - mMeasurementPeriodTotalStart;

			BTLOG("STATS:");
			sprintf((char *) stats, (char *) "Period\t\t%lu", ulTotalTime);
			BTLOG((const char*)stats);

			if (tasksMeasurementPeriodStart)
			{
				/* Create a human readable table from the binary data. */
				for( x = 0; x < arraySize; x++ )
				{
					for (y = 0; y < tasksMeasurementPeriodStartSize; y++)
					{
						if (tasksMeasurementPeriodStart[y].taskHandle == taskStatusArray[x].xHandle)
						{
							if (ulRunTimeStatsClock > mMeasurementPeriodTotalStart)
							{
								taskStats = taskStatusArray[x].ulRunTimeCounter - tasksMeasurementPeriodStart[y].runTimeCounter;
								taskStatsAsPercentage = taskStats / (ulTotalTime / 100);
								if (taskStatsAsPercentage)
									sprintf((char *)stats, (char *) "%s\t\t%lu\t\t%lu%%", taskStatusArray[x].pcTaskName, taskStats, taskStatsAsPercentage);
								else
									sprintf((char *)stats, (char *) "%s\t\t%lu\t\t<1%%", taskStatusArray[x].pcTaskName, taskStats);

								BTLOG((const char*)stats);
							}

							break;
						}
					}
				}

				if (tasksMeasurementPeriodStartSize != arraySize)
				{
					vPortFree(tasksMeasurementPeriodStart);
					tasksMeasurementPeriodStart = NULL;
				}
			}
			BTLOG("\r\n");

			mMeasurementPeriodTotalStart = ulRunTimeStatsClock;

			if (!tasksMeasurementPeriodStart)
				tasksMeasurementPeriodStart = pvPortMalloc(arraySize * sizeof(stats_TaskStart));
			if (tasksMeasurementPeriodStart)
			{
				tasksMeasurementPeriodStartSize = arraySize;
				for( x = 0; x < arraySize; x++ )
				{
					tasksMeasurementPeriodStart[x].taskHandle = taskStatusArray[x].xHandle;
					tasksMeasurementPeriodStart[x].runTimeCounter = taskStatusArray[x].ulRunTimeCounter;
				}
			}
		}

		/* Free the array again. */
		vPortFree(taskStatusArray);
	}

//	vTaskGetRunTimeStats(stats);
//
//	BTLOG("STATS:");
//	BTLOG((const char*)stats);

	btapp_setTimer(BTAPP_TIMER_STATS, STATS_UPDATE_FREQUENCY, &timerCallback);
}

#endif // BTAPP_ENABLE_STATS
