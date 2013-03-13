/****************************************************************************
 *   $Id:: wdt.c 5752 2010-12-01 00:01:10Z usb00423                         $
 *   Project: NXP LPC17xx Watchdog Timer example
 *
 *   Description:
 *     This file contains WDT code example which include WDT initialization, 
 *     WDT interrupt handler, and APIs for WDT access.
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
#include "wdt.h"

volatile uint32_t wdt_counter;

/*****************************************************************************
** Function name:		WDT_IRQHandler
**
** Descriptions:		Watchdog timer interrupt handler
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void WDT_IRQHandler(void) 
{
  LPC_WDT->MOD &= ~WDTOF;		/* clear the time-out terrupt flag */
  wdt_counter++;
  return;
}

/*****************************************************************************
** Function name:		WDTInit
**
** Descriptions:		Initialize watchdog timer, install the
**				watchdog timer interrupt handler
**
** parameters:			None
** Returned value:		true or false, return false if the VIC table
**				is full and WDT interrupt handler can be
**				installed.
** 
*****************************************************************************/
uint32_t WDTInit( void )
{
  wdt_counter = 0;

  NVIC_EnableIRQ(WDT_IRQn);

  LPC_WDT->TC = WDT_FEED_VALUE;	/* once WDEN is set, the WDT will start after feeding */
  LPC_WDT->MOD = WDEN;

  LPC_WDT->FEED = 0xAA;		/* Feeding sequence */
  LPC_WDT->FEED = 0x55;    
  return( TRUE );
}

/*****************************************************************************
** Function name:		WDTFeed
**
** Descriptions:		Feed watchdog timer to prevent it from timeout
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void WDTFeed( void )
{
  LPC_WDT->FEED = 0xAA;		/* Feeding sequence */
  LPC_WDT->FEED = 0x55;
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
