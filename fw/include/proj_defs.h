//******************************************************************************
//
// Project INDEX source file
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
// MODULE:       proj_defs.h
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:  Project-wide definitions
//
//******************************************************************************

#if !defined(_PROJ_DEFS_H_)
#define _PROJ_DEFS_H_

//******************************************************************************
// Includes
//******************************************************************************

//******************************************************************************
// Constants
//******************************************************************************

// set to 1 to enable the watchdog feature. This will have a bad effect on the
// debugger when the processor is reset 5 seconds after the code comes to a stop.
#ifdef DEBUG
#define WATCHDOG_HARDWARE_ENABLED                ( 0 )
#else
#define WATCHDOG_HARDWARE_ENABLED                ( 1 )
#endif

// firmware version number - used by a post-compile hook to auto create a vx.x.x tag if changed
#define INDEX_REVISION_NUMBER         5

//
// Processor clock frequency
//
#define CCLK_KHZ                      ( 12000UL )
#define PCLK_HZ                       ( SystemCoreClock )

#define TIMER_TICK_INTERVAL_MS        ( 1000 / configTICK_RATE_HZ )    // milliseconds
#define TIMER_TICK_RATE               ( 1000 / TIMER_TICK_INTERVAL_MS )

#define MS_TO_TICKS( ms )             ( ( ms ) / TIMER_TICK_INTERVAL_MS )
#define SECS_TO_TICKS( s )            MS_TO_TICKS( ( s ) * 1000 )
#define TICKS_TO_MS( ticks )          ( ( ticks ) * TIMER_TICK_RATE )
#define TICKS_TO_S( ticks )           ( TICKS_TO_MS( ticks ) / 1000 )

#define WATCHDOG_TIMEOUT_SECS         ( 5 )

//******************************************************************************
// Definitions
//******************************************************************************

// Scanner connections
#define CONSOLE_UART                  USART0_IRQn
#define BT_UART                       UART1_IRQn
#define UN20_UART                     USART2_IRQn

#ifdef DEBUG_OUTPUT
#define TASK_DEBUG_OVERHEAD           ( 160 )
#else
#define TASK_DEBUG_OVERHEAD           ( 20 )
#endif

#define CLI_TASK_STACK_SIZE           ( 512 + TASK_DEBUG_OVERHEAD )
#define CLI_TASK_PRIORITY             ( tskIDLE_PRIORITY + 1 )

#define LED_TASK_STACK_SIZE           ( 512 + TASK_DEBUG_OVERHEAD )
#define LED_TASK_PRIORITY             ( tskIDLE_PRIORITY + 1 )

#define USB_TASK_STACK_SIZE           ( 512 + TASK_DEBUG_OVERHEAD )
#define USB_TASK_PRIORITY             ( tskIDLE_PRIORITY + 1 )

// this definition not used. Here for documentation
#define BT_TASK_PRIORITY              ( tskIDLE_PRIORITY + 2 )

// watchdog task runs at high priority with little stack
#define WDOG_TASK_STACK_SIZE          ( configMINIMAL_STACK_SIZE + TASK_DEBUG_OVERHEAD )
#define WDOG_TASK_PRIORITY            ( tskIDLE_PRIORITY + 4 )
#define WDOG_ACTIVITY_CHECK_MS        30000

// - interrupt priorities - max 7 on LPC18xx
#if 0
#define USB_INTERRUPT_PRIORITY        ( configMAX_LIBRARY_INTERRUPT_PRIORITY + 1 )
#define GPIO_INTERRUPT_PRIORITY       ( configMAX_LIBRARY_INTERRUPT_PRIORITY + 1 )
#define GPDMA_INTERRUPT_PRIORITY      ( configMAX_LIBRARY_INTERRUPT_PRIORITY + 1 )
#else
//#define GPIO_INTERRUPT_PRIORITY       ( configMAX_SYSCALL_INTERRUPT_PRIORITY + 1 )
#endif

// GPIO_DD configuration
static const dword                    GPIO_DD_INTERRUPT_PRIORITY     = configGPIO_INTERRUPT_PRIORITY;

// SER_DD configuration
static const dword                    SER_DD_INTERRUPT_PRIORITY     = configSER_INTERRUPT_PRIORITY;
static const dword                    DEFAULT_BAUD_RATE             = 38400;

//******************************************************************************
// Function Prototypes
//******************************************************************************

#endif  // <HEADER_DEFINE>
