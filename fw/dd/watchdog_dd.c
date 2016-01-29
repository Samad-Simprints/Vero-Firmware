//******************************************************************************
//
// Project NIMBUS Application source file
//
// (c) Fen Technology Ltd. 2013. All rights reserved.
//
// All rights reserved. Copying, compilation, modification, distribution or
// any other use whatsoever of this material is strictly prohibited except in
// accordance with a Software License Agreement.
//
//******************************************************************************
//
// COMPONENT:    NIMBUS
// MODULE:       watchdog_dd.c
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

#include "global.h"

#include "watchdog_dd.h"

//******************************************************************************
// Constants
//******************************************************************************
#define WATCHDOG_MODE                 (0x03)

//******************************************************************************
// Definitions
//******************************************************************************

//******************************************************************************
// Private Function Prototypes
//******************************************************************************

//******************************************************************************
// Local Storage
//******************************************************************************

//******************************************************************************
//******************************************************************************
// Private Functions
//******************************************************************************
//******************************************************************************

/* -----------------------------------------------------------------------
   <FUNCTION NAME>
   .......................................................................
   Description  : <FUNCTION DESCRIPTION>

   Params : <PARAM1 NAME> - <PARAM1 DESCRIPTION>
            <PARAM2 NAME> - <PARAM2 DESCRIPTION>
            .....
            <PARAMn NAME> - <PARAMn DESCRIPTION>

   Returns : <DESCRIPTION OF RETURN PARAM>
   e.g.
   Returns : TRUE if succeeded. FALSE if failed.
   Returns : File Handle, -1 if failed to open
   Returns : Nothing
   Returns : int - Number of Hex data bytes written to pabOutputData
   ----------------------------------------------------------------------- */

//******************************************************************************
//******************************************************************************
// Public Functions
//******************************************************************************
//******************************************************************************

/* -----------------------------------------------------------------------
   vWDOGDDinit
   .......................................................................
   Description  : Initialise the watchdog

   Params : None

   Returns : Nothing
   ----------------------------------------------------------------------- */

void vWDOGDDinit(void)
{
#if !defined(CM_HOSTED)
  // initialise the watchdog timer
  // - select the internal RC clock to run it (4MHz) (locks the setting so it can't be changed now)
  LPC_WDT->WDCLKSEL = 0x80000000ul;
  // - time out after WATCHDOG_TIMEOUT_MS milliseconds
  LPC_WDT->WDTC = ( 4000ul * WATCHDOG_TIMEOUT_MS );
  // - configure the timer
  LPC_WDT->WDMOD = WATCHDOG_MODE;
#endif
  vWDOGDDkick();
}

/* -----------------------------------------------------------------------
   vWDOGDDkick
   .......................................................................
   Description  : Kick the watchdog to keep it awake

   Params : None

   Returns : Nothing
   ----------------------------------------------------------------------- */

void vWDOGDDkick(void)
{
#if !defined(CM_HOSTED)
  taskDISABLE_INTERRUPTS();
  LPC_WDT->WDFEED = 0x000000aa;
  LPC_WDT->WDFEED = 0x00000055;
  taskENABLE_INTERRUPTS();
#endif
}

/* -----------------------------------------------------------------------
   vWDOGDDreboot
   .......................................................................
   Description  : Reboot the unit, using the watchdog

   Params : None

   Returns : Doesn't return
   ----------------------------------------------------------------------- */

void vWDOGDDreboot(void)
{
  // we initialise the watchdog here, as we're about to go down anyway
  vWDOGDDinit();

#if !defined(CM_HOSTED)
  // feed the watchdog correctly to start it running if it isn't already
  LPC_WDT->WDFEED = 0x000000aa;
  LPC_WDT->WDFEED = 0x00000055;
  vTaskDelay( MS_TO_TICKS( 1 ) );

  // deliberately mis-feed the watchdog to cause a reset
  LPC_WDT->WDFEED = 0x000000aa;
  LPC_WDT->WDFEED = 0x000000aa;
#endif

  for( ;; )
  {
    // wait for the inevitable...
  }
}
