/*
 * software_timer.h
 *
 *  Created on: Sep 14, 2024
 *      Author: thuyh
 */
#include "main.h"

#ifndef INS_SOFTWARE_TIMER_H_
#define INS_SOFTWARE_TIMER_H_

extern int timer1_flag[10];
void setTimer1(int index,int duration);
void SetTimerForTimerSet(int index,int duration,int timer_set);
void timerRun();
#endif /* INS_SOFTWARE_TIMER_H_ */
