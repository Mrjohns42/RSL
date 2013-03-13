/****************************************************************************
 *   $Id:: udmatest.c 6098 2011-01-08 02:26:20Z nxp12832                    $
 *   Project: NXP LPC17xx UART DMA example
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
#include "LPC17xx.h"
#include "uart_dma.h"
#include "dma.h"

extern volatile uint32_t UARTDMA0Done;
extern volatile uint32_t UARTDMA1Done;
extern volatile uint32_t UARTDMA2Done;
extern volatile uint32_t UARTDMA3Done;
volatile uint8_t *u0_src, *u0_dst, *u1_src, *u1_dst;

/*****************************************************************************
** Function name:		ConvertDigital
**
** Descriptions:		Convert Digital to ASCII
**
** parameters:			digit
** Returned value:		ASCII char
** 
*****************************************************************************/
static uint8_t ConvertDigital ( uint8_t digit )
{
  static uint8_t hex[] = "0123456789ABCDEF";
  return hex[digit & 0xf];
}

/******************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void) {
  uint32_t i;

  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemClockUpdate();

  UARTInit(0, 115200);
  UARTInit(1, 115200);

  u0_src = (uint8_t *)UART0_DMA_TX_SRC;
  u0_dst = (uint8_t *)UART0_DMA_RX_DST;
  u1_src = (uint8_t *)UART1_DMA_TX_SRC;
  u1_dst = (uint8_t *)UART1_DMA_RX_DST;
  for ( i = 0; i < UART_DMA_SIZE; i++ )
  {
	*u0_src++ = *u1_src++ = ConvertDigital(i);
	*u0_dst++ = *u1_dst++ = 0;
  }

  DMA_Init();

  /* Connect UART0 and UART1 via a RS232 cable. The sequence is, UART DMA starts 
  from UART0 TX, lookback to UART1 RX using channel 0 and 1. Then, UART DMA starts
  from UART1 TX, lookback to UART0 RX using channel 2 and 3.
  So, both M2P+P2M on UART0 and UART1 are tested. */

  /* DMA from UART0 TX to UART1 RX using channel 0 and 1. */
  DMAChannel_Init( 0, M2P );
  LPC_GPDMACH0->CConfig |= 0x0C001|(0x00<<1)|(DMA_UART0_TX<<6)|(0x01 << 11);
  LPC_UART0->FCR |= 0x08;

  /* DMA request was sent inside DMA ISR when DMA is done.
  The data should arrive at UART1 RX already. */
  while ( !UARTDMA0Done );
  UARTDMA0Done = 0;
  while ( !UARTDMA1Done );
  UARTDMA1Done = 0;

  /* Between UART0 TX and UART1 RX, the data should match, or fatal DMA error. */
  u1_src = (uint8_t *)UART0_DMA_TX_SRC;
  u1_dst = (uint8_t *)UART1_DMA_RX_DST;
  for ( i = 0; i < UART_DMA_SIZE; i++ )
  {
	if ( *u1_src++ != *u1_dst++ )
	{
	  while ( 1 );
	}
  }

  /* DMA from UART1 TX to UART0 RX using channel 2 and 3. */
  DMAChannel_Init( 2, M2P );
  NVIC_EnableIRQ(DMA_IRQn);
  LPC_GPDMACH2->CConfig |= 0x0C001|(0x00<<1)|(DMA_UART1_TX<<6)|(0x01 << 11);
  LPC_UART1->FCR |= 0x08;

  /* DMA request was sent inside DMA ISR when DMA is done.
  The data should arrive at UART1 RX already. */
  while ( !UARTDMA2Done );
  UARTDMA2Done = 0;
  while ( !UARTDMA3Done );
  UARTDMA3Done = 0;
  LPC_GPDMA->Sync = (0x1<<DMA_UART0_RX)|(0x1<<DMA_UART1_TX);

  /* The data is transferred from M2U0TX-U1RX-, M2U1TX-U0RX. */
  /* Between UART0 TX and UART0 RX, the data should match, or fatal DMA error. */
  u1_src = (uint8_t *)UART1_DMA_TX_SRC;
  u1_dst = (uint8_t *)UART0_DMA_RX_DST;
  for ( i = 0; i < UART_DMA_SIZE; i++ )
  {
	if ( *u1_src++ != *u1_dst++ )
	{
	  while ( 1 );
	}
  }
  while ( 1 );
  return(0);
}
