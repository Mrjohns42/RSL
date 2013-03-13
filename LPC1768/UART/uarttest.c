/****************************************************************************
 *   $Id:: uarttest.c 6098 2011-01-08 02:26:20Z nxp12832                    $
 *   Project: NXP LPC17xx UART example
 *
 *   Description:
 *     This file contains UART test modules, main entry, to test UART APIs.
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
#include "uart.h"

extern volatile uint32_t UART0Count;
extern volatile uint8_t UART0Buffer[BUFSIZE];
extern volatile uint32_t UART1Count;
extern volatile uint8_t UART1Buffer[BUFSIZE];

/*****************************************************************************
**   Main Function  main()
This program has been test on Keil LPC1700 board.
*****************************************************************************/
int main (void)
{
  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemClockUpdate();

  UARTInit(0, 57600);	/* baud rate setting */
  UARTInit(1, 57600);	/* baud rate setting */

  while (1) 
  {				/* Loop forever */
	if ( UART0Count != 0 )
	{
	  LPC_UART0->IER = IER_THRE | IER_RLS;			/* Disable RBR */
	  UARTSend( 0, (uint8_t *)UART0Buffer, UART0Count );
	  UART0Count = 0;
	  LPC_UART0->IER = IER_THRE | IER_RLS | IER_RBR;	/* Re-enable RBR */
	}
	if ( UART1Count != 0 )
	{
	  LPC_UART1->IER = IER_THRE | IER_RLS;			/* Disable RBR */
	  UARTSend( 1, (uint8_t *)UART1Buffer, UART1Count );
	  UART1Count = 0;
	  LPC_UART1->IER = IER_THRE | IER_RLS | IER_RBR;	/* Re-enable RBR */
	}
  }
}

/*****************************************************************************
**                            End Of File
*****************************************************************************/
