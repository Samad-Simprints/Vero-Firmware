#include "FreeRTOS.h"
#include "task.h"
#include "bttimer.h"
#include "error.h"
#include "button.h"

//
// FreeRTOS tick hook.
// It is called by FreeRTOS from its system tick interrupt handler.
// We use it to implement software timers and poll keyboard.
// -------------------------------------------------------------------
//
void vApplicationTickHook(void)
{
	bttimer_onSystemTick();
       
    button_checkButtonState();
}

#if 0
void vApplicationMallocFailedHook( void )
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues or
	semaphores. */
	taskDISABLE_INTERRUPTS();
	error_onFatalError();
}


void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
{
	( void ) pxTask;
	( void ) pcTaskName;
	
	/* Run time stack overflow checking is performed if
	configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	error_onFatalError();
}

#endif

void vConfigureTimerForRunTimeStats(void)
{
}
