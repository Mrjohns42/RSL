/*
**************************************************************************************************************
*                                                 NXP USB Host Stack
*
*                                     (c) Copyright 2008, NXP SemiConductors
*                                     (c) Copyright 2008, OnChip  Technologies LLC
*                                                 All Rights Reserved
*
*                                                  www.nxp.com
*                                               www.onchiptech.com
*
* File           : usbhost_uart.c
* Programmer(s)  : Prasad.K.R.S.V
* Version        :
*
**************************************************************************************************************
*/

/*
**************************************************************************************************************
*                                           INCLUDE HEADER FILES
**************************************************************************************************************
*/

#include "usbhost_uart.h"

/*
**************************************************************************************************************
*                                         INITIALIZE UART
*
* Description: This function initializes UART port, setup pin select, clock, parity, stopbits, FIFO etc
*
* Arguments  : baud_rate    UART baud rate (115200)
*
* Returns    : None
*
**************************************************************************************************************
*/

void  UART_Init(uint32_t baudrate)
{
    uint32_t  Fdiv;
	uint32_t  pclkdiv, pclk;
          
    LPC_PINCON->PINSEL0 |= 0x00000050;       /* RxD0 and TxD0 */

	/* Bit 6~7 is for UART0 clock divider. */
	pclkdiv = (LPC_SC->PCLKSEL0 >> 6) & 0x03;
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
	
    LPC_UART0->LCR = 0x83;		/* 8 bits, no Parity, 1 Stop bit */
	Fdiv = ( pclk / 16 ) / baudrate ;	/*baud rate */
    LPC_UART0->DLM = Fdiv / 256;							
    LPC_UART0->DLL = Fdiv % 256;
    LPC_UART0->LCR = 0x03;		/* DLAB = 0 */
    LPC_UART0->FCR = 0x07;		/* Enable and reset TX and RX FIFO. */
}

/*
**************************************************************************************************************
*                                         PRINT CHARECTER
*
* Description: This function is used to print a single charecter through UART1.
*
* Arguments  : ch    charecter to be printed
*
* Returns    : None
*
**************************************************************************************************************
*/

void  UART_PrintChar (uint8_t ch)
{

   while (!(LPC_UART0->LSR & 0x20));
   LPC_UART0->THR  = ch;
}

/*
**************************************************************************************************************
*                                         PRINT STRING
*
* Description: This function is used to print a string
*
* Arguments  : str    Pointer to the string
*
* Returns    : None
*
**************************************************************************************************************
*/

void  UART_PrintStr (const uint8_t * str)
{

   while ((*str) != 0) {
      if (*str == '\n') {
         UART_PrintChar(*str++);
         UART_PrintChar('\r');
      } else {
         UART_PrintChar(*str++);
      }    
   }
}

/*
**************************************************************************************************************
*                                        PRINT FORMATTED STRING
*
* Description: This function is used to print formatted string. This function takes variable length arguments
*
* Arguments  : variable length arguments
*
* Returns    : None
*
**************************************************************************************************************
*/

void  UART_Printf (const  uint8_t *format, ...)
{
    static  uint8_t  buffer[40 + 1];
            va_list     vArgs;

    va_start(vArgs, format);
    vsprintf((char *)buffer, (char const *)format, vArgs);
    va_end(vArgs);
    UART_PrintStr((uint8_t *) buffer);
}
