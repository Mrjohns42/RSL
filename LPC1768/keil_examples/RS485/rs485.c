/****************************************************************************
 *   $Id:: rs485.c 6097 2011-01-07 04:31:25Z nxp12832                       $
 *   Project: NXP LPC17xx RS485 example
 *
 *   Description:
 *     This file contains RS485 code example which include RS485 
 *     initialization, RS485 interrupt handler, and APIs for RS485 access.
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
#include "rs485.h"

volatile uint32_t UARTStatus;
volatile uint8_t  UARTTxEmpty = 1;
volatile uint8_t  UARTBuffer[BUFSIZE];
volatile uint32_t UARTCount = 0;
volatile uint32_t garbageCount = 0;


/*****************************************************************************
** Function name:		UART1_IRQHandler
**
** Descriptions:		UART interrupt handler
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void UART1_IRQHandler(void)
{
  volatile uint8_t IIRValue, LSRValue;
  volatile uint8_t Dummy = Dummy;

  IIRValue = LPC_UART1->IIR;
    
  IIRValue >>= 1;			/* skip pending bit in IIR */
  IIRValue &= 0x07;			/* check bit 1~3, interrupt identification */
  if (IIRValue == IIR_RLS)		/* Receive Line Status */
  {
    LSRValue = LPC_UART1->LSR;
    /* Receive Line Status */
	if ( ((LSRValue & (LSR_PE|LSR_RDR)) == (LSR_PE|LSR_RDR)) 
						&& (LPC_UART1->RS485CTRL & RS485_NMMEN) )
	{
	  Dummy = LPC_UART1->RBR;
	  /* if address match, enable RX, otherwise, disable RX. */
	  if ( Dummy == LPC_UART1->ADRMATCH )
	  {
		LPC_UART1->RS485CTRL &= ~RS485_RXDIS;	/* Enable RX */
	  }
	  else
	  {
		LPC_UART1->RS485CTRL |= RS485_RXDIS;		/* Disable RX */
		while ( LPC_UART1->LSR & LSR_RDR ){
		  Dummy = LPC_UART1->RBR;	/* Clear data from RX FIFO */
		  garbageCount++;
		}
		return;
	  }
	}
    else if (LSRValue & (LSR_OE | LSR_PE | LSR_FE | LSR_RXFE | LSR_BI))
    {
      /* There are errors or break interrupt */
      /* Read LSR will clear the interrupt */
      UARTStatus = LSRValue;
      Dummy = LPC_UART1->RBR;/* Dummy read on RX to clear 
							interrupt, then bail out */
      return;
    }
    else if (LSRValue & LSR_RDR)	/* Receive Data Ready */			
    {
      /* If no error on RLS, normal ready, save into the data buffer. */
      /* Note: read RBR will clear the interrupt */
      UARTBuffer[UARTCount++] = LPC_UART1->RBR;
      if (UARTCount == BUFSIZE)
      {
        UARTCount = 0;		/* buffer overflow */
      }	
    }
  }
  else if (IIRValue == IIR_RDA)	/* Receive Data Available */
  {
    /* Receive Data Available */
    UARTBuffer[UARTCount++] = LPC_UART1->RBR;
    if (UARTCount == BUFSIZE)
    {
      UARTCount = 0;		/* buffer overflow */
    }
  }
  else if (IIRValue == IIR_CTI)	/* Character timeout indicator */
  {
    /* Character Time-out indicator */
    UARTStatus |= 0x100;		/* Bit 9 as the CTI error */
  }
  else if (IIRValue == IIR_THRE)	/* THRE, transmit holding register empty */
  {
    /* THRE interrupt */
    LSRValue = LPC_UART1->LSR;		/* Check status in the LSR to see if
							valid data in U0THR or not */
    if (LSRValue & LSR_THRE)
    {
      UARTTxEmpty = 1;
    }
    else
    {
      UARTTxEmpty = 0;
    }
  }
  return;
}

/*****************************************************************************
** Function name:		RS485Init
**
** Descriptions:		Initialize UART0 port, setup pin select,
**				clock, parity, stop bits, FIFO, etc.
**
** parameters:			UART baudrate
** Returned value:		None
** 
*****************************************************************************/
void RS485Init(uint32_t baudrate)
{
  uint32_t Fdiv;
  uint32_t regVal = regVal;
  uint32_t pclkdiv, pclk;

  UARTCount = 0;

  NVIC_DisableIRQ(UART1_IRQn);

  LPC_PINCON->PINSEL4 &= ~0x0000000F;
  LPC_PINCON->PINSEL4 |= 0x0000000A;	/* Enable RxD1 P2.1, TxD1 P2.0 */
	
  /* By default, the PCLKSELx value is zero, thus, the PCLK for
  all the peripherals is 1/4 of the SystemFrequency. */
  /* Bit 8,9 are for UART1 */
  pclkdiv = (LPC_SC->PCLKSEL0 >> 8) & 0x03;
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

  LPC_UART1->LCR = 0x83;             /* 8 bits, no Parity, 1 Stop bit */
  Fdiv = ( pclk / 16 ) / baudrate ;	/*baud rate */
  LPC_UART1->DLM = Fdiv / 256;							
  LPC_UART1->DLL = Fdiv % 256;
  LPC_UART1->LCR = 0x03;		/* DLAB = 0 */
  LPC_UART1->FCR = 0x07;		/* Enable and reset TX and RX FIFO. */

  /* Read to clear the line status. */
  regVal = LPC_UART1->LSR;
  /* Ensure a clean start, no data in either TX or RX FIFO. */
  while ( LPC_UART1->LSR & (LSR_THRE|LSR_TEMT) != (LSR_THRE|LSR_TEMT) );
  while ( LPC_UART1->LSR & LSR_RDR )
  {
	regVal = LPC_UART1->RBR;	/* Dump data from RX FIFO */
  }

  LPC_UART1->LCR |= ((0x3<<4)|(0x1<<3));	/* forced "0" sticky parity, parity enable */
  LPC_UART1->ADRMATCH = RS485_SLAVE_ADR;	/* UARTA 485 address, 0xC0 */
#if RS485_NMM
  LPC_UART1->RS485CTRL = RS485_NMMEN;		/* NMM mode */
#else
  LPC_UART1->RS485CTRL = RS485_NMMEN|RS485_AADEN|RS485_RXDIS;	/* AAD mode */
#endif

  /* Below is the direction control setting. */
#if DIR_CTRL
  /* Either DTR or RTS can be used as direction pin out. */
#if 1
  LPC_PINCON->PINSEL1 &= ~(0x03<<8);
  LPC_PINCON->PINSEL1 |= (0x01<<8);		/* DTR1 as direction control */
  LPC_UART1->RS485CTRL |= RS485_DCTRL;
#else
  LPC_PINCON->PINSEL1 &= ~(0x03<<12);
  LPC_PINCON->PINSEL1 |= (0x01<<12);	/* RTS1 as direction control */
  LPC_UART1->RS485CTRL |= (RS485_DCTRL|RS485_SEL);
#endif
#endif

  /* Enable UART1 Interrupt */
  NVIC_EnableIRQ(UART1_IRQn);
  LPC_UART1->IER = IER_RBR | IER_THRE | IER_RLS;	/* Enable UART interrupt */
  return;
}

/*****************************************************************************
** Function name:		RS485Send
**
** Descriptions:		Send a block of data to the UART 0 port based
**				on the data length
**
** parameters:		buffer pointer, and data length
** Returned value:	None
** 
*****************************************************************************/
void RS485Send(uint8_t *BufferPtr, uint32_t Length)
{
  uint32_t i;
  
  for ( i = 0; i < Length; i++ )
  {
	/* THRE status, contain valid data */
    while ( !(UARTTxEmpty & 0x01) );
    LPC_UART1->THR = *BufferPtr;
    UARTTxEmpty = 0;	/* not empty in the THR until it shifts out */
	/* At this point, bit 5 of LCR is always 1 for RS485 test */
	if ( i == 0 )
	  LPC_UART1->LCR &= ~(0x1<<4);		/* Sticky parity to 1 */
	else
	  LPC_UART1->LCR |= (0x1<<4);		/* Sticky parity to 0 */
	LPC_UART1->LCR |= (0x1<<3);
	BufferPtr++;
  }
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
