/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __BTAPP_CONFIG_H_INCLUDED__
#define __BTAPP_CONFIG_H_INCLUDED__

// Uncomment the following line to configure
// the SPP sample for "fast" speed.
#define FAST_CONFIG

// Uncomment the following line to prevent TI C256x controllers
// from going to deep sleep.
// Does not have effect on CSR controller.
//#define DISABLE_DEEP_SLEEP

// Uncomment the following line to enable BT task CPU usage
// output. The statistics is output to the log, so ENABLE_LOG
// must also be defined in the project's preprocessor settings.
// This functionality is currently available only for dsPIC33E
// and STM32F4 ports.
//#define BTAPP_ENABLE_STATS

#endif // __BTAPP_CONFIG_H_INCLUDED__
