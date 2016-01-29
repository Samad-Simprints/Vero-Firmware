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
// MODULE:       emc_dd.c
// $Date$
// $Revision$
// $Author$
// DESCRIPTION:  Device driver for NXP's LPC External Memory Controller
//
//******************************************************************************

//******************************************************************************
// Includes
//******************************************************************************

#include <stdlib.h>

#include "emc_dd.hpp"

#include "global.h"

#include "gpio_dd.hpp"

//******************************************************************************
// Constants
//******************************************************************************

// register definition
// control register
#define EMC_ENABLE                  ( 1 << 0 )
#define ADDR_MIRROR_RESET           ( 1 << 1 )
#define LOW_PWR_MODE                ( 1 << 2 )

// configuration register
#define BIG_ENDIAN                  ( 1 << 0 )

// Static memory configuration register
#define MEMORY_BYTE_WIDTH(n)        ( n >> 1 )
#define PAGE_MODE                   ( 1 << 3 )
#define CHIP_SEL_POL_ACTIVE_HIGH    ( 1 << 6 )
#define BYTE_LANE_STATE_READ_LOW    ( 1 << 7 )
#define EXTENDED_WAIT_EN            ( 1 << 8 )
#define BUFFER_EN                   ( 1 << 19 )
#define WRITE_PROTECT               ( 1 << 20 )

// static memory
#define STATIC_MEM_CS_MAX_SIZE      ( 0x00FFFFFF ) // maximum address per chip select (16MB)

// dynamic memory
#define DYNAMIC_MEM_DYCS0_MAX_SIZE  ( 0x08FFFFFF ) // (128MB)
#define DYNAMIC_MEM_DYCX_MAX_SIZE   ( 0x0FFFFFFF ) // (256MB)

//@TODO: number of chip selects depends on device package
#define NUM_OF_STATIC_CHIP_SELECTS  ( 4 )

//******************************************************************************
// Function Prototypes
//******************************************************************************

//******************************************************************************
// Definitions
//******************************************************************************

class tEMCDD : public IEMCDD
{
public:
                            tEMCDD();
  virtual                  ~tEMCDD();

  void                      vInit();
};

//******************************************************************************
// Local Storage
//******************************************************************************

static tEMCDD oEMCDD;

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
   poGetEMCDD
   .......................................................................
   Description  : Gives access to EMCDD object from other modules

   Params : None

   Returns : IEMCDD * pointer to EMCDD object
   ----------------------------------------------------------------------- */

IEMCDD *poGetEMCDD()
{
  return &oEMCDD;
}

/* -----------------------------------------------------------------------
   tEMCDD::tEMCDD
   .......................................................................
   Description  : Constructor

   Params : None

   Returns : Nothing
   ----------------------------------------------------------------------- */

tEMCDD::tEMCDD()
{
}

/* -----------------------------------------------------------------------
   tEMCDD::~tEMCDD
   .......................................................................
   Description  : Destructor

   Params : None

   Returns : Nothing
   ----------------------------------------------------------------------- */

tEMCDD::~tEMCDD()
{
}

/* -----------------------------------------------------------------------
   tEMCDD::vInit
   .......................................................................
   Description  : Initialise the EMC DD

   Params : None

   Returns : Nothing
   ----------------------------------------------------------------------- */

void tEMCDD::vInit()
{
  // configure pins
  // optional chip select
  vGPIODDconfigurePin( EMC_nCS0 );

  // standard interface
  vGPIODDconfigurePin( EMC_nOE );
  vGPIODDconfigurePin( EMC_nWEN );
  vGPIODDconfigurePin( EMC_nUB );
  vGPIODDconfigurePin( EMC_nLB );
  vGPIODDconfigurePin( EMC_A0 );
  vGPIODDconfigurePin( EMC_A1 );
  vGPIODDconfigurePin( EMC_A2 );
  vGPIODDconfigurePin( EMC_A3 );
  vGPIODDconfigurePin( EMC_A4 );
  vGPIODDconfigurePin( EMC_A5 );
  vGPIODDconfigurePin( EMC_A6 );
  vGPIODDconfigurePin( EMC_A7 );
  vGPIODDconfigurePin( EMC_A8 );
  vGPIODDconfigurePin( EMC_A9 );
  vGPIODDconfigurePin( EMC_A10 );
  vGPIODDconfigurePin( EMC_A11 );
  vGPIODDconfigurePin( EMC_A12 );
  vGPIODDconfigurePin( EMC_A13 );
  vGPIODDconfigurePin( EMC_A14 );
  vGPIODDconfigurePin( EMC_A15 );
  vGPIODDconfigurePin( EMC_A16 );
  vGPIODDconfigurePin( EMC_A17 );
  vGPIODDconfigurePin( EMC_A18 );
  vGPIODDconfigurePin( EMC_D0 );
  vGPIODDconfigurePin( EMC_D1 );
  vGPIODDconfigurePin( EMC_D2 );
  vGPIODDconfigurePin( EMC_D3 );
  vGPIODDconfigurePin( EMC_D4 );
  vGPIODDconfigurePin( EMC_D5 );
  vGPIODDconfigurePin( EMC_D6 );
  vGPIODDconfigurePin( EMC_D7 );
  vGPIODDconfigurePin( EMC_D8 );
  vGPIODDconfigurePin( EMC_D9 );
  vGPIODDconfigurePin( EMC_D10 );
  vGPIODDconfigurePin( EMC_D11 );
  vGPIODDconfigurePin( EMC_D12 );
  vGPIODDconfigurePin( EMC_D13 );
  vGPIODDconfigurePin( EMC_D14 );
  vGPIODDconfigurePin( EMC_D15 );

  // disable the EMC, and set it to little-endian mode
  LPC_EMC->CONTROL = 0;
  LPC_EMC->CONFIG = 0;
  

#if DIVIDE_EMC_CLK_BY_TWO
  LPC_CCU1->CLK_M3_EMCDIV_CFG = ( (1 << 5) | (1 << 0) );
  LPC_CREG->CREG6 |= CREG_CREG6_EMC_CLK_SEL_Msk;       // divide EMC_CLK by 2
#else
  LPC_CCU1->CLK_M3_EMCDIV_CFG = ( (0 << 5) | (1 << 0) );
  LPC_CREG->CREG6 &= ~CREG_CREG6_EMC_CLK_SEL_Msk;     // do not divide EMC_CLK by 2
#endif

  // Chip select 1 - async SRAM mode
  LPC_EMC->STATICCONFIG1    = ( BYTE_LANE_STATE_READ_LOW | MEMORY_BYTE_WIDTH(4) );   // 16 bit data bus
  LPC_EMC->STATICWAITWEN1   = 3;
  LPC_EMC->STATICWAITOEN1   = 2;
  LPC_EMC->STATICWAITRD1    = 12;     // we need 3-4 clock cycles @ FPGA clock rate
  LPC_EMC->STATICWAITPAG1   = 31;
  LPC_EMC->STATICWAITWR1    = 10;
  LPC_EMC->STATICWAITTURN1  = 8;

  // finally enable the EMC
  LPC_EMC->CONTROL = EMC_ENABLE;
}
