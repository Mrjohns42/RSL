/****************************************************************************
 *   $Id:: dac.h 5795 2010-12-03 00:02:09Z usb00423                         $
 *   Project: NXP LPC17xx DAC example
 *
 *   Description:
 *     This file contains DAC code header definition.
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
#ifndef __DAC_H 
#define __DAC_H

#define DAC_DMA_ENABLED		0

#define DAC_BIAS			(0x1<<16)

extern void DACInit( void );

#endif /* end __DAC_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
