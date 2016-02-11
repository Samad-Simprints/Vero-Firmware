/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#include "board.h"
#include "btdisplay.h"

void btdisplay_onUpdate(const btdisplay_Data* data)
{
  if (data->modified & BTDISPLAY_FLAG_CONNECTED)
  {
    if (data->connected)
    {
      LED1_ON();
    }
    else
    {
      LED1_OFF();
    }
  }
}

void btdisplay_clear(void)
{
}
