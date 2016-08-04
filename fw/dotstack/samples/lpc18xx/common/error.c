/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#include "board.h"

#include "FreeRTOS.h"
#include "task.h"
#include "error.h"
#include "btapp.h"


static void DELAY(int ms)
{
	int count = MILLIS_TO_TICKS(ms);
	
	if (count == 0) count = 1;
	
	// COUNTFLAG bit is set to 1 on each wrapping of SysTick counter.
	// Reading COUNTFLAG clears it.
	for (; count; count--)
	{
		while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0) {}
	}
}

extern void vBTFatalError(void);

void error_onFatalError(void)
{
  vBTFatalError();
#if 0
  taskDISABLE_INTERRUPTS();
  
  LED1_ON();
  LED2_OFF();
  
  for (;;)
  {
    DELAY(60);
    LED1_TOGGLE();
    LED2_TOGGLE();
  }
#endif
}


void btapp_onFatalError(void)
{
	error_onFatalError();
}


void bt_oem_assert(const char* file, int line)
{
	error_onFatalError();
}
