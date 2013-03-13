/****************************************************************************
 *   $Id:: i2cslvtst.c 6097 2011-01-07 04:31:25Z nxp12832                   $
 *   Project: NXP LPC17xx I2C Slave example
 *
 *   Description:
 *     This file contains I2C slave test modules, main entry, to test I2C 
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
****************************************************************************/
#include "LPC17xx.h"			/* LPC11xx Peripheral Registers */
#include "type.h"
#include "i2cslave.h"

volatile uint32_t I2CMasterState = I2C_IDLE;
volatile uint32_t I2CSlaveState = I2C_IDLE;

volatile uint32_t I2CMode;

volatile uint8_t I2CWrBuffer[BUFSIZE];
volatile uint8_t I2CRdBuffer[BUFSIZE];
volatile uint32_t I2CReadLength;
volatile uint32_t I2CWriteLength;

volatile uint32_t RdIndex = 0;
volatile uint32_t WrIndex = 0;

/*******************************************************************************
**   Main Function  main()
*******************************************************************************/
int main (void)
{
  uint32_t i;

  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemClockUpdate();

  for ( i = 0; i < BUFSIZE; i++ )
  {
	I2CRdBuffer[i] = 0x00;
  }
  
  I2CSlave0Init();			/* initialize I2c */
//  I2CSlave1Init();			/* initialize I2c */
//  I2CSlave2Init();			/* initialize I2c */

  while ( I2CSlaveState != DATA_NACK );
  /* When the NACK occurs, the master has stopped the 
  communication. Just check the content of I2CRd/WrBuffer. */
  while ( 1 );	/* Never exit from main for easy debugging. */
  return ( 0 );
}

/******************************************************************************
**                            End Of File
******************************************************************************/
