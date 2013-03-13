/****************************************************************************
 *   $Id:: mcpwm.h 5747 2010-11-30 23:45:33Z usb00423                       $
 *   Project: NXP LPC17xx MCPWM example
 *
 *   Description:
 *     This file contains MCPWM code header definition.
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
#ifndef __MCPWM_H 
#define __MCPWM_H

#define SHADOW_UPDATE		0
#define AC_MODE				0
#define DC_MODE				0
#define CHANNEL_0_CAPTURE	0
#define CHANNEL_1_CAPTURE	0
#define CHANNEL_2_CAPTURE	0

typedef struct {
  uint32_t channelType;
  uint32_t channelPolarity;
  uint32_t channelDeadtimeEnable;
  uint32_t channelDeadtimeValue;
  uint32_t channelUpdateEnable;
  uint32_t channelTimercounterValue;
  uint32_t channelPeriodValue;
  uint32_t channelPulsewidthValue;
} MCPWM_CHANNEL_SETUP_T;

void MCPWM_IRQHandler(void);
void MCPWM_Init(void);
uint32_t MCPWM_Config(uint32_t channelNum, MCPWM_CHANNEL_SETUP_T * channelSetup);
uint32_t MCPWM_WriteToShadow(uint32_t channelNum, MCPWM_CHANNEL_SETUP_T * channelSetup);
void MCPWM_CaptureEvent(uint32_t channelNum, uint32_t captureEvent, uint32_t timerReset, uint32_t hncEnable);
void MCPWM_countControl(uint32_t channelNum, uint32_t countMode, uint32_t captureEvent);
void MCPWM_Start(uint32_t channel0, uint32_t channel1, uint32_t channel2);
void MCPWM_Stop(uint32_t channel0, uint32_t channel1, uint32_t channel2);
void MCPWM_acMode(uint32_t acMode);
void MCPWM_dcMode(uint32_t dcMode, uint32_t outputInvered, uint32_t outputPattern);

#endif /* end __MCPWM_H */
/****************************************************************************
**                            End Of File
****************************************************************************/
