/*
 * modbus_rtu.h
 *
 *  Created on: Feb 19, 2022
 *      Author: dat_pham
 */

#ifndef SRC_MODBUS_RTU_H_
#define SRC_MODBUS_RTU_H_
#include "RingBuffer.h"
#include "main.h"
#endif /* SRC_MODBUS_RTU_H_ */

typedef enum {
	Master_to_slave = 0,
	Slave_to_master = 1
}rd_direction_t;

typedef struct {
	unsigned char addr;
	unsigned char function_code;
	unsigned char lenght;
	unsigned char data [20];
	unsigned char crc [2];
	unsigned char message_flag;
}rd_rx_message_t;

typedef struct {
	unsigned char addr ;
	unsigned char function_code;
	unsigned char start_reg_addr [2];
	unsigned char lenght[2];
	unsigned char crc [2];
}modbusTxNormal_t;

typedef struct {
	unsigned char function_code ;
	int(*cb_cmd_function)(rd_rx_message_t messgae);
}rd_message_handle_t;








typedef enum {
	functReadCoid = 1,
	functReadDiscreteInput = 2,
	functReadHooldingReg= 3,
	functReadInputReg= 4,
	functWriteSingleCoid = 5,
	functWriteSingleReg = 6,
}modbus_tdef;
#define NUM_OF_FUNCTION			10


#define MODBUS_FUNC_CODE_READ_COIL						0x01	// ADDR 0XXXX VD: 00001
#define MODBUS_FUNC_CODE_READ_DISCRETE_INPUTS			0x02	// ADDR 1XXXX VD: 10001
#define MODBUS_FUNC_CODE_READ_HOLDING_REG				0x03	// ADDR 4XXXX VD: 40001
#define MODBUS_FUNC_CODE_READ_INPUT_REG					0x04	// ADDR 3XXXX VD: 30001
#define MODBUS_FUNC_CODE_WRITE_SINGLE_COID				0x05	//write single bit data to slave register
#define MODBUS_FUNC_CODE_WRITE_SINGLE_REG				0x06	//write single byte data to slave register













int rd_modbus_read_coil_handle(rd_rx_message_t messgae,unsigned char *coilRead);
int rd_modbus_read_discrete_handle(rd_rx_message_t messgae);
int rd_modbus_read_holding_register_handle(rd_rx_message_t messgae);
int rd_modbus_read_input_register_handle(rd_rx_message_t messgae);
int rd_modbus_write_single_coid_handle(rd_rx_message_t messgae);
int rd_modbus_write_single_byte_handle(rd_rx_message_t messgae);




void modbus_message_rx(unsigned char Rx_In, rd_rx_message_t *Modbus_Rx);
unsigned short modbus_rtu_crc(unsigned char *buf, int len);
void uartRead(ringbuffer_t *handle);
void modbus_message_handle_ (ringbuffer_t *handle);
void mobusSendNormalFunction (unsigned char slave_addr, modbus_tdef functionCode,unsigned short start_reg_addr,unsigned short lenght);
