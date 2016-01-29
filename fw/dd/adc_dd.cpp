//******************************************************************************
//
// Project INDEX Application source file
//
// (c) Fen Technology Ltd. 2016. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// COMPONENT:    INDEX
// MODULE:       adc_dd.cpp
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
#include "lpc18xx.h"
#include "adc_dd.hpp"

#include "global.h"

#include "proj_defs.h"


//******************************************************************************
// Constants
//******************************************************************************
static const int ADC_MAX_CLOCK    = 4500000;  // Max ADC clock is 4.5MHz
static const int ADC_MAX_TIMEOUT  = 1000;     // How long to wait for conversion

//******************************************************************************
// Definitions
//******************************************************************************

#if defined(CM_HOSTED)
extern "C" void vADCInit(const IRQn_Type iSetNVICchannel, const xQueueHandle poRxedChars, const xQueueHandle poCharsForTx);
#endif

class tAdcUnit : public IAdcUnit
{
public:
  tAdcUnit( LPC_ADCn_Type const *psSetAdc );

  int                       iReadChannel( int iChannel );

private:
  bool                      boInitialised;
  //
  IRQn_Type                 iNVICchannel;       // channel assigned to the ADC in the NVIC
  LPC_ADCn_Type            *psADC;              // actual ADC
};

//******************************************************************************
// Private Function Prototypes
//******************************************************************************

//******************************************************************************
// Local Storage
//******************************************************************************

static tAdcUnit oAdc0( LPC_ADC0 );
static tAdcUnit oAdc1( LPC_ADC1 );



//******************************************************************************
//******************************************************************************
// Private Functions
//******************************************************************************
//******************************************************************************


/* -----------------------------------------------------------------------
   tAdcUnit::tAdcUnit
   .......................................................................
   Description  : Constructor

   Params :  psSetADC - pointer to hardware registers for AC unit

   Returns : Nothing
   ----------------------------------------------------------------------- */

tAdcUnit::tAdcUnit( LPC_ADCn_Type const *psSetAdc )
  : //IAdcUnit(),
    boInitialised( false ),
    psADC( (LPC_ADCn_Type*)psSetAdc )
{
}

/* -----------------------------------------------------------------------
   tAdcUnit::iReadChannel
   .......................................................................
   Description  : read value on specified ADC channel

   Params : None

   Returns : Nothing
   ----------------------------------------------------------------------- */

int tAdcUnit::iReadChannel( int iChannel )
{
  int iResult = 0;
  bool boDone = false;
  int iTmp;

  // work out control register settings
  int iAdcChannel = ((1 << iChannel) & ADC0_CR_SEL_Msk);
  int iClkDiv = (( SystemCoreClock / ADC_MAX_CLOCK ) << ADC0_CR_CLKDIV_Pos);
  int iAdcControl = (1 << ADC0_CR_PDN_Pos) | (1 << ADC0_CR_START_Pos);

  // No interrupts
  psADC->INTEN = 0;
  // select channel, powerup converter and start conversion
  psADC->CR = ( iAdcChannel | iClkDiv | iAdcControl );

  for ( int iWait = 0; (iWait < ADC_MAX_TIMEOUT) && !boDone; iWait++ )
  {
    boDone = (psADC->GDR & ADC0_GDR_DONE_Msk);
  }

  // get raw ADC reading
  iResult = (( psADC->GDR & ADC0_GDR_V_VREF_Msk ) >> ADC0_GDR_V_VREF_Pos );

  return iResult;
}


//******************************************************************************
//******************************************************************************
// Public Functions
//******************************************************************************
//******************************************************************************

IAdcUnit *poADCDDgetUnit( const int iUnit )
{
  IAdcUnit *poRes = 0;

  switch ( iUnit )
  {
  case 0:
    poRes = &oAdc0;
    break;
  case 1:
    poRes = &oAdc1;
    break;
  default:
    PBC_ERROR( "Invalid ADC unit selected" );
    break;
  }

  return poRes;
}

