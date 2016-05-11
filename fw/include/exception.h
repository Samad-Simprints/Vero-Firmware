//******************************************************************************
//
// Project INDEX Application header file
//
// (c) Fen Technology Ltd. 2009, 2010. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// COMPONENT:    INDEX
// MODULE:       exception.h
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:  Defines exception handling and recovery API
//
//******************************************************************************

#if !defined(_EXCEPTION_H_)
#define _EXCEPTION_H_

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Includes
//******************************************************************************

#include "global.h"

//******************************************************************************
// Constants
//******************************************************************************
#define ASSERT_MAX_DATA    128
#define ASSERT_MAX_FILE     32
#define ASSERT_MAX_REASON  128

enum {
  ERROR_HARDWARE_HARD_FAULT,
  ERROR_HARDWARE_UNEXPECTED_INTERRUPT,
  ERROR_SOFTWARE_RTOS_STACK,
  ERROR_SOFTWARE_RTOS_MALLOC,
  ERROR_SOFTWARE_RTOS_ASSERT,
  ERROR_SOFTWARE_THREAD_STUCK,
  ERROR_SOFTWARE_ASSERT,
  ERROR_SOFTWARE_ERROR,
  ERROR_SOFTWARE_ABORT,
  ERROR_SOFTWARE_CPP_PURE_VIRTUAL,
  ERROR_RESET,
  ERROR_REQUESTED,
  ERROR_UNKNOWN
};

//******************************************************************************
// Definitions
//******************************************************************************

// - information logged on a hardware fault
PACK(
struct tExceptionHardware_tag
{
  // execution context
  dword r0;
  dword r1;
  dword r2;
  dword r3;
  dword r12;
  dword lr;    // Link register.
  dword pc;    // Program counter.
  dword psr;   // Program status register.

  // exception context
  dword ccr;    // configuration and control register
  dword shcsr;  // hard fault status register

  dword cfsr;   // fault status register
  dword hfsr;   // hard fault status register
  dword mmfar;  // memory management fault address register
  dword bfar;   // Bus fault address register
});

typedef struct tExceptionHardware_tag tExceptionHardware;

PACK(
struct tAssert_tag
{
  dword dwLineNumber;
  char  acFile[ASSERT_MAX_FILE];
  char  acReason[ASSERT_MAX_REASON];
});
typedef struct tAssert_tag tAssert;

PACK(
struct tExceptionHeader_tag
{
  dword dwReason;       // the reason for the entry
  dword dwVersion;      // firmware version
  dword dwTimerCount;   // timer tick count since last boot
  dword dwInterruptId;  // active interrupt
});
typedef struct tExceptionHeader_tag tExceptionHeader;

PACK(
struct tExceptionRecord_tag
{
  tExceptionHeader  sHeader;

  union {
    byte  abData[ ASSERT_MAX_DATA ];
    tExceptionHardware sHWFault;
    tAssert sAssert;
  };
});
typedef struct tExceptionRecord_tag tExceptionRecord;

PACK(
struct tExceptionContainer_tag
{
  word  wCRC;
  word  wLength;
  tExceptionRecord sException;
});
typedef struct tExceptionContainer_tag tExceptionContainer;

//******************************************************************************
// Function Prototypes
//******************************************************************************
extern volatile dword dwLogTimerTick;

extern void vLogInit( dword dwVersion );
extern void vLogClear( void );
extern bool boLogGet( tExceptionRecord **poException, word *pwLength );
extern void vLogUnexpectedInterrupt( void );
extern void vLogRTOSAssert( const char *pzFile, int iLine, const char *pzCondition );
extern void vLogAssert( const byte bReason, const char *pzFile, dword dwLine, const char *pzCondition );
extern void vLogGeneral( byte bReason, void *pvRecord, word wLength );
extern void vLogWatchdog( void );
extern void vPrintCrashRecord( tExceptionRecord *poException );
extern bool boLogCache( tExceptionRecord *poLogCache );
extern bool boLogCacheGet( tExceptionRecord **poException, word *pwLength );
extern void vLogCacheClear();

#ifdef __cplusplus
}
#endif

#endif  // _EXCEPTION_H_

