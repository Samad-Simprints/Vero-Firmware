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
#include "LPC18xx.h"
#include "adc_dd.hpp"
#include "lpc18xx_cgu.h"
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
#if 1
  if ( psSetAdc == LPC_ADC0 )
  {
    CGU_ConfigPWR( CGU_PERIPHERAL_ADC0, ENABLE );
    CGU_EntityConnect(CGU_CLKSRC_XTAL_OSC, CGU_BASE_APB1);
  }
  else if ( psSetAdc == LPC_ADC1 )
  {
    CGU_ConfigPWR( CGU_PERIPHERAL_ADC1, ENABLE );
    CGU_EntityConnect(CGU_CLKSRC_XTAL_OSC, CGU_BASE_APB3);
  }
#endif
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

  // setup pin mapping
#if 0
  if ( psADC == LPC_ADC0 )
  {
    LPC_SCU->ENAIO0 |= ( 1 << iChannel );
  }
  else
  {
    LPC_SCU->ENAIO1 |= ( 1 << iChannel );
  }
#endif
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

int iReadBandGap()
{
  int iResult;
  int SFSPF_7 = LPC_SCU->SFSPF_7;
  int ENAIO1 = LPC_SCU->ENAIO1;
  int ENAIO2 = LPC_SCU->ENAIO2;

  LPC_SCU->SFSPF_7 = 0x10;
  LPC_SCU->ENAIO1 =  0x80;
  LPC_SCU->ENAIO2 = 0x10;

  iResult = poADCDDgetUnit(1)->iReadChannel(7);

  LPC_SCU->SFSPF_7 = SFSPF_7;
  LPC_SCU->ENAIO1 =  ENAIO1;
  LPC_SCU->ENAIO2 = ENAIO2;

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

