//******************************************************************************
//
// Project NIMBUS header file
//
// (c) Fen Technology Limited 2014. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// COMPONENT:    NIMBUS
// MODULE:       ssp_dd.hpp
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:  Synchronous Serial Port Device Driver
//
//******************************************************************************

#if !defined(_SSP_DD_HPP_)
#define _SSP_DD_HPP_

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

class ISSP
{
public:
  virtual void              vInit()                                         = 0;
  virtual void              vReadBlock( byte * const pabCommand,
                                        const int iCommandLen,
                                        byte * const pabData,
                                        const int iDataLen )                = 0;
  virtual void              vWriteBlock( byte * const pabCommand,
                                         const int iCommandLen,
                                         byte * const pabData,
                                         int iDataLen )                     = 0;
  virtual void              vSetRTOSrunning( bool boSetRTOSrunning )        = 0;
};

//******************************************************************************
// Function Prototypes
//******************************************************************************

extern ISSP *poGetSSP();

#endif  // <HEADER_DEFINE>
