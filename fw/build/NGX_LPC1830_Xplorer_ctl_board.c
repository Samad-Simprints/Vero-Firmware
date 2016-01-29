// Copyright (c) 2012 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#include <ctl_api.h>
#include <LPC18xx.h>

void SystemInit (void) __attribute__ ((section (".init")));
extern uint32_t SystemCoreClock  __attribute__ ((section(".non_init")));

#ifdef SPIFI_BOOT
void SPIFIInit(void) __attribute__ ((section (".init")));
void SPIFIInit(void)
{
  // SPIFI clock runs on DIVB
  LPC_CGU->IDIVB_CTRL = (0x9<<24)|(1<<11)|(3<<2);
}
#endif

uint32_t SystemCoreClock;

void SystemInit()
{ 
#if defined(SPIFI_BOOT)
  typedef void (*voidfnptr)(void);
  volatile unsigned *SPIFIInitPtr = (unsigned *)((unsigned)&SPIFIInit & ~1);
  volatile unsigned *SRAMPtr = (unsigned *)0x10000000;
  unsigned i;
  for (i=0;i<64;i++)
    *SRAMPtr++ = *SPIFIInitPtr++;
  ((voidfnptr)(0x10000001))();
#endif 

  // Enable the oscillator  
  LPC_CGU->XTAL_OSC_CTRL = 0;
  {
  volatile int i=0;
  while (i++<1000000);
  }

  // Change the PLL to 180Mhz  
  unsigned pll1_ctrl = LPC_CGU->PLL1_CTRL;
  pll1_ctrl &= ~((0xf<<24)|(0xff<<16)|1);  
  pll1_ctrl |= (14<<16); // Set MSEL to 15 - 15 * 12 = 180
  pll1_ctrl |= (0x6<<24);
  LPC_CGU->PLL1_CTRL = pll1_ctrl;
  while ((LPC_CGU->PLL1_STAT & 1)==0);
  // Switch M3 clock to PLL1
  LPC_CGU->BASE_M3_CLK = (0x9<<24)|(1<<11);

  SystemCoreClock = 180000000;
}

void
delay(int count)
{
  volatile int i=0;
  while (i++<count);
}

#define LED_BIT	11
#define LED_PORT 1

void
ctl_board_init(void)
{ 
  LPC_GPIO_PORT->DIR[LED_PORT] |= 1<<LED_BIT;
}

void 
ctl_board_set_leds(unsigned on)
{
  if (on)
    LPC_GPIO_PORT->CLR[LED_PORT] = 1<<LED_BIT;
  else
    LPC_GPIO_PORT->SET[LED_PORT] = 1<<LED_BIT;
}
#if 0
static CTL_ISR_FN_t userButtonISR;

void
PIN_INT0_IRQHandler(void)
{
  ctl_enter_isr();
  if (LPC_GPIO_PIN_INT->FALL & (1 << 0))
    {              
      userButtonISR();
      LPC_GPIO_PIN_INT->FALL = (1 << 0);
    }
  ctl_exit_isr();
}

#define BUTTON_BIT 7
#define BUTTON_PORT 0

void 
ctl_board_on_button_pressed(CTL_ISR_FN_t isr)
{
  userButtonISR = isr;

  LPC_SCU->SFSP2_7 = (1<<6)|(1<<4)|0; // Select P2_7 as GPIO0[7] with pull-up and input buffer
  LPC_GPIO_PORT->DIR[BUTTON_PORT] &= ~(1<<BUTTON_BIT);
  // Configure button to generate GPIO0 interrupt on falling edges
  LPC_SCU->PINTSEL0 &= ~(0xFF << 0);
  LPC_SCU->PINTSEL0 |= (BUTTON_PORT << 5) | (BUTTON_BIT << 0);
  LPC_GPIO_PIN_INT->ISEL &= ~(1 << 0);    // edge sensitive  
  LPC_GPIO_PIN_INT->SIENF = (1 << 0);     // falling edge enable    
  ctl_set_priority(PIN_INT0_IRQn, ctl_adjust_isr_priority(ctl_highest_isr_priority(), -1));
  ctl_unmask_isr(PIN_INT0_IRQn);
}
#endif
