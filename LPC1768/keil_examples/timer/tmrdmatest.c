/****************************************************************************
 *   $Id:: tmrdmatest.c 6098 2011-01-08 02:26:20Z nxp12832                  $
 *   Project: NXP LPC17xx Timer DMA example
 *
 *   Description:
 *     This file contains Timer trigger DMA test modules, main entry.
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
#include "dma.h"

#define BLINK_INTERVAL	20

extern uint32_t timer0_m0_counter, timer1_m0_counter, timer2_m0_counter, timer3_m0_counter;
extern uint32_t timer0_m1_counter, timer1_m1_counter, timer2_m1_counter, timer3_m1_counter;
extern volatile uint32_t DMATCCount, CH0DMADone, CH1DMADone, CH2DMADone, CH3DMADone;

/*****************************************************************************
**   Main Function  main()
*****************************************************************************/
int main (void)
{
  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemClockUpdate();

  LPC_GPIO2->FIODIR = 0x0000000F;		/* P2.0..3 defined as Outputs */
  LPC_GPIO2->FIOCLR = 0x0000000F;		/* turn off all the LEDs */

  /* Do a P2M DMA transfer using timer match out as a trigger */  
  DMA_Init(P2M);
#if 0
  LPC_GPDMACH0->CConfig |= 0x0C001|(TMR0_M0_REQ<<1)|(0x00<<6)|(0x02<<11);
  LPC_SC->DMAREQSEL = (0x1<<0);
  init_timer( 0, TIME_INTERVAL );	
  enable_timer( 0 );
#endif
#if 0
  LPC_GPDMACH0->CConfig |= 0x0C001|(TMR0_M1_REQ<<1)|(0x00<<6)|(0x02<<11);
  LPC_SC->DMAREQSEL = (0x1<<1);
  init_timer( 0, TIME_INTERVAL );	
  enable_timer( 0 );
#endif
#if 1
  LPC_GPDMACH0->CConfig |= 0x0C001|(TMR1_M0_REQ<<1)|(0x00<<6)|(0x02<<11);
  LPC_SC->DMAREQSEL = (0x1<<2);
  init_timer( 1, TIME_INTERVAL );	
  enable_timer( 1 );
#endif
#if 0
  LPC_GPDMACH0->CConfig |= 0x0C001|(TMR1_M1_REQ<<1)|(0x00<<6)|(0x02<<11);
  LPC_SC->DMAREQSEL = (0x1<<3);
  init_timer( 1, TIME_INTERVAL );	
  enable_timer( 1 );
#endif

  /* When DMADone is set, it indicates that DMA has been triggered 
  by MAT_OUT(0.0). */
  while ( !CH0DMADone );
  CH0DMADone = 0;

  /* Loop forever */
  while (1) 
  {		
	/* Timer 0 blinky LED 0 */
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
