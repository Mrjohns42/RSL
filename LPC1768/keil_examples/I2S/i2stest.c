/****************************************************************************
 *   $Id:: i2stest.c 6097 2011-01-07 04:31:25Z nxp12832                     $
 *   Project: NXP LPC17xx I2S example
 *
 *   Description:
 *     This file contains I2S test modules, main entry, to test I2S APIs.
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
#include "i2s.h"
#include "dma.h"

extern volatile uint8_t *I2STXBuffer, *I2SRXBuffer;
extern volatile uint32_t I2SReadLength;
extern volatile uint32_t I2SWriteLength;
extern volatile uint32_t I2SRXDone, I2STXDone;
extern volatile uint32_t I2SDMA0Done, I2SDMA1Done;

/*******************************************************************************
**   Main Function  main()
*******************************************************************************/
int main (void)
{
  uint32_t i;

  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemClockUpdate();

  /* The test program is to connect CLK, WS, and SDA lines between the
  TX and RX. DAO is the master output, DAI is the slave input. This test
  is not a real-world application but test both TX and RX channel in
  a single program. However, due to the sync issue, the TX will start with 
  MUTE data 0 once it starts while RX is not ready. So, it may begins
  with some MUTE value in the beginning of the RX buffer. This is beyond
  control unless throw away the very first few bytes in the RX buffer. 
  In this program, the very first byte is not compared between the TX buffer 
  and RX buffer due to this synchonization issue. */

  /* Configure temp register before reading */
  for ( i = 0; i < BUFSIZE; i++ )	/* clear buffer */
  {
	I2STXBuffer[i] = i;
	I2SRXBuffer[i] = 0;
  }

  I2SInit();				/* initialize I2S */
 
#if I2S_DMA_ENABLED
  DMA_Init();

  /* Select secondary function(I2S) in DMA channels */
  LPC_SC->DMAREQSEL = (0x1<<DMA_I2S_REQ0)|(0x1<<DMA_I2S_REQ1);

  /* on DMA channel 0, Source is memory, destination is I2S TX FIFO, 
  on DMA channel 1, source is I2S RX FIFO, Destination is memory */
  /* Enable channel and IE bit */
  DMAChannel_Init( 0, M2P );
  LPC_GPDMACH0->CConfig |= (0x18001|(0x00<<1)|(DMA_I2S_REQ0<<6)|(0x01<<11));
  DMAChannel_Init( 1, P2M );
  LPC_GPDMACH1->CConfig |= (0x08001|(DMA_I2S_REQ1<<1)|(0x00<<6)|(0x02<<11));
 
  NVIC_EnableIRQ(DMA_IRQn);

  I2SStart();	  

  LPC_I2S->DMA2 = (0x01<<0) | (0x08<<8);	/* Channel 2 is for RX, enable RX first. */
  LPC_I2S->DMA1 = (0x01<<1) | (0x01<<16);	/* Channel 1 is for TX. */

  /* Wait for both DMA0 and DMA1 to finish before verifying. */
  while ( !I2SDMA0Done || !I2SDMA1Done );
#else
  /* Not DMA mode, enable I2S interrupts. */
  NVIC_EnableIRQ(I2S_IRQn);

  /* RX FIFO depth is 1, TX FIFO depth is 8. */
  I2SStart();
  LPC_I2S->IRQ = (8 << 16) | (1 << 8) | (0x01 << 0);

  while ( I2SWriteLength < BUFSIZE )
  {
	while (((LPC_I2S->STATE >> 16) & 0xFF) == TXFIFO_FULL);
	LPC_I2S->TXFIFO = I2STXBuffer[I2SWriteLength++];
  }

  I2STXDone = 1;
  /* Wait for RX and TX complete before comparison */
  while ( !I2SRXDone || !I2STXDone );
#endif

  /* Validate TX and RX buffer */
  for ( i=1; i<BUFSIZE; i++ )
  {
	if ( I2SRXBuffer[i] != I2STXBuffer[i-1] )
	{
	  while ( 1 );	/* Validation error */
	}
  }
  while ( 1 );	/* Don't exit from main when finishing. */
}

/******************************************************************************
**                            End Of File
******************************************************************************/
