/****************************************************************************
 *   $Id:: BODtest.c 6088 2011-01-06 04:19:59Z nxp12832                     $
 *   Project: NXP LPC17xx BOD(Brown-Out Detect) example
 *
 *   Description:
 *     Main entry for BOD code example.
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
#include "LPC17xx.h"
#include "type.h"
#include "timer.h"
#include "bod.h"

extern volatile uint32_t timer0_counter;

int main (void)
{	    		
  uint32_t counter = 0;
  
  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemClockUpdate();

  BOD_Init();

  LPC_GPIO1->FIODIR = 0xF0000000;		/* P1.28-31 defined as Outputs */
  LPC_GPIO1->FIOCLR = 0xF0000000;		/* turn off all the LEDs */
  LPC_GPIO2->FIODIR = 0x000000FF;		/* P2.0-8 defined as Outputs */
  LPC_GPIO2->FIOCLR = 0x000000FF;		/* turn off all the LEDs */

  init_timer( 0, TIME_INTERVAL ); // 10ms	
  enable_timer( 0 );

  counter = 1;
  while(1){
	
	/*	Just cycle through LEDs P2.2-P2.6 */
	if ( timer0_counter >= (50 * counter) ){
		if( counter == 7 ){
			counter = 1;
			timer0_counter = 0;
			LPC_GPIO2->FIOCLR = 0xFF;
		}else{
			counter++;
			LPC_GPIO2->FIOSET = 1 << counter;
		}
	}

	/*	If BOD was detected, set P1.28 LED */
	if ((LPC_SC->RSID & 0x9) == 0x8){
		LPC_SC->RSID |= 0x8;
		LPC_GPIO1->FIOSET = (1UL<<28);
	}

  }
}





