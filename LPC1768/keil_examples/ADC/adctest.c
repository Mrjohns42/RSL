/****************************************************************************
 *   $Id:: adctest.c 6089 2011-01-06 04:38:09Z nxp12832                     $
 *   Project: NXP LPC17xx ADC example
 *
 *   Description:
 *     This file contains ADC test modules, main entry, to test ADC APIs.
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
#include "adc.h"

extern volatile uint32_t ADCValue[ADC_NUM];
extern volatile uint32_t ADCIntDone;
extern volatile uint32_t OverRunCounter;

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemClockUpdate();
  
  /* Initialize ADC  */
  ADCInit( ADC_CLK );

  while(1)
  {
#if BURST_MODE				/* Interrupt driven only */
	ADCBurstRead();
	while ( !ADCIntDone );
	ADCIntDone = 0;
  	if ( OverRunCounter != 0 )
  	{
  	  while ( 1 );
  	}
#else						/* Not burst mode */
	#if ADC_INTERRUPT_FLAG		/* Interrupt driven */
		for ( i = 0; i < ADC_NUM; i++ )
		{
		  ADCRead( i );
		  while ( !ADCIntDone );
		  ADCIntDone = 0;
		}
	#else  						/* Polling */
		for ( i = 0; i < ADC_NUM; i++ )
		{
		  ADCValue[i] = ADCRead( i );
		}
	#endif	/* Endif interrupt driven */
#endif	/* Endif BURST mode */
	}
}

/*****************************************************************************
**                            End Of File
*****************************************************************************/

