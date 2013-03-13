/****************************************************************************
 *   $Id:: wdttest.c 6097 2011-01-07 04:31:25Z nxp12832                     $
 *   Project: NXP LPC17xx WDT example
 *
 *   Description:
 *     This file contains WDT test modules, main entry, to test WDT APIs.
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
#include "wdt.h"

extern volatile uint32_t timer0_counter;
volatile uint32_t feed_counter = 0;

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemClockUpdate();

  /*** The main Function is an endless loop ****/
  init_timer( 0, TIME_INTERVAL);
    
  WDTInit();
  enable_timer( 0 );

  while( 1 ) 
  {
	if ( timer0_counter > 0 )
	{
	  /* Feed the watchdog timer */
	  feed_counter++;
	  WDTFeed();
	  timer0_counter = 0;
	}
  }
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
