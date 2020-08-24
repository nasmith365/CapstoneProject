/*
 * LongSensor.h
 *
 * Created: 2/17/2013 4:37:57 PM
 *  Author: nasmith
 */ 


#ifndef LONGSENSOR_H_
#define LONGSENSOR_H_

#include "State_Machine.h"
#include "Queue.h"
#include "Brain.h"

void LongSensor();
void Wall_State_Long();
void Center_State_Long();
void Start_Turn_State_Long();
void NoWall_State_Long();

FSM_transition_t Long_Sensor_Transition[12];

FSM_model_t Long_Sensor_State_Machine;//Makes the State Machine and attaches Transitions
ListOfStates Long_Sensor_Table[4];//the array that the binary search is going to use to find the correct transition

typedef enum	{	GoToWallLong,
					GoToCenterLong,
					GoToStartTurnLong,
					GoToNoWallLong
				}Long_Sensor_Triggers;

#endif /* LONGSENSOR_H_ */