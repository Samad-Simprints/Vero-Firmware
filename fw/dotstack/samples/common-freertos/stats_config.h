/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/
#ifndef __STATS_CONFIG_H__
#define __STATS_CONFIG_H__

extern unsigned long ulRunTimeStatsClock;
#define configGENERATE_RUN_TIME_STATS           1
#define configUSE_STATS_FORMATTING_FUNCTIONS    1
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() ulRunTimeStatsClock = 0
#define portGET_RUN_TIME_COUNTER_VALUE() ulRunTimeStatsClock

#endif // __STATS_CONFIG_H__
