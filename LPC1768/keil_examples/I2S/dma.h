/****************************************************************************
 *   $Id:: dma.h 5797 2010-12-03 00:27:00Z usb00423                         $
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
#ifndef __I2S_DMA_H 
#define __I2S_DMA_H

#define DMA_I2S_REQ0	5
#define DMA_I2S_REQ1	6

#define DMA_SRC			0x2007C000	
#define DMA_DST			0x2007D000
#define DMA_I2S_TX_FIFO	(LPC_I2S_BASE + 0x08)
#define	DMA_I2S_RX_FIFO	(LPC_I2S_BASE + 0x0C)
 
#define DMA_SIZE		0x200

/* DMA mode */
#define M2M				0x00
#define M2P				0x01
#define P2M				0x02
#define P2P				0x03

extern void DMA_IRQHandler( void ); 
extern void DMA_Init( void );
extern uint32_t DMAChannel_Init( uint32_t ChannelNum, uint32_t DMAMode );

#endif /* end __I2S_DMA_H */
/****************************************************************************
**                            End Of File
****************************************************************************/
