/****************************************************************************
 *   $Id:: adc.c 6089 2011-01-06 04:38:09Z nxp12832                         $
 *   Project: NXP LPC17xx ADC example
 *
 *   Description:
 *     This file contains ADC code example which include ADC 
 *     initialization, ADC interrupt handler, and APIs for ADC
 *     reading.
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
#include "lpc17xx.h"
#include "type.h"
#include "adc.h"

#ifndef _BV
#define _BV(_x_) (1UL << (_x_))
#endif

volatile uint32_t ADCValue[ADC_NUM];
volatile uint32_t ADCIntDone = 0;
volatile uint32_t BurstCounter = 0;
volatile uint32_t OverRunCounter = 0;

#if BURST_MODE
volatile uint32_t channel_flag = 0; 
#endif

#if ADC_INTERRUPT_FLAG
/******************************************************************************
** Function name:		ADC_IRQHandler
**
** Descriptions:		ADC interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void ADC_IRQHandler (void) 
{
  uint32_t regVal;
  volatile uint32_t dummy;
  int i;
  
  regVal = LPC_ADC->STAT;		/* Read ADC will clear the interrupt */
  if ( regVal & 0x0000FF00 )	/* check OVERRUN error first */
  {
	OverRunCounter++;
  	for ( i = 0; i < ADC_NUM; i++ )
  	{
  	  regVal = (regVal & 0x0000FF00) >> 0x08;
  	  /* if overrun, just read ADDR to clear */
  	  /* regVal variable has been reused. */
      if ( regVal & _BV(i) )
  	  {
        dummy = LPC_ADC->DR[i];
  	  }
  	}
	LPC_ADC->CR &= ~((0x7<<24)|(0x1<<16));	/* stop ADC now, turn off BURST bit. */ 
  	ADCIntDone = 1;
  	return;	
  }

  for ( i = 0; i < ADC_NUM; i++ )
  {
    if ( regVal & _BV(i) )
	{
	  ADCValue[i] = ( LPC_ADC->DR[i] >> 4 ) & 0xFFF;
	}
  }

#if BURST_MODE
  BurstCounter++;
  channel_flag |= (regVal & 0xFF);
  if ( (channel_flag & 0xFF) == 0xFF )
  {
	/* All the bits in have been set, it indicates all the ADC 
	channels have been converted. */
	LPC_ADC->CR &= ~(0x1<<16);	/* Clear BURST mode, stop ADC now */ 
  	channel_flag = 0; 
	ADCIntDone = 1;
  }
#else
  LPC_ADC->CR &= ~(0x7<<24);	/* stop ADC now */ 
  ADCIntDone = 1;
#endif
  return;
}
#endif

/*****************************************************************************
** Function name:		ADCInit
**
** Descriptions:		initialize ADC channel
**
** parameters:			ADC clock rate
** Returned value:		None
** 
*****************************************************************************/
void ADCInit( uint32_t ADC_Clk )
{
  uint32_t i, pclkdiv, pclk;

  /* Enable CLOCK into ADC controller */
  LPC_SC->PCONP |= (1 << 12);

  for ( i = 0; i < ADC_NUM; i++ )
  {
	ADCValue[i] = 0x0;
  }

  /* all the related pins are set to ADC inputs, AD0.0~7 */
  LPC_PINCON->PINSEL0 &= ~0x000000F0;	/* P0.2~3, A0.6~7, function 10 */
  LPC_PINCON->PINSEL0 |= 0x000000A0;	
  LPC_PINCON->PINSEL1 &= ~0x003FC000;	/* P0.23~26, A0.0~3, function 01 */
  LPC_PINCON->PINSEL1 |= 0x00154000;
  LPC_PINCON->PINSEL3 |= 0xF0000000;	/* P1.30~31, A0.4~5, function 11 */
  /* No pull-up no pull-down (function 10) on these ADC pins. */
  LPC_PINCON->PINMODE0 &= ~0x000000F0;
  LPC_PINCON->PINMODE0 |= 0x000000A0;
  LPC_PINCON->PINMODE1 &= ~0x003FC000;
  LPC_PINCON->PINMODE1 |= 0x002A8000;
  LPC_PINCON->PINMODE3 &= ~0xF0000000;
  LPC_PINCON->PINMODE3 |= 0xA0000000;

  /* By default, the PCLKSELx value is zero, thus, the PCLK for
  all the peripherals is 1/4 of the SystemFrequency. */
  /* Bit 24~25 is for ADC */
  pclkdiv = (LPC_SC->PCLKSEL0 >> 24) & 0x03;
  switch ( pclkdiv )
  {
	case 0x00:
	default:
	  pclk = SystemFrequency/4;
	break;
	case 0x01:
	  pclk = SystemFrequency;
	break; 
	case 0x02:
	  pclk = SystemFrequency/2;
	break; 
	case 0x03:
	  pclk = SystemFrequency/8;
	break;
  }

  LPC_ADC->CR = ( 0x01 << 0 ) |  /* SEL=1,select channel 0~7 on ADC0 */
		( ( pclk  / ADC_Clk - 1 ) << 8 ) |  /* CLKDIV = Fpclk / ADC_Clk - 1 */ 
		( 0 << 16 ) | 		/* BURST = 0, no BURST, software controlled */
		( 0 << 17 ) |  		/* CLKS = 0, 11 clocks/10 bits */
		( 1 << 21 ) |  		/* PDN = 1, normal operation */
		( 0 << 24 ) |  		/* START = 0 A/D conversion stops */
		( 0 << 27 );		/* EDGE = 0 (CAP/MAT singal falling,trigger A/D conversion) */ 

  /* If POLLING, no need to do the following */
#if ADC_INTERRUPT_FLAG
  NVIC_EnableIRQ(ADC_IRQn);
#if BURST_MODE
  LPC_ADC->INTEN = 0xFF;		/* Enable all interrupts */
#else
  LPC_ADC->INTEN = 0x1FF;		/* Enable all interrupts */
#endif
#endif
  return;
}

/*****************************************************************************
** Function name:		ADCRead
**
** Descriptions:		Read ADC channel
**
** parameters:			Channel number
** Returned value:		Value read, if interrupt driven, return channel #
** 
*****************************************************************************/
uint32_t ADCRead( uint8_t channelNum )
{
#if !ADC_INTERRUPT_FLAG
  uint32_t regVal, ADC_Data;
#endif

  /* channel number is 0 through 7 */
  if ( channelNum >= ADC_NUM )
  {
	channelNum = 0;		/* reset channel number to 0 */
  }
  LPC_ADC->CR &= 0xFFFFFF00;
  LPC_ADC->CR |= (1 << 24) | (1 << channelNum);	
				/* switch channel,start A/D convert */
#if !ADC_INTERRUPT_FLAG
  while ( 1 )			/* wait until end of A/D convert */
  {
	regVal = LPC_ADC->DR[channelNum];
	/* read result of A/D conversion */
	if ( regVal & ADC_DONE )
	{
	  break;
	}
  }	
        
  LPC_ADC->CR &= 0xF8FFFFFF;	/* stop ADC now */    
  if ( regVal & ADC_OVERRUN )	/* save data when it's not overrun, otherwise, return zero */
  {
	return ( 0 );
  }
  ADC_Data = ( regVal >> 4 ) & 0xFFF;
  return ( ADC_Data );	/* return A/D conversion value */
#else
  return ( channelNum );	/* if it's interrupt driven, the ADC reading is 
							done inside the handler. so, return channel number */
#endif
}

/*****************************************************************************
** Function name:		ADC0BurstRead
**
** Descriptions:		Use burst mode to convert multiple channels once.
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void ADCBurstRead( void )
{
  /* Start bits need to be zero before BURST mode can be set. */
  if ( LPC_ADC->CR & (0x7<<24) )
  {
	LPC_ADC->CR &= ~(0x7<<24);
  }
  LPC_ADC->CR &= ~0xFF;
  /* Read all channels, 0 through 7. */
  LPC_ADC->CR |= 0xFF;
  LPC_ADC->CR |= (0x1<<16);		/* Set burst mode and start A/D convert */
  return;						/* the ADC reading is done inside the 
								handler, return 0. */
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
