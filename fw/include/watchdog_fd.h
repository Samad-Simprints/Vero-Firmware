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
// MODULE:       watchdog_fd.h
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:  Definitions relating to the watchdog monitor
//
//******************************************************************************

#if !defined(_WATCHDOG_FD_H_)
#define _WATCHDOG_FD_H_

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

//******************************************************************************
// Definitions
//******************************************************************************


//******************************************************************************
// Function Prototypes
//******************************************************************************

void vWATCHDOGstart( void );

#ifdef __cplusplus
}
#endif

#endif  // _WATCHDOG_FD_H_

