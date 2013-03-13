/****************************************************************************
 *   $Id:: pwmtest.c 6097 2011-01-07 04:31:25Z nxp12832                     $
 *   Project: NXP LPC17xx PWM example
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
****************************************************************************/
#include "lpc17xx.h"
#include "type.h"
#include "pwm.h"

#define CHANNEL_NUM			1

extern volatile uint32_t match_counter1;

/******************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{	    
  uint32_t cycle = PWM_CYCLE, offset = 0;

  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemClockUpdate();
 
  if ( PWM_Init( CHANNEL_NUM, 0 ) != TRUE )
  {
	while( 1 );			/* fatal error */
  }

  PWM_Set( CHANNEL_NUM, cycle, offset );
  PWM_Start( CHANNEL_NUM );

  while ( 1 )
  {
	if ( match_counter1 != 0 )
	{
	  match_counter1 = 0;
	  PWM_Set( CHANNEL_NUM, cycle, offset );
	}
  }
}

/******************************************************************************
**                            End Of File
******************************************************************************/
