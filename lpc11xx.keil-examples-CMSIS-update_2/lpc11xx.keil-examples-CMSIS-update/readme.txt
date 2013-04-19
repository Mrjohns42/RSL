/*****************************************************************************
 *   readme.txt:  Description of the LPC11xx Example Software Package
 *
 *   Copyright(C) 2010, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.10.20  ver 1.00    Preliminary version, first Release. Comparing
 *                           with LPC13xx, most of the peripherals are identical.
 *                           The major difference includes:
 *                           (1) LPC13xx uses Cortex M3 while LPC11xx uses Cortex M0.
 *                           (2) The NVIC alignment and level of priority has been changed. 
 *                           (3) LPC11xx added second SSP, SSP1.                             
 *                           (4) Multi-location for UART Modem pins
 *                           (5) PMU update in Low-power regulator and main regulator.
 *                           (6) Interrupt latency register is added.
 *                           (7) Known problems from LPC13xx V01.
 *
 *   2009.12.09  ver 1.01    (1) Major change in the header file which matches the
 *                           Users Manual now. Removed all the section related to
 *                           USB and CAN which may need to add back in the future.
 *                           The header is compliant with new CMSIS standard. 
 *                           (2) Removed USB clock config section in the startup, 
 *                           which may need to add back later.
 *                           (3) Added Keil uVison MDK 4.x support with new projects.
 *                           (4) Retested peripherals based on the new header and startup.
 *                           (5) Added examples for both MAT_OUT and CAP_IN for the timers. 
 *
 *   2010.04.26  ver 1.02    (1) Fixed a PCON setting issue that, unlike that in LPC17xx, 
 *                           SLEEP mode and DEEP SLEEP mode flag bits no longer exist 
 *                           in PCON register.
 *                           (2) Added PWM driver example.   
 *                           (3) Added CAN driver example.
 *   2010.05.24  ver 1.03    (1) Removed all the JTAG related definition and retested all 
 *
 *                           modules.
 *                           (2) Added WDT Window and Protected Mode support.
 *                           (3) Changed all the headers for *.c and *.h that date and
 *                           revision number will be updated automatically.
 *                           (4) Updated I2C engine.
 *
 *   2010.09.07  ver 1.04    (1) Added Initial version of CAN on-chip drivers
 *                           (2) Added Initial version of CANopen on-chip drivers
 *
 *   2010.11.05  ver 1.05    (1) Fixed Hardfault issue with the on_chip driver projects
 *                           (2) Added BOD example project
 *                           (3) Update in ADC.c (ADC example project)
 *                           (4) Update in GPIO.c & GPIO.h
 *                           (5) Update in SSP.h
 *
 *   2011.12.22  ver 1.06    (1) Added FLASH memory access register definitions
 *                           (2) Added FLASH signature generature register definitions
 *   2012.01.27  ver 1.07    (1) Pointed all the projects to product device
 *                           
 *	 2012.04.19  ver 1.08	(1) CMSIS upgrade
 *							(2) Added Read-me.txt to all projects
 *							(3) Code compatible with Keil MCB 1000 board
 *							(4) In ADC project: Changed ADC output displayed on the serial terminal
 *							(5) Removed USB examples since LPC11xx does not support USB
 *
 *   2012.05.30 ver  1.09   (1) LPC11xx.h updated with FlashCtrl register definitions
 *							(2) WDT example's readme modified
 *							(3) Minor changes to register access rights like SYSPLLSTAT, GPIO_RIS and few more
 *
 *   2012.07.11 ver  1.10   (1) SWD communication failure during debug session issue fixed
 *							(2) timer16.c has been changed. PIO10_10/SCK0/CT16B0_MAT2 intialization has been disabled. User should enable if necessary.
 *							(3) As a consequence, the following projects have been changed ADC, SSP.
 *							(4) PMU project modified: Sleep/DeepSleep mode configuration changed. Read-me modified.
 ******************************************************************************/

/*****************************************************************************/
Software that is described herein is for illustrative purposes only which 
provides customers with programming information regarding the products.
This software is supplied "AS IS" without any warranties. NXP Semiconductors 
assumes no responsibility or liability for the use of the software, conveys no 
license or title under any patent, copyright, or mask work right to the 
product. NXP Semiconductors reserves the right to make changes in the 
software without notification. NXP Semiconductors also make no representation 
or warranty that such application will be suitable for the specified use without 
further testing or modification.
/*****************************************************************************/


The Description of the Example software
===================

This example demonstrates the use of build-in peripherals on the NXP
LPC11xx family microcontrollers.

The Example software includes, common library, peripheral APIs, and test modules
for the APIs. The common library include startup file, standard definition and
header files, processor specific setup module, generic interrupt related APIs, 
timer routine. The peripheral directories include, ADC, GPIO, timer, SPI, I2C, 
Watchdog timer, UART, external interrupt, various USB class drivers ,etc.

Depending on the configuration of LPC11xx, it may include a CAN controller
or a USB Device controller, or none of them. The header file of register 
definition, LPC11xx.h, the clock configuration, and the NVIC APIs, includes 
both CAN and USB, but may not apply to the MCU you are using.
  
The development environment is Keil's, an ARM company now, uVision MDK 3.85 
or later, and ULINK2 ICE. The target boards is NXP Internal Eval board with 
LPC11xx MCU on it.

Please note that, the latest Keil MDK may or may not include the startup file, 
the header files, and Cortex related API files for NVIC and Systick driver. These 
files are also included in our common library. If the users wish not to use Keil 
MDK, the compiler needs to: 
  (1) set up the include path to COMMON/INC
  (2) change #include <lpc11xx.h> to "#include "lpc11xx.h" where is located
under COMMON/INC directory. 
  (3) Add "core_cm0.c" from COMMON/SRC into your own project.

The project is created for both target option "FLASH(Release)" and "RAM(Debug)". 
For "FLASH", the image code can be programmed into the flash and executed;
for "RAM", the image code can be loaded into internal SRAM(IRAM)
for easy debugging. The "RAM.ini" in each directory is a Keil uVision debugger
script file, once the debugger starts, the code will be loaded to the IRAM
first, then, "RAM.ini" will be executed and reset program counter(PC) to 
0x10000000(IRAM starting address) that code will be executed from.

For Flash Update, a FLM file for Keil MDK built-n flash utility is included if 
the revision of Keil MDK you have doesn't support LPC11xx yet. If the LPC11xx 
is not listed in the Keil MDK Device Database, choose "Cortex M0" as shown in 
our example project file, then, copy the file "LPC_IAP_CD0_32.FLM" from Sample Code
Bundle Root directory to "C:\Keil\ARM\Flash". Now, you go to "Options/Utilities/Flash 
Programming Setting", add "LPC114x IAP 32kB Flash" from a list of flash algorithm,
and use the Keil Built-in flash utility to program the flash.

Some external components, such as I2C temperature, SPI serial EEPROM, will be 
required to add on the board in order to complete the test below.

Known problems: 
    (1) Keil MDK configuration Wizard has not been tested for Clock configuration.
    (2) When using Flash download, at the end of the flash update, GUI shows
"Error: Flash download failed, Cortex M0". although, the flash has been updated
correctly. So, this message should be ignored. It depends on the revision of the
Keil MDK you use. For MDK with LPC11xx support, this shouldn't be an issue.


The directory tree and content of the sample software
===================

common
    -- inc
	-- lpc11xx.h		CMSIS Cortex-M0 Core Peripheral Access Layer Header 
				File for NXP LPC11xx Device Series 
	-- core_cm0.h		CMSIS Cortex-M0 Core Peripheral Access Layer Header File
	-- system_LPC11xx.h	CMSIS Cortex-M0 Device Peripheral Access Layer Header 
				File for the NXP LPC11xx Device Series
	-- type.h		Type definition Header file for NXP LPC11xx Family
	-- systick.h		Cortex M0 Systemtick header and definition 
	-- gpio.h		GPIO setting and I/O pin connfigured as external interrupt
				definition
	-- uart.h		UART setting header and definiton
	-- timer16.h		16-bit TIMER setting header and definition
	-- timer32.h		32-bit TIMER setting header and definition
	-- clkconfig.h          clock configuration header and definition  
    -- src
	-- core_cm0.c		CMSIS Cortex-M0 Core Peripheral Access Layer Source File
	-- startup_LPC11xx.s	CMSIS Cortex-M0 Core Device Startup File 
           			for the NXP LPC11xx Device Series
	-- system_LPC11xx.c	CMSIS Cortex-M0 Device Peripheral Access Layer Source
    				File for the NXP LPC11xx Device Series
	-- gpio.c		GPIO setting APIs and I/O pin connfigured as external 
				interrupt and interrupt handler
	-- uart.c		UART setting API and interrupt handler
	-- timer16.c		16-bit TIMER setting APIs and interrupt handler
	-- timer32.c		32-bit TIMER setting APIs and interrupt handler
	-- clkconfig.c		Misc. clock configuration setting and APIs in addition
				to system_lpc11xx.c  

Blinky
    -- blinky.c			General test, GPIO, Timer and NVIC test module, note: 
				API modules are in the COMMON directory and 
				shared and used by some other peripheral testing.
    -- blinky.uvproj		uVision 4.x project file    

ADC
    -- adc.h			ADC header
    -- adc.c			ADC APIs
    -- adctest.c		ADC controller test module
    -- adc.uvproj		uVision 4.x project file


GPIO
    -- gpiotest.c		External interrupt test module, APIs are 
				from COMMON directory
    -- gpio.uvproj		uVision 4.x project file

Systick
    -- systick.c		ARM Cortext M0 Core system tick test module, APIs 
				are from COMMON directory
    -- systick.uvproj		uVision 4.x project file
                
I2C
    -- i2c.h			I2C header
    -- i2c.c			I2C APIs
    -- i2ctest.c		I2C test module
    -- i2ctest.uvproj		uVision 4.x project file

I2CSlave
    -- i2cslave.h		I2C slave header
    -- i2cslave.c		I2C slave APIs
    -- i2cslvtst.c		I2C slave test module
    -- i2cslave.uvproj		uVision 4.x project file

Timer32
    -- timer32test.c		32-bit Timer test module, note: API modules
				are in the COMMON directory and 
				shared and used by some other peripheral testing.
    -- timer32.uvproj		uVision 4.x project file

PWM
    -- pwmtest.c		PWM test module, note: PWM API modules
				are in the COMMON directory, shared and used 
				by some other peripheral testing.
    -- pwm.uvproj		uVision 4.x project file

UART
    -- uarttest.c		UART test module, note: UART API modules
				are in the COMMON directory, shared and used 
				by some other peripheral testing.
    -- uart.uvproj		uVision 4.x project file

RS485
    -- rs485.h			RS485 header
    -- rs485.c			RS485 APIs
    -- rs485test.c		RS485 test module
    -- rs485.uvproj		uVision 4.x project file

WDT
    -- wdt.h			Watchdog timer header
    -- wdt.c			Watchdog timer APIs
    -- wdttest.c		Watchdog timer test module
    -- wdt.uvproj		uVision 4.x project file

CAN
    -- can.h			CAN header
    -- can.c			CAN APIs
    -- cantest.c		CAN test module
    -- can.uvproj		uVision 4.x project file

CAN on_chip
    -- rom_driver_CAN.h		CAN on-chip API function header
    -- rom_drivers.h		ROM drivers header
    -- main.c			CAN on-chip test module
    -- CAN on_chip.uvproj	uVision 4.x project file

CANopen on_chip
    -- rom_driver_CAN.h		CAN on-chip API function header
    -- rom_drivers.h		ROM drivers header
    -- main.c			CAN on-chip test module
    -- CANopen on_chip.uvproj	uVision 4.x project file

