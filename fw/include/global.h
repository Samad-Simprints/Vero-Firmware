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
#if !defined( UN20_APP )
extern int _printf(const char *format, ...);
#define printf                        _printf
#endif
#ifdef __cplusplus
}
#endif

#ifndef NULL
#define NULL                          (0)
#endif

#if !defined( UN20_APP )
#include <LPC18xx.h>
#include "FreeRTOS.h"
#include "task.h"
#else
#define configSER_INTERRUPT_PRIORITY  0
#define configGPIO_INTERRUPT_PRIORITY 0
#endif

// Include proj_defs here so it can make use of the above typedefs
#include "proj_defs.h"

// Include the debug api macros
#include "dbgapi.h"
#include "exception.h"

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

// If programming by Contract Enabled
#ifdef    PROG_BY_CONTRACT
#define PBC_ASSERT( cond )          do { if( !(cond) ) { vLogAssert( ERROR_SOFTWARE_ASSERT, __FILE__, __LINE__, #cond ); } } while(0)
#define PBC_ERROR( errortext )      do { vLogAssert( ERROR_SOFTWARE_ERROR, __FILE__, __LINE__, errortext ); } while(0)
#else
  #define PBC_ASSERT( cond )          do { (void)sizeof(cond); } while(0)
  #define PBC_ERROR( errortext )      do { (void)sizeof(errortext); } while(0)
#endif

#ifdef PROG_BY_CONTRACT
  #define PRECOND(str)                PBC_ASSERT(str)
  #define POSTCOND(str)               PBC_ASSERT(str)
  #define INVARIANT(str)              PBC_ASSERT(str)
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

#define _QUOTEME(x)                              #x
#define QUOTEME(x)                               _QUOTEME(x)

#endif
