/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __CONFIG_H_INCLUDED__
#define __CONFIG_H_INCLUDED__

//------------------------------------------------------------------------------
// Configuration section.
// This section defines configuration settings. Change them to configure the
// system to your needs.
//------------------------------------------------------------------------------

#define BT_UART_INITIAL_RATE  115200

// Bluetooth UART baud rate.
#define BT_UART_WORKING_BAUD_RATE     (2000000)
//#define BT_UART_WORKING_BAUD_RATE     (115200)

// Log UART baud rate.
//#define LOG_UART_BAUD_RATE    (2 * 460800)
#define LOG_UART_BAUD_RATE    (115200)

//#define LOG_USE_USB

// BT controller type (select one from the list of supported controllers below)
//#define BT_CONTROLLER BT_CONTROLLER_CC2560
//#define BT_CONTROLLER BT_CONTROLLER_CC2564
//#define BT_CONTROLLER BT_CONTROLLER_CC2564B
//#define BT_CONTROLLER BT_CONTROLLER_CSR8811
//#define BT_CONTROLLER BT_CONTROLLER_CSR8811A06
#define BT_CONTROLLER BT_CONTROLLER_CSR8811A08

//------------------------------------------------------------------------------
// Supported options section.
// This sections defines possible options for the configuration settings defined
// in the configuration section above.
// Normally, these definitions should not be changed.
//------------------------------------------------------------------------------

// Supported Bluetooth controllers:
#include "btcontroller.h"

// After reset TI controllers are configured for 115200
#define BT_UART_INITIAL_BAUD_RATE  115200


////////////////////////////////////////////////////////////////////////

#endif // __CONFIG_H_INCLUDED__
