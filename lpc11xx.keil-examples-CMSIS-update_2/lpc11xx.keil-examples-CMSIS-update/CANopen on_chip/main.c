/****************************************************************************
 *   $Id:: main.c 9364 2012-04-19 22:42:30Z nxp41306                        $
 *   Project: NXP LPC11xx CAN example
 *
 *   Description:
 *     This file is part of the CAN and CANopen on-chip driver examples.
 *
 ****************************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.

* Permission to use, copy, modify, and distribute this software and its 
* documentation is hereby granted, under NXP Semiconductors' 
* relevant copyright in the software, without fee, provided that it 
* is used in conjunction with NXP Semiconductors microcontrollers.  This 
* copyright, permission, and disclaimer notice must appear in all copies of 
* this code.
****************************************************************************/
#include "LPC11xx.h"
#include "rom_drivers.h"

#ifndef NULL
#define NULL    ((void *)0)
#endif

ROM **rom = (ROM **)0x1fff1ff8;

CAN_MSG_OBJ msg_obj;

/* Initialize CAN Controller */
uint32_t ClkInitTable[2] = {
  0x00000000UL, // CANCLKDIV
  0x00001C57UL  // CAN_BTR
};

/* Callback function prototypes */
void CAN_rx(uint8_t msg_obj_num);
void CAN_tx(uint8_t msg_obj_num);
void CAN_error(uint32_t error_info);

/* CANopen Callback function prototypes */
uint32_t CANOPEN_sdo_exp_read (uint16_t index, uint8_t subindex);
uint32_t CANOPEN_sdo_exp_write(uint16_t index, uint8_t subindex, uint8_t *dat_ptr);

/* Publish CAN Callback Functions */
CAN_CALLBACKS callbacks = {
	CAN_rx,
	CAN_tx,
	CAN_error,
	CANOPEN_sdo_exp_read,
	CANOPEN_sdo_exp_write,
	NULL,
	NULL,
	NULL
};

/*	CANopen read-only (constant) Object Dictionary (OD) entries
	Used with Expidited SDO only. Lengths = 1/2/4 bytes */
CAN_ODCONSTENTRY myConstOD [] = {
	/* index, subindex,	length,	value */
	{ 0x1000, 0x00, 	4, 		0x54534554UL },  // "TEST"
	{ 0x1018, 0x00, 	1, 		0x00000003UL },
	{ 0x1018, 0x01, 	4, 		0x00000003UL },
	{ 0x2000, 0x00, 	1, 		(uint32_t)'M' },
};

/* Application variables used in variable OD */
uint8_t  error_register;
uint32_t device_id;
uint32_t fw_ver;
uint16_t param;

/*	CANopen list of variable Object Dictionary (OD) entries
	Expedited SDO with length=1/2/4 bytes */
CAN_ODENTRY myOD [] = {
	/* index, subindex,	access_type | length,	value_pointer */
	{ 0x1001, 0x00, 	OD_EXP_RO | 1,			(uint8_t *)&error_register },
	{ 0x1018, 0x02, 	OD_EXP_RO | 4,			(uint8_t *)&device_id },
	{ 0x1018, 0x03, 	OD_EXP_RO | 4,			(uint8_t *)&fw_ver },
	{ 0x2001, 0x00, 	OD_EXP_RW | 2,			(uint8_t *)&param },
};

/* CANopen configuration structure */
static const CAN_CANOPENCFG myCANopen = {
	0x14,									// node_id
	5,										// msgobj_rx
	6,										// msgobj_tx
	1,										// isr_handled
	sizeof(myConstOD)/sizeof(myConstOD[0]),	// od_const_num 
	(CAN_ODCONSTENTRY *)myConstOD,			// od_const_table
	sizeof(myOD)/sizeof(myOD[0]),			// od_num
	(CAN_ODENTRY *)myOD,					// od_table
};

/*	CAN receive callback */
/*	Function is executed by the Callback handler after
	a CAN message has been received */
void CAN_rx(uint8_t msg_obj_num){
  
  /* Determine which CAN message has been received */
  msg_obj.msgobj = msg_obj_num;

  /* Now load up the msg_obj structure with the CAN message */
  (*rom)->pCAND->can_receive(&msg_obj);

  if (msg_obj_num == 1)
  {
    /* Simply transmit CAN frame (echo) with with ID +0x100 via buffer 2 */
    msg_obj.msgobj = 2;
    msg_obj.mode_id += 0x100;
    (*rom)->pCAND->can_transmit(&msg_obj);
  }

  return;
}

/*	CAN transmit callback */
/*	Function is executed by the Callback handler after
	a CAN message has been transmitted */
void CAN_tx(uint8_t msg_obj_num){
  return;
}

/*	CAN error callback */
/*	Function is executed by the Callback handler after
	an error has occured on the CAN bus */
void CAN_error(uint32_t error_info){
  return;
}

/*	CAN interrupt handler */
/*	The CAN interrupt handler must be provided by the user application.
	It's function is to call the isr() API located in the ROM */
void CAN_IRQHandler (void){
  (*rom)->pCAND->isr();
}

/* CANopen Callback for expedited read accesses */
uint32_t CANOPEN_sdo_exp_read(uint16_t index, uint8_t subindex){
	if (index == 0x2001)
	param++;
	
	return 0;  // Return 0 for successs, SDO Abort code for error
}

/* CANopen Callback for expedited write accesses */
uint32_t CANOPEN_sdo_exp_write(uint16_t index, uint8_t subindex, uint8_t *dat_ptr){
  if ((index == 0x2001) && (*(uint16_t *)dat_ptr != 0))
    return SDO_ABORT_VALUE_RANGE;
  else
    return 0;  // Return 0 for successs, SDO Abort code for error
}


int main(void) {

	SystemCoreClockUpdate();

	/* Output the Clk onto the CLKOUT Pin PIO0_1 to monitor the freq on a scope */
	LPC_IOCON->PIO0_1	= (1<<0);
	/* Select the MAIN clock as the clock out selection since it's driving the core */
	LPC_SYSCON->CLKOUTCLKSEL = 3;
	/* Set CLKOUTDIV to 6 */
	LPC_SYSCON->CLKOUTDIV = 10;		//	CLKOUT Divider = 10
	/* Enable CLKOUT */
	LPC_SYSCON->CLKOUTUEN = 0;
	LPC_SYSCON->CLKOUTUEN = 1;
	while (!(LPC_SYSCON->CLKOUTUEN & 0x01));

	/* Initialize the CAN controller */
	(*rom)->pCAND->init_can(&ClkInitTable[0], 1);

	/* Configure the CAN callback functions */
	(*rom)->pCAND->config_calb(&callbacks);

	/* Initialize CANopen handler and variables */
	error_register = 0x00;
	device_id      = 0xAA55AA55;
	fw_ver         = 0x00010002;
	param          = 33;
	(*rom)->pCAND->config_canopen((CAN_CANOPENCFG *)&myCANopen);

	/* Enable the CAN Interrupt */
	NVIC_EnableIRQ(CAN_IRQn);

	/* Send a simple one time CAN message */
	msg_obj.msgobj  = 0;
	msg_obj.mode_id = 0x700+0x20;
	msg_obj.mask    = 0x0;
	msg_obj.dlc     = 4;
	msg_obj.data[0] = 'T';	//0x54
	msg_obj.data[1] = 'E';	//0x45
	msg_obj.data[2] = 'S';	//0x53
	msg_obj.data[3] = 'T';	//0x54
	(*rom)->pCAND->can_transmit(&msg_obj);

	/* Configure message object 1 to receive all 11-bit messages 0x400-0x4FF */
	msg_obj.msgobj = 1;
	msg_obj.mode_id = 0x400;
	msg_obj.mask = 0x700;
	(*rom)->pCAND->config_rxmsgobj(&msg_obj);	

	while(1){
		__WFI();	/* Go to Sleep */
	}
}
