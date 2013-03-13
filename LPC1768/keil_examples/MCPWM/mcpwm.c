/****************************************************************************
 *   $Id:: mcpwm.c 5747 2010-11-30 23:45:33Z usb00423                       $
 *   Project: NXP LPC17xx Motor Control PWM example
 *
 *   Description:
 *     This file contains MCPWM code example which include MCPWM 
 *     initialization, MCPWM interrupt handler, and APIs for MCPWM access.
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
#include "LPC17xx.h"
#include "type.h"
#include "mcpwm.h"

/******************************************************************************
** Function name:		MCPWM_IRQHandler
**
** Descriptions:		MCPWM interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void MCPWM_IRQHandler(void)
{
  uint32_t regVal;
#if (CHANNEL_0_CAPTURE || CHANNEL_1_CAPTURE || CHANNEL_2_CAPTURE)
  uint32_t regCap0, regCap1, regCap2;
#endif

  regVal = LPC_MCPWM->INTF;
#if (CHANNEL_0_CAPTURE || CHANNEL_1_CAPTURE || CHANNEL_2_CAPTURE)
  regCap0 = LPC_MCPWM->CAP0;
  regCap1 = LPC_MCPWM->CAP1;
  regCap2 = LPC_MCPWM->CAP2;
  LPC_MCPWM->CAP_CLR = 0x00000007;
  regCap0 = LPC_MCPWM->CAP0;
  regCap1 = LPC_MCPWM->CAP1;
  regCap2 = LPC_MCPWM->CAP2;
#endif
  LPC_MCPWM->INTF_CLR = regVal;
  return;
}

/******************************************************************************
** Function name:		MCPWM_Init
**
** Descriptions:		MCPWM initialization.
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void MCPWM_Init(void)
{
  LPC_SC->PCONP |= 0x00020000;		   /* Turn On MCPWM PCLK */

  /* P1.19~1.26, P1.28~29 as Motor Control PWM pins. */
  LPC_PINCON->PINSEL3 &= ~(0xFFFF<<6);
  LPC_PINCON->PINSEL3 &= ~(0x5555<<6);

  LPC_PINCON->PINSEL3 &= ~(0x0F<<24);
  LPC_PINCON->PINSEL3 &= ~(0x05<<24);
  return;
}

/******************************************************************************
** Function name:		MCPWM_Config
**
** Descriptions:		MCPWM configuration
**
** parameters:			ChannelNum, setup structure
** Returned value:		true or fase.
** 
******************************************************************************/
uint32_t MCPWM_Config(uint32_t channelNum, MCPWM_CHANNEL_SETUP_T * channelSetup)
{
  if (channelNum == 0)
  {
    LPC_MCPWM->TC0 = channelSetup->channelTimercounterValue;
    LPC_MCPWM->LIM0 = channelSetup->channelPeriodValue;
    LPC_MCPWM->MAT0 = channelSetup->channelPulsewidthValue;

	if (channelSetup->channelType)
	{
	  LPC_MCPWM->CON_SET = 1 << 1;
	}
	else
	{
	  LPC_MCPWM->CON_CLR = 1 << 1;
	}

	if (channelSetup->channelPolarity)
	{
	  LPC_MCPWM->CON_SET = 1 << 2;
	}
	else
	{
	  LPC_MCPWM->CON_CLR = 1 << 2;
	}

	if (channelSetup->channelDeadtimeEnable)
	{
	  LPC_MCPWM->CON_SET = 1 << 3;
	}
	else
	{
	  LPC_MCPWM->CON_CLR = 1 << 3;
	}
	LPC_MCPWM->DT &= ~0x3FF;
	LPC_MCPWM->DT |= channelSetup->channelDeadtimeValue & 0x3FF; 

	if (channelSetup->channelUpdateEnable)
	{
	  LPC_MCPWM->CON_CLR = 1 << 4;
	}
	else
	{
	  LPC_MCPWM->CON_SET = 1 << 4;
	}

    NVIC_EnableIRQ(MCPWM_IRQn);

    LPC_MCPWM->INTEN_SET = 7;
  }
  else if (channelNum == 1)
  {
    LPC_MCPWM->TC1 = channelSetup->channelTimercounterValue;
    LPC_MCPWM->LIM1 = channelSetup->channelPeriodValue;
    LPC_MCPWM->MAT1 = channelSetup->channelPulsewidthValue;

	if (channelSetup->channelType)
	{
	  LPC_MCPWM->CON_SET = 1 << 9;
	}
	else
	{
	  LPC_MCPWM->CON_CLR = 1 << 9;
	}

	if (channelSetup->channelPolarity)
	{
	  LPC_MCPWM->CON_SET = 1 << 10;
	}
	else
	{
	  LPC_MCPWM->CON_CLR = 1 << 10;
	}

	if (channelSetup->channelDeadtimeEnable)
	{
	  LPC_MCPWM->CON_SET = 1 << 11;
	}
	else
	{
	  LPC_MCPWM->CON_CLR = 1 << 11;
	}
	LPC_MCPWM->DT &= ~(0x3FF << 10);
	LPC_MCPWM->DT |= (channelSetup->channelDeadtimeValue & 0x3FF) << 10; 

	if (channelSetup->channelUpdateEnable)
	{
	  LPC_MCPWM->CON_CLR = 1 << 12;
	}
	else
	{
	  LPC_MCPWM->CON_SET = 1 << 12;
	}

    NVIC_EnableIRQ(MCPWM_IRQn);

    LPC_MCPWM->INTEN_SET = 7 << 4;
  }
  else if (channelNum == 2)
  {
    LPC_MCPWM->TC2 = channelSetup->channelTimercounterValue;
    LPC_MCPWM->LIM2 = channelSetup->channelPeriodValue;
    LPC_MCPWM->MAT2 = channelSetup->channelPulsewidthValue;

	if (channelSetup->channelType)
	{
	  LPC_MCPWM->CON_SET = 1 << 17;
	}
	else
	{
	  LPC_MCPWM->CON_CLR = 1 << 17;
	}

	if (channelSetup->channelPolarity)
	{
	  LPC_MCPWM->CON_SET = 1 << 18;
	}
	else
	{
	  LPC_MCPWM->CON_CLR = 1 << 18;
	}

	if (channelSetup->channelDeadtimeEnable)
	{
	  LPC_MCPWM->CON_SET = 1 << 19;
	}
	else
	{
	  LPC_MCPWM->CON_CLR = 1 << 19;
	}
	LPC_MCPWM->DT &= ~(0x3FF << 20);
	LPC_MCPWM->DT |= (channelSetup->channelDeadtimeValue & 0x3FF) << 20; 

	if (channelSetup->channelUpdateEnable)
	{
	  LPC_MCPWM->CON_CLR = 1 << 20;
	}
	else
	{
	  LPC_MCPWM->CON_SET = 1 << 20;
	}

    NVIC_EnableIRQ(MCPWM_IRQn);

    LPC_MCPWM->INTEN_SET = 7 << 8;
  }
  else
  {
	return ( FALSE );		/* Unknown channel number */
  }
	
  return (TRUE);
}

/******************************************************************************
** Function name:		MCPWM_WriteToShadow
**
** Descriptions:		Write to MCPWM shadow registers.
**
** parameters:			ChannelNum, setup structure
** Returned value:		true or fase.
** 
******************************************************************************/
uint32_t MCPWM_WriteToShadow(uint32_t channelNum, MCPWM_CHANNEL_SETUP_T * channelSetup)
{
  if (channelNum == 0)
  {
    LPC_MCPWM->LIM0 = channelSetup->channelPeriodValue;
    LPC_MCPWM->MAT0 = channelSetup->channelPulsewidthValue;
  }
  else if (channelNum == 1)
  {
    LPC_MCPWM->LIM1 = channelSetup->channelPeriodValue;
    LPC_MCPWM->MAT1 = channelSetup->channelPulsewidthValue;
  }
  else if (channelNum == 2)
  {
    LPC_MCPWM->LIM2 = channelSetup->channelPeriodValue;
    LPC_MCPWM->MAT2 = channelSetup->channelPulsewidthValue;
  }
  else
  {
	return ( FALSE );		/* Unknown channel number */
  }
	
  return (TRUE);
}

/******************************************************************************
** Function name:		MCPWM_CaptureEvent
**
** Descriptions:		MCPWM channel capture event setup
**
** parameters:			channel number, capture event input, timer reset, hnc enable
** Returned value:		None
** 
******************************************************************************/
void MCPWM_CaptureEvent(uint32_t channelNum, uint32_t captureEvent, uint32_t timerReset, uint32_t hncEnable)
{
  if (channelNum == 0)
  {
    LPC_MCPWM->CAPCON_CLR = 0x0024003F;
    LPC_MCPWM->CAPCON_SET = captureEvent & 0x3F;
	if (timerReset)
	{
      LPC_MCPWM->CAPCON_SET = 0x00040000;
	}
	if (hncEnable)
	{
      LPC_MCPWM->CAPCON_SET = 0x00200000;
	}
  }
  else if (channelNum == 1)
  {
    LPC_MCPWM->CAPCON_CLR = 0x00480FC0;
    LPC_MCPWM->CAPCON_SET = (captureEvent & 0x3F) << 6;
	if (timerReset)
	{
      LPC_MCPWM->CAPCON_SET = 0x00080000;
	}
	if (hncEnable)
	{
      LPC_MCPWM->CAPCON_SET = 0x00400000;
	}
  }
  else if (channelNum == 2)
  {
    LPC_MCPWM->CAPCON_CLR = 0x0093F000;
    LPC_MCPWM->CAPCON_SET = (captureEvent & 0x3F) << 12;
	if (timerReset)
	{
      LPC_MCPWM->CAPCON_SET = 0x00100000;
	}
	if (hncEnable)
	{
      LPC_MCPWM->CAPCON_SET = 0x00800000;
	}
  }
	
  return;
}

/******************************************************************************
** Function name:		countControl
**
** Descriptions:		MCPWM count control
**
** parameters:			channel number, count mode, capture event input
** Returned value:		None
** 
******************************************************************************/
void MCPWM_countControl(uint32_t channelNum, uint32_t countMode, uint32_t captureEvent)
{
  if (channelNum == 0)
  {
	if (countMode)
	{
      LPC_MCPWM->CNTCON_SET = 0x20000000 | (captureEvent & 0x3F);
	}
	else
	{
      LPC_MCPWM->CNTCON_CLR = 0x2000003F;
	}
  }
  else if (channelNum == 1)
  {
	if (countMode)
	{
      LPC_MCPWM->CNTCON_SET = 0x40000000 | ((captureEvent & 0x3F) << 6);
	}
	else
	{
      LPC_MCPWM->CNTCON_CLR = 0x4000FC00;
	}
  }
  else if (channelNum == 2)
  {
	if (countMode)
	{
      LPC_MCPWM->CNTCON_SET = 0x80000000 | ((captureEvent & 0x3F) << 12);
	}
	else
	{
      LPC_MCPWM->CNTCON_CLR = 0x803F0000;
	}
  }
	
  return;
}

/******************************************************************************
** Function name:		EMPWM_Start
**
** Descriptions:		Stop MCPWM channels
**
** parameters:			channel number
** Returned value:		None
** 
******************************************************************************/
void MCPWM_Start(uint32_t channel0, uint32_t channel1, uint32_t channel2)
{
  uint32_t regVal = 0;
  
  if (channel0 == 1)
  {
    regVal |= 1;
  }
  if (channel1 == 1)
  {
    regVal |= (1 << 8);
  }
  if (channel2 == 1)
  {
    regVal |= (1 << 16);
  }

  LPC_MCPWM->CON_SET = regVal;
  return;
}

/******************************************************************************
** Function name:		MCPWM_Stop
**
** Descriptions:		Stop MCPWM channels
**
** parameters:			channel number
** Returned value:		None
** 
******************************************************************************/
void MCPWM_Stop(uint32_t channel0, uint32_t channel1, uint32_t channel2)
{
  uint32_t regVal = 0;

  if (channel0 == 1)
  {
    regVal |= 1;
  }
  if (channel1 == 1)
  {
    regVal |= (1 << 8);
  }
  if (channel2 == 1)
  {
    regVal |= (1 << 16);
  }

  LPC_MCPWM->CON_CLR = regVal;
  return;
}

/******************************************************************************
** Function name:		MCPWM_acMode
**
** Descriptions:		MCPWM ac Mode enable/disable
**
** parameters:			acMode
** Returned value:		None
** 
******************************************************************************/
void MCPWM_acMode(uint32_t acMode)
{
  if (acMode)
  {
    LPC_MCPWM->CON_SET = 0x40000000;
  }
  else
  {
    LPC_MCPWM->CON_CLR = 0x40000000;
  }

  return;
}

/******************************************************************************
** Function name:		MCPWM_dcMode
**
** Descriptions:		MCPWM ac Mode enable/disable, output invert/non-invert
**
** parameters:			dcMode, outputInverted, outputPattern
** Returned value:		None
** 
******************************************************************************/
void MCPWM_dcMode(uint32_t dcMode, uint32_t outputInvered, uint32_t outputPattern)
{
  if (dcMode)
  {
    LPC_MCPWM->CON_SET = 0x80000000;
  }
  else
  {
    LPC_MCPWM->CON_CLR = 0x80000000;
  }

  if (outputInvered)
  {
    LPC_MCPWM->CON_SET = 0x20000000;
  }
  else
  {
    LPC_MCPWM->CON_CLR = 0x20000000;
  }

  LPC_MCPWM->CP = outputPattern;

  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
