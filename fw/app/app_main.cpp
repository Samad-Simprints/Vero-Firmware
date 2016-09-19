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

#ifndef IGNORE_CROSSWORKS
#include <ctl_api.h>
#include <cross_studio_io.h>
#endif 

#include <LPC18xx.h>

//#include "global.h"

#include <stdio.h>
#include <stdlib.h>

#include "cli.h"
#include "gpio_dd.hpp"
#include "ser_dd.hpp"
#include "watchdog_fd.h"
#include "watchdog_dd.h"

#include "FreeRTOS.h"
#include "task.h"
#undef TRUE
#undef FALSE
#include "lpc18xx_cgu.h"
#include "lpc18xx_pwr.h"
#include "hal.h"
#include "lpcapp.h"
#include "helpers.hpp"
#include "delay.h"

#include "flash_iap.h"
#include "crc.h"


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
  void                      vRecoverCrashLog();

  // Accessors
  tLock                    *poGetPrintfLock()                       { return &oPrintfLock; }

  inline ISerialPort       *poGetDebugPort()                        { return poDebugPort; }

private:

  tLock                     oPrintfLock;

  ISerialPort              *poDebugPort;
};

unsigned long ulRunTimeStatsClock;

// structure in CPU flash that contains the BTADDR
PACK(
struct flash_bt_addr
{
  byte BDADDR[6];
  word crc;
});


//******************************************************************************
// Local Storage
//******************************************************************************

static tMain oMain;
static tExceptionRecord oExceptionRecord;

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
static bool boSleep(char **papzArgs, int iInstance, int iNumArgs);
static bool boCrashLog(char **papzArgs, int iInstance, int iNumArgs);
static bool boLogEntry(char **papzArgs, int iInstance, int iNumArgs);
static void vMilliSleep(long ms);

const tParserEntry asMainCLI[] =
{
#if defined(CM_HOSTED)
  CLICMD("quit", "Quit CM", 1, "", boQuit, 0),
#endif
  CLICMD( "un20ser", "Send to UN20 Uart", 2, "", boUN20Echo, 0 ),
  CLICMD( "sleep", "Sleep", 1, "", boSleep, 0 ),
  CLICMD( "clog", "Crash log", 1, "", boCrashLog, 0 ),
  CLICMD( "abort","make crash log entry", 1, "", boLogEntry, 0 )
};

//******************************************************************************
// Terminal IO
//******************************************************************************

// Serial port config
static const int            MAX_DEBUG_RX_QUEUE_SIZE         = 300;
static const int            MAX_DEBUG_TX_QUEUE_SIZE         = 300;

static const tLineCoding sDebugPortConfig = {
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

static bool boCrashLog(char **papzArgs, int iInstance, int iNumArgs)
{
  tExceptionRecord *poException;
  word wLogEventLength;

  if ( boLogCacheGet( &poException, &wLogEventLength ) )
  {
    vPrintCrashRecord( poException );
  }
  else
  {
    CLI_PRINT(("Crash log contains no data\n"));
  }

  return true;
}

static bool boUN20Echo(char **papzArgs, int iInstance, int iNumArgs)
{
  ISerialPort              *poUN20Port;
  int iRes;

  poUN20Port = poSERDDgetPort( UN20_UART );
  poUN20Port->vConfigurePort( &sUN20portConfig, 300, 64 );

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

static bool boSleep(char **papzArgs, int iInstance, int iNumArgs)
{
  PWR_PowerDown();
  return true;
}

static bool boLogEntry(char **papzArgs, int iInstance, int iNumArgs)
{
  abort();
  return true;
}

static void vMilliSleep(long ms) {
  for (long t = 0; t < ms * CCLK_KHZ / 4; t++) {
    vSetDebugPin(t % 2 == 0);
  }
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
extern "C" void storage_erase_keys(void);
extern "C" byte *pbGetBluetoothAddress(void);
extern "C" void vBTFatalError(void);

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

void check_failed(uint8_t *file, uint32_t line)
{
  vLogAssert( ERROR_SOFTWARE_ASSERT, (const char*)file, line, "LPClib" );
}

uint32_t	xCGU_Init(void)
{
  CGU_SetXTALOSC(12000000);
  CGU_EnableEntity(CGU_CLKSRC_XTAL_OSC, ENABLE);
  CGU_EntityConnect(CGU_CLKSRC_XTAL_OSC, CGU_CLKSRC_PLL1);
  /* Disable PLL1 - Flash mode hang */
  //CGU_EnableEntity(CGU_CLKSRC_PLL1, DISABLE);
  //CGU_SetPLL1(15);
  CGU_SetPLL1(8);
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

  poDebugPort = poSERDDgetPort( CONSOLE_UART );
  poDebugPort->vConfigurePort( &sDebugPortConfig,MAX_DEBUG_RX_QUEUE_SIZE, MAX_DEBUG_TX_QUEUE_SIZE );

  // set to blocking mode for transmit but not receive
  poDebugPort->vSetBlockingMode( ISerialPort::bmTransmitOnly );

  // flush buffers before we start
  poDebugPort->vFlush();
}

// Are both the buttons pushed to indicate pairing erase
bool vCheckForPairingClear()
{
  /* configure the buttons so we can sense them early*/
  BUTTON_0_POWER->vConfigure();
  BUTTON_1_SCAN->vConfigure();

  if ( boHalPowerButtonPressed() && boHalScanButtonPressed() )
  {
    CLI_PRINT(("*** Erase link keys ***\n"));
    storage_erase_keys();
  }
}

// return a pointer to a 6 byte BTADDR, called from the bluetooth stack
// must return something that can be used as a Bluetooth address
byte *pbGetBluetoothAddress(void)
{
  static flash_bt_addr default_addr = { { 0xF0, 0xAC, 0xD7, 0xC0, 0x00, 0x00 }, 0 };
  struct flash_bt_addr *paddr = (struct flash_bt_addr *)FLASH_ADDR(FLASH_BANK_B, FLASH_SECTOR_1);

  if ( paddr->crc != wCRCgenerateCRC( CRC_SEED, paddr->BDADDR, sizeof(paddr->BDADDR) ) )
  {
    paddr = &default_addr;
  }
  return paddr->BDADDR;
}

// callback from Bluetooth stack when its world falls apart
void vBTFatalError(void)
{
  vLogAssert( ERROR_SOFTWARE_ERROR, __FILE__, __LINE__, "Bluetooth Abort" );
}

int main( void )
{
  bool boPowerButtonAtStartup;

  DEBUG_MODULE_INIT( INDEX_DEFAULTS );

#if 1
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


  int count = 0;
  int NB_MEASURES = 10;
  bool measures[NB_MEASURES];
  for (int i = 0; i < NB_MEASURES; i++) {
    measures[i] = false;
  }

  // wait ~50 ms before actually doing any measure
  vMilliSleep(50);

  for (long t = 0; t < 60 * 20; t++) {
    if (measures[t % NB_MEASURES]) count--;
    measures[t % NB_MEASURES] = boHalPowerButtonPressed() || boHalUSBChargePresent();
    if (measures[t % NB_MEASURES]) count++;

    //vUiLedSet(LED_RING_0, measures[t % NB_MEASURES] ? GREEN : RED);
    if (count == NB_MEASURES) {
      break;
    }
    vMilliSleep(20);
  }

  if (count < NB_MEASURES) {
    vPowerSelfOff();
  }


  // preserve the crash log
  boLogCache( &oExceptionRecord );

  vLogInit( INDEX_REVISION_NUMBER );

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

  // clear link keys if scan button pressed at power up
  vCheckForPairingClear();  

  // remember if power button was pressed at startup
  boPowerButtonAtStartup = boHalPowerButtonPressed();

  // start up the watchdog and its monitor
  //vWDOGDDinit();
  vWATCHDOGstart();

  /* Create the LPC App task. */
  vLpcAppInit( boPowerButtonAtStartup );

  // initialise the Bluetooth stack
  bt_main();

  // initialise the USB stack
  // REINSTATE usb_main();

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

