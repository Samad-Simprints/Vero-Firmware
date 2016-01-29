
#include <string.h>

#define   _INSIDE_GPIODD_             ( 1 )
#include "gpio_dd.hpp"

#include "global.h"

#include "freertos.h"

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
  #define P(a,b)    ( 0x##a * 32 + b )
  #define NO_GPIO   ( 0xffff )
  switch ((dwReg - LPC_GPIO0_BASE) / 4)
  {
  case 0:   // GPIO0[n]
    {
      const word awOrder[] = {  P(0,0), P(0,1), P(1,15), P(1,16), P(1,0), P(6,6), P(3,6), P(2,7), P(1,1), P(1,2), P(1,3), P(1,4), P(1,17), P(1,18), P(2,10), P(1,20) };
      if ( ( iBit < sizeof( awOrder ) / sizeof( awOrder[ 0 ] ) ) && ( awOrder[ iBit ] != NO_GPIO ) )
      {
        pdwResult = &pdwPinSel[ awOrder[ iBit ] ];
      }
    }
    break;
  case 1:   // GPIO1[n]
    {
      const word awOrder[] = { P(1,7), P(1,8), P(1,9), P(1,10), P(1,11), P(1,12), P(1,13), P(1,14), P(1,5), P(1,6), P(2,9), P(2,11), P(2,12), P(2,13), P(3,4), P(3,5) };
      if ( ( iBit < sizeof( awOrder ) / sizeof( awOrder[ 0 ] ) ) && ( awOrder[ iBit ] != NO_GPIO ) )
      {
        pdwResult = &pdwPinSel[ awOrder[ iBit ] ];
      }
    }
    break;
  case 2:   // GPIO2[n]
    {
      const word awOrder[] = { P(4,0), P(4,1), P(4,2), P(4,3), P(4,4), P(4,5), P(4,6), P(5,7), P(6,12), P(5,0), P(5,1), P(5,2), P(5,3), P(5,4), P(5,5), P(5,6) };
      if ( ( iBit < sizeof( awOrder ) / sizeof( awOrder[ 0 ] ) ) && ( awOrder[ iBit ] != NO_GPIO ) )
      {
        pdwResult = &pdwPinSel[ awOrder[ iBit ] ];
      }
    }
    break;
  case 3:   // GPIO3[n]
    {
      const word awOrder[] = { P(6,1), P(6,2), P(6,3), P(6,4), P(6,5), P(6,9), P(6,10), P(6,11), P(7,0), P(7,1), P(7,2), P(7,3), P(7,4), P(7,5), P(7,6), P(7,7) };
      if ( ( iBit < sizeof( awOrder ) / sizeof( awOrder[ 0 ] ) ) && ( awOrder[ iBit ] != NO_GPIO ) )
      {
        pdwResult = &pdwPinSel[ awOrder[ iBit ] ];
      }
    }
    break;
  case 4:   // GPIO4[n]
    {
      const word awOrder[] = { P(8,0), P(8,1), P(8,2), P(8,3), P(8,4), P(8,5), P(8,6), P(8,7), NO_GPIO, P(A,1), P(A,2), P(A,3), P(9,6), P(9,0), P(9,1), P(9,2), P(9,3) };
      if ( ( iBit < sizeof( awOrder ) / sizeof( awOrder[ 0 ] ) ) && ( awOrder[ iBit ] != NO_GPIO ) )
      {
        pdwResult = &pdwPinSel[ awOrder[ iBit ] ];
      }
    }
    break;
  case 5:   // GPIO5[n]
    {
      const word awOrder[] = { P(2,0), P(2,1), P(2,2), P(2,3), P(2,4), P(2,5), P(2,6), P(2,8), P(3,1), P(3,2), P(3,7), P(3,8), P(4,8), P(4,9), P(4,10), P(6,7), P(6,8), P(9,4), P(9,5), P(A,4), P(B,0), P(B,1), P(B,2), P(B,3), P(B,4), P(B,5), P(B,6) };
      if ( ( iBit < sizeof( awOrder ) / sizeof( awOrder[ 0 ] ) ) && ( awOrder[ iBit ] != NO_GPIO ) )
      {
        pdwResult = &pdwPinSel[ awOrder[ iBit ] ];
      }
    }
    break;
  case 6:   // GPIO6[n]
    {
      const word awOrder[] = { P(C,1), P(C,2), P(C,3), P(C,4), P(C,5), P(C,6), P(C,7), P(C,8), P(C,9), P(C,10), P(C,11), P(C,12), P(C,13), P(C,14), P(D,0), P(D,1), P(D,2), P(D,3), P(D,4), P(D,5), P(D,6), P(D,7), P(D,8), P(D,9), P(D,10), P(D,11), P(D,12), P(D,13), P(D,14), P(D,15), P(D,16) };
      if ( ( iBit < sizeof( awOrder ) / sizeof( awOrder[ 0 ] ) ) && ( awOrder[ iBit ] != NO_GPIO ) )
      {
        pdwResult = &pdwPinSel[ awOrder[ iBit ] ];
      }
    }
    break;
  case 7:   // GPIO7[n]
    {
      const word awOrder[] = { P(E,0), P(E,1), P(E,2), P(E,3), P(E,4), P(E,5), P(E,6), P(E,7), P(E,8), P(E,9), P(E,10), P(E,11), P(E,12), P(E,13), P(E,14), P(E,15), P(F,1), P(F,2), P(F,3), P(F,5), P(F,6), P(F,7), P(F,8), P(F,9), P(F,10), P(F,11) };
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
            eStrength );
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
      NVIC_SetPriority( eNVICchannel, GPIO_INTERRUPT_PRIORITY );
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
                         const tDriveStrength eDriveStrength )
{
  REG32 pdwPinSel = pdwGetConfigRegister();

  if ( pdwPinSel )
  {
    dword dwSetting = (((*pdwPinSel) & ~( SCU_GPIO_EPD | SCU_GPIO_EHS | SCU_GPIO_ZIF | SCU_GPIO_EHD_MASK )) | SCU_GPIO_EPUN_DISABLE) ;
    // always enable the input buffer
    dwSetting |= SCU_GPIO_EZI;

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
