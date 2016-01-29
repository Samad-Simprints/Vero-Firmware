#include "btapp.h"
#include "button.h"
#include "bttask.h"
#include "lpc18xx_gpio.h"
// NsT #include "lpc18xx_pinsel.h"

#define BUTTON_PORT      2
#define BUTTON_PIN       10
#define BUTTON_PIN_MASK  (1 << BUTTON_PIN)

#define BTLOG_TAG "BTN"
#include "btlog_define.h"

static uint32_t        mButtonState;

void bttask_pal_initButtons(void)
{
#if 0
	PINSEL_CFG_Type pinsel;
	
	GPIO_SetDir(BUTTON_PORT, BUTTON_PIN_MASK, 0); // configure as input
	
	pinsel.Portnum = BUTTON_PORT;
	pinsel.Pinnum = BUTTON_PIN;
	pinsel.Funcnum = 0;
	pinsel.Pinmode = PINSEL_PINMODE_TRISTATE;
	pinsel.OpenDrain = PINSEL_PINMODE_NORMAL;
	PINSEL_ConfigPin(&pinsel);
#endif	
	mButtonState = BUTTON_PIN_MASK; // UP
}

void bttask_pal_handleButtonsSignal(void)
{
	uint32_t newState = GPIO_ReadValue(BUTTON_PORT) & BUTTON_PIN_MASK;
	if (newState != mButtonState)
	{
		mButtonState = newState;
		if (newState & BUTTON_PIN_MASK)
		{
		  	BTLOG1("Up   button=%d", BTAPP_BUTTON_S1);
			btapp_onButtonUp(BTAPP_BUTTON_S1, 1);
		}
		else
		{
		  	BTLOG1("Down button=%d", BTAPP_BUTTON_S1);
			btapp_onButtonDown(BTAPP_BUTTON_S1, 0);
		}
	}
}

void bttask_pal_handleButtonRepeatSignal(void)
{
}

void button_checkButtonState(void)
{
#if 0
	portBASE_TYPE higherPriorityTaskWoken;
	
	if ((GPIO_ReadValue(BUTTON_PORT) & BUTTON_PIN_MASK) != mButtonState)
	{
		bttask_setSignalFromISR(BTTASK_SIG_BUTTONS, &higherPriorityTaskWoken);
	}
#endif
}
