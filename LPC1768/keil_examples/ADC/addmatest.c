/****************************************************************************
 *   $Id:: addmatest.c 6089 2011-01-06 04:38:09Z nxp12832                   $
 *   Project: NXP LPC17xx ADC DMA example
 *
 *   Description:
 *     This file contains ADC DMA test modules, main entry, to test ADC APIs.
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
#include "LPC17xx.h"                        /* LPC17xx definitions */
#include "adc.h"
#include "dma.h"
#if ADC_DEBUG
#include "uart.h"
#endif

extern volatile uint32_t DMATCCount;
extern volatile uint32_t ADCDMA0Done;
extern volatile uint32_t ADCDMA1Done;
extern volatile uint32_t ADCValue[ADC_NUM];

#if ADC_DEBUG
volatile uint8_t UARTBuffer[BUFSIZE];

static uint8_t ConvertDigital ( uint8_t digit )
{
  static uint8_t hex[] = "0123456789ABCDEF";
  return hex[digit & 0xf];
}

#endif

/******************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  uint32_t i;

  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemClockUpdate();
   
  /* Initialize ADC  */
  ADCInit( ADC_CLK );
  
  /* ADC Interrupt is needed, but NVIC is not necessary for ADC DMA. */
  NVIC_DisableIRQ(ADC_IRQn);

  DMA_Init();

  /* on DMA channel 0, source is ADC, Destination is Memory */
  /* Enable channel and IE bit */
  DMAChannel_Init( 0, P2M );
  
  /* Set TC/Err int mask, channel enabled, dest. peripheral is memory(0x0), 
  src peripheral is ADC(8), and transfer type is the P2M(2) */
  LPC_GPDMACH0->CConfig = 0xC001|(DMA_ADC<<1)|(0x00<<6)|(0x02<<11);
  
  ADCBurstRead();

  while ( !ADCDMA0Done );
  ADCDMA0Done = 0;

  for ( i = 0; i < ADC_NUM; i++ )
  {
	ADCValue[i] = *(volatile unsigned long *)(DMA_DST + ADC_INDEX * i);
	ADCValue[i] >>= 6;
	ADCValue[i] &= 0x3FF;
  }
#if ADC_DEBUG
  /* NVIC is installed inside UARTInit file. */
  for ( i = 0; i < 0x10000; i++ );
  UARTInit(0, 9600);
    
  for ( i = 0; i < ADC_NUM; i++ )
  {
	LPC_UART0->IER = IER_THRE | IER_RLS;			/* Disable RBR */
	UARTBuffer[0] = ConvertDigital( (uint8_t)(ADCValue[i]>>28));
	UARTBuffer[1] = ConvertDigital( (uint8_t)((ADCValue[i]>>24)&0xF));
	UARTBuffer[2] = ConvertDigital( (uint8_t)((ADCValue[i]>>20)&0xF));
	UARTBuffer[3] = ConvertDigital( (uint8_t)((ADCValue[i]>>16)&0xF));
	UARTBuffer[4] = ConvertDigital( (uint8_t)((ADCValue[i]>>12)&0xF));
	UARTBuffer[5] = ConvertDigital( (uint8_t)((ADCValue[i]>>8)&0xF));
	UARTBuffer[6] = ConvertDigital( (uint8_t)((ADCValue[i]>>4)&0xF));
	UARTBuffer[7] = ConvertDigital( (uint8_t)(ADCValue[i]&0xF));
	UARTBuffer[8] = '\r';
	UARTBuffer[9] = '\n';
	UARTSend( 0, (uint8_t *)UARTBuffer, 10 );
	LPC_UART0->IER = IER_THRE | IER_RLS | IER_RBR;	/* Re-enable RBR */
  }
  UARTBuffer[0] = '\r';
  UARTBuffer[1] = '\n';
  UARTSend( 0, (uint8_t *)UARTBuffer, 2 );
#endif 
  while ( 1 );		/* Never exit from main for easier debugging. */
}

/******************************************************************************
**                            End Of File
******************************************************************************/

