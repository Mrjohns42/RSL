/*
 * @author
 * Copyright (C) 2012 Luis R. Hilario http://www.luisdigital.com
 *
 * SSP0 pins:   
 * MISO: P0.8
 * MOSI: P0.9
 * SCK : P0.6
 * CSN : P0.2
 * CE  : P0.3
 */

#include "driver_config.h"

#include "ssp.h"
#include "cpu_lpc1000.h"
#include "nrf24l01.h"
#include "serial.h"

#include <string.h>

#define	_CH 1			// Channel 0..125
#define	_Address_Width	5	// 3..5
#define _Buffer_Size 16 // 1..32

// Define OUT/ON/OFF Demo board LED
#define LED_DIR_OUT LPC_GPIO0->DIR |= (1<<7);
#define LED_ON LPC_GPIO0->MASKED_ACCESS[(1<<7)] = 0
#define LED_OFF  LPC_GPIO0->MASKED_ACCESS[(1<<7)] = (1<<7)

void NRF24L01_Receive(char Buf[_Buffer_Size]) {
	NRF24L01_CE_HIGH;
	Delay_us(130);

	while ((NRF24L01_Get_Status() & _RX_DR) != _RX_DR);

	NRF24L01_CE_LOW;

	NRF24L01_Read_RX_Buf(Buf, _Buffer_Size);
	NRF24L01_Clear_Interrupts();
}

void NRF24L01_Send(char Buf[_Buffer_Size]) {
	NRF24L01_Write_TX_Buf(Buf, _Buffer_Size);															    

	NRF24L01_RF_TX();

	while ((NRF24L01_Get_Status() & _TX_DS) != _TX_DS);

	NRF24L01_Clear_Interrupts();
}

void Led_Blink(void) {
	LED_ON;
	Delay_us(500000);
	LED_OFF;
	Delay_us(50000);
}

int main(void) {
	//RECEIVE		   
	int i;
	char Buf[_Buffer_Size];
	char Address[_Address_Width] = { 0x11, 0x22, 0x33, 0x44, 0x55 };

    SSP_IOConfig(0);
	SSP_Init(0);

	configureGPIO();
	SER_init();

	Delay_Init();

	LED_DIR_OUT;
	Led_Blink();
	

	NRF24L01_Init(_RX_MODE, _CH, _1Mbps, Address, _Address_Width, _Buffer_Size);

	while (1) {
		Buf[0] = 0x0;
		NRF24L01_Receive(Buf);

		
		if (strncmp(Buf,"HOLA\0",_Buffer_Size) == 0) {
			LED_ON;
			Delay_us(100000);
			LED_OFF;
		}
	
		for(i = 0; i < _Buffer_Size; i++)
		{
			sendchar(Buf[i]);
		}
		

	}
//	//SEND
//	char Buf[_Buffer_Size] = "HOLA\0"; //Hello :)
//	char Address[_Address_Width] = { 0x11, 0x22, 0x33, 0x44, 0x55 };
//
//	SSP_IOConfig(0);
//	SSP_Init(0);
//
//	Delay_Init();
//
//	LED_DIR_OUT;
//	Led_Blink();
//
//	NRF24L01_Init(_TX_MODE, _CH, _1Mbps, Address, _Address_Width, _Buffer_Size);
//
//	while (1) {
//		NRF24L01_Send(Buf);
//		Led_Blink();
//	}
}
