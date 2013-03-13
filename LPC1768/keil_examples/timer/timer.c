/****************************************************************************
 *   $Id:: timer.c 5823 2010-12-07 19:01:00Z usb00423                       $
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

volatile uint32_t timer0_m0_counter = 0;
volatile uint32_t timer1_m0_counter = 0;
volatile uint32_t timer2_m0_counter = 0;
volatile uint32_t timer3_m0_counter = 0;
volatile uint32_t timer0_m1_counter = 0;
volatile uint32_t timer1_m1_counter = 0;
volatile uint32_t timer2_m1_counter = 0;
volatile uint32_t timer3_m1_counter = 0;

volatile uint32_t timer0_capture0 = 0;
volatile uint32_t timer1_capture0 = 0;
volatile uint32_t timer2_capture0 = 0;
volatile uint32_t timer3_capture0 = 0;
volatile uint32_t timer0_capture1 = 0;
volatile uint32_t timer1_capture1 = 0;
volatile uint32_t timer2_capture1 = 0;
volatile uint32_t timer3_capture1 = 0;

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
	LPC_TIM0->MR0 = delayInMs * (9000000 / 1000-1);
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
	LPC_TIM1->MR0 = delayInMs * (9000000 / 1000-1);
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
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void TIMER0_IRQHandler (void) 
{  
  if ( LPC_TIM0->IR & (0x1<<0) )
  {
    LPC_TIM0->IR = 0x1<<0;		/* clear interrupt flag */
    timer0_m0_counter++;
  }
  if ( LPC_TIM0->IR & (0x1<<1) )
  {
    LPC_TIM0->IR = 0x1<<1;		/* clear interrupt flag */
    timer0_m1_counter++;
  }
  if ( LPC_TIM0->IR & (0x1<<4) )
  {
	LPC_TIM0->IR = 0x1<<4;		/* clear interrupt flag */
	timer0_capture0++;
  }
  if ( LPC_TIM0->IR & (0x1<<5) )
  {
	LPC_TIM0->IR = 0x1<<5;		/* clear interrupt flag */
	timer0_capture1++;
  }
  return;
}

/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void TIMER1_IRQHandler (void)  
{  
  if ( LPC_TIM1->IR & (0x1<<0) )
  {
    LPC_TIM1->IR = 0x1<<0;		/* clear interrupt flag */
    timer1_m0_counter++;
  }
  if ( LPC_TIM1->IR & (0x1<<1) )
  {
    LPC_TIM1->IR = 0x1<<1;		/* clear interrupt flag */
    timer1_m1_counter++;
  }
  if ( LPC_TIM1->IR & (0x1<<4) )
  {
	LPC_TIM1->IR = 0x1<<4;		/* clear interrupt flag */
	timer1_capture0++;
  }
  if ( LPC_TIM1->IR & (0x1<<5) )
  {
	LPC_TIM1->IR = 0x1<<5;		/* clear interrupt flag */
	timer1_capture1++;
  }
  return;
}

/******************************************************************************
** Function name:		enable_timer
**
** Descriptions:		Enable timer
**
** parameters:			timer number: 0 or 1 or 2 or 3
** Returned value:		None
** 
******************************************************************************/
void enable_timer( uint8_t timer_num )
{
  if ( timer_num == 0 )
  {
	LPC_TIM0->TCR = 1;
  }
  else if ( timer_num == 1 )
  {
	LPC_TIM1->TCR = 1;
  }
  else if ( timer_num == 2 )
  {
	LPC_TIM2->TCR = 1;
  }
  else if ( timer_num == 3 )
  {
	LPC_TIM3->TCR = 1;
  }
  return;
}

/******************************************************************************
** Function name:		disable_timer
**
** Descriptions:		Disable timer
**
** parameters:			timer number: 0 or 1 oe 2 or 3
** Returned value:		None
** 
******************************************************************************/
void disable_timer( uint8_t timer_num )
{
  if ( timer_num == 0 )
  {
	LPC_TIM0->TCR = 0;
  }
  else if ( timer_num == 1 )
  {
	LPC_TIM1->TCR = 0;
  }
  else if ( timer_num == 2 )
  {
	LPC_TIM2->TCR = 0;
  }
  else if ( timer_num == 3 )
  {
	LPC_TIM2->TCR = 0;
  }
  return;
}

/******************************************************************************
** Function name:		reset_timer
**
** Descriptions:		Reset timer
**
** parameters:			timer number: 0 or 1 or 2 or 3
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
  else if ( timer_num == 1 )
  {
	regVal = LPC_TIM1->TCR;
	regVal |= 0x02;
	LPC_TIM1->TCR = regVal;
  }
  else if ( timer_num == 2 )
  {
	regVal = LPC_TIM2->TCR;
	regVal |= 0x02;
	LPC_TIM2->TCR = regVal;
  }
  else if ( timer_num == 3 )
  {
	regVal = LPC_TIM3->TCR;
	regVal |= 0x02;
	LPC_TIM3->TCR = regVal;
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
  uint32_t pclkdiv, pclk;

  if ( timer_num == 0 )
  {
	timer0_m0_counter = 0;
	timer0_m1_counter = 0;
	timer0_capture0 = 0;
	timer0_capture1 = 0;
	LPC_SC->PCONP |= (0x01<<1);
#if TIMER_MATCH
	LPC_PINCON->PINSEL3 &= ~((0x3<<24)|(0x3<<26));
	LPC_PINCON->PINSEL3 |= ((0x3<<24)|(0x3<<26));
#else
	LPC_PINCON->PINSEL3 &= ~((0x3<<20)|(0x3<<22));
	LPC_PINCON->PINSEL3 |= ((0x3<<20)|(0x3<<22));
#endif
	LPC_TIM0->IR = 0x0F;          /* Clear MATx interrupt include DMA request */ 

	/* By default, the PCLKSELx value is zero, thus, the PCLK for
	all the peripherals is 1/4 of the SystemFrequency. */
	/* Bit 2~3 is for TIMER0 */
	pclkdiv = (LPC_SC->PCLKSEL0 >> 2) & 0x03;
	switch ( pclkdiv )
	{
	  case 0x00:
	  default:
		pclk = SystemFrequency/4;
		break;
	  case 0x01:
		pclk = SystemFrequency;
		break; 
	  case 0x02:
		pclk = SystemFrequency/2;
		break; 
	  case 0x03:
		pclk = SystemFrequency/8;
		break;
	}
	LPC_TIM0->PR  = pclk/1000000; /* set prescaler to get 1 M counts/sec */

	LPC_TIM0->MR0 = TIME_INTERVALmS * 10; /* Set up 10 mS interval */
	LPC_TIM0->MR1 = TIME_INTERVALmS * 10; /* Set up 10 mS interval */
#if TIMER_MATCH
	LPC_TIM0->EMR &= ~(0xFF<<4);
	LPC_TIM0->EMR |= ((0x3<<4)|(0x3<<6));
#else
	/* Capture 0 and 1 on rising edge, interrupt enable. */
	LPC_TIM0->CCR = (0x1<<0)|(0x1<<2)|(0x1<<3)|(0x1<<5);
#endif
	LPC_TIM0->MCR = (0x3<<0)|(0x3<<3);	/* Interrupt and Reset on MR0 and MR1 */
	NVIC_EnableIRQ(TIMER0_IRQn);
	return (TRUE);
  }
  else if ( timer_num == 1 )
  {
	timer1_m0_counter = 0;
	timer1_m1_counter = 0;
	timer1_capture0 = 0;
	timer1_capture1 = 0;
	LPC_SC->PCONP |= (0x1<<2);
#if TIMER_MATCH
	LPC_PINCON->PINSEL3 &= ~((0x3<<12)|(0x3<<18));
	LPC_PINCON->PINSEL3 |= ((0x3<<12)|(0x3<<18));
#else
	LPC_PINCON->PINSEL3 &= ~((0x3<<4)|(0x3<<6));
	LPC_PINCON->PINSEL3 |= ((0x3<<4)|(0x3<<6));
#endif
	LPC_TIM1->IR = 0x0F;          /* Clear MATx interrupt include DMA request */
	/* By default, the PCLKSELx value is zero, thus, the PCLK for
	all the peripherals is 1/4 of the SystemFrequency. */
	/* Bit 4~5 is for TIMER0 */
	pclkdiv = (LPC_SC->PCLKSEL0 >> 4) & 0x03;
	switch ( pclkdiv )
	{
	  case 0x00:
	  default:
		pclk = SystemFrequency/4;
		break;
	  case 0x01:
		pclk = SystemFrequency;
		break; 
	  case 0x02:
		pclk = SystemFrequency/2;
		break; 
	  case 0x03:
		pclk = SystemFrequency/8;
		break;
	} 
	LPC_TIM1->PR  = pclk/1000000; /* set prescaler to get 1 M counts/sec */
	LPC_TIM1->MR0 = TIME_INTERVALmS * 10; /* Set up 10 mS interval */
	LPC_TIM1->MR1 = TIME_INTERVALmS * 10; /* Set up 10 mS interval */
#if TIMER_MATCH
	LPC_TIM1->EMR &= ~(0xFF<<4);
	LPC_TIM1->EMR |= ((0x3<<4)|(0x3<<6));
#else
	/* Capture 0/1 on rising edge, interrupt enable. */
	LPC_TIM1->CCR = (0x1<<0)|(0x1<<2)|(0x1<<3)|(0x1<<5);
#endif
	LPC_TIM1->MCR = (0x3<<0)|(0x3<<3);	/* Interrupt and Reset on MR0 and MR1 */
	NVIC_EnableIRQ(TIMER1_IRQn);
	return (TRUE);
  }
  return (FALSE);
}

/******************************************************************************
**                            End Of File
******************************************************************************/
