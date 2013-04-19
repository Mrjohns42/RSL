/****************************************************************************
 *   $Id:: bodtest.c 4309 2010-08-18 01:02:28Z usb00423                     $
 *   Project: NXP LPC11xx BOD(Brown-OUt Detect) example
 *
 *   Description:
 *     This file contains BOD code example which tests BOD 
 *     initialization, BOD interrupt handler, and APIs.
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
#include "timer16.h"
#include "gpio.h"
#include "bod.h"
#if DEEPSLEEP_BOD_WAKEUP
	#include "pmu.h"
#endif

extern volatile uint32_t timer16_0_counter[2];
extern volatile uint32_t bod_counter;

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
#if DEEPSLEEP_BOD_WAKEUP
  uint32_t regVal;
#endif

  SystemCoreClockUpdate();

  GPIOInit();

  /* Initialize BOD */
  BOD_Init();

  /* Set port 2_0, 2_1, and 2_2 to output.
  2.0 is for BOD reset,  2.1 is for POR reset, 2.2 is for BOD interrupt. */
  GPIOInit();
  GPIOSetDir( 2, 0, 1 );
  GPIOSetDir( 2, 1, 1 );
  GPIOSetDir( 2, 2, 1 );
#if DEEPSLEEP_BOD_WAKEUP
  /* when external event(p1.4) to trigger the deep sleep. */
  GPIOSetDir( 1, 4, 0 );
#endif

  /* Turn off all LEDs. */
  GPIOSetValue( 2, 0, 0 );
  GPIOSetValue( 2, 1, 0 );
  GPIOSetValue( 2, 2, 0 );

  init_timer16( 0, TIME_INTERVAL );
  enable_timer16( 0 );

  /* The procedure is:
  (1) POR LED should blink on after power cycle.
  (2) when voltage(VDD_MAIN) falls to the BOD interrupt level, BOD Interrupt
  (p2.2) LED will be ON, POR LED(p2.1) should be off. 
  (3) when voltage raises, BOD interrupt(p2.2) LED should be off, POR RESET 
  (p2.1) LED will blink back on again. 
  (4) when voltage drops further to the BOD RESET, BOD RESET (p2.0) LED will 
  blink. 
  (5) Once it happens, BOD interrupt is disabled. */
   
  /* Enable the BOD Interrupt */
  NVIC_EnableIRQ(BOD_IRQn);

  if ((LPC_SYSCON->SYSRSTSTAT & (POR_RESET|BOD_RESET)) == BOD_RESET)
  {
    LPC_SYSCON->SYSRSTSTAT |= BOD_RESET;
	/* Blink port 2_0 */
    while( 1 ) 
    {
	  if ( GPIOGetValue( 2, 2 ) )
	  {
		/* if BOD interrupt has occured and LED is on, turn it off. */
		GPIOSetValue( 2, 2, 0 );
	  }
	  /* Toggle p2.0 indicating it's a BOD reset */
	  if ( (timer16_0_counter[0] > 0) && (timer16_0_counter[0] <= 200) )
	  {
		GPIOSetValue( 2, 0, 0 );
	  }
	  if ( (timer16_0_counter[0] > 200) && (timer16_0_counter[0] <= 400) )
	  {
		GPIOSetValue( 2, 0, 1 );
	  }
	  else if ( timer16_0_counter[0] > 400 )
	  {
		timer16_0_counter[0] = 0;
	  }
    }
  }
  else
  {
    LPC_SYSCON->SYSRSTSTAT |= POR_RESET;
	/* Blink port 2_1 */
    while( 1 ) 
    {
#if DEEPSLEEP_BOD_WAKEUP
	  /* If DEEPSLEEP_BOD_WAKEUP is enabled. It will test, 
	  when MCU is in deep sleep mode, BOD RESET can be used 
	  as the wakeup source. 
	  The procedure is: 
	  (1) Put the MCU into deep sleep mode by pressing PIO1_4 on the board
	  (2) Decrease the VDD_MAIN voltage below BOD RESET.
	  (3) Increase the VDD MAIN voltage back to BOD RESET range.
	  (4) The system will be waken up, and BOD RESET LED
	  should blink. */
	  if ( !GPIOGetValue( 1, 4 ) )
	  {
		regVal = LPC_SYSCON->PDSLEEPCFG;
		regVal &= ~(WDT_OSC_PD | BOD_PD);
		PMU_Sleep( MCU_DEEP_SLEEP, regVal );
	  }
#endif	  
	  if ( GPIOGetValue( 2, 2 ) )
	  {
		/* if BOD interrupt has occured and LED is on, turn it off. */
		GPIOSetValue( 2, 2, 0 );
	  }
	  /* Toggle p2.1 indicating it's a POR reset */
	  if ( (timer16_0_counter[0] > 0) && (timer16_0_counter[0] <= 200) )
	  {
		GPIOSetValue( 2, 1, 0 );
	  }
	  if ( (timer16_0_counter[0] > 200) && (timer16_0_counter[0] <= 400) )
	  {
		GPIOSetValue( 2, 1, 1 );
	  }
	  else if ( timer16_0_counter[0] > 400 )
	  {
		timer16_0_counter[0] = 0;
	  }
    }
  }
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
