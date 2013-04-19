/****************************************************************************
 *   $Id:: pmu.c 9376 2012-04-19 22:59:58Z nxp41306                         $
 *   Project: NXP LPC11xx PMU example
 *
 *   Description:
 *     This file contains PMU code example which include PMU 
 *     initialization, PMU interrupt handler, and APIs for PMU
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

* Permission to use, copy, modify, and distribute this software and its 
* documentation is hereby granted, under NXP Semiconductors' 
* relevant copyright in the software, without fee, provided that it 
* is used in conjunction with NXP Semiconductors microcontrollers.  This 
* copyright, permission, and disclaimer notice must appear in all copies of 
* this code.
****************************************************************************/
#include "LPC11xx.h"			/* LPC11xx Peripheral Registers */
#include "type.h"
#include "gpio.h"
#include "pmu.h"

volatile uint32_t pmu_counter = 0;

/*****************************************************************************
** Function name:		WAKEUP_IRQHandler
**
** Descriptions:		WAKEUP Interrupt Handler
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void WAKEUP_IRQHandler(void)
{
  uint32_t regVal;

  /* This handler takes care all the port pins if they
  are configured as wakeup source. */
  regVal = LPC_SYSCON->STARTSRP0;
  if ( regVal != 0 )
  {
	LPC_SYSCON->STARTRSRP0CLR = regVal;
  }
  /* See tracker for bug report. */
  __NOP();
  return;
}

/*****************************************************************************
** Function name:		PMU_Init
**
** Descriptions:		Initialize PMU and setup wakeup source.
**						For Sleep and deepsleep, any of the I/O pins can be 
**						used as the wakeup source.
**						For Deep Powerdown, only pin P1.4 can be used as 
**						wakeup source from deep powerdown. 
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void PMU_Init( void )
{
  /* Enable all clocks, even those turned off at power up. */
  LPC_SYSCON->PDRUNCFG &= ~(WDT_OSC_PD|SYS_OSC_PD|ADC_PD);

 	NVIC_EnableIRQ((IRQn_Type)(WAKEUP4_IRQn));
 

 LPC_IOCON->PIO0_4 &= ~0x07;	
 LPC_IOCON->PIO0_4 |= 0x20;	
 GPIOSetDir( PORT0, 4, 0 );	/* Input P0.4 */
 
//   /* use port1_4 as wakeup source, i/o pin */
//   LPC_IOCON->PIO1_4 &= ~0x07;	
//   LPC_IOCON->PIO1_4 |= 0xD0;	
//   GPIOSetDir( PORT1, 4, 0 );	/* Input P1.4 */
//   
  /* Only edge trigger. activation polarity on P1.0~11 and P1.0 is FALLING EDGE. */
  LPC_SYSCON->STARTAPRP0 = 0x00000000;
  /* Clear all wakeup source */ 
  LPC_SYSCON->STARTRSRP0CLR = 0xFFFFFFFF;
  /* Enable Port 1.0 as wakeup source. */
  LPC_SYSCON->STARTERP0 = (0x1<<12);
  return;
}

/*****************************************************************************
** Function name:		PMU_Sleep
**
** Descriptions:		Put some of the peripheral in sleep mode.
**
** parameters:			SleepMode: 1 is deep sleep, 0 is sleep, 
**						Sleep peripheral module(s)
** Returned value:		None
** 
*****************************************************************************/
void PMU_Sleep( uint32_t SleepMode, uint32_t SleepCtrl )
{
  LPC_SYSCON->PDAWAKECFG = LPC_SYSCON->PDRUNCFG;
  LPC_SYSCON->PDSLEEPCFG = 0x000018FF;
  /* If normal sleep, not deep sleep, don't do anything to SCR reg. */
  if ( SleepMode )
  {
	SCB->SCR |= NVIC_LP_SLEEPDEEP;
  }
  __WFI();
  return;
}

/*****************************************************************************
** Function name:		PMU_PowerDown
**
** Descriptions:		Some of the content should not be touched 
**						during the power down to wakeup process.
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void PMU_PowerDown( void )
{
  uint32_t regVal;

  if ( (LPC_PMU->PCON & (0x1<<11)) != 0x0 )
  {
    /* Check deep power down bits. If deep power down mode is entered, 
    clear the PCON bits. */
    regVal = LPC_PMU->PCON;
    regVal |= (0x1<<11);
    LPC_PMU->PCON = regVal;

    if ( (LPC_PMU->GPREG0 != 0x12345678)||(LPC_PMU->GPREG1 != 0x87654321)
         ||(LPC_PMU->GPREG2 != 0x56781234)||(LPC_PMU->GPREG3 != 0x43218765) )
    {
      while (1);
    }
  }
  else
  {
	/* If in neither sleep nor deep power mode, enter deep power
	down mode now. */
    LPC_PMU->GPREG0 = 0x12345678;
    LPC_PMU->GPREG1 = 0x87654321;
    LPC_PMU->GPREG2 = 0x56781234;
    LPC_PMU->GPREG3 = 0x43218765;
	SCB->SCR |= NVIC_LP_SLEEPDEEP;
    LPC_PMU->PCON = 0x2;
    __WFI();
  }
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
