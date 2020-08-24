/*
 *	Soft_Timer.c
 *	Created: 1/11/2013 2:38:13 PM
 *  Author: Nicholas Smith, Sean Robb
 *	About: The header file for Soft_Timer.c that declares all needed struct and functions	
 */ 


#ifndef SOFT_TIMER_H_
#define SOFT_TIMER_H_

#include "Queue.h"

extern Queue ticks;//time passed in order to update MaintainAll() properly

typedef struct{void* action; int countdown;}Thread;//what is put into our registry 
//**********************************************************************************
//change for project
//instead of hard coding 8 we would like to use a variable
//the size has to be one more than the number of functions in the registry otherwise error
Thread registry_table[16];//Makes an array of threads to complete our table
//**********************************************************************************	

void (*func_ptr)();//Empty function so we can run our functions in maintain all
void Start(void* fnctorun,int time_delay);//take in a function pointer and how many seconds until it needs to run and returns the id of the timer
void MaintainAll();//Counts down the timers and executes actions

#endif /* SOFT_TIMER_H_ */