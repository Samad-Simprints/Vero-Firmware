//******************************************************************************
//
// Project Index source file
//
// (c) Fen Technology Ltd. 2016. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// MODULE: lpcapp.h
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:  Main LPC application header file
//
//******************************************************************************

#if !defined(_LPCAPP_H_)
#define _LPCAPP_H_

//******************************************************************************
// Includes
//******************************************************************************

#include "global.h"

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Constants
//******************************************************************************

#define LPCAPP_TASK_STACK_SIZE           ( 1024 + TASK_DEBUG_OVERHEAD ) // Big enough for 2 messages on the stack
#define LPCAPP_TASK_PRIORITY             ( tskIDLE_PRIORITY + 1 )

//******************************************************************************
// Definitions
//******************************************************************************

//******************************************************************************
// Function Prototypes
//******************************************************************************

void vLpcAppInit();
void vLpcAppTask( void *pvParameters );

#ifdef __cplusplus
}
#endif

#endif  // _LPCAPP_H_
