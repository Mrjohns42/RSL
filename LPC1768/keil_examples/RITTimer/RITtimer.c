/****************************************************************************
 *   $Id:: RITtimer.c 5739 2010-11-30 22:34:36Z usb00423                    $
 *   Project: NXP LPC17xx RIT Timer example
 *
 *   Description:
 *     This file contains RIT Timer code example which include RIT 
 *     initialization, RIT interrupt handler, and APIs for RIT Timer.
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

volatile uint32_t rit_timer_counter = 0;

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		RIT timer interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void RIT_IRQHandler (void) 
{  
  LPC_RIT->RICTRL |= (0x1<<0);		/* clear interrupt flag */
  rit_timer_counter++;
  return;
}

/******************************************************************************
** Function name:		enable_rit_timer
**
** Descriptions:		Enable RIT timer
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void enable_rit_timer( void )
{
  LPC_RIT->RICTRL |= ((0x1<<0)|(0x1<<1));
  return;
}

/******************************************************************************
** Function name:		disable_rit_timer
**
** Descriptions:		Disable RIT timer
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void disable_rit_timer( void )
{
  LPC_RIT->RICTRL &= ~((0x1<<0)|(0x1<<1));
  return;
}

/******************************************************************************
** Function name:		reset_rit_timer
**
** Descriptions:		Reset RIT timer
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void reset_rit_timer( void )
{
  LPC_RIT->RICTRL |= (0x1<<1);
  return;
}

/******************************************************************************
** Function name:		init_rit_timer
**
** Descriptions:		Initialize timer, set timer interval, reset timer,
**						install timer interrupt handler
**
** parameters:			timer interval
** Returned value:		None
** 
******************************************************************************/
void init_rit_timer ( uint32_t TimerInterval ) 
{
  LPC_SC->PCONP |= (1 << 16);

  disable_rit_timer();
  rit_timer_counter = 0;
  LPC_RIT->RICOMPVAL = TimerInterval;
  NVIC_EnableIRQ(RIT_IRQn);
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
