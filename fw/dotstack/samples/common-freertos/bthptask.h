/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __BTHPTASK_H_INCLUDED__
#define __BTHPTASK_H_INCLUDED__

#include "FreeRTOS.h"
#include "task.h"
#include "cdbt/bt/bt_std.h"

// Intializes high priority task. The task should have priority higher 
// than the BT task. The purpose of this task is to test the stack's
// behavior under heavy load.
void bthptask_init(void);

#endif // __BTHPTASK_H_INCLUDED__

