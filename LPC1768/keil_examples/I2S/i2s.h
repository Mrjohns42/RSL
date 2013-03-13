/****************************************************************************
 *   $Id:: i2s.h 5797 2010-12-03 00:27:00Z usb00423                         $
 *   Project: NXP LPC17xx I2S example
 *
 *   Description:
 *     This file contains I2S code header definition.
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
#ifndef __I2S_H 
#define __I2S_H

#define I2S_DMA_ENABLED		0

#define BUFSIZE				0x200
#define RXFIFO_EMPTY		0
#define TXFIFO_FULL			8

extern void I2S_IRQHandler( void );
extern void I2SStart( void );
extern void I2SStop( void );
extern void I2SInit( void );

#endif /* end __I2S_H */
/****************************************************************************
**                            End Of File
*****************************************************************************/
