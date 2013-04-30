/*****************************************************************************
 *   uart.c:  UART API file for NXP LPC11xx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.12.07  ver 1.00    Preliminary version, first Release
 *
******************************************************************************/
#include "driver_config.h"
#if CONFIG_ENABLE_DRIVER_UART==1
#include "uart.h"

volatile uint32_t UARTStatus;
volatile uint8_t  UARTTxEmpty = 1;
volatile uint8_t  UARTBuffer[UARTBUFSIZE];
volatile uint32_t UARTCount = 0;

#if CONFIG_UART_DEFAULT_UART_IRQHANDLER==1
/*****************************************************************************
** Function name:		UART_IRQHandler
**
** Descriptions:		UART interrupt handler
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void UART_IRQHandler(void)
{
  uint8_t IIRValue, LSRValue;
  uint8_t Dummy = Dummy;

  IIRValue = LPC_UART->IIR;
    
  IIRValue >>= 1;			/* skip pending bit in IIR */
  IIRValue &= 0x07;			/* check bit 1~3, interrupt identification */
  if (IIRValue == IIR_RLS)		/* Receive Line Status */
  {
    LSRValue = LPC_UART->LSR;
    /* Receive Line Status */
    if (LSRValue & (LSR_OE | LSR_PE | LSR_FE | LSR_RXFE | LSR_BI))
    {
      /* There are errors or break interrupt */
      /* Read LSR will clear the interrupt */
      UARTStatus = LSRValue;
      Dummy = LPC_UART->RBR;	/* Dummy read on RX to clear 
								interrupt, then bail out */
      return;
    }
    if (LSRValue & LSR_RDR)	/* Receive Data Ready */			
    {
      /* If no error on RLS, normal ready, save into the data buffer. */
      /* Note: read RBR will clear the interrupt */
      UARTBuffer[UARTCount++] = LPC_UART->RBR;
      if (UARTCount == UARTBUFSIZE)
      {
        UARTCount = 0;		/* buffer overflow */
      }	
    }
  }
  else if (IIRValue == IIR_RDA)	/* Receive Data Available */
  {
    /* Receive Data Available */
    UARTBuffer[UARTCount++] = LPC_UART->RBR;
    if (UARTCount == UARTBUFSIZE)
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
    LSRValue = LPC_UART->LSR;		/* Check status in the LSR to see if
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
#endif

/*****************************************************************************
** Function name:		ModemInit
**
** Descriptions:		Initialize UART0 port as modem, setup pin select.
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void ModemInit( void )
{
  LPC_IOCON->PIO2_0 &= ~0x07;    /* UART I/O config */
  LPC_IOCON->PIO2_0 |= 0x01;     /* UART DTR */
  LPC_IOCON->PIO0_7 &= ~0x07;    /* UART I/O config */
  LPC_IOCON->PIO0_7 |= 0x01;     /* UART CTS */
  LPC_IOCON->PIO1_5 &= ~0x07;    /* UART I/O config */
  LPC_IOCON->PIO1_5 |= 0x01;     /* UART RTS */
#if 1 
  LPC_IOCON->DSR_LOC	= 0;
  LPC_IOCON->PIO2_1 &= ~0x07;    /* UART I/O config */
  LPC_IOCON->PIO2_1 |= 0x01;     /* UART DSR */

  LPC_IOCON->DCD_LOC	= 0;
  LPC_IOCON->PIO2_2 &= ~0x07;    /* UART I/O config */
  LPC_IOCON->PIO2_2 |= 0x01;     /* UART DCD */

  LPC_IOCON->RI_LOC	= 0;
  LPC_IOCON->PIO2_3 &= ~0x07;    /* UART I/O config */
  LPC_IOCON->PIO2_3 |= 0x01;     /* UART RI */

#else
  LPC_IOCON->DSR_LOC = 1;
  LPC_IOCON->PIO3_1 &= ~0x07;    /* UART I/O config */
  LPC_IOCON->PIO3_1 |= 0x01;     /* UART DSR */

  LPC_IOCON->DCD_LOC = 1;
  LPC_IOCON->PIO3_2 &= ~0x07;    /* UART I/O config */
  LPC_IOCON->PIO3_2 |= 0x01;     /* UART DCD */

  LPC_IOCON->RI_LOC = 1;
  LPC_IOCON->PIO3_3 &= ~0x07;    /* UART I/O config */
  LPC_IOCON->PIO3_3 |= 0x01;     /* UART RI */
#endif
  LPC_UART->MCR = 0xC0;          /* Enable Auto RTS and Auto CTS. */			
  return;
}

/*****************************************************************************
** Function name:		UARTInit
**
** Descriptions:		Initialize UART0 port, setup pin select,
**				clock, parity, stop bits, FIFO, etc.
**
** parameters:			UART baudrate
** Returned value:		None
** 
*****************************************************************************/
void UARTInit(uint32_t baudrate)
{
  uint32_t regVal;

  UARTTxEmpty = 1;
  UARTCount = 0;
  
  NVIC_DisableIRQ(UART_IRQn);

   /* configure PINs GPIO1.6, GPIO1.7, GPIO 1.8 for UART */
  LPC_SYSCON->SYSAHBCLKCTRL |= ((1UL <<  6) |   /* enable clock for GPIO      */
                                (1UL << 16) );  /* enable clock for IOCON     */

  LPC_IOCON->PIO1_6 &= ~0x07;    /*  UART I/O config */
  LPC_IOCON->PIO1_6 |= 0x01;     /* UART RXD */
  LPC_IOCON->PIO1_7 &= ~0x07;	
  LPC_IOCON->PIO1_7 |= 0x01;     /* UART TXD */
  LPC_GPIO1->DIR    |=  (1UL <<  8);			/* Set Dir pin to ouput       */
  LPC_GPIO1->DATA   &= ~(1UL <<  8);            /* Initialize Dir to RX mode  */

  if(baudrate == _1MHZ)
  {
	  /* configure UART0 */
	  //Baud = PCLK/(16*(DLL+DLM<<8)*(1+DIV/MUL))
	  LPC_SYSCON->SYSAHBCLKCTRL |=  (1UL << 12);    /* Enable clock to UART       */
	  LPC_SYSCON->UARTCLKDIV     =  (2UL <<  0);    /* UART clock =  CCLK / 2  = 24MHz   */
	
	  LPC_UART->LCR = 0x83;                   /* 8 bits, no Parity, 1 Stop bit    */
	  LPC_UART->DLL = 1;                      /* 1,000,000 Baud Rate @ 24.0 MHZ PCLK */
	  LPC_UART->FDR = 0x21;                   /* FR 1.5, DIVADDVAL 1, MULVAL 2  */
	  LPC_UART->DLM = 0;                      /* High divisor latch = 0           */
	  LPC_UART->LCR = 0x03;                   /* DLAB = 0                         */
  }
  else
  {
	  /* configure UART0 */
	  LPC_SYSCON->SYSAHBCLKCTRL |=  (1UL << 12);    /* Enable clock to UART       */
	  LPC_SYSCON->UARTCLKDIV     =  (4UL <<  0);    /* UART clock =  CCLK / 4     */
	
	  LPC_UART->LCR = 0x83;                   /* 8 bits, no Parity, 1 Stop bit    */
	  LPC_UART->DLL = 4;                      /* 115200 Baud Rate @ 12.0 MHZ PCLK */
	  LPC_UART->FDR = 0x85;                   /* FR 1.627, DIVADDVAL 5, MULVAL 8  */
	  LPC_UART->DLM = 0;                      /* High divisor latch = 0           */
	  LPC_UART->LCR = 0x03;                   /* DLAB = 0                         */
  }
  LPC_UART->FCR = 0x07;		/* Enable and reset TX and RX FIFO. */ 

  /* Read to clear the line status. */
  regVal = LPC_UART->LSR;

  /* Ensure a clean start, no data in either TX or RX FIFO. */
// CodeRed - added parentheses around comparison in operand of &
  while (( LPC_UART->LSR & (LSR_THRE|LSR_TEMT)) != (LSR_THRE|LSR_TEMT) );
  while ( LPC_UART->LSR & LSR_RDR )
  {
	regVal = LPC_UART->RBR;	/* Dump data from RX FIFO */
  }
 
  /* Enable the UART Interrupt */
  NVIC_EnableIRQ(UART_IRQn);

#if CONFIG_UART_ENABLE_INTERRUPT==1
#if CONFIG_UART_ENABLE_TX_INTERRUPT==1
  LPC_UART->IER = IER_RBR | IER_THRE | IER_RLS;	/* Enable UART RX/TX interrupt */
#else
  LPC_UART->IER = IER_RBR | IER_RLS;	/* Enable RX UART interrupt */
#endif
#endif
  return;
}

/*****************************************************************************
** Function name:		UARTSend
**
** Descriptions:		Send a block of data to the UART 0 port based
**				on the data length
**
** parameters:		buffer pointer, and data length
** Returned value:	None
** 
*****************************************************************************/
void UARTSend(uint8_t *BufferPtr, uint32_t Length)
{
  
  while ( Length != 0 )
  {
	  /* THRE status, contain valid data */
#if CONFIG_UART_ENABLE_TX_INTERRUPT==1
	  /* Below flag is set inside the interrupt handler when THRE occurs. */
      while ( !(UARTTxEmpty & 0x01) );
	  LPC_UART->THR = *BufferPtr;
      UARTTxEmpty = 0;	/* not empty in the THR until it shifts out */
#else
	  while ( !(LPC_UART->LSR & LSR_THRE) );
	  LPC_UART->THR = *BufferPtr;
#endif
      BufferPtr++;
      Length--;
  }
  return;
}

int UARTAvailable(void)
{
	return UARTCount;  
}

int UARTPeek(void)
{
 	return UARTBuffer[0];
}



/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int sendchar (int c) {
  
  while (!(LPC_UART->LSR & 0x20));
  LPC_UART->THR = c;
  
  return (c);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int getkey (void) {
  int key, i;	
  if(UARTCount < 1) return -1;

  key = UARTBuffer[0];	  
  for(i=0;i<UARTCount;i++) UARTBuffer[i] = UARTBuffer[i+1];
  UARTCount--;
  
  return key;
}			  

#endif

/******************************************************************************
**                            End Of File
******************************************************************************/
