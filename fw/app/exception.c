//******************************************************************************
//
// Project INDEX Application source file
//
// (c) Fen Technology Ltd. 2014. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// COMPONENT:    INDEX
// MODULE:       exception
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
#include "crc.h"

#include "exception.h"

#include "watchdog_dd.h"

//******************************************************************************
// Constants
//******************************************************************************

//******************************************************************************
// Definitions
//******************************************************************************

#define LOG_PRINT( str )              printf str

/* Cortex M3 defintions missing from the core_cm3.h eader file */
/* Memory Management Fault Status register */
#define SCB_CFSR_MMFSR_MMARVALID_Pos          7                                             /*!< SCB CFSR(MMFSR): MMARVALID Position */
#define SCB_CFSR_MMFSR_MMARVALID_Msk          (1UL << SCB_CFSR_MMFSR_MMARVALID_Pos)         /*!< SCB CFSR(MMFSR): MMARVALID Mask */
#define SCB_CFSR_MMFSR_MSTKERR_Pos            4                                             /*!< SCB CFSR(MMFSR): MSTKERR Position */
#define SCB_CFSR_MMFSR_MSTKERR_Msk            (1UL << SCB_CFSR_MMFSR_MSTKERR_Pos)           /*!< SCB CFSR(MMFSR): MSTKERR Mask */
#define SCB_CFSR_MMFSR_MUNSTKERR_Pos          3                                             /*!< SCB CFSR(MMFSR): MUNSTKERR Position */
#define SCB_CFSR_MMFSR_MUNSTKERR_Msk          (1UL << SCB_CFSR_MMFSR_MUNSTKERR_Pos)         /*!< SCB CFSR(MMFSR): MUNSTKERR Mask */
#define SCB_CFSR_MMFSR_DACCVIOL_Pos           1                                             /*!< SCB CFSR(MMFSR): DACCVIOL Position */
#define SCB_CFSR_MMFSR_DACCVIOL_Msk           (1UL << SCB_CFSR_MMFSR_DACCVIOL_Pos)          /*!< SCB CFSR(MMFSR): DACCVIOL Mask */
#define SCB_CFSR_MMFSR_IACCVIOL_Pos           0                                             /*!< SCB CFSR(MMFSR): IACCVIOL Position */
#define SCB_CFSR_MMFSR_IACCVIOL_Msk           (1UL << SCB_CFSR_MMFSR_IACCVIOL_Pos)          /*!< SCB CFSR(MMFSR): IACCVIOL Mask */

/* Bus fault status register */
#define SCB_CFSR_BFSR_BFARVALID_Pos           7                                             /*!< SCB CFSR(BFSR): BFARVALID Position */
#define SCB_CFSR_BFSR_BFARVALID_Msk           (1UL << SCB_CFSR_BFSR_BFARVALID_Pos)          /*!< SCB CFSR(BFSR): BFARVALID Mask */
#define SCB_CFSR_BFSR_STKERR_Pos              4                                             /*!< SCB CFSR(BFSR): STKERR Position */
#define SCB_CFSR_BFSR_STKERR_Msk              (1UL << SCB_CFSR_BFSR_STKERR_Pos)             /*!< SCB CFSR(BFSR): STKERR Mask */
#define SCB_CFSR_BFSR_UNSTKERR_Pos            3                                             /*!< SCB CFSR(BFSR): UNSTKERR Position */
#define SCB_CFSR_BFSR_UNSTKERR_Msk            (1UL << SCB_CFSR_BFSR_UNSTKERR_Pos)           /*!< SCB CFSR(BFSR): UNSTKERR Mask */
#define SCB_CFSR_BFSR_IMPRECISERR_Pos         2                                             /*!< SCB CFSR(BFSR): IMPRECISERR Position */
#define SCB_CFSR_BFSR_IMPRECISERR_Msk         (1UL << SCB_CFSR_BFSR_IMPRECISERR_Pos)        /*!< SCB CFSR(BFSR): IMPRECISERR Mask */
#define SCB_CFSR_BFSR_PRECISERR_Pos           1                                             /*!< SCB CFSR(BFSR): PRECISERR Position */
#define SCB_CFSR_BFSR_PRECISERR_Msk           (1UL << SCB_CFSR_BFSR_PRECISERR_Pos)          /*!< SCB CFSR(BFSR): PRECISERR Mask */
#define SCB_CFSR_BFSR_IBUSERR_Pos             0                                             /*!< SCB CFSR(BFSR): IBUSERR Position */
#define SCB_CFSR_BFSR_IBUSERR_Msk             (1UL << SCB_CFSR_BFSR_IBUSERR_Pos)            /*!< SCB CFSR(BFSR): IBUSERR Mask */

/* Usage fault status register */
#define SCB_CFSR_UFSR_DIVBYZERO_Pos           9                                             /*!< SCB CFSR(UFSR): DIVBYZERO Position */
#define SCB_CFSR_UFSR_DIVBYZERO_Msk           (1UL << SCB_CFSR_UFSR_DIVBYZERO_Pos)          /*!< SCB CFSR(UFSR): DIVBYZERO Mask */
#define SCB_CFSR_UFSR_UNALIGNED_Pos           8                                             /*!< SCB CFSR(UFSR): UNALIGNED Position */
#define SCB_CFSR_UFSR_UNALIGNED_Msk           (1UL << SCB_CFSR_UFSR_UNALIGNED_Pos)          /*!< SCB CFSR(UFSR): UNALIGNED Mask */
#define SCB_CFSR_UFSR_NOCP_Pos                3                                             /*!< SCB CFSR(UFSR): NOCP Position */
#define SCB_CFSR_UFSR_NOCP_Msk                (1UL << SCB_CFSR_UFSR_NOCP_Pos)               /*!< SCB CFSR(UFSR): NOCP Mask */
#define SCB_CFSR_UFSR_INVPC_Pos               2                                             /*!< SCB CFSR(UFSR): INVPC Position */
#define SCB_CFSR_UFSR_INVPC_Msk               (1UL << SCB_CFSR_UFSR_INVPC_Pos)              /*!< SCB CFSR(UFSR): INVPC Mask */
#define SCB_CFSR_UFSR_INVSTATE_Pos            1                                             /*!< SCB CFSR(UFSR): INVSTATE Position */
#define SCB_CFSR_UFSR_INVSTATE_Msk            (1UL << SCB_CFSR_UFSR_INVSTATE_Pos)           /*!< SCB CFSR(UFSR): INVSTATE Mask */
#define SCB_CFSR_UFSR_UNDEFINSTR_Pos          0                                             /*!< SCB CFSR(UFSR): UNDEFINSTR Position */
#define SCB_CFSR_UFSR_UNDEFINSTR_Msk          (1UL << SCB_CFSR_UFSR_UNDEFINSTR_Pos)         /*!< SCB CFSR(UFSR): UNDEFINSTR Mask */


#define        CM3_BITFIELD(reg, field)        ( ( reg & field ##_Msk ) >> field ##_Pos )

//******************************************************************************
// Private Function Prototypes
//******************************************************************************
static void vHalt( void );
static dword get_ipsr( void );
static void vErrorLogCreateAlways( byte bErrorType, void *pvData, word wLogDataLength );
static void vErrorLogCreate( byte bErrorType, void *pvData, word wLogDataLength );
void vGetRegistersFromStack( dword *pdwFaultStackAddress );

//******************************************************************************
// Local Storage
//******************************************************************************
#if !defined(CM_HOSTED)
__attribute__( (section( ".crash_log" )) )
#endif
static tExceptionContainer sFaultRecord;

// firmware version number
static dword dwVersion = 0;

// flag used to ensure only one exception is recorded incase things escalate
static volatile bool boFaultRecorded = false;

// global time counter updated by the watchdog task
volatile dword dwLogTimerTick = 0;

//******************************************************************************
//******************************************************************************
// Private Functions
//******************************************************************************
//******************************************************************************

// Read IPSR register - tells us which interrupt is active (0 is none)
#if defined(CM_HOSTED)
static dword get_ipsr ( void )
{
  return 0;
}
#else
static inline __attribute__ ((always_inline))
dword get_ipsr ( void )
{
  dword x = 0;

  __asm volatile
 (
    "mrs %0, ipsr"
    : "=r" (x)
  );

  return x;
}
#endif

// create the error log record in memory. Payload has already been stored
// in the exception record.
static void vErrorLogCreateAlways( byte bErrorType, void *pvData, word wLogDataLength )
{
  word wCRC = CRC_SEED;

  // fill in the header
  sFaultRecord.sException.sHeader.dwReason = bErrorType;
  sFaultRecord.sException.sHeader.dwVersion = dwVersion;
  sFaultRecord.sException.sHeader.dwTimerCount = dwLogTimerTick;
  sFaultRecord.sException.sHeader.dwInterruptId = get_ipsr();

  // calculate the CRC on the data
  sFaultRecord.wLength = sizeof( tExceptionHeader ) + min(wLogDataLength, ASSERT_MAX_DATA);
  sFaultRecord.wCRC =  wCRCgenerateCRC( wCRC, (byte*)&sFaultRecord.sException, sFaultRecord.wLength);
}

// create an error log entry but only allow the first call to do so
static void vErrorLogCreate( byte bErrorType, void *pvData, word wLogDataLength )
{
  boFaultRecorded = true;
  vErrorLogCreateAlways( bErrorType, pvData, wLogDataLength );
}

// this cannot be static or the code is removed in a release build
void vGetRegistersFromStack( dword *pdwFaultStackAddress )
{
  tExceptionHardware *psFault = &sFaultRecord.sException.sHWFault;
  taskDISABLE_INTERRUPTS();

  if ( !boFaultRecorded )
  {
    psFault->r0 = pdwFaultStackAddress[ 0 ];
    psFault->r1 = pdwFaultStackAddress[ 1 ];
    psFault->r2 = pdwFaultStackAddress[ 2 ];
    psFault->r3 = pdwFaultStackAddress[ 3 ];

    psFault->r12 = pdwFaultStackAddress[ 4 ];
    psFault->lr = pdwFaultStackAddress[ 5 ];
    psFault->pc = pdwFaultStackAddress[ 6 ];
    psFault->psr = pdwFaultStackAddress[ 7 ];

    psFault->ccr    = SCB->CCR;
    psFault->shcsr  = SCB->SHCSR;
    psFault->cfsr   = SCB->CFSR;
    psFault->hfsr   = SCB->HFSR;
    psFault->mmfar  = SCB->MMFAR;
    psFault->bfar   = SCB->BFAR;

    // create the error log entry
    vErrorLogCreate( ERROR_HARDWARE_HARD_FAULT, psFault, sizeof(tExceptionHardware) );
  }
  vHalt();
}

// Function to force the processor to reset
static void vHalt( void )
{
  tExceptionRecord *poException;
  word len;

  // if the debugger is attached drop into it
  if ( debug_enabled() )
  {
    // check that there is a valid error record
    if ( !boLogGet( &poException, &len ) )
    {
      debug_break();
    }

    debug_break();
  }

  // wait for the watchdog to kick in
  vWDOGDDreboot();

  for ( ;; )
  {
    // must not return as stack may not be intact
  }
}

//******************************************************************************
//******************************************************************************
// Public Functions
//******************************************************************************
//******************************************************************************

#if !defined(CM_HOSTED)
// Figure out which stack pointer is in use and pass this to a function
// to read register values from the stack
__attribute__( (naked) ) void HardFault_Handler( void )
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
#endif

// handle unexpected interrupt callback
void vLogUnexpectedInterrupt( void )
{
  taskDISABLE_INTERRUPTS();

  if ( !boFaultRecorded )
  {
    // create the error log entry
    vErrorLogCreate( ERROR_HARDWARE_UNEXPECTED_INTERRUPT, 0, 0 );
  }
  vHalt();
}

// log an App assert failure
void vLogAssert( const byte bReason, const char *pzFile, dword dwLine, const char *pzCondition )
{
  tAssert *psAssert = &sFaultRecord.sException.sAssert;
  const char *pzBaseName;
  taskDISABLE_INTERRUPTS();

  if ( !boFaultRecorded )
  {
    pzBaseName = (strrchr(pzFile, '/') ? strrchr(pzFile, '/') + 1 : pzFile); //lint !e613
    psAssert->dwLineNumber = dwLine;
    strncpy( psAssert->acFile, pzBaseName, ASSERT_MAX_FILE );
    strncpy( psAssert->acReason, pzCondition, ASSERT_MAX_REASON );

    psAssert->acFile[ ASSERT_MAX_FILE - 1 ] = 0;
    psAssert->acReason[ ASSERT_MAX_REASON - 1 ] = 0;

    vErrorLogCreate( bReason, psAssert, sizeof( sFaultRecord.sException.sAssert) );
  }
  vHalt();
}

// log an RTOS assert failure
void vLogRTOSAssert( const char *pzFile, int iLine, const char *pzCondition )
{
  vLogAssert( ERROR_SOFTWARE_RTOS_ASSERT, pzFile, iLine, pzCondition );
}

// log arbitrary data to the event record
void vLogGeneral( byte bReason, void *pvData, word wLength )
{
  taskDISABLE_INTERRUPTS();

  if ( !boFaultRecorded )
  {
    wLength = min(wLength, ASSERT_MAX_DATA);

    if ( wLength != 0 )
    {
      memcpy(sFaultRecord.sException.abData, pvData, wLength);
    }
    vErrorLogCreate( bReason, sFaultRecord.sException.abData, wLength );
  }

  vHalt();
}

// Log a watchdog timeout event (This is the default that we keep in place)
// Note it only updates the record so the timestamp is updated, it does does not block.
void vLogWatchdog( void )
{
  vErrorLogCreateAlways( ERROR_RESET, 0, 0 );
}

// Invalidate the RAM log area
void vLogClear( void )
{
  sFaultRecord.wLength = 0;
  sFaultRecord.wCRC = CRC_SEED;
}


// initialise the Error logger
void vLogInit( dword dwSetVersion )
{
  dwVersion = dwSetVersion;
}

#define RSID_REASON_MASK      ( 0x0f )

// returns a pointer to the RAM log area if the record is valid
bool boLogGet( tExceptionRecord **poException, word *pwLength )
{
  bool boValid = false;
  word wCRC = CRC_SEED;

  if ( (sFaultRecord.wLength > 0) && (sFaultRecord.wLength <= sizeof (tExceptionRecord)) )
  {
    wCRC = wCRCgenerateCRC( wCRC, (byte*)&sFaultRecord.sException, sFaultRecord.wLength );
    if ( wCRC == sFaultRecord.wCRC )
    {
      *poException = &sFaultRecord.sException;
      *pwLength = sFaultRecord.wLength;
      boValid = true;
    }
#if 0
#if !defined(CM_HOSTED)
    // if this is a reset error then store the reason register in the "interrupt id"
    // this register is only valid AFTER the reset has occurred.
    if ( boValid && ((*poException)->sHeader.dwReason == ERROR_RESET) )
    {
      (*poException)->sHeader.dwInterruptId = ( LPC_SC->RSID & RSID_REASON_MASK );
    }
#endif
#endif
  }
  #if 0
#if !defined(CM_HOSTED)
  // always clear the reset reason register
  LPC_SC->RSID = RSID_REASON_MASK;
#endif
#endif
  return boValid;
}

void vPrintCrashRecord( tExceptionRecord *poException )
{
  dword mmfsr;
  dword bfsr;
  dword ufsr;

  LOG_PRINT( ("Reason: %d, Version: %d, Timer: %d, IntId: %d\n",
                        poException->sHeader.dwReason,
                        poException->sHeader.dwVersion,
                        poException->sHeader.dwTimerCount,
                        poException->sHeader.dwInterruptId) );
  switch ( poException->sHeader.dwReason )
  {
    case ERROR_HARDWARE_HARD_FAULT:
      mmfsr = CM3_BITFIELD( poException->sHWFault.cfsr, SCB_CFSR_MEMFAULTSR );
      bfsr = CM3_BITFIELD( poException->sHWFault.cfsr, SCB_CFSR_BUSFAULTSR );
      ufsr = CM3_BITFIELD( poException->sHWFault.cfsr, SCB_CFSR_USGFAULTSR );

      LOG_PRINT( ("HARD FAULT:\n") );
      LOG_PRINT( (" pc:%08X, lr:%08X, psr:%08X\n", poException->sHWFault.pc, poException->sHWFault.lr, poException->sHWFault.psr) );

      LOG_PRINT( (" r0:%08X, r1:%08X, r2:%08X, r3:%08X, r12:%08X\n",
                    poException->sHWFault.r0, poException->sHWFault.r1, poException->sHWFault.r2, poException->sHWFault.r12) );

      LOG_PRINT( (" cfsr:%08X\n", poException->sHWFault.cfsr) );
      LOG_PRINT( ("  mmfsr: mmarvalid:%X,mstkerr:%X, munstkerr:%X, daccviol:%X, iaccviol:%X\n",
                    CM3_BITFIELD( mmfsr, SCB_CFSR_MMFSR_MMARVALID ),
                    CM3_BITFIELD( mmfsr, SCB_CFSR_MMFSR_MSTKERR ),
                    CM3_BITFIELD( mmfsr, SCB_CFSR_MMFSR_MUNSTKERR ),
                    CM3_BITFIELD( mmfsr, SCB_CFSR_MMFSR_DACCVIOL ),
                    CM3_BITFIELD( mmfsr, SCB_CFSR_MMFSR_IACCVIOL ) ));
      LOG_PRINT( ("  bfsr: bfarvalid:%X, stkerr:%X, unstkerr:%X, impreciserr:%X, preciserr:%X, ibuserr:%X\n",
                    CM3_BITFIELD( bfsr, SCB_CFSR_BFSR_BFARVALID ),
                    CM3_BITFIELD( bfsr, SCB_CFSR_BFSR_STKERR ),
                    CM3_BITFIELD( bfsr, SCB_CFSR_BFSR_UNSTKERR ),
                    CM3_BITFIELD( bfsr, SCB_CFSR_BFSR_IMPRECISERR ),
                    CM3_BITFIELD( bfsr, SCB_CFSR_BFSR_PRECISERR ),
                    CM3_BITFIELD( bfsr, SCB_CFSR_BFSR_IBUSERR) ) );
      LOG_PRINT( ("  ufsr: divbyzero:%X, unaligned:%X, nocp:%X, invpc:%X, invstate:%X, undefinstr:%X\n",
                    CM3_BITFIELD( ufsr, SCB_CFSR_UFSR_DIVBYZERO ),
                    CM3_BITFIELD( ufsr, SCB_CFSR_UFSR_UNALIGNED ),
                    CM3_BITFIELD( ufsr, SCB_CFSR_UFSR_NOCP ),
                    CM3_BITFIELD( ufsr, SCB_CFSR_UFSR_INVPC ),
                    CM3_BITFIELD( ufsr, SCB_CFSR_UFSR_INVSTATE ),
                    CM3_BITFIELD( ufsr, SCB_CFSR_UFSR_UNDEFINSTR ) ) );

      LOG_PRINT( (" hfsr:%08X\n", poException->sHWFault.hfsr) );
      LOG_PRINT( ("  hfsr: forced:%X, vecttbl:%X\n",
                    CM3_BITFIELD( poException->sHWFault.hfsr, SCB_HFSR_FORCED ),
                    CM3_BITFIELD( poException->sHWFault.hfsr, SCB_HFSR_VECTTBL ) ));

      LOG_PRINT( (" ccr:%08X, shcsr:%08X\n",
                    poException->sHWFault.ccr, poException->sHWFault.shcsr));

      LOG_PRINT( (" mmfar:%08X, bfar:%08X\n",
                    poException->sHWFault.mmfar, poException->sHWFault.bfar) );

      LOG_PRINT( ("\n") );

      break;
    case ERROR_SOFTWARE_RTOS_STACK:
      LOG_PRINT( ("STACK OVERFLOW: in %s\n", poException->abData ) );
      break;
    case ERROR_SOFTWARE_RTOS_MALLOC:
      LOG_PRINT( ("MALLOC FAIL: in %s\n", poException->abData) );
      break;
    case ERROR_SOFTWARE_THREAD_STUCK:
      LOG_PRINT( ("THREAD_STUCK: in %s\n", poException->abData) );
      break;
    case ERROR_SOFTWARE_ASSERT:
      LOG_PRINT( ("PBC_ASSERT: %s in %s (%d)\n", poException->sAssert.acReason, poException->sAssert.acFile, poException->sAssert.dwLineNumber ) );
      break;
    case ERROR_SOFTWARE_ERROR:
      LOG_PRINT( ("PBC_ERROR: %s in %s (%d)\n", poException->sAssert.acReason, poException->sAssert.acFile, poException->sAssert.dwLineNumber) );
      break;
    case ERROR_SOFTWARE_RTOS_ASSERT:
      LOG_PRINT( ("RTOS_ASSERT: %s in %s (%d)\n", poException->sAssert.acReason, poException->sAssert.acFile, poException->sAssert.dwLineNumber) );
      break;
    case ERROR_HARDWARE_UNEXPECTED_INTERRUPT:
      break;
    case ERROR_RESET:
    case ERROR_REQUESTED:
    case ERROR_SOFTWARE_ABORT:
    case ERROR_SOFTWARE_CPP_PURE_VIRTUAL:
      break;
    case ERROR_UNKNOWN:
    default:
      LOG_PRINT( ("UNKNOWN FAULT REASON\n"));
      break;
  }
}
