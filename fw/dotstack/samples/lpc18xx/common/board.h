/*******************************************************************************
* Contains proprietary and confidential information of SEARAN LLC.
* May not be used or disclosed to any other party except in accordance
* with a license from SEARAN LLC.
*
* Copyright (c) 2011-2015 SEARAN LLC. All Rights Reserved.
*******************************************************************************/

#ifndef __BOARD_H_INCLUDED__
#define __BOARD_H_INCLUDED__

#include "LPC18xx.h"
#include "board_config.h"

#define GPIO_HIGH(port, pin)      GPIO_SetValue(port, 1UL << (pin))
#define GPIO_LOW(port, pin)       GPIO_ClearValue(port, 1UL << (pin))
#define GPIO_GET(port, pin)       (GPIO_ReadValue(port) & (1uL << (pin)))

#if 0

#define LED1_ON()        GPIO_LOW(LED1_GPIO_PORT, LED1_GPIO_PIN)
#define LED1_OFF()       GPIO_HIGH(LED1_GPIO_PORT, LED1_GPIO_PIN)
#define LED1_TOGGLE()    (GPIO_GET(LED1_GPIO_PORT, LED1_GPIO_PIN) ? LED1_OFF() : LED1_ON())

#define LED2_ON()        GPIO_LOW(LED2_PORT, LED2_PIN)
#define LED2_OFF()       GPIO_HIGH(LED2_PORT, LED2_PIN)
#define LED2_TOGGLE()    (GPIO_GET(LED2_PORT, LED2_PIN) ? LED2_OFF() : LED2_ON())

#else

#define LED1_ON()
#define LED1_OFF()
#define LED1_TOGGLE()

#define LED2_ON()
#define LED2_OFF()
#define LED2_TOGGLE()

#endif

#define LED3_ON()        
#define LED3_OFF()       
#define LED3_TOGGLE()    

void board_init(void);


#endif //  __BOARD_H_INCLUDED__
