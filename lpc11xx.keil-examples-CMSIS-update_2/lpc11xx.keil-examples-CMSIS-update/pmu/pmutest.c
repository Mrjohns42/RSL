/****************************************************************************
 *   $Id:: pmutest.c 9376 2012-04-19 22:59:58Z nxp41306                     $
 *   Project: NXP LPC11xx PMU (Power Management Unit) example
 *
 *   Description:
 *     This file contains PMU slave test modules, main entry, to test PMU 
 *     slave APIs.
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
#include "gpio.h"
#include "timer16.h"
#include "pmu.h"

extern volatile uint32_t timer16_0_counter;

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
#if !TEST_POWERDOWN
  uint32_t regVal;
#endif

SystemCoreClockUpdate();

  /* Enable AHB clock to the GPIO domain. */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);

#if !TEST_POWERDOWN
  PMU_Init();
  regVal = ~(WDT_OSC_PD|BOD_PD);
  PMU_Sleep( MCU_DEEP_SLEEP, regVal );
#else
  /* Config PMU_PCON to let MCU to deep sleep mode, retain 
  pattern in general-purpose register. A low on P1.4 will wake up
  the part from deep sleep. */
  PMU_PowerDown();
#endif

  /* Using 16-bit timer 0 for LED blinky rate. */
  init_timer16(0, TIME_INTERVAL);
  enable_timer16(0);
  
  /* Set port 2_0 to output as LED control */
  GPIOSetDir( 2, 0, 1 );

  while (1)                                /* Loop forever */
  {
	/* I/O configuration and LED setting pending. */
	if ( (timer16_0_counter > 0) && (timer16_0_counter <= 200) )
	{
	  GPIOSetValue( 2, 0, 0 );
	}
	if ( (timer16_0_counter > 200) && (timer16_0_counter <= 400) )
	{
	  GPIOSetValue( 2, 0, 1 );
	}
	else if ( timer16_0_counter > 400 )
	{
	  timer16_0_counter = 0;
	}
  }
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
