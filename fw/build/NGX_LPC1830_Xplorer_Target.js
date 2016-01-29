/******************************************************************************
  Target Script for NGX LPC1830-Xplorer

  Copyright (c) 2012 Rowley Associates Limited.

  This file may be distributed under the terms of the License Agreement
  provided with this software.

  THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 ******************************************************************************/

load("targets/LPC1800/LPC1800_Target.js");

function scu_pinmux(port,	pin, mode, func)
{
  TargetInterface.pokeWord(0x40086000+(0x80*port+0x4*pin), mode+func);
}

function SPIFIReset()
{
  Reset();
  // Reset the SPIFI block
  TargetInterface.pokeWord(0x40053104, 1<<21);
  // The boot ROM appears to run the SPIFI clock of IDIVB which is set to PLL/9
  TargetInterface.pokeWord(0x4005004C, 9<<24 | 1<<11 | 8<<2); //LPC_CGU->IDIVB_CTRL
  TargetInterface.pokeWord(0x40050070, 13<<24 | 1<<11); //LPC_CGU->BASE_SPIFI_CLK  
  /* Set up SPI pins */
  scu_pinmux(3,3,0xd0,3); // SPIFI_SCK
  scu_pinmux(3,4,0xd0,3); // SPIFI_SIO3
  scu_pinmux(3,5,0xd0,3); // SPIFI_SIO2
  scu_pinmux(3,6,0xd0,3); // SPIFI_SSEL
  scu_pinmux(3,7,0xd0,3); // SPIFI_MOSI
  scu_pinmux(3,8,0xd0,3); // SPIFI_CS
}
