
#include <string.h>

#define   _INSIDE_GPIODD_             ( 1 )
#include "gpio_dd.hpp"

#include "global.h"

#include "FreeRTOS.h"

// eight handlers, for rising and falling edges or low or high level
static tGPIOintHandler aprIntHandlers[8];
static bool boIntHandlersInitialised = false;

REG32 tGPIOpin::pdwGetConfigRegister()
{
  REG32 pdwResult = 0;
  REG32 pdwPinSel = (REG32)(LPC_SCU_BASE);

  // The insanity that prevails in the NXP lpc18xx department means that we
  // have to convert the GPIO X[Y] to an offset into the SCU pin function registers
  // which can only be done through a lookup like this...
  // - each register is 32 bits wide, so we calculate the index of the register rather than the address of it.
  #define P(n,a,b)    ( 0x##a * 32 + b )
  #define NO_GPIO   ( 0xffff )
  switch ((dwReg - LPC_GPIO0_BASE) / 4)
  {
  case 0:   // GPIO0[n]
    {
      const word awOrder[] = {  P(0,0,0), P(1,0,1), P(2,1,15), P(3,1,16), P(4,1,0), P(5,6,6), P(6,3,6), P(7,2,7), P(8,1,1), P(9,1,2), P(10,1,3), P(11,1,4), P(12,1,17), P(13,1,18), P(14,2,10), P(15,1,20) };
      if ( ( iBit < sizeof( awOrder ) / sizeof( awOrder[ 0 ] ) ) && ( awOrder[ iBit ] != NO_GPIO ) )
      {
        pdwResult = &pdwPinSel[ awOrder[ iBit ] ];
      }
    }
    break;
  case 1:   // GPIO1[n]
    {
      const word awOrder[] = { P(0,1,7), P(1,1,8), P(2,1,9), P(3,1,10), P(4,1,11), P(5,1,12), P(6,1,13), P(7,1,14), P(8,1,5), P(9,1,6), P(10,2,9), P(11,2,11), P(12,2,12), P(13,2,13), P(14,3,4), P(15,3,5) };
      if ( ( iBit < sizeof( awOrder ) / sizeof( awOrder[ 0 ] ) ) && ( awOrder[ iBit ] != NO_GPIO ) )
      {
        pdwResult = &pdwPinSel[ awOrder[ iBit ] ];
      }
    }
    break;
  case 2:   // GPIO2[n]
    {
      const word awOrder[] = { P(0,4,0), P(1,4,1), P(2,4,2), P(3,4,3), P(4,4,4), P(5,4,5), P(6,4,6), P(7,5,7), P(8,6,12), P(9,5,0), P(10,5,1), P(11,5,2), P(12,5,3), P(13,5,4), P(14,5,5), P(15,5,6) };
      if ( ( iBit < sizeof( awOrder ) / sizeof( awOrder[ 0 ] ) ) && ( awOrder[ iBit ] != NO_GPIO ) )
      {
        pdwResult = &pdwPinSel[ awOrder[ iBit ] ];
      }
    }
    break;
  case 3:   // GPIO3[n]
    {
      const word awOrder[] = { P(0,6,1), P(1,6,2), P(2,6,3), P(3,6,4), P(4,6,5), P(5,6,9), P(6,6,10), P(7,6,11), P(8,7,0), P(9,7,1), P(10,7,2), P(11,7,3), P(12,7,4), P(13,7,5), P(14,7,6), P(15,7,7) };
      if ( ( iBit < sizeof( awOrder ) / sizeof( awOrder[ 0 ] ) ) && ( awOrder[ iBit ] != NO_GPIO ) )
      {
        pdwResult = &pdwPinSel[ awOrder[ iBit ] ];
      }
    }
    break;
  case 4:   // GPIO4[n]
    {
      const word awOrder[] = { P(0,8,0), P(1,8,1), P(2,8,2), P(3,8,3), P(4,8,4), P(5,8,5), P(6,8,6), P(7,8,7), P(8,A,1), P(9,A,2), P(10,A,3), P(11,9,6), P(12,9,0), P(13,9,1), P(14,9,2), P(15,9,3) };
      if ( ( iBit < sizeof( awOrder ) / sizeof( awOrder[ 0 ] ) ) && ( awOrder[ iBit ] != NO_GPIO ) )
      {
        pdwResult = &pdwPinSel[ awOrder[ iBit ] ];
      }
    }
    break;
  case 5:   // GPIO5[n]
    {
      const word awOrder[] = { P(0,2,0), P(1,2,1), P(2,2,2), P(3,2,3), P(4,2,4), P(5,2,5), P(6,2,6), P(7,2,8), P(8,3,1), P(9,3,2), P(10,3,7), P(11,3,8), P(12,4,8), P(13,4,9), P(14,4,10), P(15,6,7), P(16,6,8), P(17,9,4), P(18,9,5), P(19,A,4), P(20,B,0), P(21,B,1), P(22,B,2), P(23,B,3), P(24,B,4), P(25,B,5), P(26,B,6) };
      if ( ( iBit < sizeof( awOrder ) / sizeof( awOrder[ 0 ] ) ) && ( awOrder[ iBit ] != NO_GPIO ) )
      {
        pdwResult = &pdwPinSel[ awOrder[ iBit ] ];
      }
    }
    break;
  case 6:   // GPIO6[n]
    {
      const word awOrder[] = { P(0,C,1), P(1,C,2), P(2,C,3), P(3,C,4), P(4,C,5), P(5,C,6), P(6,C,7), P(7,C,8), P(8,C,9), P(9,C,10), P(10,C,11), P(11,C,12), P(12,C,13), P(13,C,14), P(14,D,0), P(15,D,1), P(16,D,2), P(17,D,3), P(18,D,4), P(19,D,5), P(20,D,6), P(21,D,7), P(22,D,8), P(23,D,9), P(24,D,10), P(25,D,11), P(26,D,12), P(27,D,13), P(28,D,14), P(29,D,15), P(30,D,16) };
      if ( ( iBit < sizeof( awOrder ) / sizeof( awOrder[ 0 ] ) ) && ( awOrder[ iBit ] != NO_GPIO ) )
      {
        pdwResult = &pdwPinSel[ awOrder[ iBit ] ];
      }
    }
    break;
  case 7:   // GPIO7[n]
    {
      const word awOrder[] = { P(0,E,0), P(1,E,1), P(2,E,2), P(3,E,3), P(4,E,4), P(5,E,5), P(6,E,6), P(7,E,7), P(8,E,8), P(9,E,9), P(10,E,10), P(11,E,11), P(12,E,12), P(13,E,13), P(14,E,14), P(15,E,15), P(16,F,1), P(17,F,2), P(18,F,3), P(19,F,5), P(20,F,6), P(21,F,7), P(22,F,8), P(23,F,9), P(24,F,10), P(25,F,11) };
      if ( ( iBit < sizeof( awOrder ) / sizeof( awOrder[ 0 ] ) ) && ( awOrder[ iBit ] != NO_GPIO ) )
      {
        pdwResult = &pdwPinSel[ awOrder[ iBit ] ];
      }
    }
    break;
  }

  return pdwResult;
}

void tGPIOpin::vConfigure()
{
  tPullUpDown eMode;
  tDriveStrength eStrength;

  vSetInactive();
  vSetPinFunction();
  vSetDirection();
  switch ( wMode & PIN_MODE_MASK )
  {
  case PIN_PULL_UP:
    eMode = pmPullUp;
    break;
  case PIN_REPEATER:
    eMode = pmRepeater;
    break;
  case PIN_NONE:
    eMode = pmNone;
    break;
  case PIN_PULL_DOWN:
    eMode = pmPullDown;
    break;
  default:
    eMode = pmNone;
    break;
  }

  switch ( wMode & PIN_DRIVE_MASK )
  {
  case PIN_DRIVE_NORMAL:
    eStrength = ds4ma;
    break;
  case PIN_DRIVE_MEDIUM:
    eStrength = ds8ma;
    break;
  case PIN_DRIVE_HIGH:
    eStrength = ds14ma;
    break;
  case PIN_DRIVE_ULTRA:
    eStrength = ds20ma;
    break;
  default:
    eStrength = ds4ma;
    break;
  }

  vSetMode( eMode,
            ( wMode & PIN_NO_GLITCH_FILTER ) == 0,
            ( wMode & PIN_FAST_SLEW ) != 0,
            eStrength,
            ( wMode & PIN_ANALOG ) != 0);
}

void tGPIOpin::vSetPinFunction()
{
  REG32 pdwPinSel = pdwGetConfigRegister();

  if ( pdwPinSel )
  {
    dword dwSetting = ( (*pdwPinSel) & ~SCU_GPIO_MODE_MASK ) | dwFunction;
    // always enable the input buffer
    dwSetting |= SCU_GPIO_EZI;

    *pdwPinSel = dwSetting;
  }
}

void tGPIOpin::vSetDirection()
{
  if( wMode & PIN_OUT )
  {
    (*((REG32) (dwReg + FIO_DIR_OFS))) |= ( 1 << iBit );
  }
  else
  {
    (*((REG32) (dwReg + FIO_DIR_OFS))) &= ~( 1 << iBit );
  }
}

void tGPIOpin::vSetPinInterruptHandler( const IRQn_Type eNVICchannel,
                                        const tGPIOintEdge eEdge,
                                        const tGPIOintHandler prHandler )
{
  REG32 pdwReg = 0;
  int iPort = 0;
  dword dwPattern = (1UL << iBit);

  PRECOND( ( eNVICchannel >= PIN_INT0_IRQn ) && ( eNVICchannel <= PIN_INT7_IRQn ) );

  if ( ( eNVICchannel >= PIN_INT0_IRQn ) && ( eNVICchannel <= PIN_INT7_IRQn ) )
  {
    int iChannel = eNVICchannel - PIN_INT0_IRQn;
    dword dwChannel = (1UL << iChannel);

    if ( !boIntHandlersInitialised )
    {
      // initialise
      memset( aprIntHandlers, 0, sizeof( aprIntHandlers ) );
      boIntHandlersInitialised = true;
    }

    // which port, 0..7
    // pending interrupts on edge sensitive pins are cleared
    iPort = ( dwReg - LPC_GPIO0_BASE ) / 4;
    switch ( eEdge )
    {
    case ieNone:
      // clear all
      LPC_GPIO_PIN_INT->ISEL &= ~dwChannel;
      LPC_GPIO_PIN_INT->CIENR = dwChannel;
      LPC_GPIO_PIN_INT->CIENF = dwChannel;
      break;
    case ieRising:
      LPC_GPIO_PIN_INT->ISEL &= ~dwChannel;
      LPC_GPIO_PIN_INT->SIENR = dwChannel;
      LPC_GPIO_PIN_INT->CIENF = dwChannel;
      LPC_GPIO_PIN_INT->IST = dwChannel;
      break;
    case ieFalling:
      LPC_GPIO_PIN_INT->ISEL &= ~dwChannel;
      LPC_GPIO_PIN_INT->SIENF = dwChannel;
      LPC_GPIO_PIN_INT->CIENR = dwChannel;
      LPC_GPIO_PIN_INT->IST = dwChannel;
      break;
    case ieBoth:
      LPC_GPIO_PIN_INT->ISEL &= ~dwChannel;
      LPC_GPIO_PIN_INT->SIENR = dwChannel;
      LPC_GPIO_PIN_INT->SIENF = dwChannel;
      LPC_GPIO_PIN_INT->IST = dwChannel;
      break;
    case ieLevelHigh:
      LPC_GPIO_PIN_INT->ISEL |= dwChannel;
      LPC_GPIO_PIN_INT->SIENR = dwChannel;
      LPC_GPIO_PIN_INT->CIENF = dwChannel;
      break;
    case ieLevelLow:
      LPC_GPIO_PIN_INT->ISEL |= dwChannel;
      LPC_GPIO_PIN_INT->SIENF = dwChannel;
      LPC_GPIO_PIN_INT->CIENR = dwChannel;
      break;
    }

    // select source for channel, in SCU
    switch ( iChannel )
    {
    case 0:
      LPC_SCU->PINTSEL0 &= ~( 0x000000fful );
      LPC_SCU->PINTSEL0 |= ( ( ( iPort << 5 ) | iBit ) << 0 );
      break;
    case 1:
      LPC_SCU->PINTSEL0 &= ~( 0x0000ff00ul );
      LPC_SCU->PINTSEL0 |= ( ( ( iPort << 5 ) | iBit ) << 8 );
      break;
    case 2:
      LPC_SCU->PINTSEL0 &= ~( 0x00ff0000ul );
      LPC_SCU->PINTSEL0 |= ( ( ( iPort << 5 ) | iBit ) << 16 );
      break;
    case 3:
      LPC_SCU->PINTSEL0 &= ~( 0xff000000ul );
      LPC_SCU->PINTSEL0 |= ( ( ( iPort << 5 ) | iBit ) << 24 );
      break;
    case 4:
      LPC_SCU->PINTSEL1 &= ~( 0x000000fful );
      LPC_SCU->PINTSEL1 |= ( ( ( iPort << 5 ) | iBit ) << 0 );
      break;
    case 5:
      LPC_SCU->PINTSEL1 &= ~( 0x0000ff00ul );
      LPC_SCU->PINTSEL1 |= ( ( ( iPort << 5 ) | iBit ) << 8 );
      break;
    case 6:
      LPC_SCU->PINTSEL1 &= ~( 0x00ff0000ul );
      LPC_SCU->PINTSEL1 |= ( ( ( iPort << 5 ) | iBit ) << 16 );
      break;
    case 7:
      LPC_SCU->PINTSEL1 &= ~( 0xff000000ul );
      LPC_SCU->PINTSEL1 |= ( ( ( iPort << 5 ) | iBit ) << 24 );
      break;
    }



    if ( eEdge != ieNone )
    {
      aprIntHandlers[ iChannel ] = prHandler;

      // enable or disable GPIO interrupts
      NVIC_SetPriority( eNVICchannel, GPIO_DD_INTERRUPT_PRIORITY );
      if ( prHandler )
      {
        NVIC_EnableIRQ( eNVICchannel );
      }
      else
      {
        NVIC_DisableIRQ( eNVICchannel );
      }
    }
    else
    {
      aprIntHandlers[ iChannel ] = 0;
      NVIC_DisableIRQ( eNVICchannel );
    }
  }
}

void tGPIOpin::vSetMode( const tPullUpDown ePullUpDown,
                         const bool boGlitchFilter,
                         const bool boFastSlewRate,
                         const tDriveStrength eDriveStrength,
                         const bool boDisableInput)
{
  REG32 pdwPinSel = pdwGetConfigRegister();

  if ( pdwPinSel )
  {
    dword dwSetting = (((*pdwPinSel) & ~( SCU_GPIO_EPD | SCU_GPIO_EHS | SCU_GPIO_ZIF | SCU_GPIO_EZI | SCU_GPIO_EHD_MASK )) | SCU_GPIO_EPUN_DISABLE) ;

    if ( !boDisableInput )
    {
      // enable the input buffer if its not disabled
      dwSetting |= SCU_GPIO_EZI;
    }
    // tPullUpDown is coded as the actual bits to set
    dwSetting |= (dword)ePullUpDown;

    dwSetting |= ( boGlitchFilter ? 0 : SCU_GPIO_ZIF );
    dwSetting |= ( boFastSlewRate ? SCU_GPIO_EHS : 0 );

    // tDriveStrength is coded as the actual bits to set
    dwSetting |= (dword)eDriveStrength;

    *pdwPinSel = dwSetting;
  }
}

/* --------------------------------------------------------
   vGPIODDsetPinFunction
   ........................................................
   Description  : Set the pin connect block from a pin
                  definition

   Params : psPin - pin definition

   Returns : Nothing
   ----------------------------------------------------- */

void vGPIODDsetPinFunction( const tGPIOpin * const psPin )
{
  if ( psPin )
  {
    ((tGPIOpin *)psPin)->vSetPinFunction();
  }
}

/* --------------------------------------------------------
   vGPIODDsetPinDirection
   ........................................................
   Description  : Sets the pin direction from a pin
                  definition

   Params : psPin - pin definition
            boOutput - set to TRUE to turn the pin into an
                       output

   Returns : Nothing
   ----------------------------------------------------- */

void vGPIODDsetPinDirection( const tGPIOpin * const psPin,
                             const bool boOutput )
{
  if ( psPin )
  {
    ((tGPIOpin *)psPin)->vSetDirection();
  }
}

/* --------------------------------------------------------
   vGPIODDconfigurePin
   ........................................................
   Description  : Combination function to configure and set
                  direction of GPIO pin

   Params : psPin - pin definition
            boOutput - set to TRUE to turn the pin into an
                       output

   Returns : Nothing
   ----------------------------------------------------- */

void vGPIODDconfigurePin( const tGPIOpin * const psPin )
{
  if ( psPin )
  {
    ((tGPIOpin *)psPin)->vConfigure();
  }
}

/* --------------------------------------------------------
   vGPIODDsetActive
   ........................................................
   Description  : Sets a pin to its active state

   Params : psPin - pin definition

   Returns : Nothing
   ----------------------------------------------------- */

void vGPIODDsetActive( const tGPIOpin * const psPin )
{
  if ( psPin )
  {
    ((tGPIOpin *)psPin)->vSetActive();
  }
}

/* --------------------------------------------------------
   vGPIODDsetInactive
   ........................................................
   Description  : Sets a pin to its inactive state

   Params : psPin - pin definition

   Returns : Nothing
   ----------------------------------------------------- */

void vGPIODDsetInactive( const tGPIOpin * const psPin )
{
  if ( psPin )
  {
    ((tGPIOpin *)psPin)->vSetInactive();
  }
}

/* --------------------------------------------------------
   boGPIODDgetPin
   ........................................................
   Description  : Gets the current state of a pin

   Params : psPin - pin definition

   Returns : TRUE if the pin is in its active state,
             FALSE otherwise.
   ----------------------------------------------------- */

bool boGPIODDgetPin( const tGPIOpin * const psPin )
{
  if ( psPin )
  {
    return ((tGPIOpin *)psPin)->boGet();
  }
  else
  {
    return false;
  }
}

/* --------------------------------------------------------
   vGPIODDsetPinInterruptHandler
   ........................................................
   Description  : Subscribe to a pin interrupt

   Params : psPin - pin definition
            eEdge - edge to subscribe to
            prHandler - handler (can be NULL)

   Returns : Nothing
   ----------------------------------------------------- */

void vGPIODDsetPinInterruptHandler( const tGPIOpin * const psPin,
                                    const IRQn_Type eNVICchannel,
                                    const tGPIOintEdge eEdge,
                                    const tGPIOintHandler prHandler )
{
  if ( psPin )
  {
    ((tGPIOpin *)psPin)->vSetPinInterruptHandler( eNVICchannel, eEdge, prHandler );
  }
}

/* --------------------------------------------------------
   EINT3_IRQHandler
   ........................................................
   Description  : Pin interrupt dispatcher

   Params : None

   Returns : Nothing
   ----------------------------------------------------- */

static void vISR( const IRQn_Type eNVICchannel )
{
  int iChannel = eNVICchannel - PIN_INT0_IRQn;
  // which interrupt fired?
  dword dwIntStat = LPC_GPIO_PIN_INT->IST;

  if ( aprIntHandlers[ iChannel ] )
  {
    aprIntHandlers[ iChannel ]( eNVICchannel );
  }

  // clear the interrupt, if edge triggered
  if ( ~LPC_GPIO_PIN_INT->ISEL & dwIntStat )
  {
    LPC_GPIO_PIN_INT->IST = ( 1ul << iChannel );
  }
}

extern "C" void PIN_INT0_IRQHandler( void )
{
  vISR( PIN_INT0_IRQn );
}

extern "C" void PIN_INT1_IRQHandler( void )
{
  vISR( PIN_INT1_IRQn );
}

extern "C" void PIN_INT2_IRQHandler( void )
{
  vISR( PIN_INT2_IRQn );
}

extern "C" void PIN_INT3_IRQHandler( void )
{
  vISR( PIN_INT3_IRQn );
}

extern "C" void PIN_INT4_IRQHandler( void )
{
  vISR( PIN_INT4_IRQn );
}

extern "C" void PIN_INT5_IRQHandler( void )
{
  vISR( PIN_INT5_IRQn );
}

extern "C" void PIN_INT6_IRQHandler( void )
{
  vISR( PIN_INT6_IRQn );
}

extern "C" void PIN_INT7_IRQHandler( void )
{
  vISR( PIN_INT7_IRQn );
}
