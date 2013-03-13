/****************************************************************************
 *   $Id:: extint.c 5670 2010-11-19 01:33:16Z usb00423                      $
 *   Project: NXP LPC17xx EINT example
 *
 *   Description:
 *     This file contains EINT code example which include EINT 
 *     initialization, EINT interrupt handler, and APIs for EINT.
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
#include "extint.h"

volatile uint32_t eint0_counter;

/*****************************************************************************
** Function name:		EINT0_Handler
**
** Descriptions:		external INT handler
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void EINT0_IRQHandler (void) 
{
  LPC_SC->EXTINT = EINT0;		/* clear interrupt */
		
  eint0_counter++;
  if ( eint0_counter & 0x01 )	/* alternate the LED display */
  {
	LPC_GPIO2->FIOSET = 0x0000000F;	/* turn off P2.0~3 */	
	LPC_GPIO2->FIOCLR = 0x000000F0;	/* turn on P2.4~7 */
  }
  else
  {
	LPC_GPIO2->FIOSET = 0x000000F0;	/* turn on P2.0~3 */	
	LPC_GPIO2->FIOCLR = 0x0000000F;	/* turn off P2.4~7 */
  }

}

/*****************************************************************************
** Function name:		EINTInit
**
** Descriptions:		Initialize external interrupt pin and
**						install interrupt handler
**
** parameters:			None
** Returned value:		true or false, return false if the interrupt
**						handler can't be installed to the VIC table.
** 
*****************************************************************************/
uint32_t EINTInit( void )
{
  LPC_PINCON->PINSEL4 = 0x00100000;	/* set P2.10 as EINT0 and
									P2.0~7 GPIO output */
  LPC_GPIO2->FIODIR = 0x000000FF;	/* port 2, bit 0~7 only */
  LPC_GPIO2->FIOCLR = 0x000000FF;	/* turn off LEDs */

  LPC_GPIOINT->IO2IntEnF = 0x200;	/* Port2.10 is falling edge. */
  LPC_SC->EXTMODE = EINT0_EDGE;		/* INT0 edge trigger */
  LPC_SC->EXTPOLAR = 0;				/* INT0 is falling edge by default */

  NVIC_EnableIRQ(EINT0_IRQn);
  return( TRUE );
}

/******************************************************************************
**                            End Of File
******************************************************************************/

