//******************************************************************************
//
// Project NIMBUS source file
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
// MODULE:       ssp_dd.cpp
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:  see Header file for details
//
//******************************************************************************

//******************************************************************************
// Compilation switches
//******************************************************************************

// @TODO: fix irq mode for release build to work
//#define   IRQ_SPI           ( 1 )

//******************************************************************************
// Includes
//******************************************************************************

#include "ssp_dd.hpp"

#include "global.h"
#include "gpio_dd.hpp"

#include "task.h"

//******************************************************************************
// Private Function Prototypes
//******************************************************************************

//******************************************************************************
// Constants
//******************************************************************************

// SPI port related constants
#define SSP0_PRESCALE                 ( 4 )
#define SSP0_COMMON                   ( SSP0_CR0_SPI | SSP0_CR0_CPOL_H | SSP0_CR0_CPHA_H | SSP0_CR0_SCR )

#define SSP0_CR0_5_BIT                ( 4 << 0 )
#define SSP0_CR0_8_BIT                ( 7 << 0 )
#define SSP0_CR0_9_BIT                ( 8 << 0 )
#define SSP0_CR0_10_BIT               ( 9 << 0 )
#define SSP0_CR0_16_BIT               ( 15 << 0 )
#define SSP0_CR0_SPI                  ( 0 << 4 )
#define SSP0_CR0_CPOL_L               ( 0 << 6 )
#define SSP0_CR0_CPOL_H               ( 1 << 6 )
#define SSP0_CR0_CPHA_L               ( 0 << 7 )
#define SSP0_CR0_CPHA_H               ( 1 << 7 )
#define SSP0_CR0_SCR                  ( 1 << 8 )

#define SSP0_CR1_LM                   ( 1 << 0 )
#define SSP0_CR1_SSE                  ( 1 << 1 )

#define SSP0_SR_TFE                   ( 1 << 0 )
#define SSP0_SR_TNF                   ( 1 << 1 )
#define SSP0_SR_RNE                   ( 1 << 2 )
#define SSP0_SR_RFF                   ( 1 << 3 )
#define SSP0_SR_BSY                   ( 1 << 4 )

#define SSP0_RIS_RORRIS               ( 1 << 0 )
#define SSP0_RIS_RTRIS                ( 1 << 1 )
#define SSP0_RIS_RXRIS                ( 1 << 2 )
#define SSP0_RIS_TXRIS                ( 1 << 3 )

#define SSP0_ICR_RORIC                ( 1 << 0 )
#define SSP0_ICR_RTIC                 ( 1 << 1 )

#define NVIC_SSP0_SRC                 ( 14 )

//******************************************************************************
// Definitions
//******************************************************************************

typedef struct tSSPtransactionTag
{
  byte *pabCommand;
  int iCommandLen;
  byte *pabData;
  int iTxDataLen;
  int iRxDataLen;
  int iRxDataIgnore;
  bool boInput;
  bool boComplete;
} tSSPtransaction;

class tSSP : public ISSP
{
public:
  tSSP() : boRTOSRunning( true ) {}

  void                      vInit();
  void                      vReadBlock( byte * const pabCommand,
                                        const int iCommandLen,
                                        byte * const pabData,
                                        const int iDataLen );
  void                      vWriteBlock( byte * const pabCommand,
                                         const int iCommandLen,
                                         byte * const pabData,
                                         int iDataLen );
  void                      vSetRTOSrunning( bool boSetRTOSrunning ) { boRTOSRunning = boSetRTOSrunning; }
private:
  void                      vRunTransaction( byte * const pabCommand,
                                             const int iCommandLen,
                                             byte * const pabData,
                                             const int iTxDataLen,
                                             const int iRxDataLen,
                                             const int iDataIgnore );
  void                      vDelay( dword dwTickCount );

  bool                      boRTOSRunning;
};


//******************************************************************************
// Local Storage
//******************************************************************************

static volatile tSSPtransaction sTrans = {
  /*.pabCommand =*/ 0,
  /*.iCommandLen =*/ 0,
  /*.pabData =*/ 0,
  /*.iTxDataLen =*/ 0,
  /*.iRxDataLen =*/ 0,
  /*.iRxDataIgnore =*/ 0,
  /*.boInput =*/ false,
  /*.boComplete =*/ false
};

static tSSP oSSP;

//******************************************************************************
//******************************************************************************
// Private Functions
//******************************************************************************
//******************************************************************************

void tSSP::vDelay( dword dwTickCount )
{ 
  if ( boRTOSRunning )
  {
    vTaskDelay( dwTickCount );
  }
  else
  {
    while (dwTickCount-- > 0)
    {
      volatile int iDelay;
      for (iDelay = 0; iDelay < 10000; iDelay++);
    }
  }  
}

// function renamed so we do not override default unsupported interrupt handler
void xxxSSP0_IRQHandler( void )
{
  byte bIrqSrc = LPC_SSP0->MIS;

  if ( bIrqSrc & ( SSP0_RIS_RTRIS | SSP0_RIS_RXRIS ) )
  {
    // receive interrupt
    byte bData = LPC_SSP0->DR;

    // we should ignore any data coming in from sending the command. we also ignore
    // any data coming in from writing data to the flash.
    if ( sTrans.iRxDataIgnore )
    {
      --sTrans.iRxDataIgnore;
    }
    else
    {
      // only do something if an input transaction
      if ( sTrans.boInput )
      {
        if ( ( sTrans.pabData ) && ( sTrans.iRxDataLen > 0 ) )
        {
          // store data if we're still expecting any
          sTrans.pabData[0] = bData;
          ++sTrans.pabData;
          --sTrans.iRxDataLen;
          sTrans.boComplete = ( sTrans.iRxDataLen == 0 );
        }
        else
        {
          sTrans.boComplete = true;
        }
      }
    }
  }

  if ( bIrqSrc & SSP0_RIS_TXRIS )
  {
    // transmit interrupt
    if ( !sTrans.boComplete )
    {
      // write command bytes, if any left
      if ( ( sTrans.iCommandLen > 0 ) && ( sTrans.pabCommand ) )
      {
         LPC_SSP0->DR = *sTrans.pabCommand;
         ++sTrans.pabCommand;
         --sTrans.iCommandLen;
      }
      else
      {
        if ( !sTrans.boInput )
        {
          // output transaction, write any data we've got to the port
          if ( sTrans.iTxDataLen > 0 )
          {
            LPC_SSP0->DR = sTrans.pabData[0];
            ++sTrans.pabData;
            --sTrans.iTxDataLen;
          }
          else
          {
            // no more data to write
            sTrans.boComplete = true;
          }
        }
        else
        {
          // we're receiving, so we have to write dummy data out.
          if ( sTrans.iTxDataLen > 0 )
          {
            LPC_SSP0->DR = 0;
            --sTrans.iTxDataLen;
          }
          else
          {
            // complete, switch off transmit interrupts
            LPC_SSP0->IMSC &= ~SSP0_RIS_TXRIS;
          }
        }
      }
    }
    else
    {
      // complete, switch off transmit interrupts
      LPC_SSP0->IMSC &= ~SSP0_RIS_TXRIS;
    }
  }
}

void tSSP::vRunTransaction( byte * const pabCommand,
                            const int iCommandLen,
                            byte * const pabData,
                            const int iTxDataLen,
                            const int iRxDataLen,
                            const int iDataIgnore )
{
  sTrans.pabCommand = pabCommand;
  sTrans.iCommandLen = iCommandLen;
  sTrans.iRxDataIgnore = iDataIgnore;
  sTrans.pabData = pabData;
  sTrans.iTxDataLen = iTxDataLen;
  sTrans.iRxDataLen = iRxDataLen;
  sTrans.boComplete = false;

  NVIC_EnableIRQ( SSP0_IRQn );

  // activate CS
  vGPIODDsetActive( SSP_SSELn );

  LPC_SSP0->IMSC = ( SSP0_RIS_RTRIS | SSP0_RIS_RXRIS | SSP0_RIS_TXRIS );

  while ( ( !sTrans.boComplete ) || ( LPC_SSP0->SR & SSP0_SR_BSY ) )
  {
    vDelay( 1 );
  }

  // de-activate CS
  vGPIODDsetInactive( SSP_SSELn );

  NVIC_DisableIRQ( SSP0_IRQn );
}

//******************************************************************************
//******************************************************************************
// Public Functions
//******************************************************************************
//******************************************************************************

/* -----------------------------------------------------------------------
   tSSP::vInit
   .......................................................................
   Description  : Initialise the SPI device driver

   Params : None

   Returns : Nothing
   ----------------------------------------------------------------------- */

void tSSP::vInit( void )
{
#if !defined(CM_HOSTED)
  // enable power to SSP0
  LPC_SC->PCONP |= ( 1 << 21 );

  // select clk/1 for SSP0
  LPC_SC->PCLKSEL1 |= ( 1 << 10 );

  // configure pins
  vGPIODDconfigurePin( DATAFLASH_RSTn );
  vGPIODDsetActive( DATAFLASH_RSTn );
  vGPIODDconfigurePin( SSP_MOSI );
  vGPIODDconfigurePin( SSP_MISO );
  vGPIODDconfigurePin( SSP_SSELn );
  vGPIODDconfigurePin( SSP_SCK );

  LPC_SSP0->CPSR = SSP0_PRESCALE;
  LPC_SSP0->CR0 = SSP0_CR0_8_BIT | SSP0_COMMON;
  LPC_SSP0->CR1 = SSP0_CR1_SSE;

  vGPIODDsetInactive( DATAFLASH_RSTn );
#endif
}

/* -----------------------------------------------------------------------
   tSSP::vWriteBlock
   .......................................................................
   Description  : Writes data out of the SSP to the external serial
                  dataflash

   Params : pabCommand - pointer to command buffer
            iCommandLen - length of command in bytes
            pabData - pointer to data to write
            iDataLen - length of data in bytes

   Returns : Nothing
   ----------------------------------------------------------------------- */

void tSSP::vWriteBlock( byte *pabCommand, int iCommandLen, byte *pabData, int iDataLen )
{
#if defined(CM_HOSTED)
  extern void vWriteBlockHosted( byte *pabCommand, int iCommandLen, byte *pabData, int iDataLen );
  vWriteBlockHosted( pabCommand, iCommandLen, pabData, iDataLen );
#else
#ifdef IRQ_SPI
  sTrans.boInput = false;

  // kick things off
  vRunTransaction( pabCommand, iCommandLen, pabData, iDataLen, 0, iCommandLen + iDataLen );

#else
  // activate CS
  vGPIODDsetActive( SSP_SSELn );

  // send the command
  while ( iCommandLen > 0 )
  {
    while ( 0 == ( LPC_SSP0->SR & SSP0_SR_TNF ) )
    {
      // fifo is full - wait a short while, then try again
      vDelay( 0 );
    }

    // send the command byte
    LPC_SSP0->DR = *pabCommand;

    iCommandLen--;
    pabCommand++;
  }

  // send the data
  while ( iDataLen > 0 )
  {
    while ( 0 == ( LPC_SSP0->SR & SSP0_SR_TNF ) )
    {
      // fifo is full - wait a short while, then try again
      vDelay( 0 );
    }

    // send the data byte
    LPC_SSP0->DR = *pabData;

    iDataLen--;
    pabData++;
  }

  // wait for SSP to become idle (all data sent at serial bus speeds)
  while ( LPC_SSP0->SR & SSP0_SR_BSY )
  {
    vDelay( 0 );
  }

  // flush any data received as a result of sending the command/data
  while ( LPC_SSP0->SR & SSP0_SR_RNE )
  {
    (void)LPC_SSP0->DR;
  }

  // reset any error bits that may have resulted during transmission
  LPC_SSP0->ICR = ( SSP0_ICR_RORIC | SSP0_ICR_RTIC );

  // de-activate CS
  vGPIODDsetInactive( SSP_SSELn );
#endif
#endif
}

//******************************************************************************
// tSSP::vReadBlock
//
// Description ; Reads data from the external serial dataflash using SSP
//
// Params : pabCommand - pointer to command buffer
//          iCommandLen - length of command in bytes
//          pabData - pointer to data buffer for read data
//          iDataLen - length of data in bytes
//
// Returns : Nothing
//
//******************************************************************************
void tSSP::vReadBlock( byte *pabCommand, int iCommandLen, byte *pabData, int iDataLen )
{
#if defined(CM_HOSTED)
  extern void vReadBlockHosted(byte *pabCommand, int iCommandLen, byte *pabData, int iDataLen );
  vReadBlockHosted(pabCommand, iCommandLen, pabData, iDataLen );
#else
#ifdef IRQ_SPI
  sTrans.boInput = true;

  // kick things off
  vRunTransaction( pabCommand, iCommandLen, pabData, iDataLen, iDataLen, iCommandLen );

#else
  int iDataRecv = iDataLen;
  bool boReadResult = true;

  // activate CS
  vGPIODDsetActive( SSP_SSELn );

  // send the command bytes
  while ( iCommandLen > 0 )
  {
    if ( LPC_SSP0->SR & SSP0_SR_TNF )
    {
      // send a command byte
      LPC_SSP0->DR = *pabCommand;

      iCommandLen--;
      pabCommand++;
    }
    else
    {
      // fifo is full, wait for a byte to be serialised
      vDelay( 0 );
    }
  }

  // wait for SSP to become idle (all command bytes sent at serial bus speeds)
  while ( LPC_SSP0->SR & SSP0_SR_BSY )
  {
    vDelay( 0 );
  }

  // flush any data received as a result of sending the command
  while ( LPC_SSP0->SR & SSP0_SR_RNE )
  {
    (void)LPC_SSP0->DR;
  }

  // reset any error bits that may have resulted
  LPC_SSP0->ICR = ( SSP0_ICR_RORIC | SSP0_ICR_RTIC );

  int iTxFifoCount = 0;

  // now read the data from the device
  while ( iDataRecv > 0 )
  {
    // do we have a report of an overrun condition in the receive fifo.
    // This should never happen as we track the RX fifo free slots
    if ( LPC_SSP0->RIS & SSP0_RIS_RORRIS )
    {
      boReadResult = false;
      iDataRecv = 0;
      iDataLen = 0;
    }

    // if any data in the input fifo, transfer it to the buffer
    while ( (iDataRecv > 0) && ( LPC_SSP0->SR & SSP0_SR_RNE ) )
    {
      *pabData = LPC_SSP0->DR;

      pabData++;
      iDataRecv--;
      iTxFifoCount--;
    }

    // generate clocks to recover the data
    while ( (iDataLen > 0) && ( iTxFifoCount < 8 ) )
    {
      // write a data byte to generate clocks
      LPC_SSP0->DR = 0;

      iDataLen--;
      iTxFifoCount++;
    }

    // if no data is currently available then delay while a byte is being assembled
    if ( (iDataRecv > 0) && ((LPC_SSP0->SR & SSP0_SR_RNE) == 0) )
    {
      vDelay( 0 );
    }
  }

  // de-activate CS
  vGPIODDsetInactive( SSP_SSELn );
#endif
#endif
}



ISSP *poGetSSP()
{
  return &oSSP;
}

