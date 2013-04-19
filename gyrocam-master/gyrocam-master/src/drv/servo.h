/*
 * servo.h
 *
 *  Created on: Feb 7, 2013
 *      Author: bumi
 */

#ifndef SERVO_H_
#define SERVO_H_

#define TIMER_1MS 			(SystemCoreClock / 1000)
#define TIMER_1US	 		(SystemCoreClock / 1000000)

#define US_PER_DEGREE		(1000 / SERVO_MAX_ANGLE) /* 1000us = 1ms 1ms-1.5ms-2ms */
#define TICK_TIME			(1 / SystemCoreClock)
#define TICKS_PER_US		(1 / TICK_TIME )


void SERVO_init(void);
void SERVO_set(float angle);
void SERVO_set_slew(float angle);


#endif /* SERVO_H_ */
