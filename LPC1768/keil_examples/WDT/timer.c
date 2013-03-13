/****************************************************************************
 *   $Id:: timer.c 5752 2010-12-01 00:01:10Z usb00423                       $
 *   Project: NXP LPC17xx Timer for PWM example
 *
 *   Description:
 *     This file contains timer code example which include timer 
 *     initialization, timer interrupt handler, and APIs for timer access.
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

volatile uint32_t timer0_counter = 0;
volatile uint32_t timer1_counter = 0;

/*****************************************************************************
** Function name:		delayMs
**
** Descriptions:		Start the timer delay in milo seconds
**						until elapsed
**
** parameters:			timer number, Delay value in milo second			 
** 						
** Returned value:		None
** 
*****************************************************************************/
void delayMs(uint8_t timer_num, uint32_t delayInMs)
{
  if ( timer_num == 0 )
  {
	LPC_TIM0->TCR = 0x02;		/* reset timer */
	LPC_TIM0->PR  = 0x00;		/* set prescaler to zero */
	LPC_TIM0->MR0 = delayInMs * (18000000 / 1000-1);	 //Fpclk:18MHz
	LPC_TIM0->IR  = 0xff;		/* reset all interrrupts */
	LPC_TIM0->MCR = 0x04;		/* stop timer on match */
	LPC_TIM0->TCR = 0x01;		/* start timer */
  
	/* wait until delay time has elapsed */
	while (LPC_TIM0->TCR & 0x01);
  }
  else if ( timer_num == 1 )
  {
	LPC_TIM1->TCR = 0x02;		/* reset timer */
	LPC_TIM1->PR  = 0x00;		/* set prescaler to zero */
	LPC_TIM1->MR0 = delayInMs * (18000000 / 1000-1);
	LPC_TIM1->IR  = 0xff;		/* reset all interrrupts */
	LPC_TIM1->MCR = 0x04;		/* stop timer on match */
	LPC_TIM1->TCR = 0x01;		/* start timer */
  
	/* wait until delay time has elapsed */
	while (LPC_TIM1->TCR & 0x01);
  }
  return;
}

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**						
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void TIMER0_IRQHandler (void) 
{  
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  timer0_counter++;
  return;
}

/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**						executes each 10ms @ 60 MHz CPU Clock
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void TIMER1_IRQHandler (void) 
{  
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  timer1_counter++;
  return;
}

/******************************************************************************
** Function name:		enable_timer
**
** Descriptions:		Enable timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
** 
******************************************************************************/
void enable_timer( uint8_t timer_num )
{
  if ( timer_num == 0 )
  {
	LPC_TIM0->TCR = 1;
  }
  else
  {
	LPC_TIM1->TCR = 1;
  }
  return;
}

/******************************************************************************
** Function name:		disable_timer
**
** Descriptions:		Disable timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
** 
******************************************************************************/
void disable_timer( uint8_t timer_num )
{
  if ( timer_num == 0 )
  {
	LPC_TIM0->TCR = 0;
  }
  else
  {
	LPC_TIM1->TCR = 0;
  }
  return;
}

/******************************************************************************
** Function name:		reset_timer
**
** Descriptions:		Reset timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
** 
******************************************************************************/
void reset_timer( uint8_t timer_num )
{
  uint32_t regVal;

  if ( timer_num == 0 )
  {
	regVal = LPC_TIM0->TCR;
	regVal |= 0x02;
	LPC_TIM0->TCR = regVal;
  }
  else
  {
	regVal = LPC_TIM1->TCR;
	regVal |= 0x02;
	LPC_TIM1->TCR = regVal;
  }
  return;
}

/******************************************************************************
** Function name:		init_timer
**
** Descriptions:		Initialize timer, set timer interval, reset timer,
**						install timer interrupt handler
**
** parameters:			timer number and timer interval
** Returned value:		true or false, if the interrupt handler can't be
**						installed, return false.
** 
******************************************************************************/
uint32_t init_timer ( uint8_t timer_num, uint32_t TimerInterval ) 
{

  if ( timer_num == 0 )
  {
	timer0_counter = 0;
	LPC_TIM0->MR0 = TimerInterval;
	LPC_TIM0->MCR = 3;				/* Interrupt and Reset on MR0 */

	NVIC_EnableIRQ(TIMER0_IRQn);
	return (TRUE);
  }
  else if ( timer_num == 1 )
  {
	timer1_counter = 0;
	LPC_TIM1->MR0 = TimerInterval;
	LPC_TIM1->MCR = 3;				/* Interrupt and Reset on MR1 */

	NVIC_EnableIRQ(TIMER1_IRQn);
	return (TRUE);
  }
  return (FALSE);
}

/******************************************************************************
**                            End Of File
******************************************************************************/
