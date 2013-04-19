/*
 * servo.c
 *
 *  Created on: Feb 7, 2013
 *      Author: bumi
 */

#include "driver_config.h"
#include "target_config.h"

#include "servo.h"


float gLastAngle = 0;
void SERVO_init(){
	/* enable timer 1
	 * periode SystemCoreClock / 1000 -> 1ms * 20 -> 20ms -> 50Hz
	 */
	/* enable clk for timer 0 */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<9);
	/* reset on MR0 */
	LPC_TMR32B0->MCR = (1<<1);
	/* external match on EM0 */
	LPC_TMR32B0->EMR = (1<<10) | (1<<3);
	/* enable MAT3 for PIO11_0 */
	LPC_IOCON->R_PIO0_11 &= ~0x07;
	LPC_IOCON->R_PIO0_11 |= 0x03;
	/* enable PWM control for MAT0 and MAT3 */
	LPC_TMR32B0->PWMC = (1<<3) | (1<<0);
	/* set frequency (MAT3) */
	LPC_TMR32B0->MR0 = SERVO_PERIODE;
	/* 1.5ms for servo */
	LPC_TMR32B0->MR3 = SERVO_ZERO;
	/* enable timer */
	LPC_TMR32B0->TCR = 1;
}
void SERVO_set(float angle){
	uint32_t match;
	float tmp;

	tmp = (angle + SERVO_MAX_ANGLE/2) * (1400/SERVO_MAX_ANGLE);
	tmp = tmp * TIMER_1US;
	match = (uint32_t) tmp + SERVO_MAX_R;
	LPC_TMR32B0->MR3 =  match;
}

void SERVO_set_slew(float angle){
	uint32_t match;
	float tmp;

	angle = gLastAngle + (angle - gLastAngle) / SLEW_RATE;
	gLastAngle = angle;

	tmp = (angle + SERVO_MAX_ANGLE/2) * (1400/SERVO_MAX_ANGLE);
	tmp = tmp * TIMER_1US;
	match = (uint32_t) tmp + SERVO_MAX_R;
	LPC_TMR32B0->MR3 =  match;
}

