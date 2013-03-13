/*****************************************************************************
 *   uart.c:  UART API file for NXP LPC17xx Family Microprocessors
 *
 *   Copyright(C) 2009, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.05.27  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include <lpc17xx.h>
#include "type.h"
#include "uart.h"

volatile uint32_t UART0Status, UART1Status;
volatile uint8_t UART0TxEmpty = 1, UART1TxEmpty = 1;
volatile uint8_t UART0Buffer[BUFSIZE], UART1Buffer[BUFSIZE];
volatile uint32_t UART0Count = 0, UART1Count = 0;

/*****************************************************************************
** Function name:		UART0Handler
**
** Descriptions:		UART0 interrupt handler
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void UART0_IRQHandler (void) 
{
  uint8_t IIRValue, LSRValue;
  uint8_t Dummy = Dummy;
	
  IIRValue = LPC_UART0->IIR;
    
  IIRValue >>= 1;			/* skip pending bit in IIR */
  IIRValue &= 0x07;			/* check bit 1~3, interrupt identification */
  if ( IIRValue == IIR_RLS )		/* Receive Line Status */
  {
	LSRValue = LPC_UART0->LSR;
	/* Receive Line Status */
	if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
	{
	  /* There are errors or break interrupt */
	  /* Read LSR will clear the interrupt */
	  UART0Status = LSRValue;
	  Dummy = LPC_UART0->RBR;		/* Dummy read on RX to clear 
							interrupt, then bail out */
	  return;
	}
	if ( LSRValue & LSR_RDR )	/* Receive Data Ready */			
	{
	  /* If no error on RLS, normal ready, save into the data buffer. */
	  /* Note: read RBR will clear the interrupt */
	  UART0Buffer[UART0Count] = LPC_UART0->RBR;
	  UART0Count++;
	  if ( UART0Count == BUFSIZE )
	  {
		UART0Count = 0;		/* buffer overflow */
	  }	
	}
  }
  else if ( IIRValue == IIR_RDA )	/* Receive Data Available */
  {
	/* Receive Data Available */
	UART0Buffer[UART0Count] = LPC_UART0->RBR;
	UART0Count++;
	if ( UART0Count == BUFSIZE )
	{
	  UART0Count = 0;		/* buffer overflow */
	}
  }
  else if ( IIRValue == IIR_CTI )	/* Character timeout indicator */
  {
	/* Character Time-out indicator */
	UART0Status |= 0x100;		/* Bit 9 as the CTI error */
  }
  else if ( IIRValue == IIR_THRE )	/* THRE, transmit holding register empty */
  {
	/* THRE interrupt */
	LSRValue = LPC_UART0->LSR;		/* Check status in the LSR to see if
							valid data in U0THR or not */
	if ( LSRValue & LSR_THRE )
	{
	  UART0TxEmpty = 1;
	}
	else
	{
	  UART0TxEmpty = 0;
	}
  }
    
}

/*****************************************************************************
** Function name:		UART1Handler
**
** Descriptions:		UART1 interrupt handler
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void UART1_IRQHandler (void) 
{
  uint8_t IIRValue, LSRValue;
  uint8_t Dummy = Dummy;
	
  IIRValue = LPC_UART1->IIR;
    
  IIRValue >>= 1;			/* skip pending bit in IIR */
  IIRValue &= 0x07;			/* check bit 1~3, interrupt identification */
  if ( IIRValue == IIR_RLS )		/* Receive Line Status */
  {
	LSRValue = LPC_UART1->LSR;
	/* Receive Line Status */
	if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
	{
	  /* There are errors or break interrupt */
	  /* Read LSR will clear the interrupt */
	  UART1Status = LSRValue;
	  Dummy = LPC_UART1->RBR;		/* Dummy read on RX to clear 
								interrupt, then bail out */
	  return;
	}
	if ( LSRValue & LSR_RDR )	/* Receive Data Ready */			
	{
	  /* If no error on RLS, normal ready, save into the data buffer. */
	  /* Note: read RBR will clear the interrupt */
	  UART1Buffer[UART1Count] = LPC_UART1->RBR;
	  UART1Count++;
	  if ( UART1Count == BUFSIZE )
	  {
		UART1Count = 0;		/* buffer overflow */
	  }	
	}
  }
  else if ( IIRValue == IIR_RDA )	/* Receive Data Available */
  {
	/* Receive Data Available */
	UART1Buffer[UART1Count] = LPC_UART1->RBR;
	UART1Count++;
	if ( UART1Count == BUFSIZE )
	{
	  UART1Count = 0;		/* buffer overflow */
	}
  }
  else if ( IIRValue == IIR_CTI )	/* Character timeout indicator */
  {
	/* Character Time-out indicator */
	UART1Status |= 0x100;		/* Bit 9 as the CTI error */
  }
  else if ( IIRValue == IIR_THRE )	/* THRE, transmit holding register empty */
  {
	/* THRE interrupt */
	LSRValue = LPC_UART1->LSR;		/* Check status in the LSR to see if
								valid data in U0THR or not */
	if ( LSRValue & LSR_THRE )
	{
	  UART1TxEmpty = 1;
	}
	else
	{
	  UART1TxEmpty = 0;
	}
  }

}

/*****************************************************************************
** Function name:		UARTInit
**
** Descriptions:		Initialize UART0 port, setup pin select,
**						clock, parity, stop bits, FIFO, etc.
**
** parameters:			portNum(0 or 1) and UART baudrate
** Returned value:		true or false, return false only if the 
**						interrupt handler can't be installed to the 
**						VIC table
** 
*****************************************************************************/
uint32_t UARTInit( uint32_t PortNum, uint32_t baudrate )
{
  uint32_t Fdiv;

  if ( PortNum == 0 )
  {
	LPC_PINCON->PINSEL0 = 0x00000050;       /* RxD0 is P0.3 and TxD0 is P0.2 */

    LPC_UART0->LCR = 0x83;		/* 8 bits, no Parity, 1 Stop bit */
	/* By default, the PCLKSELx value is zero, thus, the PCLK for
	all the peripherals is 1/4 of the SystemFrequency. */ 
    Fdiv = ( SystemFrequency/4/16 ) / baudrate ;	/*baud rate, Fpclk: 18MHz */
    LPC_UART0->DLM = Fdiv / 256;							
    LPC_UART0->DLL = Fdiv % 256;
	LPC_UART0->LCR = 0x03;		/* DLAB = 0 */
    LPC_UART0->FCR = 0x07;		/* Enable and reset TX and RX FIFO. */

   	NVIC_EnableIRQ(UART0_IRQn);

    LPC_UART0->IER = IER_RBR | IER_THRE | IER_RLS;	/* Enable UART0 interrupt */
    return (TRUE);
  }
  else if ( PortNum == 1 )
  {
	LPC_PINCON->PINSEL4 |= 0x0000000A;	/* Enable RxD1 P2.1, TxD1 P2.0 */
	
    LPC_UART1->LCR = 0x83;		/* 8 bits, no Parity, 1 Stop bit */
	/* By default, the PCLKSELx value is zero, thus, the PCLK for
	all the peripherals is 1/4 of the SystemFrequency. */ 
    Fdiv = ( SystemFrequency/4/16 ) / baudrate ;	/*baud rate */
    LPC_UART1->DLM = Fdiv / 256;							
    LPC_UART1->DLL = Fdiv % 256;
	LPC_UART1->LCR = 0x03;		/* DLAB = 0 */
    LPC_UART1->FCR = 0x07;		/* Enable and reset TX and RX FIFO. */

   	NVIC_EnableIRQ(UART1_IRQn);

    LPC_UART1->IER = IER_RBR | IER_THRE | IER_RLS;	/* Enable UART1 interrupt */
    return (TRUE);
  }
  return( FALSE ); 
}

/*****************************************************************************
** Function name:		UARTSend
**
** Descriptions:		Send a block of data to the UART 0 port based
**						on the data length
**
** parameters:			portNum, buffer pointer, and data length
** Returned value:		None
** 
*****************************************************************************/
void UARTSend( uint32_t portNum, uint8_t *BufferPtr, uint32_t Length )
{
  if ( portNum == 0 )
  {
    while ( Length != 0 )
    {
	  /* THRE status, contain valid data */
	  while ( !(UART0TxEmpty & 0x01) );	
	  LPC_UART0->THR = *BufferPtr;
	  UART0TxEmpty = 0;	/* not empty in the THR until it shifts out */
	  BufferPtr++;
	  Length--;
	}
  }
  else
  {
	while ( Length != 0 )
    {
	  /* THRE status, contain valid data */
	  while ( !(UART1TxEmpty & 0x01) );	
	  LPC_UART1->THR = *BufferPtr;
	  UART1TxEmpty = 0;	/* not empty in the THR until it shifts out */
	  BufferPtr++;
	  Length--;
    }
  }
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
