/****************************************************************************
 *   $Id:: MCPWMtest.c 6097 2011-01-07 04:31:25Z nxp12832                   $
 *   Project: NXP LPC17xx Motor Control PWM example
 *
 *   Description:
 *     This file contains MCPWM test modules, main entry, to test MCPWM APIs.
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
#include "timer.h"
#include "mcpwm.h"

int main (void) 
{
  MCPWM_CHANNEL_SETUP_T channelsetup[3];
  
  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemClockUpdate();
  
  MCPWM_Init();

  channelsetup[0].channelType = 0;
  channelsetup[0].channelPolarity = 0;
  channelsetup[0].channelDeadtimeEnable = 0;
  channelsetup[0].channelDeadtimeValue = 0;
#if SHADOW_UPDATE
  channelsetup[0].channelUpdateEnable = 1;
#else
  channelsetup[0].channelUpdateEnable = 0;
#endif
  channelsetup[0].channelTimercounterValue = 0;
  channelsetup[0].channelPeriodValue = 300;
  channelsetup[0].channelPulsewidthValue = 150;

  channelsetup[1].channelType = 0;
  channelsetup[1].channelPolarity = 0;
  channelsetup[1].channelDeadtimeEnable = 0;
  channelsetup[1].channelDeadtimeValue = 0;
#if SHADOW_UPDATE
  channelsetup[1].channelUpdateEnable = 1;
#else
  channelsetup[1].channelUpdateEnable = 0;
#endif
  channelsetup[1].channelTimercounterValue = 0;
  channelsetup[1].channelPeriodValue = 300;
  channelsetup[1].channelPulsewidthValue = 150;

  channelsetup[2].channelType = 0;
  channelsetup[2].channelPolarity = 0;
  channelsetup[2].channelDeadtimeEnable = 0;
  channelsetup[2].channelDeadtimeValue = 0;
#if SHADOW_UPDATE
  channelsetup[2].channelUpdateEnable = 1;
#else
  channelsetup[2].channelUpdateEnable = 0;
#endif
  channelsetup[2].channelTimercounterValue = 0;
  channelsetup[2].channelPeriodValue = 300;
  channelsetup[2].channelPulsewidthValue = 150;

  if (!MCPWM_Config(0, &channelsetup[0]))
  {
    return 0;
  }
  if (!MCPWM_Config(1, &channelsetup[1]))
  {
    return 0;
  }
  if (!MCPWM_Config(2, &channelsetup[2]))
  {
    return 0;
  }

#if CHANNEL_0_CAPTURE
  MCPWM_CaptureEvent(0, 0x01, 1, 0);
#endif
#if CHANNEL_1_CAPTURE
  MCPWM_CaptureEvent(1, 0x04, 1, 0);
#endif
#if CHANNEL_2_CAPTURE
  MCPWM_CaptureEvent(2, 0x10, 1, 0);
#endif

#if AC_MODE
  MCPWM_acMode(1);
#endif

#if DC_MODE
  MCPWM_dcMode(1, 1, 0x3F);
#endif

  LPC_GPIO3->FIODIR = 0x02000000;
  LPC_GPIO3->FIOSET = 0x02000000;
  delayMs(0, 20);
  LPC_GPIO3->FIOCLR = 0x02000000;

  MCPWM_Start(1,1,1);

  for (;;) {
#if SHADOW_UPDATE
    delayMs(0, 10000);
    channelsetup[0].channelPeriodValue = 150;
    channelsetup[0].channelPulsewidthValue = 75;

    channelsetup[1].channelPeriodValue = 150;
    channelsetup[1].channelPulsewidthValue = 75;

    channelsetup[2].channelPeriodValue = 150;
    channelsetup[2].channelPulsewidthValue = 75;
    if (!MCPWM_WriteToShadow(0, &channelsetup[0]))
    {
      return 0;
    }
    if (!MCPWM_WriteToShadow(1, &channelsetup[1]))
    {
      return 0;
    }
    if (!MCPWM_WriteToShadow(2, &channelsetup[2]))
    {
      return 0;
    }

    delayMs(0, 10000);
    channelsetup[0].channelPeriodValue = 300;
    channelsetup[0].channelPulsewidthValue = 150;

    channelsetup[1].channelPeriodValue = 300;
    channelsetup[1].channelPulsewidthValue = 150;

    channelsetup[2].channelPeriodValue = 300;
    channelsetup[2].channelPulsewidthValue = 150;
    if (!MCPWM_WriteToShadow(0, &channelsetup[0]))
    {
      return 0;
    }
    if (!MCPWM_WriteToShadow(1, &channelsetup[1]))
    {
      return 0;
    }
    if (!MCPWM_WriteToShadow(2, &channelsetup[2]))
    {
      return 0;
    }
#endif
  }
}
