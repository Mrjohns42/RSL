/****************************************************************************
 *   $Id:: einttest.c 6097 2011-01-07 04:31:25Z nxp12832                    $
 *   Project: NXP LPC17xx EINT example
 *
 *   Description:
 *     This file contains EINT test modules, main entry, to test EINT APIs.
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
#include "extint.h"

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemClockUpdate();
	    		
  /* initialize GPIO pins as external interrupts */          
  EINTInit();

  /****** It's an endless loop waiting for external interrupt ******/
  while( 1 );
}

/******************************************************************************
**                            End Of File
******************************************************************************/
