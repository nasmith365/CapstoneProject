/*
 *	Motor_Driver.h
 *	Created: 1/13/2013 1:44:02 PM
 *  Author: Nicholas Smith, Sean Robb
 *	About:	
 */ 

#ifndef MOTOR_DRIVER_H_
#define MOTOR_DRIVER_H_

#include "State_Machine.h"
#include "Queue.h"
extern Queue Motor;
extern Queue Centering;

void Motor_Driver();//main function that will be called to drive the motors

//These are the functions that will be run in the different states
int Three_Sixty_State();
int Fourty_Five_State();
int Ninety_State();
int One_Thirty_Five_State();
int One_Eighty_State();
int Two_Twenty_Five_State();
int Two_Seventy_State();
int Three_Fifteen_State();
//Breaking States
int Inverse_360();
int Inverse_90();
int Inverse_180();
int Inverse_270();

//all current states the motor can go into (degrees of the angle)
typedef enum	{	three_sixty,
					fourty_five,
					ninety,
					one_thirty_five,
					one_eighty,
					two_twenty_five,
					two_seventy,
					three_fifteen,
					inverse_360,
					inverse_90,
					inverse_180,
					inverse_270
				}Motor_States;
			 
//All of our input and output for our transitions
	FSM_transition_t Motor_Transitions[48];
		
	FSM_model_t Motor_State_Machine;//Makes the State Machine and attaches Transitions 
	ListOfStates State_Location_Table[8];//the array that the binary search is going to use to find the correct transition
	
#endif /* MOTOR_DRIVER_H_ */