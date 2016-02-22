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
#include <LPC18xx.h>
#include <cross_studio_io.h>

//#include "global.h"

#include <stdio.h>

#include "cli.h"
#include "gpio_dd.hpp"
#include "ser_dd.hpp"

#include "FreeRTOS.h"
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
// CLI
//******************************************************************************

static bool boShowVersion( char **papzArgs, int iInstance, int iNumArgs );
#if defined(CM_HOSTED)
static bool boQuit(char **papzArgs, int iInstance, int iNumArgs);
#endif
static bool boUN20Echo(char **papzArgs, int iInstance, int iNumArgs);

const tParserEntry asMainCLI[] =
{
#if defined(CM_HOSTED)
  CLICMD("quit", "Quit CM", 1, "", boQuit, 0),
#endif
  CLICMD( "un20ser", "Send to UN20 Uart", 2, "", boUN20Echo, 0 )
};

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

static const tLineCoding sUN20portConfig = {
  /*.dwDTERate =*/ 9600,                            // Data terminal rate in bits per second
  /*.bCharFormat =*/ 0,                             // Number of stop bits
  /*.bParityType =*/ 0,                             // Parity bit type
  /*.bDataBits =*/ 8                                // Number of data bits
};

static bool boUN20Echo(char **papzArgs, int iInstance, int iNumArgs)
{
  ISerialPort              *poUN20Port;
  int iRes;

  poUN20Port = poSERDDgetPort( UN20_UART );
  poUN20Port->vConfigurePort( &sUN20portConfig, 64, 300 );

  // set to blocking mode for transmit but not receive
  poUN20Port->vSetBlockingMode( ISerialPort::bmTransmitOnly );

  // flush buffers before we start
  poUN20Port->vFlush();

  while ( iRes = poUN20Port->iPutBuf( papzArgs[1], strlen(papzArgs[1]) ) == EOF )
  {
    vTaskDelay( 1 );
  }
  poUN20Port->iPutchar( '\n' );

  return true;
}

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
extern "C" void usb_main(void);
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

static void vTestAppTask(void* params)
{
  vTaskDelay(SECS_TO_TICKS(1));
  DEBUGMSG(ZONE_DBG_ALWAYS,("\n== Poweringup UN20 ==\n"));

  vPowerUn20On();

  for(;;)
  {
    vTaskDelay(1);
  }
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

  poDebugPort = poSERDDgetPort( CONSOLE_UART );
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

  // get the hardware under control
  vHalInit();

  // latch the power on
  vPowerSelfOn();

  //vHalTest();

  oMain.vInit();

  // initialise CLI
  vCLIinit();

  // report build information on console
  CLI_PRINT(("\nINDEX - SW: " INDEX_VERSION "\n"));

  // register CLI commands
  for ( int i = 0; i < ( sizeof( asMainCLI ) / sizeof( asMainCLI[ 0 ] ) ); ++i )
  {
    boCLIregisterEntry( &asMainCLI[i] );
  }

  /* Create the LPC App task. */
  vLpcAppInit();

  // initialise the Bluetooth stack
  bt_main();

  // initialise the USB stack
  //usb_main();

#if 0
  /* Create the test App task. */
  xTaskCreate( vTestAppTask, ( signed char * ) "TEST", LPCAPP_TASK_STACK_SIZE, ( void * ) NULL, LPCAPP_TASK_PRIORITY, NULL );
#endif
  /* Start the scheduler. */
  vTaskStartScheduler();

  /* Will only get here if there was insufficient memory to create the idle
  task.  The idle task is created within vTaskStartScheduler(). */
  for( ;; );

  // should never get here
  return 0;
}

tLock *poGetPrintfLock()
{
  return oMain.poGetPrintfLock();
}

static void vGetRegistersFromStack( dword *pdwFaultStackAddress )
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
 __attribute__( ( naked ) ) void HardFault_Handler(void)
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
