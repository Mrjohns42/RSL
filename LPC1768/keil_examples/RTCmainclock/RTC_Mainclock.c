/****************************************************************************
 *   $Id:: RTC_Mainclock.c 6097 2011-01-07 04:31:25Z nxp12832               $
 *   Project: NXP LPC17xx RTC example
 *
 *   Description:
 *     This file contains RTC test modules, main entry, to test RTC APIs.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
****************************************************************************/
#include "lpc17xx.h"
#include "type.h"

int main (void)
{	    		
  uint32_t i, j;

  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemClockUpdate();

  LPC_GPIO2->FIODIR = 0x000000FF;		/* P2.xx defined as Outputs */
  LPC_GPIO2->FIOCLR = 0x000000FF;		/* turn off all the LEDs */

  LPC_PINCON->PINSEL3 &= ~(3<<22);    /* P0.25 is GPIO                      */
  LPC_PINCON->PINSEL3 |=  (1<<22);    /* P0.25 is CLKOUT                     */

  while(1)
  {
	for(i = 0; i < 8; i++)
	{
	  LPC_GPIO2->FIOSET = 1 << i;
	  for(j = 1000000; j > 0; j--);
	}
	LPC_GPIO2->FIOCLR = 0x000000FF;
	for(j = 1000000; j > 0; j--);
  }
}
