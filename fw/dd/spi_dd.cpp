//******************************************************************************
//
// Project BLISS Application source file
//
// (c) Fen Technology Ltd. 2009, 2010. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// COMPONENT:    BLISS
// MODULE:       spi_dd.c
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

#include "spi_dd.hpp"

#include "global.h"
#include "gpio_dd.hpp"

#include "freertos.h"
#include "task.h"

//******************************************************************************
// Private Function Prototypes
//******************************************************************************

//******************************************************************************
// Constants
//******************************************************************************

// SPI port related constants
#define SSP_PRESCALE                 ( 4 )
#define SSP_COMMON                   ( SSP_CR0_SPI | SSP_CR0_CPOL_H | SSP_CR0_CPHA_H | SSP_CR0_SCR )

#define SSP_CR0_5_BIT                ( 4 << 0 )
#define SSP_CR0_8_BIT                ( 7 << 0 )
#define SSP_CR0_9_BIT                ( 8 << 0 )
#define SSP_CR0_10_BIT               ( 9 << 0 )
#define SSP_CR0_16_BIT               ( 15 << 0 )
#define SSP_CR0_SPI                  ( 0 << 4 )
#define SSP_CR0_CPOL_L               ( 0 << 6 )
#define SSP_CR0_CPOL_H               ( 1 << 6 )
#define SSP_CR0_CPHA_L               ( 0 << 7 )
#define SSP_CR0_CPHA_H               ( 1 << 7 )
#define SSP_CR0_SCR                  ( 1 << 8 )

#define SSP_CR1_SSE                  ( 1 << 1 )

#define SSP_SR_TFE                   ( 1 << 0 )
#define SSP_SR_TNF                   ( 1 << 1 )
#define SSP_SR_RNE                   ( 1 << 2 )
#define SSP_SR_RFF                   ( 1 << 3 )
#define SSP_SR_BSY                   ( 1 << 4 )

//******************************************************************************
// Definitions
//******************************************************************************

class tSPIDD : public ISPIDD
{
public:
  void                      vInit( void );

  void                      vWriteBlock( word *pwData, int iDataLen );
  void                      vReadBlock( word *pwCommand, word *pwData, int iDataLen );
  void                      vWriteByte( byte bData );
};

//******************************************************************************
// Local Storage
//******************************************************************************

static tSPIDD oSPIDD;

//******************************************************************************
//******************************************************************************
// Private Functions
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
// Public Functions
//******************************************************************************
//******************************************************************************

/* -----------------------------------------------------------------------
   poGetSPIDD
   .......................................................................
   Description  : Allows other modules access to the SPI DD object

   Params : None

   Returns : ISPIDD * - pointer to SPI DD object
   ----------------------------------------------------------------------- */

ISPIDD *poGetSPIDD()
{
  return &oSPIDD;
}

/* -----------------------------------------------------------------------
   tSPIDD::vInit
   .......................................................................
   Description  : Initialise the SPI device driver

   Params : None

   Returns : Nothing
   ----------------------------------------------------------------------- */

void tSPIDD::vInit( void )
{
  // select clk/1 for SSP1
//  LPC_CGU->BASE_SSP1_CLK = ( 0x09 << 24 ); // change clock source to PLL1
//  LPC_CCU1->CLK_M3_SSP1_CFG |= ( 1 << 0 );  // enable M3 clock to SSP1

  // configure pins
  vGPIODDconfigurePin( OSD_MOSI );
  vGPIODDconfigurePin( OSD_MISO );
  vGPIODDconfigurePin( OSD_CSn );

  // OSD_CLK (P 1_19)is not available as a GPIO pin so gpio_dd cannot be used to set value
  LPC_SCU->SFSP1_19 = PINFUNC_ALT1; // SSP mode

  LPC_SSP1->CPSR = SSP_PRESCALE;
  LPC_SSP1->CR0  = SSP_CR0_16_BIT | SSP_COMMON;
  LPC_SSP1->CR1  = SSP_CR1_SSE;

}

/* -----------------------------------------------------------------------
   tSPIDD::vWriteBlock
   .......................................................................
   Description  : Writes data out of the SSP

   Params : pwData - pointer to data to write
            iDataLen - length of data in words

   Returns : Nothing
   ----------------------------------------------------------------------- */

void tSPIDD::vWriteBlock( word *pwData, int iDataLen )
{
  // assume less than one FIFO's worth of data will be sent
  PRECOND( iDataLen <= 8 );

  // wait for the SSP to be not busy
  while ( 0 != ( LPC_SSP1->SR & SSP_SR_BSY ) )
  {
  }

  // send the data
  while ( iDataLen > 0 )
  {
    // send the data word
    LPC_SSP1->DR = *pwData;

    --iDataLen;
    ++pwData;

    // remove any data coming in, from the FIFO
    if ( 0 != ( LPC_SSP1->SR & SSP_SR_RNE ) )
    {
      LPC_SSP1->DR;
    }
  }

  // wait for the SSP to finish sending
  while ( 0 != ( LPC_SSP1->SR & SSP_SR_BSY ) )
  {
  }

  // flush any data received as a result of sending
  while ( 0 != ( LPC_SSP1->SR & SSP_SR_RNE ) )
  {
    LPC_SSP1->DR;
  }
}

//******************************************************************************
// tSPIDD::vReadBlock
//
// Description ; Reads data from the SSP
//
// Params : pwCommand - pointer to data to send
//          pwData - pointer to data buffer for read data
//          iDataLen - length of data in words
//
// Returns : Nothing
//
//******************************************************************************
void tSPIDD::vReadBlock( word *pwCommand, word *pwData, int iDataLen )
{
  int iSendCount = 0;
  int iRecvCount = 0;

  // assume less than one FIFO's worth of data will be sent
  PRECOND( iDataLen <= 8 );

  // wait for the SSP to be not busy
  while ( 0 != ( LPC_SSP1->SR & SSP_SR_BSY ) )
  {
  }

  // send the command and read the data
  while ( iSendCount < iDataLen )
  {
    // send the command
    LPC_SSP1->DR = *pwCommand;
    ++pwCommand;
    ++iSendCount;

    // if any data has come in, process it
    if ( 0 != ( LPC_SSP1->SR & SSP_SR_RNE ) )
    {
      *pwData = (word)LPC_SSP1->DR;
      ++pwData;
      ++iRecvCount;
    }
  }

  // wait for byte to exit the SSP
  while ( LPC_SSP1->SR & SSP_SR_BSY )
  {
  }

  // receive any leftover data
  while ( iRecvCount < iDataLen )
  {
    if ( 0 != ( LPC_SSP1->SR & SSP_SR_RNE ) )
    {
      *pwData = (word)LPC_SSP1->DR;
      ++pwData;
      ++iRecvCount;
    }
  }
}

/* -----------------------------------------------------------------------
   tSPIDD::vWriteByte
   .......................................................................
   Description  : Writes data out of the SSP byte at a time

   Params : pwData - pointer to data to write
            iDataLen - length of data in words

   Returns : Nothing
   ----------------------------------------------------------------------- */

void tSPIDD::vWriteByte( byte bData )
{
  LPC_SSP1->CR0  = SSP_CR0_8_BIT | SSP_COMMON;

  word wData = bData;
  vWriteBlock( &wData, 1 );

  LPC_SSP1->CR0  = SSP_CR0_16_BIT | SSP_COMMON;
}

