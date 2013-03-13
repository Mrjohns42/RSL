/****************************************************************************
 *   $Id:: dma.c 5797 2010-12-03 00:27:00Z usb00423                         $
 *   Project: NXP LPC17xx DMA for I2S example
 *
 *   Description:
 *     This file contains DMA code example which include DMA initialization, 
 *     DMA interrupt handler, and APIs for DMA access.
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

#if I2S_DMA_ENABLED
volatile uint32_t DMATCCount = 0;
volatile uint32_t DMAErrCount = 0;
volatile uint32_t I2SDMA0Done = 0;
volatile uint32_t I2SDMA1Done = 0;

/******************************************************************************
** Function name:		DMA_IRQHandler
**
** Descriptions:		DMA interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void DMA_IRQHandler(void) 
{
  uint32_t regVal;

  regVal = LPC_GPDMA->IntTCStat;
  if ( regVal )
  {
	DMATCCount++;
	LPC_GPDMA->IntTCClear |= regVal;
	if ( regVal & 0x01 )
	{
	  I2SDMA0Done = 1;
	}
	else if ( regVal & 0x02 )
	{
	  I2SDMA1Done = 1;
	}
  } 

  regVal = LPC_GPDMA->IntErrStat;
  if ( regVal )
  {
	DMAErrCount++;
	LPC_GPDMA->IntErrClr |= regVal;
  }

}

/******************************************************************************
** Function name:		DMA_Init
**
** Descriptions:		clock to GPDMA in PCONP, GPDMA init before channel init		
**
** parameters:			None			
** Returned value:		None		
** 
******************************************************************************/
void DMA_Init( void )
{
  /* Enable CLOCK into GPDMA controller */
  LPC_SC->PCONP |= (1 << 29);

  LPC_GPDMA->IntTCClear = 0x03;
  LPC_GPDMA->IntErrClr = 0x03;

  LPC_GPDMA->Config = 0x01;	/* Enable DMA channels, little endian */
  while ( !(LPC_GPDMA->Config & 0x01) );

  return;
}

/******************************************************************************
** Function name:		DMAChannel_Init
**
** Descriptions:		
**
** parameters:			
** Returned value:		
** 
******************************************************************************/
uint32_t DMAChannel_Init( uint32_t ChannelNum, uint32_t DMAMode )
{
  if ( ChannelNum == 0 )
  {
	I2SDMA0Done = 0;
	LPC_GPDMA->IntTCClear = 0x01;   
	if ( DMAMode == M2P )
	{
	  /* Ch0 set for M2P transfer from mempry to I2S TX FIFO. */
	  LPC_GPDMACH0->CSrcAddr = DMA_SRC;
	  LPC_GPDMACH0->CDestAddr = DMA_I2S_TX_FIFO;
	  /* The burst size is set to 1. Terminal Count Int enable */
	  LPC_GPDMACH0->CControl = (DMA_SIZE & 0x0FFF) | (0x00 << 12) | (0x00 << 15)
		| (1 << 26) | 0x80000000;
	}
	else if ( DMAMode == P2M )
	{
	  /* Ch0 set for P2M transfer from I2S RX FIFO to memory. */
	  LPC_GPDMACH0->CSrcAddr = DMA_I2S_RX_FIFO;
	  LPC_GPDMACH0->CDestAddr = DMA_DST;
	  /* The burst size is set to 1. Terminal Count Int enable. */
	  LPC_GPDMACH0->CControl = (DMA_SIZE & 0x0FFF) | (0x00 << 12) | (0x00 << 15)
		| (1 << 27) | 0x80000000;
	}
	else if ( DMAMode == P2P )
	{
	  /* Ch0 set for P2P transfer from I2S DAO to I2S DAI. */
	  LPC_GPDMACH0->CSrcAddr = DMA_I2S_TX_FIFO;
	  LPC_GPDMACH0->CDestAddr = DMA_I2S_RX_FIFO;
	  /* The burst size is set to 32. */
	  LPC_GPDMACH0->CControl = (DMA_SIZE & 0x0FFF) | (0x04 << 12) | (0x04 << 15) 
		| 0x80000000;
	}
	else
	{
	  return ( FALSE );
	}
  }
  else if ( ChannelNum == 1 )
  {   
	I2SDMA1Done = 0;   
	LPC_GPDMA->IntTCClear = 0x02;
	if ( DMAMode == M2P )
	{
	  /* Ch1 set for M2P transfer from mempry to I2S TX FIFO. */
	  LPC_GPDMACH1->CSrcAddr = DMA_SRC;
	  LPC_GPDMACH1->CDestAddr = DMA_I2S_TX_FIFO;
	  /* The burst size is set to 1. Terminal Count Int enable. */
	  LPC_GPDMACH1->CControl = (DMA_SIZE & 0x0FFF) | (0x00 << 12) | (0x00 << 15)
		| (1 << 26) | 0x80000000;
	}
	else if ( DMAMode == P2M )
	{
	  /* Ch1 set for P2M transfer from I2S RX FIFO to memory. */
	  LPC_GPDMACH1->CSrcAddr = DMA_I2S_RX_FIFO;
	  LPC_GPDMACH1->CDestAddr = DMA_DST;
	  /* The burst size is set to 1. Terminal Count Int enable. */
	  LPC_GPDMACH1->CControl = (DMA_SIZE & 0x0FFF) | (0x00 << 12) | (0x00 << 15)
		| (1 << 27) | 0x80000000;
	}
	else if ( DMAMode == P2P )
	{
	  /* Ch1 set for P2P transfer from I2S DAO to I2S DAI. */
	  LPC_GPDMACH1->CSrcAddr = DMA_I2S_TX_FIFO;
	  LPC_GPDMACH1->CDestAddr = DMA_I2S_RX_FIFO;
	  /* The burst size is set to 32. */
	  LPC_GPDMACH1->CControl = (DMA_SIZE & 0x0FFF) | (0x04 << 12) | (0x04 << 15) 
		| 0x80000000;
	}
	else
	{
	  return ( FALSE );
	}
  }
  else
  {
	return ( FALSE );
  }
  return( TRUE );
}

#endif	/* end if DMA_ENABLED */

/******************************************************************************
**                            End Of File
******************************************************************************/
