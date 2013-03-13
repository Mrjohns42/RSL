/****************************************************************************
 *   $Id:: dma.h 5804 2010-12-04 00:32:12Z usb00423                         $
 *   Project: NXP LPC17xx DMA example
 *
 *   Description:
 *     This file contains DMA code header definition.
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

/* Second half of the second RAM is used for GPDMA 
operation. */

#define DMA_SSP0_TX		0
#define DMA_SSP0_RX		1
#define DMA_SSP1_TX		2
#define DMA_SSP1_RX		3

/* SSP0 TX and RX */
#define SSP0_DMA_TX_SRC		0x2007C800		/* addr of DATA register in SSP0 */
#define SSP0_DMA_TX_DST		(LPC_SSP0_BASE + 0x08)
#define SSP0_DMA_RX_SRC		(LPC_SSP0_BASE + 0x08)
#define SSP0_DMA_RX_DST		0x2007C900

/* SSP1 TX and RX */
#define SSP1_DMA_TX_SRC		0x2007CA00		/* addr of DATA register in SSP1 */
#define SSP1_DMA_TX_DST		(LPC_SSP1_BASE + 0x08)
#define SSP1_DMA_RX_SRC		(LPC_SSP1_BASE + 0x08)
#define SSP1_DMA_RX_DST		0x2007CB00

#define SSP_DMA_SIZE		0x100			/* DMA transfer size */

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
