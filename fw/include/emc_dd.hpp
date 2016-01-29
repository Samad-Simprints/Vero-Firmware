//******************************************************************************
//
// Project BLISS Application source file
//
// (c) Fen Technology Ltd. 2013. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// COMPONENT:    BLISS
// MODULE:       emc_dd.h
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:  Header file
//
//******************************************************************************

#ifndef _EMC_DD_H_
#define _EMC_DD_H_

//******************************************************************************
// Includes
//******************************************************************************

#include "global.h"

//******************************************************************************
// Constants
//******************************************************************************

#define EMC_CS0_START_ADDR          ( 0x1C000000 ) // 16MB
#define EMC_CS1_START_ADDR          ( 0x1D000000 ) // 16MB
#define EMC_CS2_START_ADDR          ( 0x1E000000 ) // 16MB
#define EMC_CS3_START_ADDR          ( 0x1F000000 ) // 16MB

//******************************************************************************
// Definitions
//******************************************************************************

class IEMCDD
{
public:
  virtual void              vInit() = 0;
};

//******************************************************************************
// Public functions
//******************************************************************************

extern IEMCDD *poGetEMCDD();

#endif
//EOF

