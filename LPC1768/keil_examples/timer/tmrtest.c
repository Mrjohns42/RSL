/****************************************************************************
 *   $Id:: tmrtest.c 6098 2011-01-08 02:26:20Z nxp12832                      $
 *   Project: NXP LPC17xx Timer example
 *
 *   Description:
 *     This file contains Timer test modules, main entry, to test Timer APIs.
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
#include "timer.h"

#define BLINK_INTERVAL	20

extern uint32_t timer0_m0_counter, timer1_m0_counter;
extern uint32_t timer0_m1_counter, timer1_m1_counter;

/*****************************************************************************
**   Main Function  main()
*****************************************************************************/
int main (void)
{	    		
  uint32_t i;

  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemClockUpdate();
   
  LPC_GPIO2->FIODIR = 0x000000FF;		/* P2.0..7 defined as Outputs */
  LPC_GPIO2->FIOCLR = 0x000000FF;		/* turn off all the LEDs */
    
  for ( i = 0; i < 2; i++ )
  {  
	init_timer( i , TIME_INTERVAL );	
	enable_timer( i );
  }

  /* Loop forever */
  while (1) 
  {					/* Loop forever */
	if ( (timer0_m0_counter > 0) && (timer0_m0_counter <= BLINK_INTERVAL) )
	{
	  LPC_GPIO2->FIOSET = 1 << 2;
	}
	if ( (timer0_m0_counter > BLINK_INTERVAL) && (timer0_m0_counter <= (BLINK_INTERVAL * 2)) )
	{
	  LPC_GPIO2->FIOCLR = 1 << 2;
	}
	else if ( timer0_m0_counter > (BLINK_INTERVAL * 2) )
	{
	  timer0_m0_counter = 0;
	}
	/* Timer 1 blinky LED 1 */
	if ( (timer1_m0_counter > 0) && (timer1_m0_counter <= BLINK_INTERVAL) )
	{
	  LPC_GPIO2->FIOSET = 1 << 3;
	}
	if ( (timer1_m0_counter > BLINK_INTERVAL) && (timer1_m0_counter <= (BLINK_INTERVAL * 2)) )
	{
	  LPC_GPIO2->FIOCLR = 1 << 3;
	}
	else if ( timer1_m0_counter > (BLINK_INTERVAL * 2) )
	{
	  timer1_m0_counter = 0;
	}
  }
}

/*****************************************************************************
**                            End Of File
******************************************************************************/
