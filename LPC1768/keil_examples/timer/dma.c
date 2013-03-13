/****************************************************************************
 *   $Id:: dma.c 5823 2010-12-07 19:01:00Z usb00423                         $
 *   Project: NXP LPC17xx DMA example
 *
 *   Description:
 *     This file contains DMA code example which include DMA 
 *     initialization, DMA interrupt handler, and APIs for DMA
 *     transfer.
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
#include "dma.h"

volatile uint32_t DMATCCount = 0;
volatile uint32_t DMAErrCount = 0;
volatile uint32_t CH0DMADone = 0;
volatile uint32_t CH1DMADone = 0;
volatile uint32_t CH2DMADone = 0;
volatile uint32_t CH3DMADone = 0;
volatile uint32_t CH4DMADone = 0;
volatile uint32_t CH5DMADone = 0;

/******************************************************************************
** Function name:		DMA_IRQHandler
**
** Descriptions:		DMA interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void DMA_IRQHandler (void) 
{
  uint32_t regVal;

  regVal = LPC_GPDMA->IntTCStat;
  if ( regVal )
  {
	DMATCCount++;
	LPC_GPDMA->IntTCClear |= regVal;
	if ( regVal & (0x01<<0) )
	{
	  CH0DMADone = 1;
	}
	else if ( regVal & (0x01<<1) )
	{
	  CH1DMADone = 1;
	}
	else if ( regVal & (0x01<<2) )
	{
	  CH2DMADone = 1;
	}
	else if ( regVal & (0x01<<3) )
	{
	  CH3DMADone = 1;
	}
	else if ( regVal & (0x01<<4) )
	{
	  CH4DMADone = 1;
	}
	else if ( regVal & (0x01<<5) )
	{
	  CH5DMADone = 1;
	}
  } 

  regVal = LPC_GPDMA->IntErrStat;
  if ( regVal )
  {
	DMAErrCount++;
	LPC_GPDMA->IntErrClr |= regVal;
  } 
  return;
}

/******************************************************************************
** Function name:		DMA_Init
**
** Descriptions:		
**
** parameters:			
** Returned value:		
** 
******************************************************************************/
uint32_t DMA_Init( uint32_t DMAMode )
{
  LPC_SC->PCONP |= (1 << 29);	/* Enable GPDMA clock */

  /* clear all interrupts on channel 0~5 */
  LPC_GPDMA->IntTCClear = 0x03F;
  LPC_GPDMA->IntErrClr = 0x03F;
   
  if ( DMAMode == M2M )
  {
	/* Ch0 is used for M2M test, for M2P and P2M, go to peripheral directories
	where both DAM and non DMA examples are provided. */
	LPC_GPDMACH0->CSrcAddr = DMA_M2M_SRC;
	LPC_GPDMACH0->CDestAddr = DMA_M2M_DST;
	/* Terminal Count Int enable */
	LPC_GPDMACH0->CControl = (DMA_M2M_SIZE & 0x0FFF) | (0x04 << 12) | (0x04 << 15) 
		| (0x02 << 18) | (0x02 << 21) | (1 << 26) | (1 << 27) | 0x80000000;	
  }
  else if ( DMAMode == M2P )
  {
	/* The M2P test is to use DMA to access GPIO pins. */
	LPC_GPDMACH0->CSrcAddr = DMA_M2P_GPIO_SRC;
	LPC_GPDMACH0->CDestAddr = DMA_M2P_GPIO0_DST;
	/* Terminal Count Int enable */
	LPC_GPDMACH0->CControl = (DMA_SIZE & 0x0FFF) | (0x00 << 12) | (0x00 << 15) 
		| (0x02 << 18) | (0x02 << 21) | (1 << 26) | (0 << 27) | 0x80000000;	
  }
  else if ( DMAMode == P2M )
  {
	/* The P2M test is to use DMA to read GPIO pins. */
	LPC_GPDMACH0->CSrcAddr = DMA_P2M_GPIO0_SRC;
	LPC_GPDMACH0->CDestAddr = DMA_P2M_GPIO0_DST;
	/* Terminal Count Int enable */
	LPC_GPDMACH0->CControl = (DMA_SIZE & 0x0FFF) | (0x00 << 12) | (0x00 << 15) 
		| (0x02 << 18) | (0x02 << 21) | (0 << 26) | (1 << 27) | 0x80000000;	
  }
  else
  {
	return ( FALSE );
  }
	
  LPC_GPDMA->Config = 0x01;	/* Enable DMA channels, little endian */
  while ( !(LPC_GPDMA->Config & 0x01) );    

  NVIC_EnableIRQ(DMA_IRQn);
  return (TRUE);
}

/******************************************************************************
**                            End Of File
******************************************************************************/
