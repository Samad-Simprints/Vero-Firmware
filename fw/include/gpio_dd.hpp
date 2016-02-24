//******************************************************************************
//
// Project BLISS Application header file
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
// MODULE:       gpio_dd.hpp
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:
//
//******************************************************************************

#ifndef _GPIO_DD_H_
#define _GPIO_DD_H_

//******************************************************************************
// Includes
//******************************************************************************

#include "global.h"

//******************************************************************************
// Constants
//******************************************************************************

#define REG32                     volatile dword *

//
// bit patterns for alternate pin functions.
//
enum tPinFunction
{
  PINFUNC_GPIO = 0,
  PINFUNC_ALT1,
  PINFUNC_ALT2,
  PINFUNC_ALT3,
  PINFUNC_ALT4,
  PINFUNC_ALT5,
  PINFUNC_ALT6,
  PINFUNC_ALT7
};

//
// Offsets to the direction register and PIN register
//
#define FIO_SET_OFS               ( 0x00002200 )
#define FIO_CLR_OFS               ( 0x00002280 )
#define FIO_DIR_OFS               ( 0x00002000 )
#define FIO_PIN_OFS               ( 0x00002100 )

//
// Active high and low works by swapping the SET and CLR register offsets round
//
#define GPIO_ACTIVE_L             .dwClr = FIO_SET_OFS, .dwSet = FIO_CLR_OFS
#define GPIO_ACTIVE_H             .dwClr = FIO_CLR_OFS, .dwSet = FIO_SET_OFS
//#define GPIO_ACTIVE_L             FIO_SET_OFS, FIO_CLR_OFS
//#define GPIO_ACTIVE_H             FIO_CLR_OFS, FIO_SET_OFS

//
// Start addresses of GPIO blocks of registers
//
#define LPC_GPIO0_BASE            ( LPC_GPIO_PORT_BASE + 0x00000 )
#define LPC_GPIO1_BASE            ( LPC_GPIO_PORT_BASE + 0x00004 )
#define LPC_GPIO2_BASE            ( LPC_GPIO_PORT_BASE + 0x00008 )
#define LPC_GPIO3_BASE            ( LPC_GPIO_PORT_BASE + 0x0000C )
#define LPC_GPIO4_BASE            ( LPC_GPIO_PORT_BASE + 0x00010 )
#define LPC_GPIO5_BASE            ( LPC_GPIO_PORT_BASE + 0x00014 )
#define LPC_GPIO6_BASE            ( LPC_GPIO_PORT_BASE + 0x00018 )
#define LPC_GPIO7_BASE            ( LPC_GPIO_PORT_BASE + 0x0001C )

// Parameters for vGPIODDsetDirection, OR together for desired function
#define   PIN_IN                  ( 0x00 )
#define   PIN_OUT                 ( 0x01 )

#define   PIN_PULL_UP             ( 0x00 )
#define   PIN_REPEATER            ( 0x10 )
#define   PIN_NONE                ( 0x20 )
#define   PIN_PULL_DOWN           ( 0x30 )
#define   PIN_MODE_MASK           ( 0x30 )

#define   PIN_NO_GLITCH_FILTER    ( 0x40 )
#define   PIN_FAST_SLEW           ( 0x80 )

#define   PIN_DRIVE_NORMAL        ( 0x000 )
#define   PIN_DRIVE_MEDIUM        ( 0x100 )
#define   PIN_DRIVE_HIGH          ( 0x200 )
#define   PIN_DRIVE_ULTRA         ( 0x300 )
#define   PIN_DRIVE_MASK          ( 0x300 )

#define   PIN_SPECIAL             ( 0x800 ) // pin is not a GPIO. Just set function
#define   PIN_ANALOG              ( 0x1000 )

//******************************************************************************
// Definitions
//******************************************************************************

typedef enum
{
  ieNone,
  ieRising,
  ieFalling,
  ieBoth,
  ieLevelHigh,
  ieLevelLow
} tGPIOintEdge;

#define SCU_GPIO_MODE_MASK            ( 7ul << 0 )
#define SCU_GPIO_EPD                  ( 1ul << 3 )
#define SCU_GPIO_EPUN_DISABLE         ( 1ul << 4 )
#define SCU_GPIO_EHS                  ( 1ul << 5 )
#define SCU_GPIO_EZI                  ( 1ul << 6 )
#define SCU_GPIO_ZIF                  ( 1ul << 7 )
#define SCU_GPIO_EHD_MASK             ( 3ul << 8 )

typedef enum
{
  pmPullUp                    = 0ul,
  pmRepeater                  = SCU_GPIO_EPD,
  pmNone                      = (SCU_GPIO_EPUN_DISABLE ),
  pmPullDown                  = SCU_GPIO_EPD | SCU_GPIO_EPUN_DISABLE,
  // expand enum to 32 bits
  pmDummy                     = 0x7ffffffful
} tPullUpDown;

typedef enum
{
  ds4ma                       = ( 0ul << 8 ),
  ds8ma                       = ( 1ul << 8 ),
  ds14ma                      = ( 2ul << 8 ),
  ds20ma                      = ( 3ul << 8 ),
  // expand enum to 32 bits
  dsDummy                     = 0x7ffffffful
} tDriveStrength;

#ifdef __cplusplus
extern "C" {
#endif
  typedef void (* tGPIOintHandler)( const IRQn_Type eNVICchannel );
#ifdef __cplusplus
}
#endif

//
// Three versions of the same struct:
//    1) inside GPIODD (which is CPP) we declare all the methods,
//    2) outside GPIODD in 'cpp' modules, the methods are available
//    3) outside GPIODD in 'c' modules the struct has no members at all.
//
typedef struct tGPIOpinTag
{
#ifdef __cplusplus
public:
  // setup
  void vConfigure();
  void vSetPinFunction();
  void vSetDirection();
  void vSetPinInterruptHandler( const IRQn_Type eNVICchannel,
                                const tGPIOintEdge eEdge,
                                const tGPIOintHandler prHandler );
  void vSetMode( const tPullUpDown ePullUpDown,
                 const bool boGlitchFilter,
                 const bool boFastSlewRate,
                 const tDriveStrength eDriveStrength,
                 const bool boDisableInput );

  // accessors
  inline void vSetActive()
  {
    (*((REG32)(dwReg + dwSet))) = ( 1 << iBit );
  }

  inline void vSetInactive()
  {
    (*((REG32) (dwReg + dwClr))) = ( 1 << iBit );
  }

  inline void vSet( const bool boActive )
  {
    boActive ? vSetActive() : vSetInactive();
  }

  inline bool boGet()
  {
    dword dwPins = *((REG32) (dwReg + FIO_PIN_OFS));
    if ( dwSet > dwClr )
    {
      dwPins = ~dwPins;
    }
    return ((dwPins & ( 1 << iBit )) != 0 );
  }

  inline void vToggle()
  {
    vSet( !boGet() );
  }
private:
  REG32 pdwGetConfigRegister();
public:
#endif
  dword dwReg;
  dword dwClr;
  dword dwSet;
  int   iBit;
  dword dwFunction;
  word  wMode;
} tGPIOpin;

#ifdef _INSIDE_GPIODD_
  //
  // this version is for use inside the gpio module
  //
  #define PINFUNC_DEF( name, port, bit, act, func, mode ) \
    const tGPIOpin s##name = {                            \
      .dwReg = LPC_GPIO##port##_BASE,                     \
      GPIO_ACTIVE_##act,                                  \
      .iBit = bit,                                        \
      .dwFunction = (dword)PINFUNC_##func,                \
      .wMode = mode                                       \
    };                                                    \
    extern const tGPIOpin * const name = &s##name;
#else
  #define PINFUNC_DEF( name, port, bit, act, func, mode ) \
    extern tGPIOpin * const name;
#endif

//
// include the hardware pin allocations from another file
//
#include "gpio_pindefs.hpp"

//******************************************************************************
// Function Prototypes
//******************************************************************************

#ifdef __cplusplus
extern "C" {
#endif

extern void vGPIODDsetPinFunction( const tGPIOpin * const psPin );
extern void vGPIODDsetPinDirection( const tGPIOpin * const psPin,
                                    const bool boOutput );
extern void vGPIODDconfigurePin( const tGPIOpin * const psPin );
extern void vGPIODDsetActive( const tGPIOpin * const psPin );
extern void vGPIODDsetInactive( const tGPIOpin * const psPin );
extern bool boGPIODDgetPin( const tGPIOpin * const psPin );
extern void vGPIODDsetPinInterruptHandler( const tGPIOpin * const psPin,
                                           const IRQn_Type eNVICchannel,
                                           const tGPIOintEdge eEdge,
                                           const tGPIOintHandler prHandler );

#ifdef __cplusplus
}
#endif

#endif  // <HEADER_DEFINE>
