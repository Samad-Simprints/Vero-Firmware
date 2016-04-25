//******************************************************************************
//
// Project INDEX Application source file
//
// (c) Fen Technology Ltd. 2016. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// COMPONENT:    INDEX
// MODULE:       watchdog_fd
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:  see Header file for details
//
//******************************************************************************

//******************************************************************************
// Compilation switches
//******************************************************************************

//******************************************************************************
// Includes
//******************************************************************************
#include <string.h>
#include "global.h"
#include "exception.h"
#include "watchdog_dd.h"
#include "FreeRTOS.h"

//******************************************************************************
// Constants
//******************************************************************************
#undef WATCHDOG_HARDWARE_ENABLED
#define WATCHDOG_HARDWARE_ENABLED (1)

#define WDOG_MAX_TASKS            20
#define WDOG_SLEEP_PERIOD_MS    1000

//******************************************************************************
// Definitions
//******************************************************************************

//******************************************************************************
// Private Function Prototypes
//******************************************************************************
static void vWatchdogTask( void *pvParams );

//******************************************************************************
// Local Storage
//******************************************************************************
static xTaskStatusType pxTaskStatusArray[ WDOG_MAX_TASKS ];
static dword dwLastRuntime[ WDOG_MAX_TASKS ];

//******************************************************************************
//******************************************************************************
// Private Functions
//******************************************************************************
//******************************************************************************

/* -----------------------------------------------------------------------
   vWatchdogTask
   .......................................................................
   Description  : Manages the watchdog operation.
   Observes other threads and declares a thread hung if it
   is not consuming measurable CPU time over a period of time.

   Params : pvParams - Task parameters (not used)

   Returns : Nothing
   ----------------------------------------------------------------------- */

static void vWatchdogTask( void *pvParams )
{
  portBASE_TYPE uxArraySize;
  int iTaskCheckDelay = WDOG_ACTIVITY_CHECK_MS;

  memset( dwLastRuntime, 0, sizeof(dwLastRuntime) );

  while ( 1 )
  {
    vTaskDelay( MS_TO_TICKS( WDOG_SLEEP_PERIOD_MS ) );

    // increment the tick timer and kick the watchdog
    dwLogTimerTick++;
    vWDOGDDkick();

#if WATCHDOG_HARDWARE_ENABLED
    // Update the time stamp on the default watchdog exception
    vLogWatchdog();
#endif

    // periodically check that tasks are consuming cpu time
    iTaskCheckDelay -= WDOG_SLEEP_PERIOD_MS;
    if ( iTaskCheckDelay <= 0 )
    {
      word wTask;
      PBC_ASSERT( WDOG_MAX_TASKS >= uxTaskGetNumberOfTasks() );

      // get current state of tasks
      uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, WDOG_MAX_TASKS, NULL );
      for ( wTask = 0; wTask < uxArraySize; wTask++ )
      {
        bool boRunning = (pxTaskStatusArray[ wTask ].ulRunTimeCounter != dwLastRuntime[ wTask ]);
        dwLastRuntime[ wTask ] = pxTaskStatusArray[ wTask ].ulRunTimeCounter;

        // if we have found a task that is not running log and restart
        if ( !boRunning )
        {
          const char *pcTaskName = pxTaskStatusArray[ wTask ].pcTaskName;
          vLogGeneral( ERROR_SOFTWARE_THREAD_STUCK, ( void* )pcTaskName, strlen( pcTaskName ) + 1 );
        }
      }
      iTaskCheckDelay = WDOG_ACTIVITY_CHECK_MS;
    }
  }

  // does not return
}

//******************************************************************************
//******************************************************************************
// Public Functions
//******************************************************************************
//******************************************************************************

/* -----------------------------------------------------------------------
   vWATCHDOGstart
   .......................................................................
   Description  : Initialises the watchdog timer and creates the thread
                  monitor thread.

   Params : None

   Returns : Nothing
   ----------------------------------------------------------------------- */

void vWATCHDOGstart( void )
{
#if WATCHDOG_HARDWARE_ENABLED
  // log a default watchdog event which will be inplace should the watchdog trigger
  vLogWatchdog();
  // enable the watchdog hardware
  vWDOGDDinit();

  xTaskCreate( vWatchdogTask,
              (const char *)"wdog",
              WDOG_TASK_STACK_SIZE,
              NULL,
              WDOG_TASK_PRIORITY,
              NULL );
#endif

}
