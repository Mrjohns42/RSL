/****************************************************************************
 *   $Id:: uart_dma.h 5793 2010-12-02 23:08:27Z usb00423                    $
 *   Project: NXP LPC17xx UART DMA example
 *
 *   Description:
 *     This file contains UART code header definition.
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
#ifndef __UART_DMA_H 
#define __UART_DMA_H

#define TEST_2_TO_3				0	/* 1 is to test U2 to U3, 0 is to test U2 to U4. */

uint32_t UARTInit( uint32_t portNum, uint32_t Baudrate );

#endif /* end __UART_DMA_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
