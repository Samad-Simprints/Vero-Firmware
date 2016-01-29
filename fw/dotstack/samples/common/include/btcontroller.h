/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __BTCONTROLLER_H_INCLUDED__
#define __BTCONTROLLER_H_INCLUDED__

// Supported Bluetooth controllers:
#define BT_CONTROLLER_CC2560           1     // used in PAN1315/PAN1325
#define BT_CONTROLLER_CC2564           2     // used in PAN1316/PAN1326
#define BT_CONTROLLER_BC6              3     // CSR BlueCore6
#define BT_CONTROLLER_CSR8810          4     // CSR 8810
#define BT_CONTROLLER_CSR8811          5     // CSR 8811
#define BT_CONTROLLER_RN42             6     // RN-42
#define BT_CONTROLLER_MARVELL_88W8790  7
#define BT_CONTROLLER_BC6_NAMSUNG      8     // NAMSUNG controller based on CSR BlueCore6
#define BT_CONTROLLER_CC2564B          9
#define BT_CONTROLLER_RDA              10
#define BT_CONTROLLER_CSR8811A06       11     // CSR 8811 A06
#define BT_CONTROLLER_CSR8811A08       12     // CSR 8811 A08
#define BT_CONTROLLER_BC6_BOSCH        13     // CSR BlueCore6
#define BT_CONTROLLER_TOSHIBA_TC35661  14
#define BT_CONTROLLER_MARVELL_88W8777  15
#define BT_CONTROLLER_IS1662S          16
#define BT_CONTROLLER_MARVELL_88W8897  17
#define BT_CONTROLLER_RTL8761ATL       18

// Supported host protocols for CSR's chips:
#define BT_UART_HOST_BC_PROTOCOL_BCSP  1
#define BT_UART_HOST_BC_PROTOCOL_H4    3
#define BT_UART_HOST_BC_PROTOCOL_H5    6

#endif // __BTCONTROLLER_H_INCLUDED__
