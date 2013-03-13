/****************************************************************************
 *   $Id:: pwm.c 5748 2010-11-30 23:48:37Z usb00423                         $
 *   Project: NXP LPC17xx PWM example
 *
 *   Description:
 *     This file contains PWM code example which include PWM initialization, 
 *     PWM interrupt handler, and APIs for PWM access.
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

volatile uint32_t match_counter0, match_counter1;

/******************************************************************************
** Function name:		PWM1_IRQHandler
**
** Descriptions:		PWM1 interrupt handler
**						For now, it only deals with PWM1 match 0
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void PWM1_IRQHandler (void) 
{
  uint32_t regVal;

  regVal = LPC_PWM1->IR;
  if ( regVal & MR0_INT )
  {
	match_counter1++;	
  }
  LPC_PWM1->IR |= regVal;		/* clear interrupt flag on match 0 */
  return;
}

/******************************************************************************
** Function name:		PWM_Init
**
** Descriptions:		PWM initialization, setup all GPIOs to PWM0~6,
**						reset counter, all latches are enabled, interrupt
**						on PWMMR0, install PWM interrupt to the VIC table.
**
** parameters:			ChannelNum, Duty cycle
** Returned value:		true or fase, if VIC table is full, return false
** 
******************************************************************************/
uint32_t PWM_Init( uint32_t ChannelNum, uint32_t cycle )
{
  if ( ChannelNum == 1 )
  {
	match_counter1 = 0;
	LPC_PINCON->PINSEL4 = 0x00001555;	/* set GPIOs for all PWM pins on PWM */

	LPC_PWM1->TCR = TCR_RESET;	/* Counter Reset */ 
	LPC_PWM1->PR = 0x00;		/* count frequency:Fpclk */
	LPC_PWM1->MCR = PWMMR0I;	/* interrupt on PWMMR0, reset on PWMMR0, reset 
								TC if PWM matches */				
	LPC_PWM1->MR0 = cycle;		/* set PWM cycle */
	LPC_PWM1->MR1 = cycle * 5/6;
	LPC_PWM1->MR2 = cycle * 2/3;
	LPC_PWM1->MR3 = cycle * 1/2;
	LPC_PWM1->MR4 = cycle * 1/3;
	LPC_PWM1->MR5 = cycle * 1/6;
	LPC_PWM1->MR6 = 0;	

	/* all PWM latch enabled */
	LPC_PWM1->LER = LER0_EN | LER1_EN | LER2_EN | LER3_EN | LER4_EN | LER5_EN | LER6_EN;
  }
  else
  {
	return ( FALSE );		/* Unknown channel number */
  }

  NVIC_EnableIRQ(PWM1_IRQn);
  return (TRUE);
}

/******************************************************************************
** Function name:		PWM_Set
**
** Descriptions:		PWM cycle setup
**
** parameters:			Channel number, PWM cycle, and offset
** Returned value:		None
** 
******************************************************************************/
void PWM_Set( uint32_t ChannelNum, uint32_t cycle, uint32_t offset )
{			
  if ( ChannelNum == 1 )
  {
	LPC_PWM1->MR0 = cycle;		/* set PWM cycle */
	LPC_PWM1->MR1 = cycle * 5/6 + offset;
	LPC_PWM1->MR2 = cycle * 2/3 + offset;
	LPC_PWM1->MR3 = cycle * 1/2 + offset;
	LPC_PWM1->MR4 = cycle * 1/3 + offset;
	LPC_PWM1->MR5 = cycle * 1/6 + offset;
	LPC_PWM1->MR6 = offset;

	/* The LER will be cleared when the Match 0 takes place, in order to
	load and execute the new value of match registers, all the PWMLERs need to
	reloaded. all PWM latch enabled */
	LPC_PWM1->LER = LER0_EN | LER1_EN | LER2_EN | LER3_EN | LER4_EN | LER5_EN | LER6_EN;
  }
  return;
}

/******************************************************************************
** Function name:		PWM_Start
**
** Descriptions:		Enable PWM by setting the PCR, PTCR registers
**
** parameters:			channel number
** Returned value:		None
** 
******************************************************************************/
void PWM_Start( uint32_t channelNum )
{
  if ( channelNum == 1 )
  {
	/* All single edge, all enable */
	LPC_PWM1->PCR = PWMENA1 | PWMENA2 | PWMENA3 | PWMENA4 | PWMENA5 | PWMENA6;
	LPC_PWM1->TCR = TCR_CNT_EN | TCR_PWM_EN;	/* counter enable, PWM enable */
  }
  return;
}

/******************************************************************************
** Function name:		PWM_Stop
**
** Descriptions:		Stop all PWM channels
**
** parameters:			channel number
** Returned value:		None
** 
******************************************************************************/
void PWM_Stop( uint32_t channelNum )
{
  if ( channelNum == 1 )
  {
	LPC_PWM1->PCR = 0;
	LPC_PWM1->TCR = 0x00;		/* Stop all PWMs */
  }
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
