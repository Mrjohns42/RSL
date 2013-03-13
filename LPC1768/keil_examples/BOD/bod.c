/****************************************************************************
 *   $Id:: bod.c 6088 2011-01-06 04:19:59Z nxp12832                         $
 *   Project: NXP LPC17xx BOD(Brown-Out Detect) example
 *
 *   Description:
 *     This file contains BOD code example which include BOD 
 *     initialization, BOD interrupt handler, and APIs.
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
#include "LPC17xx.h"			/* LPC177x_8x Peripheral Registers */
#include "bod.h"

volatile uint32_t bod_counter;

/******************************************************************************
** Function name:		BOD_IRQHandler
**
** Descriptions:		BOD interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void BOD_IRQHandler (void) 
{  
  uint32_t i, j;

  /* Toggle P1.31 */
  for(i = 0; i < 4; i++){	
  	LPC_GPIO1->FIOPIN ^= (1UL<<31);
	for(j = 1000000; j > 0; j--);
  }

  return;
}

/*****************************************************************************
** Function name:		BOD_Init
**
** Descriptions:		Initialize BOD control register
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void BOD_Init( void )
{
  /* Turn on BOD. */
  LPC_SC->PCON &= ~(0x1<<3);

  /* Enable the BOD Interrupt */
  NVIC_EnableIRQ(BOD_IRQn);
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
