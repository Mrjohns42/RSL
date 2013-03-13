/****************************************************************************
 *   $Id:: RITtimer.h 5739 2010-11-30 22:34:36Z usb00423                    $
 *   Project: NXP LPC17xx RIT example
 *
 *   Description:
 *     This file contains RIT code header definition.
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
#ifndef __RITTIMER_H 
#define __RITTIMER_H

#define TIME_INTERVAL	(9000000/100 - 1)

extern void init_rit_timer( uint32_t timerInterval );
extern void enable_rit_timer( void );
extern void disable_rit_timer( void );
extern void reset_rit_timer( void );
extern void RIT_IRQHandler (void);

#endif /* end __RITTIMER_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
