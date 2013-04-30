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
#include "uart.h"
#include "mpu6050.h"
#include "kalman.h"
#include "i2c.h"
#include "i2c.c"
#include "timer32.h"
#include <math.h>
#include "AX12.h"
#include <stdio.h>
#include <stdlib.h>
#include <rt_misc.h>
#include <string.h>	 

#define MASTER 0x0
#define SLAVE  0x1


/*** CHOOSE BOARD ***/
/********************/
#define BOARD SLAVE
//#define BOARD MASTER
/********************/


#define	_CH1 1			// Channel 0..125
#define _CH2 16
#define	_Address_Width	5	// 3..5
#define _Buffer_Size 16 // 1..32

// Define OUT/ON/OFF Demo board LED
#define LED_DIR_OUT LPC_GPIO0->DIR |= (1<<7);
#define LED_ON LPC_GPIO0->MASKED_ACCESS[(1<<7)] = 0
#define LED_OFF  LPC_GPIO0->MASKED_ACCESS[(1<<7)] = (1<<7)

volatile uint8_t MPU_Request = 0;
char Buf[_Buffer_Size] = {'\0'};


void NRF24L01_Receive(char Buf[_Buffer_Size]) {
	uint32_t i = 0;
	uint32_t j = 0;

	while ((NRF24L01_Get_Status() & _RX_DR) != _RX_DR && i < 0xFF) i++;
	if(i >= 0xFF)
	{
		NRF24L01_WriteReg(W_REGISTER | STATUS, _RX_DR );
		for(j=0; j < _Buffer_Size; j++) Buf[j] = '\0';
		return;
	}

	NRF24L01_CE_LOW;

	NRF24L01_Read_RX_Buf(Buf, _Buffer_Size);
	NRF24L01_WriteReg(W_REGISTER | STATUS, _RX_DR );
}

void NRF24L01_Send(char Buf[_Buffer_Size]) {
	uint32_t i = 0;
	NRF24L01_Write_TX_Buf(Buf, _Buffer_Size);															    

	NRF24L01_RF_TX();

	while ((NRF24L01_Get_Status() & _TX_DS) != _TX_DS && i < 0xFFFF) i++;

	NRF24L01_WriteReg(W_REGISTER | STATUS, _TX_DS);
}


void Led_Blink(void) {
 	LED_ON;
	delay32Ms(1, 1000);
	LED_OFF;
	delay32Ms(1, 500);

}

int main(void) {

#if BOARD == MASTER
	//BASE STATION MASTER
	uint8_t i;
	int j;
	int position[] = {512, 512, 512, 512, 512, 512};
	uint8_t robot1_2;
	uint16_t CH;
	char Address[_Address_Width] = { 0x11, 0x22, 0x33, 0x44, 0x55 };
	//char * sensor[] = {"R_ROTATOR", "R_SHOULDER", "R_ELBOW", "L_ROTATOR", "L_SHOULDER", "L_ELBOW"};

	delay32Ms(1, 2000);//allow time for devices to power up

	//initialize serial, also initializes GPIO
	AX12_begin(_1MHZ);
	LED_DIR_OUT;		 

	AX12_ledStatus(BROADCAST_ID, ON);
	delay32Ms(1,1000);
	AX12_ledStatus(BROADCAST_ID, OFF);

	for(i=0; i < 6; i++)
	{
		 AX12_move(i, position[i]);
		 delay32Ms(1, 500);
	}

	//initialize transceiver
	SSP_IOConfig(0);
	SSP_Init(0); 
	
	robot1_2 =  LPC_GPIO1->MASKED_ACCESS[(1<<2)];
	if(robot1_2) CH = _CH1;
	else CH = _CH2;		
	NRF24L01_Init(_TX_MODE, CH, _1Mbps, Address, _Address_Width, _Buffer_Size);

	while(1)
	{
		int pitch;
		for(i=0; i < 6; i++)
		{
			if( i%3 == 0) continue;  //ignore rotators
			Address[2] = i;
			NRF24L01_Set_TX_Address(Address, _Address_Width);
			NRF24L01_Set_RX_Pipe(0, Address, _Address_Width, _Buffer_Size);
			for(j=0; j < _Buffer_Size; j++) Buf[j] = '\0'; 
			strcpy(Buf,"XYZ\0");	   		
			NRF24L01_Set_Device_Mode(_TX_MODE);
			NRF24L01_Send(Buf);	
			NRF24L01_Set_Device_Mode(_RX_MODE);
			LED_ON;
			delay32Ms(1,5);
			NRF24L01_Receive(Buf);
			LED_OFF;
			if(Buf[0] == '\0') continue;
			pitch = atoi(Buf);
			pitch += 150;
		    if (pitch > 300) pitch = 300;
		    if (pitch < 0) pitch = 0; 		    
		    pitch = (int) ((float)pitch/300.0 * 1023);
			position[i] = pitch;
			if(i == 2 || i == 5) pitch = pitch - position[i-1] + 512;
			AX12_move(i, pitch);

			//printf("%s\r\n", Buf);
		} 			
	}
#endif

/******************************************************************************************************/

#if BOARD == SLAVE
	//MPU POLLING SLAVE
	char Address[_Address_Width] = { 0x11, 0x22, 0x33, 0x44, 0x55 };
	
	uint16_t i, j;
	uint8_t CH;
	uint8_t top_bot, right_left, robot1_2, ID = 0; //first option when VCC, second when GND
	float acc_x, acc_z, gyro_x;
	float acc_angle, kal_angle, dt;	

	delay32Ms(1, 1000);  //allow time for devices to power up

	//initialize serial, also initializes GPIO
	UARTInit(_100KHZ);
	LED_DIR_OUT;

	top_bot =  LPC_GPIO1->MASKED_ACCESS[(1<<0)];
	if(top_bot) ID += 1;
	else ID += 2;

	right_left =  LPC_GPIO1->MASKED_ACCESS[(1<<1)];
	if(right_left) ID += 0;
	else ID += 3;
	Address[2] = ID;
	
	robot1_2 =  LPC_GPIO1->MASKED_ACCESS[(1<<2)]; 
	if(robot1_2) CH = _CH1;
	else CH = _CH2;	

	//debug info
	if(top_bot) printf("Top\r\n");
	else printf("Bottom\r\n");
	if(right_left) printf("Right\r\n");
	else printf("Left\r\n");
	if(robot1_2) printf("Robot 1\r\n\r\n");
	else printf("Robot 2\r\n\r\n");
	printf("ID: %d",ID);

	delay32Ms(1,2000);   

	//initialize NRF24L01
	SSP_IOConfig(0);
	SSP_Init(0);
	NRF24L01_Init(_RX_MODE, CH, _1Mbps, Address, _Address_Width, _Buffer_Size); 
 	NRF24L01_DRint_Init();

	//initialize MPU6050
	I2CInit(I2CMASTER);
	printf("Initializing MPU6050\r\n");
	while(MPU6050_init() != 0)
	{
		delay32Ms(1,1);
	} 
	MPU6050_setZero();
	kalman_init();
	printf("MPU6050 Ready!\r\n");

	i = 0; 		
	init_timer32(0, 48); //us
	enable_timer32(0);
	
	while (1) 
	{
			gyro_x 	= 	MPU6050_getGyroRoll_degree();
			acc_x 	=   MPU6050_getAccel_x();
			acc_z 	= 	-MPU6050_getAccel_z();	

			acc_angle = atan2(acc_x , -acc_z) * 180/3.14159265358979323 ;
			dt = (float)read_timer32(0) / 1000000;
			kal_angle = kalman_update(acc_angle,gyro_x, dt);
			reset_timer32(0);
			enable_timer32(0);	

			i++;
			if(i==0xF)
			{  
				printf("Kalman_X = %.4f\r\n", kal_angle);
				i=0;  
			}	  			
			
			if( LPC_GPIO1->MASKED_ACCESS[(1<<5)] == 0 )	//data ready
			{
				NRF24L01_Receive(Buf);
				if (strncmp(Buf,"HOLA\0",_Buffer_Size) == 0)
				{
					Led_Blink();		
					//printf("Buf: %s ",Buf);
				}
				else if(strncmp(Buf,"XYZ\0",_Buffer_Size) == 0)
				{
					int angle;
					LED_ON;
				 	NRF24L01_Set_Device_Mode(_TX_MODE);
					for(j=0; j < _Buffer_Size; j++) Buf[j] = '\0';
					angle = (int)kal_angle;			 
					snprintf(Buf,_Buffer_Size,"%d",angle);
					NRF24L01_Send(Buf);
					NRF24L01_Set_Device_Mode(_RX_MODE);
					LED_OFF;
				} 					 
			}	 
	}
#endif

}
