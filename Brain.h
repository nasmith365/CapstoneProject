/*
 *	Brain.h
 *	Created: 1/20/2013 12:34:16 PM
 *  Author: Nicholas Smith, Sean Robb
 *	About: 	  
 */


#ifndef BRAIN_H_
#define BRAIN_H_

#include "CombinedSensors.h"
#include "Motor_Driver.h"

extern Queue SensorStates;//Has all the states gotten from the sensors

void Brain();//going to be the brains for the program
//These are the functions that will be run in the different states
int Front_State();
int Back_State();
int Right_State();
int Left_State();

//The States of our sensors
typedef enum	{	Wall=1,
					Center,
					StartTurn,				
					NoWall
				}Sensor_States;


typedef enum	{	FrontToRight,
					FrontToLeft,
					
					BackToRight,
					BackToLeft,
					
					RightToFront,
					RightToBack,
					
					LeftToFront,
					LeftToBack
				}Triggers;

//All the directions the robot can consider forward
typedef enum	{	front=1,
					back,
					right,
					left
				}Direction_States;

//All of our input and output for our transitions			 
FSM_transition_t Direction_Transitions[8];

FSM_model_t Brain_State_Machine;//Makes the State Machine and attaches Transitions
ListOfStates Direction_Table[4];//the array that the binary search is going to use to find the correct transition


#endif /* BRAIN_H_ */