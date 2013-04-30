/*
 * driver_config.h
 *
 *  Created on: Aug 31, 2010
 *      Author: nxp28548
 */

#ifndef DRIVER_CONFIG_H_
#define DRIVER_CONFIG_H

#include <LPC11xx.h>

#define CONFIG_ENABLE_DRIVER_CRP						1
#define CONFIG_CRP_SETTING_NO_CRP						1

#define CONFIG_ENABLE_DRIVER_SSP						1  
#define CONFIG_ENABLE_DRIVER_GPIO						0
#define CONFIG_ENABLE_DRIVER_I2C						1
#define CONFIG_ENABLE_DRIVER_TIMER32                    1
#define CONFIG_ENABLE_DRIVER_UART                       1
#define CONFIG_UART_ENABLE_INTERRUPT                    1
#define CONFIG_UART_ENABLE_TX_INTERRUPT				    0
#define CONFIG_UART_DEFAULT_UART_IRQHANDLER             1
 /* DRIVER_CONFIG_H_ */
#endif
