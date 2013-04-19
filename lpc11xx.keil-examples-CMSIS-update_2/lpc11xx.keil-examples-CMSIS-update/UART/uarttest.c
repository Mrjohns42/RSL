/****************************************************************************
 *   $Id:: uarttest.c 9387 2012-04-19 23:14:07Z nxp41306                    $
 *   Project: NXP LPC11xx UART example
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
#include "LPC11xx.h"
#include "uart.h"

extern volatile uint32_t UARTCount;
extern volatile uint8_t UARTBuffer[BUFSIZE];

#if AUTOBAUD_ENABLE
extern volatile uint32_t UARTAutoBaud;
#endif


int main (void) {

  SystemCoreClockUpdate();

  /* if AUTOBAUD_ENABLE flag is set, the baud rate will be NOT set inside 
  the initialization routine. */
  UARTInit(115200);

#if AUTOBAUD_ENABLE
    LPC_UART->ACR = 0x01<<2;		/* Auto Restart, UART mode 0 */
	LPC_UART->ACR |= (0x01<<0);	/* Start */
    while( !UARTAutoBaud );
	UARTAutoBaud = 0;

	UARTSend((uint8_t *)"Auto Baud detected\r\n\r\n", 22);
#endif

#if MODEM_TEST
  ModemInit();
#endif

  while (1) 
  {				/* Loop forever */
	if ( UARTCount != 0 )
	{
	  LPC_UART->IER = IER_THRE | IER_RLS;			/* Disable RBR */
	  UARTSend( (uint8_t *)UARTBuffer, UARTCount );
	  UARTCount = 0;
	  LPC_UART->IER = IER_THRE | IER_RLS | IER_RBR;	/* Re-enable RBR */
	}
  }
}
