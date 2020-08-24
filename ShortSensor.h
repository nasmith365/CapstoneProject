/*
 *	Sensor.h
 *
 *	Created: 1/22/2013 2:44:02 PM
 *  Author:Nicholas Smith, Sean Robb
 *	About:
 */ 


#ifndef SENSOR_H_
#define SENSOR_H_

#include "State_Machine.h"
#include "Queue.h"
#include "Brain.h"

void ShortSensor();
void Wall_State();
void NoWall_State();

FSM_transition_t Short_Sensor_Transition[2];

FSM_model_t Short_Sensor_State_Machine;//Makes the State Machine and attaches Transitions
ListOfStates Short_Sensor_Table[2];//the array that the binary search is going to use to find the correct transition

typedef enum	{	GoToWall,
					GoToNoWall
				}Short_Sensor_Triggers;

#endif /* SENSOR_H_ */