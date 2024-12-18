/*
 * Lab5_FSM.c
 *
 *  Created on: Nov 13, 2024
 *      Author: thuyh
 */


#include "Lab5_FSM.h"

char temp = 0;
char buffer[MAX_BUFFER_SIZE];
char cmd[MAX_BUFFER_SIZE];
int index_buffer = 0 ;
uint8_t start_cmd,end_cmd;
uint8_t buffer_flag = 0;
uint8_t cmd_flag = 0;
uint8_t receive_flag = 0;
uint8_t wait_flag;
uint8_t num_send = 0;
uint8_t adc_value = 0;
int status = 1;
void fsm_init(){
	srand((unsigned int)time(NULL));
	index_buffer = 0;
	Receive_IT(&temp, 2);
}

void receive_data(){
	if(receive_flag == 1){
		if(status == IDLE || status == WAIT){
			if(temp!='\r' && temp != '\b'){
				if (index_buffer < MAX_BUFFER_SIZE){
					buffer[index_buffer++] = temp;
				}
				else{
					index_buffer = 0;
					buffer[index_buffer++] = temp;
				}
			}
			else if(temp == '\b'){
				if(index_buffer>0)
					index_buffer --;
			}
			else{
				buffer_flag = 1;
				//index_buffer = 0;
			}
			Receive_IT(&temp, 2);
			receive_flag = 0;
			Transmit(&temp, 1);
		}
	}
}
void CommandParser_fsm(){
	switch(status){
	case IDLE:
		idle_CommandParser();
		break;
	case FIND_SOC:
		find_SOC();
		break;
	case FIND_EOC:
		find_EOC();
		break;
	case CAPTURE_DATA:
		capture_data();
		break;
	default:
		break;
	}
}
void  UartCommunication_fsm(){
	switch(status){
	case CMD_CHECK:
		cmd_check();
		break;
	case SEND_DATA:
		send_data();
		break;
	case WAIT:
		wait_ok();
		break;
	}
}

void idle_CommandParser(){
	if(buffer_flag == 1){
		status = FIND_SOC;
		buffer_flag = 0;
	}
	else if (cmd_flag == 1){
		status = CMD_CHECK;
		cmd_flag = 0;
	}
}
void find_SOC(){
	uint8_t isStart = 0;
	for(int i = 0 ;i<index_buffer;i++){
		if(buffer[i] == '!')
		{
			isStart = 1;
			start_cmd = i;
			HAL_GPIO_TogglePin(GPIOA, LED_GREEN_Pin);
			break;
		}
	}
	if(isStart)
		status = FIND_EOC;
	else
	{
		status = IDLE;
		index_buffer = 0;
	}
}
void find_EOC(){
	for (int i = start_cmd ; i<index_buffer;i++){
		if(buffer[i] == '#'){
			end_cmd = i;
			status = CAPTURE_DATA;
			return;
		}
		if(buffer[i] == '!'){
			start_cmd = i;
		}
	}
	status = CAPTURE_DATA;
	index_buffer = 0;
}

void capture_data(){
	int size = end_cmd - start_cmd -1;
	if(size <= 0 ){
		status = IDLE;
		return;
	}
	int index = 0;
	int i = start_cmd+1;
	while (index < size && i < end_cmd){
		cmd[index++] = buffer[i++];
	}
	cmd[index] = '\0';
	cmd_flag = 1;
	status = IDLE;
}

void cmd_check(){
	if(wait_flag == 1 ){
		if(strcmp(cmd,"OK") == 0){
			HAL_GPIO_TogglePin(GPIOA, LED_BLUE_Pin);
			status = IDLE;
			wait_flag = 0;
			num_send = 0;
		}
		else{
			status = WAIT;
		}
		index_buffer = 0;
	}
	else if(strcmp(cmd,"RST") == 0 ){
		adc_value = rand() % 4096; // 12 bit
		status = SEND_DATA;
	}
	else{
		index_buffer = 0;
		status = IDLE;
	}
}
void send_data(){
		char str[20];
		sprintf(str, "!ADC = %u#\r",adc_value);
		Transmit(str, 1);
		index_buffer = 0;
		wait_flag = 1;
		status = WAIT;
		num_send ++;
		setTimer1(0, 1000);
}
void wait_ok(){
	if(num_send >= 3){
		status = IDLE;
		wait_flag = 0;
		num_send = 0;
	}
	else{
		if(buffer_flag == 1){
			status = FIND_SOC;
		}
		else {
			if(timer1_flag[0] == 1){
				status = SEND_DATA;
			}
		}
	}
}
