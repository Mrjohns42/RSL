/****************************************************************************
 *   $Id:: dac.c 5667 2010-11-19 01:15:32Z usb00423                         $
 *   Project: NXP LPC17xx DAC example
 *
 *   Description:
 *     This file contains DAC code example which include DAC 
 *     initialization, and APIs for DAC.
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
#include "dac.h"

/*****************************************************************************
** Function name:		DACInit
**
** Descriptions:		initialize DAC channel
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void DACInit( void )
{
  /* setup the related pin to DAC output */
  LPC_PINCON->PINSEL1 = 0x00200000;	/* set p0.26 to DAC output */   
    
  LPC_DAC->CNTVAL = 0x00FF;
  LPC_DAC->CTRL = (0x1<<1)|(0x1<<2);	  
  return;
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
