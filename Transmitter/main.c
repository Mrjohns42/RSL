 /*
 * SSP0 pins:
 * MOSI: P0.9
 * MISO: P0.8
 * SCK : P0.6
 * CSN : P0.2
 * CE  : P2.0
 */

#include "LPC11xx.h"
#include <stdio.h>
#include <rt_misc.h>

#include "driver_config.h"
#include "ssp.h"
#include "cpu_lpc1000.h"
#include "nrf24l01.h"

#define	_CH 1			// Channel 0..125
#define	_Address_Width	5	// 3..5
#define _Buffer_Size 5 // 1..32

/* Import external functions from Serial.c file                               */
extern void SER_init (void);

void configureGPIO()
{
	//Enable CLKOUT
	LPC_IOCON->PIO0_1 &= ~0x3F; // Select clkout function for P0.1
	LPC_IOCON->PIO0_1 |= 0x01;
	LPC_SYSCON->CLKOUTCLKSEL = 0x00; // IRC: 0x00 System osci: 0x01 WTD: 0x02 Main clk: 0x03
	LPC_SYSCON->CLKOUTUEN = 0x01; // Update clock
	LPC_SYSCON->CLKOUTUEN = 0x00; // Toggle update register once
	LPC_SYSCON->CLKOUTUEN = 0x01;
	while ( !(LPC_SYSCON->CLKOUTUEN & 0x01) ); // Wait until updated
	LPC_SYSCON->CLKOUTDIV = 1; // Divided by 255

	//set port 0_7 to output (high current drain in LPC1114)
    LPC_GPIO0->DIR |= (1<<7);
}

void ledOn()
{
	LPC_GPIO0->DATA &= ~(1<<7);
}

void ledOff()
{						 
	LPC_GPIO0->DATA |= (1<<7);
}

void Led_Blink(void)
{
//	ledOn();
//	Delay_us(5000000);
//	ledOff();
//	Delay_us(5000000);
	unsigned int i;
	ledOn();
	for (i = 0; i < 0x0000FFFF; i++)
	{
	}
	ledOff();
	for (i = 0; i < 0x0000FFFF; i++)
	{
	}
}

void NRF24L01_Receive(char Buf[_Buffer_Size])
{
	NRF24L01_CE_HIGH;
	Delay_us(130);

	while ((NRF24L01_Get_Status() & _RX_DR) != _RX_DR);

	NRF24L01_CE_LOW;

	NRF24L01_Read_RX_Buf(Buf, _Buffer_Size);
	NRF24L01_Clear_Interrupts();
}

void NRF24L01_Send(char Buf[_Buffer_Size])
{
	NRF24L01_Write_TX_Buf(Buf, _Buffer_Size);

	NRF24L01_RF_TX();

	while ((NRF24L01_Get_Status() & _TX_DS) != _TX_DS);

	NRF24L01_Clear_Interrupts();
}



int main()
{

	int i, j = 0;
	char Buf[_Buffer_Size] = "HOLA\0"; //Hello :)
	char Address[_Address_Width] = { 0x11, 0x22, 0x33, 0x44, 0x55 };
		while(1)
	{
		Led_Blink();
	}
	//SER_init();
	configureGPIO();



	SSP_IOConfig(0);
	SSP_Init(0);		
//	Delay_Init();
//	while(1)
	{
		Led_Blink();
	}			
	NRF24L01_Init(_TX_MODE, _CH, _1Mbps, Address, _Address_Width, _Buffer_Size);

	while (1) {
		NRF24L01_Send(Buf);
		ledOn();
		for (i = 0; i < 0x0007FFFF; i++)
		{
		}
		ledOff();
		for (i = 0; i < 0x0007FFFF; i++)
		{
		}
	}

//	while (1)
//	{
//		ledOn();
//		printf("Led On, Iteration %d\n\r", j);
//		for (i = 0; i < 0x0007FFFF; i++)
//		{
//		}
//		ledOff();
//		printf("Led Off, Iteration %d\n\r", j);
//		for (i = 0; i < 0x0007FFFF; i++)
//		{
//		}
//		j++;
//	}
}
