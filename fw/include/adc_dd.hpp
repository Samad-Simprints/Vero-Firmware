//******************************************************************************
//
// Project INDX Application header file
//
// (c) Fen Technology Ltd. 2016. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// COMPONENT:    NIMBUS
// MODULE:       adc_dd.h
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:  ADC Device Driver
//
//******************************************************************************

#if !defined(_ADC_DD_HPP_)
#define _ADC_DD_HPP_

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

class IAdcUnit
{
public:
  enum tBlockingMode {
    bmNone,
    bmTransmitOnly,
    bmReceiveOnly,
    bmBoth
  };
                            IAdcUnit() {}
  virtual                  ~IAdcUnit() {}

  virtual int               iReadChannel( int iChannel ) = 0;

};

//******************************************************************************
// Function Prototypes
//******************************************************************************

extern IAdcUnit *poADCDDgetUnit( const int iUnit );

#endif  // <HEADER_DEFINE>
