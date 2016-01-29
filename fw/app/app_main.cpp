//******************************************************************************
//
// Project INDEX Application header file
//
// (c) Fen Technology Ltd. 2016. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// COMPONENT:    INDEX main loop
// MODULE:       main.c
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

#include <ctl_api.h>
#include <lpc18xx.h>
#include <cross_studio_io.h>

//#include "global.h"

#include <stdio.h>

#include "cli.h"
#include "gpio_dd.hpp"
#include "ser_dd.hpp"

#include "freertos.h"
#include "task.h"
#undef TRUE
#undef FALSE
#include "lpc18xx_cgu.h"
#include "hal.h"
#include "lpcapp.h"
#include "helpers.hpp"

//******************************************************************************
// Private Function Prototypes
//******************************************************************************

//******************************************************************************
// Constants
//******************************************************************************
#define _QUOTEME(x)                              #x
#define QUOTEME(x)                               _QUOTEME(x)

#define INDEX_VERSION  QUOTEME(INDEX_REVISION_NUMBER) " : " \
                        "Built: " __DATE__ " " __TIME__


DEBUG_MODULE_DEFINE( INDEX_DEFAULTS ) = {
    "INDEX_DEFAULT", {
    "Error","Hi","Medium","Low" },
    DEBUGZONE(ZONE_DBG_ERROR) | DEBUGZONE(ZONE_DBG_HI) | DEBUGZONE(ZONE_DBG_MED) | DEBUGZONE(ZONE_DBG_LO)
};

//******************************************************************************
// Definitions
//******************************************************************************

class tMain
{
public:
                            tMain();
  virtual                  ~tMain();

  void                      vInit();

  // Accessors
  tLock                    *poGetPrintfLock()                       { return &oPrintfLock; }

  inline ISerialPort       *poGetDebugPort()                        { return poDebugPort; }

private:
  tLock                     oPrintfLock;

  ISerialPort              *poDebugPort;

};

//******************************************************************************
// Local Storage
//******************************************************************************

static tMain oMain;

//******************************************************************************
//******************************************************************************
// Private Functions
//******************************************************************************
//******************************************************************************

//******************************************************************************
// Terminal IO
//******************************************************************************

// Serial port config
static const int            MAX_DEBUG_RX_QUEUE_SIZE         = 300;
static const int            MAX_DEBUG_TX_QUEUE_SIZE         = 10;

static const tLineCoding sALMportConfig = {
  /*.dwDTERate =*/ 115200,                          // Data terminal rate in bits per second
  /*.bCharFormat =*/ 0,                             // Number of stop bits
  /*.bParityType =*/ 0,                             // Parity bit type
  /*.bDataBits =*/ 8                                // Number of data bits
};


//******************************************************************************
// main
//
// Description : Entry point
//
// Params : None
//
// Returns : None
//
//******************************************************************************

extern "C" void bt_main(void);
extern "C" void check_failed(uint8_t *file, uint32_t line);

extern "C" void vHalTest(void);

//******************************************************************************
// Terminal IO
//******************************************************************************

/* -----------------------------------------------------------------------
   __putchar
   .......................................................................
   Description  : Writes character to debug console

   Params : iChar - character to write

   Returns : int - EOF if failed to write, otherwise iChar
   ----------------------------------------------------------------------- */

static bool boOutputEnable = false;

extern "C" int __putchar( int iChar )
{
  int iRes = EOF;

  if ( ( oMain.poGetDebugPort() != 0 ) && boOutputEnable )
  {
    while ( ( iRes = oMain.poGetDebugPort()->iPutchar( iChar ) ) == EOF )
    {
      vTaskDelay( 1 );
    }
  }

  return iRes;
}

/* -----------------------------------------------------------------------
   __getchar
   .......................................................................
   Description  : Reads character from debug console

   Params : None

   Returns : int - EOF if nothing to read, otherwise character read
   ----------------------------------------------------------------------- */

extern "C" int __getchar(void)
{
  int iRes = EOF;

  if ( oMain.poGetDebugPort() != 0 )
  {
    iRes = oMain.poGetDebugPort()->iGetchar();
  }

  return iRes;
}

/* -----------------------------------------------------------------------
   kbhit
   .......................................................................
   Description  : Checks if there are characters to be read from console

   Params : None

   Returns : int - 0 if no characters to read, otherwise 1
   ----------------------------------------------------------------------- */

extern "C" int kbhit( void )
{
  int iRes = 0;

  if ( oMain.poGetDebugPort() != 0 )
  {
    iRes = oMain.poGetDebugPort()->iHasChars();
  }

  return iRes;
}

void vLogAssert( const byte bReason, const char *pzFile, dword dwLine, const char *pzCondition )
{
  debug_break();
}

#if 0
int _printf(const char *format, ...)
{
  debug_break();
}
#endif

void check_failed(uint8_t *file, uint32_t line)
{
#ifdef DEBUG
  for (;;);
#endif
}

uint32_t	xCGU_Init(void)
{
  CGU_SetXTALOSC(12000000);
  CGU_EnableEntity(CGU_CLKSRC_XTAL_OSC, ENABLE);
  CGU_EntityConnect(CGU_CLKSRC_XTAL_OSC, CGU_CLKSRC_PLL1);
  /* Disable PLL1 - Flash mode hang */
  //CGU_EnableEntity(CGU_CLKSRC_PLL1, DISABLE);
  CGU_SetPLL1(15);
  /* Enable PLL1 after setting is done */
  CGU_EnableEntity(CGU_CLKSRC_PLL1, ENABLE);
  /* Distribute it to Main Base Clock */
  CGU_EntityConnect(CGU_CLKSRC_PLL1, CGU_BASE_M3);
  /* Update Clock Frequency */
  CGU_UpdateClock();
  return 0;
}

tMain::tMain()
  : oPrintfLock(),
    poDebugPort( 0 )
{
}

tMain::~tMain()
{
}

void tMain::vInit()
{
  oPrintfLock.vInit();

  vHalInit();
  vPowerInit();
  vUiInit();

  poDebugPort = poSERDDgetPort( DEBUG_UART );
  poDebugPort->vConfigurePort( &sALMportConfig, 64, 300 );

  // set to blocking mode for transmit but not receive
  poDebugPort->vSetBlockingMode( ISerialPort::bmTransmitOnly );

  // flush buffers before we start
  poDebugPort->vFlush();
}

int main( void )
{
  DEBUG_MODULE_INIT( INDEX_DEFAULTS );

#ifdef DEBUG
  // debug builds always have output unlocked
  boOutputEnable = true;
#endif

  NVIC_SetPriorityGrouping(0UL);

  xCGU_Init();
  SystemCoreClock = CGU_GetPCLKFrequency(CGU_PERIPHERAL_M3CORE);

  vPowerInit();
  vPowerSelfOn();	// latch the power on

 // vHalTest();

  oMain.vInit();

  // initialise CLI
  vCLIinit();

  // report build information on console
  CLI_PRINT(("\nINDEX - SW: " INDEX_VERSION "\n"));
#if 0
  // register CLI commands
  for ( int i = 0; i < ( sizeof( asMainCLI ) / sizeof( asMainCLI[ 0 ] ) ); ++i )
  {
    boCLIregisterEntry( &asMainCLI[i] );
  }
#endif

  //bt_main();

  //SystemCoreClockUpdate();

  // Initialise device drivers
  //vGPIODDinit();

  /* Create the Bluetooth App task. */
  //xTaskCreate( vBluetoothTask, ( signed char * ) "BT", BTAPP_TASK_STACK_SIZE, ( void * ) NULL, BTAPP_TASK_PRIORITY, NULL );

  /* Create the LPC App task. */
  xTaskCreate( vLpcAppTask, ( signed char * ) "LPC", LPCAPP_TASK_STACK_SIZE, ( void * ) NULL, LPCAPP_TASK_PRIORITY, NULL );

  /* Start the scheduler. */
  vTaskStartScheduler();

  /* Will only get here if there was insufficient memory to create the idle
  task.  The idle task is created within vTaskStartScheduler(). */
  for( ;; );

  // should never get here
  return 0;
}

void vGetRegistersFromStack( dword *pdwFaultStackAddress )
{
  /* These are volatile to try and prevent the compiler/linker optimising them
  away as the variables never actually get used.  If the debugger won't show the
  values of the variables, make them global my moving their declaration outside
  of this function. */
  volatile dword r0;
  volatile dword r1;
  volatile dword r2;
  volatile dword r3;
  volatile dword r12;
  volatile dword lr;    // Link register.
  volatile dword pc;    // Program counter.
  volatile dword psr;   // Program status register.

  r0 = pdwFaultStackAddress[ 0 ];
  r1 = pdwFaultStackAddress[ 1 ];
  r2 = pdwFaultStackAddress[ 2 ];
  r3 = pdwFaultStackAddress[ 3 ];

  r12 = pdwFaultStackAddress[ 4 ];
  lr = pdwFaultStackAddress[ 5 ];
  pc = pdwFaultStackAddress[ 6 ];
  psr = pdwFaultStackAddress[ 7 ];

  // When the following line is hit, the variables contain the register values.
  for( ;; );
}

// The fault handler implementation calls a function called
// vGetRegistersFromStack().
extern __attribute__( ( naked ) ) void HardFault_Handler(void)
{
    __asm volatile
    (
        " tst lr, #4                                                \n"
        " ite eq                                                    \n"
        " mrseq r0, msp                                             \n"
        " mrsne r0, psp                                             \n"
        " ldr r1, [r0, #24]                                         \n"
        " ldr r2, handler2_address_const                            \n"
        " bx r2                                                     \n"
        " handler2_address_const: .word vGetRegistersFromStack      \n"
    );
}
