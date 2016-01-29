/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/
#ifndef __STATS_H__
#define __STATS_H__

#ifdef CONFIG_FREERTOS_STATS
    #ifdef BTAPP_ENABLE_STATS
        extern unsigned long ulRunTimeStatsClock;
        #define configGENERATE_RUN_TIME_STATS           1
        #define configUSE_STATS_FORMATTING_FUNCTIONS    1
        #define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() ulRunTimeStatsClock = 0
        #define portGET_RUN_TIME_COUNTER_VALUE() ulRunTimeStatsClock
    #else
        #define configGENERATE_RUN_TIME_STATS           0
        #define configUSE_STATS_FORMATTING_FUNCTIONS    0
    #endif
#else
    void stats_updateClockFromISR(portBASE_TYPE *higherPriorityTaskWoken);

    void stats_pal_init(void);
    void stats_pal_start(void);
#endif



#endif // __STATS_H__
