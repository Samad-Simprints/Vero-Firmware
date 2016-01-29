//******************************************************************************
//
// Project NIMBUS Application header file
//
// (c) Fen Technology Ltd. 2009, 2010. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// COMPONENT:    NIMBUS
// MODULE:       watchdog_dd.h
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:
//
//******************************************************************************

#if !defined(_WATCHDOG_DD_H_)
#define _WATCHDOG_DD_H_

//******************************************************************************
// Includes
//******************************************************************************

#include "global.h"

//******************************************************************************
// Constants
//******************************************************************************

#define WATCHDOG_TIMEOUT_MS           ( 5000ul )

#ifdef __cplusplus
extern "C" {
#endif

//******************************************************************************
// Definitions
//******************************************************************************

//******************************************************************************
// Function Prototypes
//******************************************************************************

extern void vWDOGDDinit(void);
extern void vWDOGDDkick(void);
extern void vWDOGDDreboot(void);

#ifdef __cplusplus
}
#endif

#endif  // <HEADER_DEFINE>
