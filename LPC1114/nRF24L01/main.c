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

#include "mpu6050.h"
#include "kalman.h"
#include "i2c.h"
#include "i2c.c"
#include "timer32.h"
#include <math.h>

#include <stdio.h>
#include <rt_misc.h>
#include <string.h>


#define MASTER 0x0
#define SLAVE  0x1

#define BOARD SLAVE
//#define BOARD MASTER

#define	_CH 1			// Channel 0..125
#define	_Address_Width	5	// 3..5
#define _Buffer_Size 16 // 1..32

// Define OUT/ON/OFF Demo board LED
#define LED_DIR_OUT LPC_GPIO0->DIR |= (1<<7);
#define LED_ON LPC_GPIO0->MASKED_ACCESS[(1<<7)] = 0
#define LED_OFF  LPC_GPIO0->MASKED_ACCESS[(1<<7)] = (1<<7)

volatile uint8_t MPU_Request = 0;
char Buf[_Buffer_Size] = {'\0'};


void NRF24L01_Receive(char Buf[_Buffer_Size]) {
	//int i;
	NRF24L01_CE_HIGH;
	//Delay_us(130);
	//for(i=0;i<0xFFF;i++);
	delay32Us(1, 130);
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

/*
*****************************************************************************
** Function name:		PIOINT1_IRQHandler
**
** Descriptions:		Use one GPIO pin(port1 pin5) as interrupt source from
**						NRF24L01+ Data Ready
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/ 
void PIOINT1_IRQHandler(void)
{
	int i;
    if(LPC_GPIO1->MIS | (1<<5)) //interrupt on PIO1_5
	{
		MPU_Request = 1;
		for(i=0; i < _Buffer_Size; i++) Buf[i] = '\0';
		NRF24L01_Receive(Buf);
		LPC_GPIO1->IC |= (1<<5);
	}
    return;
}


void Led_Blink(void) {
 	//unsigned int i;
 	LED_ON;
	delay32Ms(1, 1000);
	//for(i=0;i<0xFFFFF;i++){}
	LED_OFF;
	delay32Ms(1, 500);
	//for(i=0;i<0xFFFFF;i++){}
}

int main(void) {

#if BOARD == MASTER
	//BASE STATION MASTER
	//char Buf[_Buffer_Size] = "HOLA\0"; //Hello :)
	char Address[_Address_Width] = { 0x11, 0x22, 0x33, 0x44, 0x55 };
	uint32_t i;
	SER_init();

	SSP_IOConfig(0);
	SSP_Init(0);

	LED_DIR_OUT;
    Led_Blink(); //needed for timing while NRF powers up 
		
	NRF24L01_Init(_TX_MODE, _CH, _1Mbps, Address, _Address_Width, _Buffer_Size);
	
	for(i=0;i<0xFFFFF;i++){}  //wait for slaves to initialize

	while(1)
	{
		strcpy(Buf,"XYZ\0");
		NRF24L01_Send(Buf);
		Led_Blink();
		NRF24L01_Set_Device_Mode(_RX_MODE);
		NRF24L01_Receive(Buf);
		Led_Blink();
		NRF24L01_Set_Device_Mode(_TX_MODE);
		printf("%s\r\n", Buf);
		for(i=0;i<0xFFFF;i++){}
			
	}
#endif

#if BOARD == SLAVE
	//MPU POLLING SLAVE
	uint16_t i, j;
	uint32_t k;
	
	float acc_x, acc_z, gyro_x;
	float acc_angle, kal_angle, dt;
	char Address[_Address_Width] = { 0x11, 0x22, 0x33, 0x44, 0x55 };

	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);
	SER_init();	
	LED_DIR_OUT;
	Led_Blink(); //needed for timing
	
	SSP_IOConfig(0);
	SSP_Init(0);
	NRF24L01_Init(_RX_MODE, _CH, _1Mbps, Address, _Address_Width, _Buffer_Size);

//	NRF24L01_DRint_Init();
//	NRF24L01_Clear_Interrupts();	

	I2CInit(I2CMASTER);
	printf("Initializing MPU6050\r\n");
	while(MPU6050_init() != 0)
	{
		for(i = 0; i < 10000; i++){}
	} 
	MPU6050_setZero();
	kalman_init();
	printf("MPU6050 Ready!\r\n");		


	i = 0; 		
	init_timer32(0, 48000); //ms
	enable_timer32(0);
	//init_timer32(1, 48000);
	//enable_timer32(0);	
	while (1) 
	{
			gyro_x 	= 	MPU6050_getGyroRoll_degree();
			acc_x 	=   MPU6050_getAccel_x();
			acc_z 	= 	-MPU6050_getAccel_z();	

			acc_angle = atan2(acc_x , -acc_z) * 180/3.14159265358979323 ;
			dt = (float)read_timer32(0) / 1000;
			kal_angle = kalman_update(acc_angle,gyro_x, dt);
			reset_timer32(0);
			enable_timer32(0);	

			i++;
			if(i==0xF)
			{
				//uint8_t status;
				printf("Kalman_X = %.4f\r\n", kal_angle);
				i=0;
				//status = NRF24L01_Get_Status();
				//printf("Status: %d\r\n", status);
			}
			
			NRF24L01_Receive(Buf);
			//if(MPU_Request)
			{
				//printf("Got MPU request\r\n");
				if (strncmp(Buf,"HOLA\0",_Buffer_Size) == 0)
				{
					Led_Blink();		
					//printf("Buf: %s ",Buf);
				}
				else if(strncmp(Buf,"XYZ\0",_Buffer_Size) == 0)
				{
					int angle;
					Led_Blink();		
					//printf("Buf: %s \r\n",Buf);
				 	NRF24L01_Set_Device_Mode(_TX_MODE);
//					fp = (float*) Buf;
//					fp[0] = kal_angle;
					for(j=0; j < _Buffer_Size; j++) Buf[j] = '\0';
					angle = (int)kal_angle;
					snprintf(Buf,_Buffer_Size,"X: %d \r\n",angle);
					//strcpy(Buf,"OKAY\0");
					//for(k=0;k<0xFFFFFF;k++){}
					Led_Blink();
					NRF24L01_Send(Buf);
					NRF24L01_Set_Device_Mode(_RX_MODE);
					Led_Blink();
				}
				//MPU_Request = 0;	 
			}	 

			for(j = 0; j < 0xFF; j++){}
	}
#endif

}
