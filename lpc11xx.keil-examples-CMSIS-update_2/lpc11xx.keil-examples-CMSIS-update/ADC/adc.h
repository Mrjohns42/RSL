/****************************************************************************
 *   $Id:: adc.h 9359 2012-04-19 22:30:24Z nxp41306                         $
 *   Project: NXP LPC11xx ADC example
 *
 *   Description:
 *     This file contains ADC code header definition.
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

#ifndef __ADC_H 
#define __ADC_H

#define ADC_INTERRUPT_FLAG    0	/* 1 is interrupt driven, 0 is polling */
#define BURST_MODE            0 /* Burst mode works in interrupt driven mode only. */
#define SINGLE_BURST_MODE     0 /* Single burst mode works in interrupt driven mode only. */
#define ADGINTEN              0	/* global DONE flag generate interrupt */
#define INTERNAL_VOLT         0	/* If this flag is on, it measures interval voltage,
                                   core voltage, internal Vref, and temperature. */
#define ADC_DEBUG             1
#define ADC_DEBUG_CLEAR_CNT   (100) /* Number of samples until statistics are cleared */
#define ADC_DEBUG_UPDATE_CNT  (5000) /* Number of samples until statistics are cleared */

#define ADC_OFFSET            0x10
#define ADC_INDEX             4

#define ADC_DONE              0x80000000
#define ADC_OVERRUN           0x40000000
#define ADC_ADINT             0x00010000

#define ADC_NUM               8			/* for LPC11xx */
#define ADC_CLK               4400000		/* set to 4.4Mhz */


extern void ADC_IRQHandler( void );
extern void ADCInit( uint32_t ADC_Clk );
extern uint32_t ADCRead( uint8_t channelNum );
extern void ADCBurstRead( void );
#endif /* end __ADC_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
