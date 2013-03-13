/****************************************************************************
 *   $Id:: dma.h 5823 2010-12-07 19:01:00Z usb00423                         $
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

#define TMR0_M0_REQ			8
#define TMR0_M1_REQ			9
#define TMR1_M0_REQ			10
#define TMR1_M1_REQ			11
#define TMR2_M0_REQ			12
#define TMR2_M1_REQ			13
#define TMR3_M0_REQ			14
#define TMR3_M1_REQ			15

#define DMA_M2M_SRC			0x2007C000
#define DMA_M2M_DST			0x2007D000

#define DMA_M2P_GPIO_SRC	0x2007C000
#define DMA_M2P_GPIO0_DST	(LPC_GPIO0_BASE+0x14)
#define DMA_M2P_GPIO1_DST	(LPC_GPIO1_BASE+0x14)
#define DMA_M2P_GPIO2_DST	(LPC_GPIO2_BASE+0x14)
#define DMA_M2P_GPIO3_DST	(LPC_GPIO3_BASE+0x14)
#define DMA_M2P_GPIO4_DST	(LPC_GPIO4_BASE+0x14)
#define DMA_M2P_GPIO5_DST	(LPC_GPIO5_BASE+0x14)

/* Below definition is to test P2M, when transfer
type is P2M, an external trigger is needed. MATx_OUT
is used as a external trigger. */
#define DMA_P2M_GPIO0_SRC	(LPC_GPIO0_BASE+0x14)
#define DMA_P2M_GPIO0_DST	0x2007D000

#define DMA_M2M_SIZE		0x1000
#define DMA_SIZE			0x10
							
#define M2M					0x00
#define M2P					0x01
#define P2M					0x02
#define P2P					0x03

extern void DMA_IRQHandler (void);
extern uint32_t DMA_Init( uint32_t DMAMode );

#endif /* end __DMA_H */
/****************************************************************************
**                            End Of File
****************************************************************************/
