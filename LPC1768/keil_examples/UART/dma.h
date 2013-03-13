/****************************************************************************
 *   $Id:: dma.h 5793 2010-12-02 23:08:27Z usb00423                         $
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

#define DMA_UART0_TX		8
#define DMA_UART0_RX		9
#define DMA_UART1_TX		10
#define DMA_UART1_RX		11
#define DMA_UART2_TX		12
#define DMA_UART2_RX		13
#define DMA_UART3_TX		14
#define DMA_UART3_RX		15

/* UART0 TX and RX */
#define UART0_DMA_TX_SRC	0x2007C800		/* starting addr of DATA register in UART0 */
#define UART0_DMA_TX_DST	LPC_UART0_BASE
#define UART0_DMA_RX_SRC	LPC_UART0_BASE
#define UART0_DMA_RX_DST	0x2007C900

/* UART1 TX and RX */
#define UART1_DMA_TX_SRC	0x2007CA00		/* starting addr of DATA register in UART1 */
#define UART1_DMA_TX_DST	LPC_UART1_BASE
#define UART1_DMA_RX_SRC	LPC_UART1_BASE
#define UART1_DMA_RX_DST	0x2007CB00

/* UART2 TX and RX */
#define UART2_DMA_TX_SRC	0x2007CC00		/* starting addr of DATA register in UART2 */
#define UART2_DMA_TX_DST	LPC_UART2_BASE
#define UART2_DMA_RX_SRC	LPC_UART2_BASE
#define UART2_DMA_RX_DST	0x2007CD00

/* UART3 TX and RX */ 
#define UART3_DMA_TX_SRC	0x2007CE00		/* starting addr of DATA register in UART3 */
#define UART3_DMA_TX_DST	LPC_UART3_BASE
#define UART3_DMA_RX_SRC	LPC_UART3_BASE
#define UART3_DMA_RX_DST	0x2007CF00

#define UART_DMA_SIZE	0x10			/* DMA transfer size */

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
