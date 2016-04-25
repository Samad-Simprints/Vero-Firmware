/*
    FreeRTOS V7.1.1 - Copyright (C) 2012 Real Time Engineers Ltd.


    ***************************************************************************
     *                                                                       *
     *    FreeRTOS tutorial books are available in pdf and paperback.        *
     *    Complete, revised, and edited pdf reference manuals are also       *
     *    available.                                                         *
     *                                                                       *
     *    Purchasing FreeRTOS documentation will not only help you, by       *
     *    ensuring you get running as quickly as possible and with an        *
     *    in-depth knowledge of how to use FreeRTOS, it will also help       *
     *    the FreeRTOS project to continue with its mission of providing     *
     *    professional grade, cross platform, de facto standard solutions    *
     *    for microcontrollers - completely free of charge!                  *
     *                                                                       *
     *    >>> See http://www.FreeRTOS.org/Documentation for details. <<<     *
     *                                                                       *
     *    Thank you for using FreeRTOS, and thank you for your support!      *
     *                                                                       *
    ***************************************************************************


    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    >>>NOTE<<< The modification to the GPL is included to allow you to
    distribute a combined work that includes FreeRTOS without being obliged to
    provide the source code for proprietary components outside of the FreeRTOS
    kernel.  FreeRTOS is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    ***************************************************************************
     *                                                                       *
     *    Having a problem?  Start by reading the FAQ "My application does   *
     *    not run, what could be wrong?                                      *
     *                                                                       *
     *    http://www.FreeRTOS.org/FAQHelp.html                               *
     *                                                                       *
    ***************************************************************************


    http://www.FreeRTOS.org - Documentation, training, latest information,
    license and contact details.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool.

    Real Time Engineers ltd license FreeRTOS to High Integrity Systems, who sell
    the code with commercial support, indemnification, and middleware, under
    the OpenRTOS brand: http://www.OpenRTOS.com.  High Integrity Systems also
    provide a safety engineered and independently SIL3 certified version under
    the SafeRTOS brand: http://www.SafeRTOS.com.
*/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "btapp_config.h"

#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)    // protects from including by assembler
#include "LPC18xx.h"
#endif

extern uint32_t SystemCoreClock;

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *----------------------------------------------------------*/

#define RTOS_FOREVER                              ( 0xfffffffful )

#define configUSE_PREEMPTION                        1
#define configUSE_IDLE_HOOK                         1
#define configMAX_PRIORITIES                        ( ( unsigned portBASE_TYPE ) 5 )
#define configUSE_TICK_HOOK                         1
#define configCPU_CLOCK_HZ                          ( SystemCoreClock )
#define configTICK_RATE_HZ                          ( ( portTickType ) 1000 )
#define configMINIMAL_STACK_SIZE                    ( ( unsigned short ) 80 )
#define configTOTAL_HEAP_SIZE_MAIN                  ( ( size_t ) ( 15 * 1024 ) )
#define configMAX_TASK_NAME_LEN                     ( 12 )
#define configUSE_TRACE_FACILITY                    1
#define configUSE_16_BIT_TICKS                      0
#define configIDLE_SHOULD_YIELD                     0
#define configUSE_CO_ROUTINES                       0
#define configUSE_MUTEXES                           1

#define configMAX_CO_ROUTINE_PRIORITIES             ( 2 )

#define configUSE_COUNTING_SEMAPHORES               1
#define configUSE_ALTERNATIVE_API                   0
#define configCHECK_FOR_STACK_OVERFLOW              2
#define configUSE_RECURSIVE_MUTEXES                 1
#define configQUEUE_REGISTRY_SIZE                   10
#define configGENERATE_RUN_TIME_STATS               1

#define configUSE_TIMERS                            1
#define configTIMER_TASK_PRIORITY                 ( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH                  20
#define configTIMER_TASK_STACK_DEPTH              800 //configMINIMAL_STACK_SIZE

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet                    1
#define INCLUDE_uxTaskPriorityGet                   1
#define INCLUDE_vTaskDelete                         1
#define INCLUDE_vTaskCleanUpResources               0
#define INCLUDE_vTaskSuspend                        1
#define INCLUDE_vTaskDelayUntil                     1
#define INCLUDE_vTaskDelay                          1
#define INCLUDE_uxTaskGetStackHighWaterMark         1

/* Use the system definition, if there is one */
#ifdef __NVIC_PRIO_BITS
  #define configPRIO_BITS                           __NVIC_PRIO_BITS
#else
  #define configPRIO_BITS                           5        /* 32 priority levels */
#endif

/* The lowest priority. */
#define configKERNEL_INTERRUPT_PRIORITY             ( 31 << (8 - configPRIO_BITS) )
/* Priority 5, or 160 as only the top three bits are implemented. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY        ( 5 << (8 - configPRIO_BITS) )

/* Priorities passed to NVIC_SetPriority() do not require shifting as the
function does the shifting itself.  Note these priorities need to be equal to
or lower than configMAX_SYSCALL_INTERRUPT_PRIORITY - therefore the numeric
value needs to be equal to or greater than 5 (on the Cortex-M3 the lower the
numeric value the higher the interrupt priority). */
#define configEMAC_INTERRUPT_PRIORITY               5
#define configUSB_INTERRUPT_PRIORITY                6
#define configSER_INTERRUPT_PRIORITY                6
#define configGPIO_INTERRUPT_PRIORITY               6

/*-----------------------------------------------------------
 * Macros required to setup the timer for the run time stats.
 *-----------------------------------------------------------*/
#if ( configGENERATE_RUN_TIME_STATS == 1 )
extern unsigned long ulRunTimeStatsClock;
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() ulRunTimeStatsClock = 0
#define portGET_RUN_TIME_COUNTER_VALUE() ulRunTimeStatsClock++;
#endif

/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); vLogRTOSAssert( __FILE__, __LINE__, #x); }	

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

// Depending on the compiler (GCC or IAR) memory requirements are slightly different.
// These differences are handled in compiler specific configuration files.
#if defined(__GNUC__)
	#include "FreeRTOSConfig_GCC.h"
#elif defined(__ICCARM__)
	#include "FreeRTOSConfig_IAR.h"
#elif defined(__CC_ARM)
	#include "FreeRTOSConfig_ARM.h"
#endif

// Macro for converting milliseconds to ticks
#define MILLIS_IN_ONE_TICK()  (1000 / configTICK_RATE_HZ)
#define MILLIS_TO_TICKS(ms)   ((configTICK_RATE_HZ * (ms + MILLIS_IN_ONE_TICK() - 1)) / 1000)

#endif /* FREERTOS_CONFIG_H */
