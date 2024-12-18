/*
 * LAB5_FSM.h
 *
 *  Created on: Nov 13, 2024
 *      Author: thuyh
 */
#include "main.h"
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "software_timer.h"
#ifndef INC_LAB5_FSM_H_
#define INC_LAB5_FSM_H_
#define MAX_BUFFER_SIZE 30
#define IDLE 1
#define FIND_SOC 2 //Start of command
#define FIND_EOC 3 // End of command
#define CAPTURE_DATA 4
#define CMD_CHECK 5
#define SEND_DATA 6
#define WAIT 7
extern int status;
extern uint8_t receive_flag;
extern uint8_t buffer_flag;
extern uint8_t cmd_flag ;
#endif /* INC_LAB5_FSM_H_ */
void fsm_init();
void receive_data();
void CommandParser_fsm();
void UartCommunication_fsm();
void Receive_IT(char* pdata,int id);
void Transmit(char* pdata,int id);
void idle_CommandParser();
void find_SOC();
void find_EOC();
void capture_data();
void cmd_check();
void send_data();
void wait_ok();
