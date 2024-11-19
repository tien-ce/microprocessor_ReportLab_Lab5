/*
 * software_timer.c
 *
 *  Created on: Sep 14, 2024
 *      Author: thuyh
 */
#include "software_timer.h"
	int time_cycle = 10;
	int timer1_counter[10];
	int timer1_flag[10];

	void setTimer1(int index,int duration){
		timer1_counter[index]=duration / time_cycle;
		timer1_flag[index] = 0;
	}
	void SetTimerForTimerSet(int index,int duration,int timer_set){
		time_cycle = timer_set;
		setTimer1(index, duration);
	}
	void timerRun(){
		for (int i = 0 ; i <10 ; i++){
			if(timer1_counter[i] >  0 ){
					timer1_counter[i]--;
			 }
			if(timer1_counter[i]<=0){
					timer1_flag[i]=1;
			}
		}


	}

