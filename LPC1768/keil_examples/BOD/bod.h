/****************************************************************************
 *   $Id:: bod.h 5794 2010-12-02 23:52:14Z usb00423                         $
 *   Project: NXP LPC17xx BOD(Brown-Out Detect) example
 *
 *   Description:
 *     This file contains headers for BOD related modules and definition.
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
#ifndef __BOD_H 
#define __BOD_H

#define BOD_DEBUG		0

extern void BOD_IRQHandler(void);
extern void BOD_Init( void );

#endif /* end __BOD_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
