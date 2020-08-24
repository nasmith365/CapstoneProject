/*
 *	CombinedSensors.c
 *	Created: 3/12/2013 2:22:13 PM
 *  Author: Nicholas Smith, Sean Robb
 *	About:
 */ 
//360 sensor is hooked up to PA0
//90 sensor is hooked up to PA1
//180 sensor is hooked up to PA2
//270 sensor is hooked up to PA3
//45 sensor is hooked up to PA4
//135 sensor is hooked up to PA5
//225 sensor is hooked up to PA6
//315 sensor is hooked up to PA7
#include "CombinedSensors.h"
#include <avr/io.h>

//************************************************************************************************
//**************************Short Sensors Start***************************************************
//need to make so it has 2 states --nowall and wall (wall will be small (less than 10%))
//						--wall state will change which way is "front"
//						--nowall will allow the robot to change if the long sensor is in the correct range

FSM_transition_t Short_Sensor_Transition[] =
{
	//Transitions for Wall State (to get out of)
	{Wall, GoToNoWall, NoWall, NoWall_State},
	//Transitions for NoWall State (to get out of)
	{NoWall, GoToWall, Wall, Wall_State},
};

FSM_model_t Short_Sensor_State_Machine = {.size = 2, .NumberPerGroup = 1, Short_Sensor_Transition};//Makes the State Machine and attaches Transitions
static int channel = 0;//keep track of what sensor we are reading from
static int nextchannel = 1;
static int SensorArray[8];
//added for motor_tuning
static int DistanceArray[8];//array of the distances of each sensor

void ShortSensor()
{	
	int Range, ADCL_Value, ADCH_Value, TenBitNumber;
	//read from ADC
	ADCL_Value = ADCL;
	ADCH_Value = ADCH;

	ADMUX = ADMUX & 0xE0;//Clear the 5 Least significant bits (MUX bits) and keep the rest
	ADMUX |= (nextchannel << 0);//read from the correct channel for the correct sensor	
	
	//start conversion
	ADCSRA = 0xC6; //on, start conversion, auto triggering off, divide by 64
	//calculate state
	TenBitNumber = (ADCL_Value >> 6) | (ADCH_Value << 2);
	Range = (5200 / (TenBitNumber + 5)) - 2;//formula to find the range for using 2.56v internal reference for the sensors
	//4 ifs to trigger(set) enums************
	if (Range < 34)//probably need to change
	{
		FSM_run(channel,GoToWall);
	}
	else
	{
		FSM_run(channel,GoToNoWall);
	}
	DistanceArray[channel] = Range;//puts the sensord distance in the array for motor tuning to use
	channel = nextchannel;
	++nextchannel;//increment the sensor to read from
}

//*********************************Short Sensors End**********************************************
//************************************************************************************************
//*********************************Long Sensors Start*********************************************

FSM_transition_t Long_Sensor_Transition[] =
{
	//Transitions for Wall State (to get out of)
	{Wall, GoToCenter, Center, Center_State},
	{Wall, GoToNoWall, NoWall, NoWall_State},
	{Wall, GoToStartTurn, StartTurn, Start_Turn_State},	
	//Transitions for Center State (to get out of)
	{Center, GoToWall, Wall, Wall_State},
	{Center, GoToNoWall, NoWall, NoWall_State},
	{Center, GoToStartTurn, StartTurn, Start_Turn_State},
	//Transitions for Center State (to get out of)
	{StartTurn, GoToWall, Wall, Wall_State},
	{StartTurn, GoToNoWall, NoWall, NoWall_State},
	{StartTurn, GoToCenter, Center, Center_State},
	//Transitions for NoWall State (to get out of)
	{NoWall, GoToWall, Wall, Wall_State},
	{NoWall, GoToCenter, Center, Center_State},
	{NoWall, GoToStartTurn, StartTurn, Start_Turn_State}
};

FSM_model_t Long_Sensor_State_Machine = {.size = 12, .NumberPerGroup = 3, Long_Sensor_Transition};//Makes the State Machine and attaches Transitions
//static int channel = 4;//keep track of what sensor we are reading from
//static int nextchannel = 5;
static int LongSensorArray[4];

void LongSensor()
{	
	int Range, ADCL_Value, ADCH_Value, TenBitNumber;
	//read from ADC
	ADCL_Value = ADCL;
	ADCH_Value = ADCH;
	ADMUX = ADMUX & 0xE0;//Clear the 5 Least significant bits (MUX bits) and keep the rest
	ADMUX |= (nextchannel << 0);//read from the correct channel for the correct sensor	
	//start conversion
	ADCSRA = 0xC6; //on, start conversion, auto triggering off, divide by 64
	//calculate state
	TenBitNumber = (ADCL_Value >> 6) | (ADCH_Value << 2);
	Range = (9550 / (TenBitNumber + 2)) - 3;
	//4 ifs to trigger(set) enums
	if (Range < 10)//true value is 8 but used the calculated value of 10
	{
		FSM_run(channel,GoToWall);
	}
	else if (Range < 14)//true value of 15 but used the calculated value of 14
	{
		FSM_run(channel,GoToCenter);
	}
	else if (Range < 18)//true value of 20 but used the calculated value of 18
	{
		FSM_run(channel,GoToStartTurn);
	}
	else{
		FSM_run(channel,GoToNoWall);
	}
	DistanceArray[channel] = Range;//puts the sensord distance in the array for motor tuning to use
	channel = nextchannel;
	++nextchannel;//increment the sensor to read from
	if (nextchannel > 7)//resets the channel to zero if it is greater than 7 because we have no more sensors
	{
		Q_enqueue((void*) &SensorArray, &SensorStates);//Sends data to the brain
		Q_enqueue((void*) &DistanceArray, &SensorDistances);//Sends distances to motor tuning
		nextchannel = 0;	
	}
}
//************************************************************************************************
//*********************************Combine Sensors Start******************************************


void CombinedSensor()
{
	if(channel<4)
	{
		ShortSensor();
	}
	else
	{
		LongSensor();
	}
}

void Wall_State()
{
	SensorArray[channel] = Wall;
}

void Center_State()
{
	SensorArray[channel] = Center;
}

void Start_Turn_State()
{
	SensorArray[channel] = StartTurn;
}
void NoWall_State()
{
	SensorArray[channel] = NoWall;
}