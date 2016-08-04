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
#define WATCHDOG_DISABLED             (0x02)

//******************************************************************************
// Definitions
//******************************************************************************
// take account of WDT /4 prescaler on any value we want to use
#define WDT_PRESCALE(x)               ( (x) / 4ul )

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
  // initialise the watchdog timer
  // - uses the internal RC clock to run it (12MHz) (locks the setting so it can't be changed now)
  // - time out after maximum WD delay (about 5 seconds!)
  LPC_WWDT->TC = 0x00FFFFFF;
  // - configure the timer
  LPC_WWDT->MOD = WATCHDOG_MODE;
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
  taskDISABLE_INTERRUPTS();
  LPC_WWDT->FEED = 0x000000aa;
  LPC_WWDT->FEED = 0x00000055;
  taskENABLE_INTERRUPTS();
}

/* -----------------------------------------------------------------------
   vWDOGDDstop
   .......................................................................
   Description  : Stop the watchdog timer

   Params : None

   Returns : Doesn't return
   ----------------------------------------------------------------------- */

void vWDOGDDstop(void)
{
  LPC_WWDT->MOD = WATCHDOG_DISABLED;
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

  // feed the watchdog correctly to start it running if it isn't already
  LPC_WWDT->FEED = 0x000000aa;
  LPC_WWDT->FEED = 0x00000055;

  // now deliberately mis-feed the watchdog to cause a reset
  LPC_WWDT->FEED = 0x000000aa;
  LPC_WWDT->FEED = 0x000000aa;

  for( ;; )
  {
    // wait for the inevitable...
  }
}
