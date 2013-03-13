/****************************************************************************
 *   $Id:: dma.h 6089 2011-01-06 04:38:09Z nxp12832                         $
 *   Project: NXP LPC17xx ADC DMA example
 *
 *   Description:
 *     This file contains ADC DMA code header definition.
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
#ifndef __DMA_H 
#define __DMA_H

/* Second half of the second RAM is used for GPDMA operation. */
#define DMA_SRC			(LPC_ADC_BASE+ADC_OFFSET)	/* DATA register entry */
#define DMA_DST			LPC_AHBRAM0_BASE

/* DMA connection number, or DMA request number */
#define DMA_ADC			4

/* DMA transfer size */ 
#define DMA_SIZE		8

/* DMA mode */
#define M2M				0x00
#define M2P				0x01
#define P2M				0x02
#define P2P				0x03

extern void DMA_IRQHandler( void ); 
extern void DMA_Init( void );
extern uint32_t DMAChannel_Init( uint32_t ChannelNum, uint32_t DMAMode );

#endif /* end __DMA_H */
/****************************************************************************
**                            End Of File
****************************************************************************/
