/****************************************************************************
 *   $Id:: gpiotest.c 9373 2012-04-19 22:57:34Z nxp41306                    $
 *   Project: NXP LPC11xx GPIO example
 *
 *   Description:
 *     This file contains GPIO test modules, main entry, to test GPIO APIs.
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

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  SystemCoreClockUpdate();

  GPIOInit();

  /* Enable Hysteresis to eliminate debouncing. */
  LPC_IOCON->PIO0_1  |= (0x01<<5);	
  LPC_IOCON->R_PIO1_1|= (0x01<<5);
  LPC_IOCON->PIO2_1  |= (0x01<<5);
  LPC_IOCON->PIO3_1  |= (0x01<<5);

  /* use port0_1 as input event, interrupt test. */
  GPIOSetDir( PORT0, 1, 0 );
  /* port0_1, single trigger, active low. */
  GPIOSetInterrupt( PORT0, 1, 0, 0, 0 );
  GPIOIntEnable( PORT0, 1 );

  /* use port1_1 as input event, interrupt test. */
  GPIOSetDir( PORT1, 1, 0 );
  /* port0_1, single edge trigger, active low. */
  GPIOSetInterrupt( PORT1, 1, 0, 0, 0 );
  GPIOIntEnable( PORT1, 1 );

  /* use port2_1 as input event, interrupt test. */
  GPIOSetDir( PORT2, 1, 0 );
  /* port0_1, single edge trigger, active low. */
  GPIOSetInterrupt( PORT2, 1, 0, 0, 0 );
  GPIOIntEnable( PORT2, 1 );

  /* use port3_1 as input event, interrupt test. */
  GPIOSetDir( PORT3, 1, 0 );
  /* port0_1, single edge trigger, active low. */
  GPIOSetInterrupt( PORT3, 1, 0, 0, 0 );
  GPIOIntEnable( PORT3, 1 );

  while( 1 );
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
