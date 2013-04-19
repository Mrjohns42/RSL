/*
 * mpu6050.c
 *
 *  Created on: Jan 24, 2013
 *      Author: bumi
 */

#include "mpu6050.h"
#include "target_config.h"
#include "driver_config.h"

#include "i2c.h"

extern volatile uint8_t 	I2CMasterBuffer[BUFSIZE], I2CSlaveBuffer[BUFSIZE];
extern volatile uint32_t 	I2CReadLength, I2CWriteLength;

float zero_acc_x,zero_acc_z;
uint16_t zero_gyro_roll;

uint8_t MPU6050_init() {

	uint8_t state;

	I2CWriteLength 		= 3;
	I2CReadLength		= 0;
	I2CMasterBuffer[0] 	= MPU6050_ADRESS;
	I2CMasterBuffer[1] 	= MPU6050_RA_PWR_MGMT_1;
	I2CMasterBuffer[2] 	= 0b00000001; // reset device y-axis gyro as clk;

	state = I2CEngine();
	if(state != I2C_OK) return 1;

	I2CWriteLength 		= 3;
	I2CReadLength		= 0;
	I2CMasterBuffer[0] 	= MPU6050_ADRESS;
	I2CMasterBuffer[1] 	= MPU6050_RA_CONFIG;
	I2CMasterBuffer[2] 	= 0b00000000; // Lowpass
	//I2CMasterBuffer[2] 	= 0b00000011; // Lowpass
	state = I2CEngine();
	if(state != I2C_OK) return 1;


	I2CWriteLength 		= 3;
	I2CReadLength		= 0;
	I2CMasterBuffer[0] 	= MPU6050_ADRESS;
	I2CMasterBuffer[1] 	= MPU6050_RA_PWR_MGMT_1;
	I2CMasterBuffer[2] 	= 0b00000100; // wakeup

	state = I2CEngine();
	if(state != I2C_OK) return 1;


	I2CWriteLength 		= 3;
	I2CReadLength		= 0;
	I2CMasterBuffer[0] 	= MPU6050_ADRESS;
	I2CMasterBuffer[1] 	= MPU6050_RA_GYRO_CONFIG;
	I2CMasterBuffer[2] 	= 0b00001000; // gyro range 600°/s

	state = I2CEngine();
	if(state != I2C_OK) return 1;

	I2CWriteLength 		= 3;
	I2CReadLength		= 0;
	I2CMasterBuffer[0] 	= MPU6050_ADRESS;
	I2CMasterBuffer[1] 	= MPU6050_RA_ACCEL_CONFIG;
	I2CMasterBuffer[2] 	= 0b00011000; // 16G range

	state = I2CEngine();
	if(state != I2C_OK) return 1;

	return 0;
}

uint8_t MPU6050_whoami(){

	uint8_t state;

	I2CWriteLength 		= 2;
	I2CReadLength		= 1;
	I2CMasterBuffer[0] 	= MPU6050_ADRESS;
	I2CMasterBuffer[1] 	= MPU6050_RA_WHO_AM_I;
	I2CMasterBuffer[2] 	= MPU6050_ADRESS | RD_BIT;

	state = I2CEngine();
	if(state != I2C_OK) return 1;

	if(I2CSlaveBuffer[0] != 0x68) return 1;

	return 0;
}

void MPU6050_setZero(){

uint16_t i;
int32_t tmp_raw;

	for(i=0;i<250;i++){
		tmp_raw += MPU6050_getGyroRoll_raw();
	}
	tmp_raw /= 250;
	zero_gyro_roll = tmp_raw;
}


int16_t MPU6050_getGyroRoll_raw(){

	int16_t tmp;
	uint8_t state;

	I2CWriteLength 		= 2;
	I2CReadLength		= 1;
	I2CMasterBuffer[0] 	= MPU6050_ADRESS;
	I2CMasterBuffer[1] 	= MPU6050_RA_GYRO_YOUT_H;
	I2CMasterBuffer[2] 	= MPU6050_ADRESS | RD_BIT;

	state = I2CEngine();
	if(state != I2C_OK) return 1;

	tmp = (I2CSlaveBuffer[0] << 8) ;

	I2CWriteLength 		= 2;
	I2CReadLength		= 1;
	I2CMasterBuffer[0] 	= MPU6050_ADRESS;
	I2CMasterBuffer[1] 	= MPU6050_RA_GYRO_YOUT_L;
	I2CMasterBuffer[2] 	= MPU6050_ADRESS | RD_BIT;

	state = I2CEngine();
	if(state != I2C_OK) return 1;
	tmp |= (I2CSlaveBuffer[0]) ;
	return tmp - zero_gyro_roll;
}

float MPU6050_getGyroRoll_degree(){

	int16_t tmp;
	float roll;

	tmp = MPU6050_getGyroRoll_raw();
	roll = (tmp / 65.5 );
	return roll;
}

float MPU6050_getGyroRoll_rad(){

	int16_t tmp;
	float roll;

	tmp = MPU6050_getGyroRoll_raw();
	roll = (float) ((tmp / 65.5) * 3.14159) / 180;
	return roll ;
}

int16_t MPU6050_getAccel_x_raw(){
	int16_t tmp;
	uint8_t state;

	I2CWriteLength 		= 2;
	I2CReadLength		= 1;
	I2CMasterBuffer[0] 	= MPU6050_ADRESS;
	I2CMasterBuffer[1] 	= MPU6050_RA_ACCEL_YOUT_H;
	I2CMasterBuffer[2] 	= MPU6050_ADRESS | RD_BIT;

	state = I2CEngine();
	if(state != I2C_OK) return 1;

	tmp = (I2CSlaveBuffer[0] << 8) ;

	I2CWriteLength 		= 2;
	I2CReadLength		= 1;
	I2CMasterBuffer[0] 	= MPU6050_ADRESS;
	I2CMasterBuffer[1] 	= MPU6050_RA_ACCEL_YOUT_L;
	I2CMasterBuffer[2] 	= MPU6050_ADRESS | RD_BIT;

	state = I2CEngine();
	if(state != I2C_OK) return 1;

	tmp |= (I2CSlaveBuffer[0]) ;

	return tmp - zero_acc_x ;
}

float MPU6050_getAccel_x(){

	int16_t tmp;
	float x;

	tmp = MPU6050_getAccel_x_raw();
	x = (float) tmp / 2048;
	return x;
}

int16_t MPU6050_getAccel_z_raw(){

		int16_t tmp;
		uint8_t state;

		I2CWriteLength 		= 2;
		I2CReadLength		= 1;
		I2CMasterBuffer[0] 	= MPU6050_ADRESS;
		I2CMasterBuffer[1] 	= MPU6050_RA_ACCEL_ZOUT_H;
		I2CMasterBuffer[2] 	= MPU6050_ADRESS | RD_BIT;

		state = I2CEngine();
		if(state != I2C_OK) return 1;

		tmp = (I2CSlaveBuffer[0] << 8) ;

		I2CWriteLength 		= 2;
		I2CReadLength		= 1;
		I2CMasterBuffer[0] 	= MPU6050_ADRESS;
		I2CMasterBuffer[1] 	= MPU6050_RA_ACCEL_ZOUT_L;
		I2CMasterBuffer[2] 	= MPU6050_ADRESS | RD_BIT;

		state = I2CEngine();
		if(state != I2C_OK) return 1;

		tmp |= (I2CSlaveBuffer[0]) ;
		return tmp - zero_acc_z; /* TBD */

}
float MPU6050_getAccel_z(){

	int16_t tmp;
	float z;

	tmp =  MPU6050_getAccel_z_raw() ;
	z = (float) tmp / 2048;
	return z;
}
