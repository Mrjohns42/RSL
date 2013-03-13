/****************************************************************************
 *   $Id:: RITtest.c 6097 2011-01-07 04:31:25Z nxp12832                     $
 *   Project: NXP LPC17xx RIT Timer example
 *
 *   Description:
 *     This file contains RIT test modules, main entry, to test RIT APIs.
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
#include "rittimer.h"

extern uint32_t rit_timer_counter;

/*****************************************************************************
**   Main Function  main()
*****************************************************************************/
int main (void)
{	    		
  uint32_t counter = 0;

  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemClockUpdate();
  
  LPC_GPIO2->FIODIR = 0x000000FF;		/* P1.16..23 defined as Outputs */
  LPC_GPIO2->FIOCLR = 0x000000FF;		/* turn off all the LEDs */
    
  init_rit_timer( TIME_INTERVAL );		/* 10ms */	
  enable_rit_timer();

  while (1) 
  {					/* Loop forever */
	if ( rit_timer_counter >= (50 * counter) )	   // 0.5s
	{
	  LPC_GPIO2->FIOSET = 1 << counter;
	  counter++;
	  if ( counter > 8 )
	  {
		counter = 0;	
		rit_timer_counter = 0;
		LPC_GPIO2->FIOCLR = 0x000000FF;
	  }
	}
  }
}

/*****************************************************************************
**                            End Of File
******************************************************************************/
