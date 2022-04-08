/*
 * modbus_rtu.c
 *
 *  Created on: Feb 19, 2022
 *      Author: dat_pham
 */


#include "modbus_rtu.h"
rd_rx_message_t *modbusRxMessage;
extern ringbuffer_t 		vrts_ringbuffer_Data;
extern UART_HandleTypeDef huart2;
unsigned char data_buff[32] = {0};
unsigned char slave_addr = 0x23;
extern modbusMessageCheck_t modbus_message_handle;
rd_message_handle_t rd_modbus_function_code_list [NUM_OF_FUNCTION] = {
	{MODBUS_FUNC_CODE_READ_COIL,rd_modbus_read_coil_handle},
	{MODBUS_FUNC_CODE_READ_DISCRETE_INPUTS,rd_modbus_read_discrete_handle},
	{MODBUS_FUNC_CODE_READ_HOLDING_REG,rd_modbus_read_holding_register_handle},
	{MODBUS_FUNC_CODE_READ_INPUT_REG,rd_modbus_read_input_register_handle},
	{MODBUS_FUNC_CODE_WRITE_SINGLE_COID,rd_modbus_write_single_coid_handle},
	{MODBUS_FUNC_CODE_WRITE_SINGLE_REG,rd_modbus_write_single_byte_handle},
};

modbusTxNormal_t modbusTxMessage;

unsigned char readCoidData[8] = {0};
unsigned char readRegisterData[32] = {0};

//void modbus_send_ (unsigned char slave_addr, modbus_tdef functionCode,unsigned char start_addr,unsigned char lenght){
//	unsigned char buff_send[32] = {0};
//	modbusTxMessage.addr = slave_addr;
//	modbusTxMessage.function_code = functionCode;
//	modbusTxMessage.data[0]
//	buff_send
//	HAL_UART_Transmit(&huart2, buff_send, Size, Timeout), pData, Size, Timeout);
//
//}


void mobusSendNormalFunction (unsigned char slave_addr, modbus_tdef functionCode,unsigned short start_reg_addr,unsigned short lenght){
	modbusTxMessage.addr = slave_addr;
	modbusTxMessage.function_code = functionCode;
	modbusTxMessage.start_reg_addr[0] = start_reg_addr>>8;
	modbusTxMessage.start_reg_addr[1] = start_reg_addr;
	modbusTxMessage.lenght[0] = lenght>>8;
	modbusTxMessage.lenght[1] = lenght;
	if(HAL_UART_Transmit(&huart2, (unsigned char *)(&modbusTxMessage), sizeof(modbusTxMessage),100) == HAL_OK){

	}
}




int rd_modbus_read_coil_handle(rd_rx_message_t messgae,unsigned char *coilRead){
/*
	//RD_EDIT: thu tu cac byte read coil from 27 -> 51
	 * byte 1: tu coil 27->20   0xDA (11011010) -> coil 27 = 1 coil 20 = 0
	 * byte 2: tu coil 35->28
	 * byte 3: tu coil 43->36
	 * byte 4: tu coil 51->44
*/
	for(unsigned char i = 0;i<messgae.lenght;i++){
		messgae.data[i] = readCoidData[i];
	}
	return 0;
}

int rd_modbus_read_discrete_handle(rd_rx_message_t messgae){
	for(unsigned char i = 0;i<messgae.lenght;i++){
		messgae.data[i] = readCoidData[i];
	}
	return 0;
}

int rd_modbus_read_holding_register_handle(rd_rx_message_t messgae){

	for(unsigned char i = 0;i<messgae.lenght;i++){
		messgae.data[i] = readRegisterData[i];
	}
	return 0;
}

int rd_modbus_read_input_register_handle(rd_rx_message_t messgae){
	for(unsigned char i = 0;i<messgae.lenght;i++){
		messgae.data[i] = readRegisterData[i];
	}
	return 0;
}

int rd_modbus_write_single_coid_handle(rd_rx_message_t messgae){
	return 0;
}

int rd_modbus_write_single_byte_handle(rd_rx_message_t messgae){
	return 0;
}



int rd_modbus_message_handle (rd_rx_message_t temp){
	for(unsigned char i = 0;i<NUM_OF_FUNCTION;i++)
	if(temp.function_code  == rd_modbus_function_code_list[i].function_code){
		rd_modbus_function_code_list[i].cb_cmd_function(temp);
		return 0;
	}
	return -1;
}


unsigned short modbus_rtu_crc(unsigned char *buf, int len)
{
  unsigned int crc = 0xFFFF;
  for (int pos = 0; pos < len; pos++)
  {
	  crc ^= (unsigned int)buf[pos];    // XOR byte into least sig. byte of crc

	  for (int i = 8; i != 0; i--) {    // Loop over each bit
		if ((crc & 0x0001) != 0) {      // If the LSB is set
		  crc >>= 1;                    // Shift right and XOR 0xA001
		  crc ^= 0xA001;
		}
		else                            // Else LSB is not set
		  crc >>= 1;                    // Just shift right
	  }
  }
  return crc;
}



//void modbus_message_rx(unsigned char  Rx_In, rd_rx_message_t *Modbus_Rx)
//{
//	const unsigned char Data_OffSet = 3;
//	if(Modbus_Rx->Leng_Rx == 0){
//		Modbus_Rx->ADDR_Slave = Rx_In;
//	}
//
//	if(Modbus_Rx->Leng_Rx == 1){
//		Modbus_Rx->Function = Rx_In;
//	}
//
//	if(Modbus_Rx->Leng_Rx == 2){
//		Modbus_Rx->Leng_Data = Rx_In;
//	}
//
//	if((Modbus_Rx->Leng_Rx >= 3) && (Modbus_Rx->Leng_Rx < Data_OffSet + Modbus_Rx->Leng_Data) ){
//		Modbus_Rx->Data[Modbus_Rx->Leng_Rx -Data_OffSet] = Rx_In;
//	}
//
//	if(Modbus_Rx->Leng_Rx == Data_OffSet + Modbus_Rx->Leng_Data){
//		Modbus_Rx->CRC_16[0] =  Rx_In;
//	}
//
//	if(Modbus_Rx->Leng_Rx == Data_OffSet + Modbus_Rx->Leng_Data +1){
//		Modbus_Rx->CRC_16[1] =  Rx_In;
//	}
//
//	Modbus_Rx->Leng_Rx++;
//	if((Modbus_Rx->Leng_Rx >= 3) && (Modbus_Rx->Leng_Rx >= Modbus_Rx->Leng_Data + 5)){
//		Modbus_Rx->Leng_Rx = 0;
//	}
//
//}


//void Modbus_RTU_Send(uint8_t ADDR_Slave, uint8_t Function, uint8_t* Data, uint16_t Data_Leng)
//{
//	const uint8_t Leng_Offset_ADR_Function = 2;		// Leng data ADR and Function
//	const uint8_t Leng_Offset_CRC		   = 2;		// Leng CRC
//	uint8_t Data_Buff[MAX_LENG_SEND_RTU] = {0};
//
//	Data_Buff[0] = ADDR_Slave;
//	Data_Buff[1] = Function;
//
//	for(int i=0; i<Data_Leng; i++){
//		Data_Buff[i+Leng_Offset_ADR_Function] = *(Data+i);				// Shift 2 by offset ADDR, Function data
//	}
//	uint16_t CRC_Buff;
//	CRC_Buff = CRC16_2((uint8_t *)Data_Buff, Data_Leng+2);				// Shift 2 by offset ADDR, Function data
//
//	Data_Buff[Data_Leng + Leng_Offset_ADR_Function]    = (uint8_t) ((CRC_Buff     ) & 0xff);
//	Data_Buff[Data_Leng + Leng_Offset_ADR_Function +1] = (uint8_t) ((CRC_Buff >> 8) & 0xff);
//
//	Modbus_TxEn_RxDis;
//	CRC_Tx(Data_Buff, Data_Leng + Leng_Offset_ADR_Function + Leng_Offset_CRC);
//	Modbus_RxEn_TxDis;
//}

//void rd_process_UART(void){
//	if(vrts_ringbuffer_Data.count >=1){
//		ring_pop_tail(&vrts_ringbuffer_Data,(void*)&temp_buffer[0]);
//		if(radar_Data_UART->Hearder == 0x59 && vrts_ringbuffer_Data.count >=5 ){
//			ring_pop_tail(&vrts_ringbuffer_Data,(void*)&temp_buffer[1]);
//			ring_pop_tail(&vrts_ringbuffer_Data,(void*)&temp_buffer[2]);
//			for(unsigned char i = 0; i < radar_Data_UART->Length;i++){
//				ring_pop_tail(&vrts_ringbuffer_Data,(void*)&temp_buffer[i+3]);
//			}
//			ring_pop_tail(&vrts_ringbuffer_Data,(void*)&temp_buffer[radar_Data_UART->Length + 3]);
//			ring_pop_tail(&vrts_ringbuffer_Data,(void*)&temp_buffer[radar_Data_UART->Length + 4]);
//			rd_message_hande(temp_buffer);
//		}
//	}
//}



void uartRead(ringbuffer_t *handle){
	if(handle->count >= 1){
		ring_pop_tail(handle,(void *)(&modbusRxMessage->addr));
		if(modbusRxMessage->addr == slave_addr){
			ring_pop_tail(handle,(void *)(&modbusRxMessage->function_code));
			ring_pop_tail(handle,(void *)(&modbusRxMessage->lenght));
			for (unsigned char i = 0; i<(modbusRxMessage->lenght);i++){
				ring_pop_tail(handle,(void *)(&modbusRxMessage->data[i]));
			}
			ring_pop_tail(handle,(void *)(modbusRxMessage->crc));
			ring_pop_tail(handle,(void *)(modbusRxMessage->crc + 1));
			modbusRxMessage->message_flag = 1;

		}
	}
}


void modbus_message_handle_ (ringbuffer_t *handle){
	if(handle->ring_push_flag){
		if(GET_TICK - handle->ring_push_time_out >= 100){
			handle->ring_push_flag = 0;
			uartRead(handle);
		}
	}
}
