/****************************************************************************
 *   $Id:: PWM16_32test.c 9389 2012-04-19 23:22:16Z nxp41306                $
 *   Project: NXP LPC11xx 16-bit/32-bit PWM example
 *
 *   Description:
 *     This file contains PWM test modules, main entry, to test PWM APIs.
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

* Permission to use, copy, modify, and distribute this software and its 
* documentation is hereby granted, under NXP Semiconductors' 
* relevant copyright in the software, without fee, provided that it 
* is used in conjunction with NXP Semiconductors microcontrollers.  This 
* copyright, permission, and disclaimer notice must appear in all copies of 
* this code.
*****************************************************************************/
#include "LPC11xx.h"                        /* LPC11xx definitions */
#include "timer32.h"
#include "timer16.h"
#include "gpio.h"

extern volatile uint32_t timer32_0_counter;

volatile uint32_t period = 1000;  //48Khz PWM frequency


/* Main Program */

int main (void) {
	  SystemCoreClockUpdate();
	
	/* Initialize 32-bits timer 0 */
	init_timer32(0, TIME_INTERVAL);
	enable_timer32(0);
	
	/* Initialize the PWM in timer32_1 enabling match0 output */
	init_timer32PWM(1, period, MATCH0);
	setMatch_timer32PWM (1, 0, period/4);  /* Freq 12KHz Duty cycle 75%. Output on PIO1_1 */
	enable_timer32(1);

	/* Initialize the PWM in timer16_1 enabling match1 output */
	init_timer16PWM(1, period, MATCH1, 0); 
	setMatch_timer16PWM (1, 1, period/2);  /* Freq 12KHz Duty cycle 50%. Output on PIO1_10 */
	enable_timer16(1);

	/* Enable AHB clock to the GPIO domain. */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);
	
	/* Set port 2_0 to output */
	GPIOSetDir( 2, 0, 1 );
	
	while (1)                                /* Loop forever */
	{
		/* I/O configuration and LED setting pending. */
		if ( (timer32_0_counter > 0) && (timer32_0_counter <= 50) )
		{
			GPIOSetValue( 2, 0, 0 );
		}
		if ( (timer32_0_counter > 50) && (timer32_0_counter <= 100) )
		{
			GPIOSetValue( 2, 0, 1 );
		}
		else if ( timer32_0_counter > 100 )
		{
	      timer32_0_counter = 0;
		}
	}
}
