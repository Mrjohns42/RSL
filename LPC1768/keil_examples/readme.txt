/*****************************************************************************
 *   readme.txt:  Description of the LPC17xx Example Software Package
 *
 *   Copyright(C) 2009, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.06.23  ver 1.00    Preliminary version, first Release 
 *   2009.08.24  ver 1.01    (1) Added ADC burst mode support, 
 *                           (2) UART and ADC
 *                           use PCLKSELx to identify PCLK to the peripheral
 *                           instead of hard coding. 
 *                           (3) Added USBBHostLite as part of code bundle. 
 *                           (4) Added workaround for MDC and MDIO bit-banging 
 *                           due to the lack of MDC/MDIO support in LPC175x 80-pin 
 *                           package. 
 *                           (5) Added USBCDC class driver, Audio class driver,
 *                           and Mass Storage Class driver using both DMA and non-DMA.
 *                           (6) Fixed CAN issue.
 *                           (7) Used the latest Keil CMSIS header definition to be 
 *                           released.
 *                           (8) Added RS485 mode.
 *                           (9) Fixed CTCR misalighment in timer data structure in the header.
 *   2010.01.07  ver 1.02    (1) Header file and startup file update for CANActivity and 
 *                           USBActivity.
 *                           (2) Added USBActivity and CANActivity interrupt examples for 
 *                           power down and wakeup.
 *                           (3) Fixed a cut/paste error in PWM example.
 *                           (4) Added uVision 4 projects for examples.
 *                           (5) Change the order of PCLKSELx setting before PLL configuration
 *                           to address the issue mentioned in the errata.
 *   2010.10.27  ver 1.03    (1) Clock source select fix inside the system_LPC17xx.c.
 *                           (2) Fixed CCLK Divider configuration range.
 *                           (3) Added check for PLL0 and CCLK Divider configuration.
 *                           (2) Added CAN Activity Wakeup project.
 *                           (3) Added RTC as the main clock project.
 *                           (4) Fixed USB Enable value in .\USBCDC\usbhw.c
 *   2011.01.10  ver 2.00    (1) Updated lpc17xx.h to address the wrong alignment issue on
 *                           CLKOUTCFG and DMAREQSEL registers.
 *                           (2) Removed FIFOLVL register from lpc17xx.h
 *                           (3) Added more DMA example for timer match trigger, UART, SSP, ADC,
 *                           DAC, etc.
 *                           (4) Added example to support both fast mode and fast mode plus on I2C0.                       
 *                           (5) Added RIT timer, BOD, I2C Slave, MCPWM, PMU examples, etc.
 *                           (6) Moved SystemInit() from main() to startup file to be compliant with 
 *                           CMSIS 2.0.
 *                           (7) Added SystemClockUpdate() to determine CPU Clock frequency
 *                           within main()
 *                           (8) Removed older uVision 3.x project files
 *                           (9) Fixed several project options to allow RAM builds
 *                
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
LPC17xx family microcontrollers.

The Example software includes, common library, peripheral APIs, and test modules
for the APIs. The common library include startup file, standard definition and
header files, processor specific setup module, generic interrupt related APIs, 
timer routine. The peripheral directories include, ADC, DAC, DMA, GPIO, PWM, 
Real-time clock, timer, SPI, I2C, Watchdog timer, UART, external 
interrupt, CAN, Ethernet, USB HID,etc.
  
The development environment is Keil's, an ARM company now, uVision MDK 4.13a 
or later, and ULINK2 ICE. The target boards are Keil's MCB1700 with a NXP's 
LPC1768 MCU on it.

Please note that, the latest Keil MDK includes the startup file, the header
files, and Cortex related API files for NVIC and Systick driver. These files
are also included in our common library. They may be slight change between the
revisions. If the users wish to use Keil MDK default setting, the compiler can 
also be set: 
Change "#include "lpc17xx.h" to "#include <lpc17xx.h> where should be 
located under C:\Keil\ARM\INC\NXP directory. This is also necessary if Keil 
releases the update of the header file. 

Many of the peripherals are similar to those in LPC23xx/LPC24xx, except NVIC
handling, not all the software modules have been ported from LPC23xx/24xx to
LPC17xx yet. e.g. once you are familiar with LPC23xx/24xx family MCU, you can
use LPC23xx/24xx as the reference and port simple driver from LPC23xx to LPC17xx.
Go to below website and check out the sample code bundle under "Support Doc." 
for more details.
http://www.standardics.nxp.com/products/lpc2000/all/~LPC2368/#LPC2368

The project is created for both target option "FLASH" and "RAM". 
For "FLASH", the image code can be programmed into the flash and executed;
for "RAM", the image code can be loaded into internal SRAM(IRAM)
for easy debugging. The "RAM.ini" in each directory is a Keil uVision debugger
script file, once the debugger starts, the code will be loaded to the IRAM
first, then, "RAM.ini" will be executed and reset program counter(PC) to 
0x10000000(IRAM starting address) that code will be executed from.

Some external components, such as I2C temperature, SPI serial EEPROM, will be 
required to add external components on the board in order to complete the test.

By default, the test program is for Keil MCB1700 board.

The directory tree and content of the sample software
===================

common
    -- inc
	-- lpc17xx.h		CMSIS Cortex-M3 Core Peripheral Access Layer Header 
				File for NXP LPC17xx Device Series 
	-- core_cm3.h		CMSIS Cortex-M3 Core Peripheral Access Layer Header File
	-- system_LPC17xx.h	CMSIS Cortex-M3 Device Peripheral Access Layer Header 
				File for the NXP LPC17xx Device Series
	-- type.h		Type definition Header file for NXP LPC17xx Family 
    -- src
	-- core_cm3.c		CMSIS Cortex-M3 Core Peripheral Access Layer Source File
	-- startup_LPC17xx.s	CMSIS Cortex-M3 Core Device Startup File 
           			for the NXP LPC17xx Device Series
	-- system_LPC17xx.c	CMSIS Cortex-M3 Device Peripheral Access Layer Source
    				File for the NXP LPC17xx Device Series

ADC
    -- adc.h			ADC header
    -- adc.c			ADC APIs
    -- dma.h			DMA header
    -- dma.c			DMA APIs
    -- adctest.c		ADC controller test module
    -- adc.uvproj		uVision 4 project file
    -- addmatest.c		ADC controller DMA test module
    -- adcdma.uvproj		uVision 4 project file

BOD
    -- bod.h			BOD header
    -- bod.c			BOD APIs
    -- timer.h			Timer header
    -- timer.c			Timer APIs
    -- bodtest.c		BOD controller test module
    -- bod.uvproj		uVision 4 project file

CAN
    -- can.h			CAN header
    -- can.c			CAN APIs
    -- cantest.c		CAN test module
    -- can.uvproj		uVision 4 project file

CANACT
    -- can.h			CAN header
    -- can.c			CAN APIs
    -- uart.h			UART header
    -- uart.c			UART APIs
    -- ONTHECAN.c		CAN Activity Wakeup test module
    -- can.uvproj		uVision 4 project file

DAC
    -- dac.h			DAC header
    -- dac.c			DAC APIs
    -- dma.h			DMA header
    -- dma.c			DMA APIs
    -- dactest.c		DAC test module
    -- dac.uvproj		uVision 4 project file
    
DMA
    -- dma.h			DMA header
    -- dma.c			GPDMA APIs, memory to memory only.
				Note: memory to peripheral or peripheral
				to memory are in each peripheral directories
				such as ADC, Timer, SSP, I2S, etc.
    -- dmatest.c		DMA test module
    -- dma.uvproj		uVision 4 project file
    
EMAC
    -- easyweb.h		easyweb header
    -- easyweb.c		easyweb (EMAC test module)
    -- emac.h			EMAC header
    -- emac.c			EMAC APIs
    -- tcpip.h			header-file for tcpip.c
    -- tcpip.c			implements the TCP/IP-stack and provides a simple API
    -- webpage.h		web content
    -- easyweb.uvproj		uVision 4 project file 
    
EXTINT
    -- extint.h			External interrupt header
    -- extint.c			External interrupt APIs
    -- einttest.c		External interrupt test module
    -- extint.uvproj		uVision 4 project file
                
GPIO
    -- iotest.c			GPIO and Fast I/O test module
    -- gpio.uvproj		uVision 4 project file

I2C
    -- i2c.h			I2C header
    -- i2c.c			I2C APIs
    -- i2cmst.c			I2C test module
    -- i2c.uvproj		uVision 4 project file

I2CSLAVE
    -- i2cslave.h		I2C slave header
    -- i2cslave0.c		I2C0 slave APIs
    -- i2cslave1.c		I2C1 slave APIs
    -- i2cslave2.c		I2C2 slave APIs
    -- i2cslvtst.c		I2C slave test module
    -- i2cslave.uvproj		uVision 4 project file
    
I2S
    -- i2s.h			I2S header
    -- i2s.c			I2S APIs
    -- dma.c			I2S DMA APIs
    -- dma.h			I2S DMA header
    -- i2stest.c		I2S test module
    -- i2s.uvproj		uVision 4 project file
        
MCPWM
    -- mcpwm.h			MCPWM header
    -- mcpwm.c			MCPWM APIs
    -- timer.h			Timer header
    -- timer.c			Timer APIs
    -- mcpwmtest.c		MCPWM test module
    -- mcpwm.uvproj		uVision 4 project file

PMU
    -- pmu.h			PMU header
    -- extint.h			External Interrupt as wakeup source			  
    -- pmu.c			PMU APIs
    -- rtc.h			RTC header
    -- rtc.c			RTC APIs for Deep Power Down test
    -- pmutest.c		PMU test module
    -- pmu.uvproj		uVision 4 project file

PWM
    -- pwmc.h			PWM header
    -- pwm.c			PWM APIs
    -- pwmtest.c		PWM test module
    -- pwm.uvproj		uVision 4 project file

RITTimer
    -- rittimer.h		RIT timer header
    -- rittime.c		RIT timer APIs
    -- rittest.c		RIT timer test module
    -- rittimer.uvproj		uVision 4 project file

RS485
    -- rs485.h			RS-485 header
    -- rs485.c			RS-485 APIs
    -- rs485test.c		RS-485 test module
    -- rs485.uvproj		uVision 4 project file
    
RTC
    -- rtc.h			Real-time clock(RTC) header
    -- rtc.c			Real-time clock(RTC) APIs
    -- rtctest.c		Real-time clock(RTC) test module
    -- rtc.uvproj		uVision 4 project file

RTCmainclock
    -- system_LPC17xx_RTCmain.c Copy of sytem_LPC17xx.c with modifications
    -- RTC_Mainclock.c		RTC as main clock test module
    -- RTCmainclock.uvproj	uVision 4 project file 

SSP
    -- ssp.h			SSP header
    -- ssp.c			SSP APIs
    -- dma.h			DMA header
    -- dma.c			DMA APIs
    -- sspdma.h			SSP DMA header
    -- sspdma.c			SSP DMA APIs
    -- ssptest.c		SSP test module
    -- sspdmatest.c		SSP DMA test module
    -- ssp.uvproj		uVision 4 project file
    -- sspdma.uvproj		uVision 4 project file
                        
Timer
    -- timer.h			Timer header
    -- timer.c			Timer APIs
    -- dma.h			DMA header
    -- dma.c			DMA APIs
    -- tmrtest.c		Timer test module.
    -- tmrdmatest.c		Timer trigger DMA test module
    -- timer.uvproj		uVision 4 project file
    -- timerdma.uvproj		uVision 4 project file

UART
    -- uart.h			UART header
    -- uart.c			UART APIs
    -- dma.h			DMA header
    -- dma.c			DMA APIs
    -- uart_dma.h		UART DMA header
    -- uart_dma.c		UART DMA APIs
    -- uarttest.c		UART test module
    -- udmatest.c		UART test module
    -- uart.uvproj		uVision 4 project file
    -- uartdma.uvproj		uVision 4 project file

USBHID
    -- hid.h			USB HID related header
    -- hiduser.h		USB HID user specific header
    -- hiduser.c		USB HID user APIs
    -- usb.h			USB header
    -- usbcfg.h			USB configuration header
    -- usbcore.h		USB Core header
    -- usbcore.c		USB Core APIs
    -- usbdesc.h		USB descriptor header
    -- usbdesc.c		USB descriptor APIs
    -- usbhw.h			USB hardware header
    -- usbhw.c			USB hardware APIs
    -- usbreg.h			USB misc. register header
    -- usbuser.h		USB user header
    -- useuser.c		USB user APIs
    -- demo.c			USB HID test module main entry
    -- demo.h                   header file for main entry
    -- usbhid.uvproj		uVision 4 project file
	
USBMEM
    -- msc.h			USB MSC related header
    -- mscuser.h		USB MSC user specific header
    -- mscuser.c		USB MSC user APIs
    -- usb.h			USB header
    -- usbcfg.h			USB configuration header
    -- usbcore.h		USB Core header
    -- usbcore.c		USB Core APIs
    -- usbdesc.h		USB descriptor header
    -- usbdesc.c		USB descriptor APIs
    -- usbhw.h			USB hardware header
    -- usbhw.c			USB hardware APIs
    -- usbreg.h			USB misc. register header
    -- usbuser.h		USB user header
    -- useuser.c		USB user APIs
    -- diskimg.c                USB Virtual file page
    -- memory.c			USB MSC test module main entry
    -- memory.h                 header file for main entry
    -- usbmem.uvproj		uVision 4 project file
    -- dma.sct                  uVision scatter file for use of DMA.

USBCDC   
    -- cdc.h			USB CDC related header
    -- cdcuser.h		USB CDC Device Class user specific header
    -- cdcuser.c		USB CDC Device Class user APIs
    -- usb.h			USB header
    -- usbcfg.h			USB configuration header
    -- usbcore.h		USB Core header
    -- usbcore.c		USB Core APIs
    -- usbdesc.h		USB descriptor header
    -- usbdesc.c		USB descriptor APIs
    -- usbhw.h			USB hardware header
    -- usbhw.c			USB hardware APIs
    -- usbreg.h			USB misc. register header
    -- usbuser.h		USB user header
    -- usbuser.c		USB user APIs
    -- vcomdemo.h		header for VCOM demo
    -- vcomdemo.c		USB CDC(Virtual COM) test module, main entry
    -- virtualCom.uvproj	uVision 4 project file
    -- serial.h			Header for UART port
    -- serial.c			UART module APIs
    -- mcb1700-vcom.inf		The host side driver installation file

USBAudio
    -- audio.h			USB Audio related header
    -- adcuser.h		USB Audio Device Class user specific header
    -- adcuser.c		USB Audio Device Class user APIs
    -- usb.h			USB header
    -- usbaudio.h		USB Audio misc. definition header
    -- usbcfg.h			USB configuration header
    -- usbcore.h		USB Core header
    -- usbcore.c		USB Core APIs
    -- usbdesc.h		USB descriptor header
    -- usbdesc.c		USB descriptor APIs
    -- usbhw.h			USB hardware header
    -- usbhw.c			USB hardware APIs
    -- usbreg.h			USB misc. register header
    -- usbuser.h		USB user header
    -- useuser.c		USB user APIs
    -- usbmain.c		USB Audio test module
    -- usbaudio.uvproj		uVision 4 project file
    -- dma.sct                  uVision scatter file for use of DMA.	

WDT
    -- wdt.h			Watchdog timer header
    -- wdt.c			Watchdog timer APIs
    -- wdttest.c		Watchdog timer test module
    -- wdt.uvproj		uVision 4 project file

