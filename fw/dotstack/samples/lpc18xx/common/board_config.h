/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#include "config.h"

//------------------------------------------------------------------------------
// IO definitions for LPC1768-H board.
//------------------------------------------------------------------------------

#ifndef __BOARD_CONFIG_H_INCLUDED__
#define __BOARD_CONFIG_H_INCLUDED__

// Configuration for the USART and GPIO connected to the Bluetooth controller.
//

//#define BOARD_NGX


#define BT_UART               LPC_UART1
//NsT #define BT_UART_PCLK          CLKPWR_PCLKSEL_UART1
//NsT #define BT_UART_PCLK_DIV      CLKPWR_PCLKSEL_CCLK_DIV_1

#if defined(BOARD_NGX)
// NGX board
#define BT_UART_TX_PORT       1
#define BT_UART_TX_PIN        13
#define BT_UART_TX_FUNC       1

#define BT_UART_RX_PORT       1
#define BT_UART_RX_PIN        14
#define BT_UART_RX_FUNC       1

#define BT_UART_CTS_PORT      1
#define BT_UART_CTS_PIN       11
#define BT_UART_CTS_FUNC      1

#define BT_UART_RTS_PORT      1
#define BT_UART_RTS_PIN       9
#define BT_UART_RTS_FUNC      1
#define BT_UART_RTS_GPIO_FUNC 0
#define BT_UART_RTS_GPIO_PORT 1
#define BT_UART_RTS_GPIO_PIN  2

#define BT_RESET_PORT         2
#define BT_RESET_PIN          8
#define BT_RESET_FUNC         4
#define BT_RESET_GPIO_PORT    5
#define BT_RESET_GPIO_PIN     7

#else
// Index hardware
#define BT_UART_TX_PORT       3
#define BT_UART_TX_PIN        4
#define BT_UART_TX_FUNC       4

#define BT_UART_RX_PORT       3
#define BT_UART_RX_PIN        5
#define BT_UART_RX_FUNC       4

#define BT_UART_CTS_PORT      0xe
#define BT_UART_CTS_PIN       7
#define BT_UART_CTS_FUNC      2

#define BT_UART_RTS_PORT      0xe
#define BT_UART_RTS_PIN       5
#define BT_UART_RTS_FUNC      2
#define BT_UART_RTS_GPIO_FUNC 4
#define BT_UART_RTS_GPIO_PORT 7
#define BT_UART_RTS_GPIO_PIN  5

#define BT_RESET_PORT         4
#define BT_RESET_PIN          3
#define BT_RESET_FUNC         0
#define BT_RESET_GPIO_PORT    2
#define BT_RESET_GPIO_PIN     3

#endif

// Configuration for the USART that used for logging:
//
#define LOG_UART              LPC_USART0
//NsT#define LOG_UART_PCLK         CLKPWR_PCLKSEL_UART0
//NsT#define LOG_UART_PCLK_DIV     CLKPWR_PCLKSEL_CCLK_DIV_1

#define LOG_UART_TX_PORT      6
#define LOG_UART_TX_PIN       4
#define LOG_UART_TX_FUNC      2

#define LOG_UART_DMA_CH_NUM   0
#define LOG_UART_DST_CONN     GPDMA_CONN_UART0_Tx

// Configuration for LEDs:

#if defined(BOARD_NGX)
// NGX board
#define LED1_PORT                        2
#define LED1_PIN                         11
#define LED1_FUNC                        0
#define LED1_GPIO_PORT                   1
#define LED1_GPIO_PIN                    11

#define LED2_PORT                        2
#define LED2_PIN                         12
#define LED2_FUNC                        0
#define LED2_GPIO_PORT                   1
#define LED2_GPIO_PIN                    12

#else
// Index hardware
#define LED1_PORT                        0xd
#define LED1_PIN                         10
#define LED1_FUNC                        4
#define LED1_GPIO_PORT                   6
#define LED1_GPIO_PIN                    24

#define LED2_PORT                        0xd
#define LED2_PIN                         11
#define LED2_FUNC                        4
#define LED2_GPIO_PORT                   6
#define LED2_GPIO_PIN                    25

#endif

#define ACP_I2C_ADDRESS                  0x20
#define ACP_I2C_SCL_PORT                 0
#define ACP_I2C_SDA_PORT                 0
#define ACP_I2C_SCL_PIN                  1
#define ACP_I2C_SDA_PIN                  0
#define ACP_RESET_PORT                   1
#define ACP_RESET_PIN                    19

#endif // __BOARD_CONFIG_H_INCLUDED__
