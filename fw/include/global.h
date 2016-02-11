//******************************************************************************
//
// Project KELLS source file
//
// (c) Fen Technology Ltd. 2014. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// COMPONENT:    KELLS
// MODULE:       global.h
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:  see Header file for details
//
//******************************************************************************

#if !defined(GLOBAL_H)
#define GLOBAL_H

#include <stddef.h>
#include <stdio.h>
//#include <lpc18xx.h>
//#include <cross_studio_io.h>
#include <string.h>

#define ELEMENTSOF(_x_) ((sizeof(_x_) / sizeof(_x_[0])))

#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))

typedef unsigned char                 byte;
typedef unsigned short                word;
typedef unsigned long                 dword;
typedef unsigned long long            uint64;

#ifndef WIN32
#ifndef __cplusplus
#ifndef LPC_TYPES_H
typedef unsigned char                 bool;
#undef FALSE
#define FALSE                         ( 0 )
#define false                         FALSE
#undef TRUE
#define TRUE                          ( !FALSE )
#define true                          TRUE
#endif
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
#if 0
#if !defined( ALMSIM_STANDALONE )
extern int _printf(const char *format, ...);
#define printf                        _printf
#endif
#endif
#ifdef __cplusplus
}
#endif

#ifndef NULL
#define NULL                          (0)
#endif

#if !defined( UN20_APP )
#include <lpc18xx.h>
#include "freertos.h"
#else
#define configSER_INTERRUPT_PRIORITY  0
#endif

// Include proj_defs here so it can make use of the above typedefs
#include "proj_defs.h"

// Include the debug api macros
#include "dbgapi.h"

#ifdef DEBUG
#include <cross_studio_io.h>

// Programming by Contract Enabled
#define   PROG_BY_CONTRACT

// Full debug output
#define   SET_DEBUG_ALL
#endif

//******************************************************************************
// Definitions
//******************************************************************************

//
// Use this to remove compiler warnings for unused variables or function params
//
#define UNUSED_VARIABLE( var )        (void)var

#define   SIMPLEASSERT(condition)                       \
  if (!(condition))                                     \
  {                                                     \
    DEBUG_ERROR(( "Assert failed: %s\n", #condition ));   \
    while (1);                                          \
    debug_break();                                      \
  }
// from exception.h
extern void vLogAssert( const byte bReason, const char *pzFile, dword dwLine, const char *pzCondition );
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

// If programming by Contract Enabled
#ifdef    PROG_BY_CONTRACT
#define PBC_ASSERT( cond )          do { if( !(cond) ) { vLogAssert( ERROR_SOFTWARE_ASSERT, __FILE__, __LINE__, #cond ); } } while(0)
#define PBC_ERROR( errortext )      do { vLogAssert( ERROR_SOFTWARE_ERROR, __FILE__, __LINE__, errortext ); } while(0)
#else
  #define PBC_ASSERT( cond )          do { (void)sizeof(cond); } while(0)
  #define PBC_ERROR( errortext )      do { (void)sizeof(errortext); } while(0)
#endif

#ifdef PROG_BY_CONTRACT
  #define PRECOND(str)                SIMPLEASSERT(str)
  #define POSTCOND(str)               SIMPLEASSERT(str)
  #define INVARIANT(str)              SIMPLEASSERT(str)
#else
  #define PRECOND(str)
  #define POSTCOND(str)
  #define INVARIANT(str)
#endif

#if !defined(min)
#define min(a, b)                     ((a) < (b) ? (a) : (b))
#endif
#if !defined(max)
#define max(a, b)                     ((a) > (b) ? (a) : (b))
#endif

#define vFreeAndNull( obj )           { if ( obj != 0 ) { obj->vFree(); obj = 0; } }

#endif
