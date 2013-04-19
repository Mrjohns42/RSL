/****************************************************************************
 *   $Id:: rs485test.c 9379 2012-04-19 23:02:45Z nxp41306                   $
 *   Project: NXP LPC11xx RS485 example
 *
 *   Description:
 *     This file contains RS485 test modules, main entry, to test RS485 APIs.
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

* Permission to use, copy, modify, and distribute this software and its 
* documentation is hereby granted, under NXP Semiconductors' 
* relevant copyright in the software, without fee, provided that it 
* is used in conjunction with NXP Semiconductors microcontrollers.  This 
* copyright, permission, and disclaimer notice must appear in all copies of 
* this code.
****************************************************************************/
#include "LPC11xx.h"
#include "rs485.h"

extern volatile uint32_t UARTCount;
extern volatile uint8_t UARTBuffer[BUFSIZE];
volatile uint8_t UARTTxBuffer[BUFSIZE];

int main (void) {
  uint8_t i;

  SystemCoreClockUpdate();

  /* Clear receive buffer, handled inside ISR. */
  for ( i = 0; i < BUFSIZE; i++ )
  {
	UARTBuffer[i] = 0x00;	
  }
  
  /* NVIC is installed inside UARTInit file. */
  RS485Init(9600);

  /* To test RS485 mode, connect two boards, one is set to TX and
  the other is RX. The test is conducted on the inter-board communication. */
#if RS485_RX
  /* If RX, do nothing, check the content of UARTBuffer */
  /* To get the test result, program the boards with both TX and RX program, 
  power up the board with the RX program first, start the debugger on the
  board with the RX program, power up the board with the TX program, stop 
  the debugger, and check the content of the UARTBuffer on the RX program,
  it should be the same as seen on UARTTxBuffer, 0x01-0x02-0x03.... */ 
  while ( 1 );

#else
  /* If TX. send a string out start with RS485 slave address */
  UARTTxBuffer[0] = RS485_SLAVE_ADR;
  for ( i = 1; i < BUFSIZE; i++ )
  {
	UARTTxBuffer[i] = i;	
  }
  RS485Send((uint8_t *)UARTTxBuffer, BUFSIZE);
  while ( 1 );
#endif			/* #endif RS485_RX */

}
