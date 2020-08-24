/*
 *	CombinedSensors.h
 *	Created: 3/12/2013 2:22:13 PM
 *  Author: Nicholas Smith, Sean Robb
 *	About:
 */ 


#ifndef COMBINEDSENSORS_H_
#define COMBINEDSENSORS_H_

#include "State_Machine.h"
#include "Motor_tuning.h"
#include "Queue.h"
#include "Brain.h"

//********************************** Short Sensors ***********************************************
void ShortSensor();
void Wall_State();
void NoWall_State();

FSM_transition_t Short_Sensor_Transition[2];

FSM_model_t Short_Sensor_State_Machine;//Makes the State Machine and attaches Transitions
ListOfStates Short_Sensor_Table[2];//the array that the binary search is going to use to find the correct transition

//typedef enum	{	GoToWall,
					//GoToNoWall
				//}Short_Sensor_Triggers;
//************************************************************************************************
//************************************ Long Sensors **********************************************
void LongSensor();
//void Wall_State();
void Center_State();
void Start_Turn_State();
//void NoWall_State();

FSM_transition_t Long_Sensor_Transition[12];

FSM_model_t Long_Sensor_State_Machine;//Makes the State Machine and attaches Transitions
ListOfStates Long_Sensor_Table[4];//the array that the binary search is going to use to find the correct transition

//typedef enum	{	GoToWallLong,
					//GoToCenterLong,
					//GoToStartTurnLong,
					//GoToNoWallLong
				//}Long_Sensor_Triggers;
//************************************************************************************************
void CombinedSensor();
typedef enum	{	GoToWall,
					GoToCenter,
					GoToStartTurn,
					GoToNoWall
				}Sensor_Triggers;

#endif /* COMBINEDSENSORS_H_ */