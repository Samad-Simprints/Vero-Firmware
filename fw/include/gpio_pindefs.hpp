
#ifndef _GPIO_PINDEFS_H_
#define _GPIO_PINDEFS_H_

#include "gpio_dd.hpp"

#ifdef __cplusplus
extern "C" {
#endif
//
// UN20B control
//
// UN20 USB pins not defined here
PINFUNC_DEF( UN20B_TXD,               4,  8, L, ALT3, PIN_OUT )               // pin A_1
PINFUNC_DEF( UN20B_RXD,               4,  9, L, ALT3, PIN_IN )                // pin A_2
PINFUNC_DEF( UN20B_nRESET,            2,  1, L, GPIO, PIN_OUT )               // pin 4_1
PINFUNC_DEF( UN20B_POWER,             2,  0, H, GPIO, PIN_OUT )               // pin 4_0
PINFUNC_DEF( USB_VBUS_PWR_EN,         5, 18, H, ALT4, PIN_OUT )               // pin 9_5
//
// Phone USB pins not defined here
PINFUNC_DEF( USB_HOST_PWR_EN,         3,  2, H, GPIO, PIN_OUT )               // pin 6_3
PINFUNC_DEF( USB_HOST_nCON,           4,  2, L, GPIO, PIN_OUT )               // pin 8_2
PINFUNC_DEF( USB_HOST_ILIM0,          4,  3, H, GPIO, PIN_OUT )               // pin 8_3
PINFUNC_DEF( USB_HOST_ILIM1,          4,  4, H, GPIO, PIN_OUT )               // pin 8_4

//
// BT Uart
//
PINFUNC_DEF( BT_TXD,                  1, 14, H, ALT4, PIN_OUT )               // pin 3_4
PINFUNC_DEF( BT_RXD,                  1, 15, H, ALT4, PIN_IN | PIN_PULL_UP )  // pin 3_5
PINFUNC_DEF( BT_CTS,                  7,  7, H, ALT2, PIN_IN | PIN_PULL_UP )  // pin E_7
PINFUNC_DEF( BT_RTS,                  7,  5, H, ALT2, PIN_OUT )               // pin E_5
PINFUNC_DEF( BT_RESET,                2,  3, H, GPIO, PIN_OUT )               // pin 4_3
PINFUNC_DEF( BT_SPARE2,               2,  4, H, GPIO, PIN_IN )                // pin 4_4
//
// Battery monitors
//
//PINFUNC_DEF( BAT_MON_0,               2,  4, H, GPIO, PIN_IN )                // Not GPIO
//PINFUNC_DEF( BAT_MON_1,               2,  4, H, GPIO, PIN_IN )                // Not GPIO
//
// LEDs
//
// Smile ring
PINFUNC_DEF( LED_0_GREEN,             6, 14, L, ALT4, PIN_OUT )                 // pin D_0 - D3
PINFUNC_DEF( LED_0_RED,               7, 16, L, ALT4, PIN_OUT )                 // pin F_1
PINFUNC_DEF( LED_1_GREEN,             6, 15, L, ALT4, PIN_OUT )                 // pin D_1 - D4
PINFUNC_DEF( LED_1_RED,               7, 17, L, ALT4, PIN_OUT )                 // pin F_2
PINFUNC_DEF( LED_2_GREEN,             6, 16, L, ALT4, PIN_OUT )                 // pin D_2 - D5
PINFUNC_DEF( LED_2_RED,               7, 18, L, ALT4, PIN_OUT )                 // pin F_3
PINFUNC_DEF( LED_3_GREEN,             6, 17, L, ALT4, PIN_OUT )                 // pin D_3 - D6
PINFUNC_DEF( LED_3_RED,               7, 19, L, ALT4, PIN_OUT )                 // pin F_5
PINFUNC_DEF( LED_4_GREEN,             6, 18, L, ALT4, PIN_OUT )                 // pin D_4 - D7
PINFUNC_DEF( LED_4_RED,               7, 20, L, ALT4, PIN_OUT )                 // pin F_6
PINFUNC_DEF( LED_5_GREEN,             6, 19, L, ALT4, PIN_OUT )                 // pin D_5 - D9
PINFUNC_DEF( LED_5_RED,               7, 21, L, ALT4, PIN_OUT )                 // pin F_7
PINFUNC_DEF( LED_6_GREEN,             6, 20, L, ALT4, PIN_OUT )                 // pin D_6 - D11
PINFUNC_DEF( LED_6_RED,               7, 22, L, ALT4, PIN_OUT )                 // pin F_8
// Scan good indication
PINFUNC_DEF( LED_SCAN_GOOD_GREEN,     6, 21, L, ALT4, PIN_OUT )                 // pin D_7 - D8
PINFUNC_DEF( LED_SCAN_GOOD_RED,       7, 23, L, ALT4, PIN_OUT )                 // pin F_9
// Scan bad indication
PINFUNC_DEF( LED_SCAN_BAD_GREEN,      6, 22, L, ALT4, PIN_OUT )                 // pin D_8 - D10
PINFUNC_DEF( LED_SCAN_BAD_RED,        7, 24, L, ALT4, PIN_OUT )                 // pin F_10
// Status Leds
PINFUNC_DEF( LED_CONNECTION,          6, 24, L, ALT4, PIN_OUT )                 // pin D_10 - D1
PINFUNC_DEF( LED_BATTERY_RED,         6, 25, L, ALT4, PIN_OUT )                 // pin D_11 - D2
//
// Vibrate motor
//
PINFUNC_DEF( VIBRATE_POWER,           6, 26, L, ALT4, PIN_OUT )                 // pin D_12
//
// Buttons
//
PINFUNC_DEF( BUTTON_0_POWER,           2,  5, H, GPIO, PIN_IN | PIN_NONE )      // pin 4_5 (rising edge)
PINFUNC_DEF( BUTTON_1_SCAN,            6, 27, H, ALT4, PIN_IN | PIN_NONE )      // pin D_13 (falling edge)
//
// SSP port for Flash
//
PINFUNC_DEF( FLASH_CLK,               3,  3, H, ALT2, PIN_OUT | PIN_SPECIAL )   // pin 3_3, not available as GPIO
PINFUNC_DEF( FLASH_SSEL,              0,  6, H, GPIO, PIN_OUT )                 // pin 3_6
PINFUNC_DEF( FLASH_MISO,              5, 10, H, ALT2, PIN_IN | PIN_PULL_UP )    // pin 3_7
PINFUNC_DEF( FLASH_MOSI,              5, 11, H, ALT2, PIN_OUT )                 // pin 3_8
//
// External Memory Interface
//
// Address bus
PINFUNC_DEF( EMC_A0,                  1, 10, H, ALT3, PIN_OUT | PIN_FAST_SLEW ) // pin 2_9
PINFUNC_DEF( EMC_A1,                  0, 14, H, ALT3, PIN_OUT | PIN_FAST_SLEW ) // pin 2_10
PINFUNC_DEF( EMC_A2,                  1, 11, H, ALT3, PIN_OUT | PIN_FAST_SLEW ) // pin 2_11
PINFUNC_DEF( EMC_A3,                  1, 12, H, ALT3, PIN_OUT | PIN_FAST_SLEW ) // pin 2_12
PINFUNC_DEF( EMC_A4,                  1, 13, H, ALT3, PIN_OUT | PIN_FAST_SLEW ) // pin 2_13
PINFUNC_DEF( EMC_A5,                  0, 4,  H, ALT2, PIN_OUT | PIN_FAST_SLEW ) // pin 1_0
PINFUNC_DEF( EMC_A6,                  0, 8,  H, ALT2, PIN_OUT | PIN_FAST_SLEW ) // pin 1_1
PINFUNC_DEF( EMC_A7,                  0, 9,  H, ALT2, PIN_OUT | PIN_FAST_SLEW ) // pin 1_2
PINFUNC_DEF( EMC_A8,                  5, 7,  H, ALT3, PIN_OUT | PIN_FAST_SLEW ) // pin 2_8
PINFUNC_DEF( EMC_A9,                  0, 7,  H, ALT3, PIN_OUT | PIN_FAST_SLEW ) // pin 2_7
PINFUNC_DEF( EMC_A10,                 5, 6,  H, ALT2, PIN_OUT | PIN_FAST_SLEW ) // pin 2_6
PINFUNC_DEF( EMC_A11,                 5, 2,  H, ALT2, PIN_OUT | PIN_FAST_SLEW ) // pin 2_2
PINFUNC_DEF( EMC_A12,                 5, 1,  H, ALT2, PIN_OUT | PIN_FAST_SLEW ) // pin 2_1
PINFUNC_DEF( EMC_A13,                 5, 0,  H, ALT2, PIN_OUT | PIN_FAST_SLEW ) // pin 2_0
PINFUNC_DEF( EMC_A14,                 5, 16, H, ALT1, PIN_OUT | PIN_FAST_SLEW ) // pin 6_8
PINFUNC_DEF( EMC_A15,                 5, 15, H, ALT1, PIN_OUT | PIN_FAST_SLEW ) // pin 6_7
PINFUNC_DEF( EMC_A16,                 6, 30, H, ALT2, PIN_OUT | PIN_FAST_SLEW ) // pin D_16
PINFUNC_DEF( EMC_A17,                 6, 29, H, ALT2, PIN_OUT | PIN_FAST_SLEW ) // pin D_15
PINFUNC_DEF( EMC_A18,                 7,  0, H, ALT3, PIN_OUT | PIN_FAST_SLEW ) // pin E_0
// Data bus
PINFUNC_DEF( EMC_D0,                  1, 0,  H, ALT3, PIN_OUT | PIN_FAST_SLEW ) // pin 1_7
PINFUNC_DEF( EMC_D1,                  1, 1,  H, ALT3, PIN_OUT | PIN_FAST_SLEW ) // pin 1_8
PINFUNC_DEF( EMC_D2,                  1, 2,  H, ALT3, PIN_OUT | PIN_FAST_SLEW ) // pin 1_9
PINFUNC_DEF( EMC_D3,                  1, 3,  H, ALT3, PIN_OUT | PIN_FAST_SLEW ) // pin 1_10
PINFUNC_DEF( EMC_D4,                  1, 4,  H, ALT3, PIN_OUT | PIN_FAST_SLEW ) // pin 1_11
PINFUNC_DEF( EMC_D5,                  1, 5,  H, ALT3, PIN_OUT | PIN_FAST_SLEW ) // pin 1_12
PINFUNC_DEF( EMC_D6,                  1, 6,  H, ALT3, PIN_OUT | PIN_FAST_SLEW ) // pin 1_13
PINFUNC_DEF( EMC_D7,                  1, 7,  H, ALT3, PIN_OUT | PIN_FAST_SLEW ) // pin 1_14
PINFUNC_DEF( EMC_D8,                  0, 2,  H, ALT6, PIN_OUT | PIN_FAST_SLEW ) // pin 1_15
PINFUNC_DEF( EMC_D9,                  0, 3,  H, ALT6, PIN_OUT | PIN_FAST_SLEW ) // pin 1_16
PINFUNC_DEF( EMC_D10,                 0, 13, H, ALT7, PIN_OUT | PIN_FAST_SLEW ) // pin 1_18
PINFUNC_DEF( EMC_D11,                 0, 15, H, ALT7, PIN_OUT | PIN_FAST_SLEW ) // pin 1_20
PINFUNC_DEF( EMC_D12,                 2, 9,  H, ALT2, PIN_OUT | PIN_FAST_SLEW ) // pin 5_0
PINFUNC_DEF( EMC_D13,                 2, 10, H, ALT2, PIN_OUT | PIN_FAST_SLEW ) // pin 5_1
PINFUNC_DEF( EMC_D14,                 2, 11, H, ALT2, PIN_OUT | PIN_FAST_SLEW ) // pin 5_2
PINFUNC_DEF( EMC_D15,                 2, 12, H, ALT2, PIN_OUT | PIN_FAST_SLEW ) // pin 5_3
// Bus control
PINFUNC_DEF( EMC_nCS0,                1, 8,  L, ALT3, PIN_OUT | PIN_FAST_SLEW ) // pin 1_5
PINFUNC_DEF( EMC_nOE,                 0, 10, L, ALT3, PIN_OUT | PIN_FAST_SLEW ) // pin 1_3
PINFUNC_DEF( EMC_nWEN,                1, 9,  L, ALT3, PIN_OUT | PIN_FAST_SLEW ) // pin 1_6
PINFUNC_DEF( EMC_nUB,                 0, 5,  L, ALT1, PIN_OUT | PIN_FAST_SLEW ) // pin 6_6
PINFUNC_DEF( EMC_nLB,                 0, 11, L, ALT3, PIN_OUT | PIN_FAST_SLEW ) // pin 1_4
//
// Misc
//
PINFUNC_DEF( nPWR_DOWN,               2,  2, L, GPIO, PIN_OUT )                 // pin 4_2
//
// Camera interface - to be defined
//
//
// Spare GPIO - to test points
//
PINFUNC_DEF( DEBUG_GPIO0,             4, 12, H, GPIO, PIN_OUT )                 // pin 9_0
PINFUNC_DEF( DEBUG_GPIO1,             4, 13, H, GPIO, PIN_OUT )                 // pin 9_1
PINFUNC_DEF( DEBUG_GPIO2,             4, 14, H, GPIO, PIN_OUT )                 // pin 9_2
PINFUNC_DEF( DEBUG_GPIO3,             4, 15, H, GPIO, PIN_OUT )                 // pin 9_3
//
// Debug port
//
PINFUNC_DEF( DEBUG_TXD0,              3,  3, L, ALT2, PIN_OUT )                 // pin 6_4
PINFUNC_DEF( DEBUG_RXD0,              3,  4, L, ALT2, PIN_IN )                  // pin 6_5


#ifdef __cplusplus
}
#endif

#endif // _GPIO_PINDEFS_H_
