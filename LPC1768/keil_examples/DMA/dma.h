/****************************************************************************
 *   $Id:: dma.h 5668 2010-11-19 01:16:11Z usb00423                         $
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

#define DMA_SRC			0x2007C000
#define DMA_DST			0x20080000
#define DMA_SIZE		0x1000

#define M2M				0x00
#define M2P				0x01
#define P2M				0x02
#define P2P				0x03

extern void DMA_IRQHandler (void);
extern uint32_t DMA_Init( uint32_t DMAMode );

#endif /* end __DMA_H */
/****************************************************************************
**                            End Of File
****************************************************************************/
