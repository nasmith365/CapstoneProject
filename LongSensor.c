/*
 * LongSensor.c
 *
 * Created: 2/17/2013 4:37:47 PM
 *  Author: nasmith
 */ 

#include "LongSensor.h"
#include <avr/io.h>

FSM_transition_t Long_Sensor_Transition[] =
{
	//Transitions for Wall State (to get out of)
	{Wall, GoToCenterLong, Center, Center_State_Long},
	{Wall, GoToNoWallLong, NoWall, NoWall_State_Long},
	{Wall, GoToStartTurnLong, StartTurn, Start_Turn_State_Long},	
	//Transitions for Center State (to get out of)
	{Center, GoToWallLong, Wall, Wall_State_Long},
	{Center, GoToNoWallLong, NoWall, NoWall_State_Long},
	{Center, GoToStartTurnLong, StartTurn, Start_Turn_State_Long},
	//Transitions for Center State (to get out of)
	{StartTurn, GoToWallLong, Wall, Wall_State_Long},
	{StartTurn, GoToNoWallLong, NoWall, NoWall_State_Long},
	{StartTurn, GoToCenterLong, Center, Center_State_Long},		
	//Transitions for NoWall State (to get out of)
	{NoWall, GoToWallLong, Wall, Wall_State_Long},
	{NoWall, GoToCenterLong, Center, Center_State_Long},
	{NoWall, GoToStartTurnLong, StartTurn, Start_Turn_State_Long}
};

FSM_model_t Long_Sensor_State_Machine = {.size = 12, .NumberPerGroup = 3, Short_Sensor_Transition};//Makes the State Machine and attaches Transitions
static int channel = 4;//keep track of what sensor we are reading from
static int nextchannel = 5;
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
	while ((ADCSRA&(1<<ADSC))==(1<<ADSC)){;}//*****************GET RID OF WHEN INTEGRATED WITH ALL CODE*********************
	//calculate state
	TenBitNumber = (ADCL_Value >> 6) | (ADCH_Value << 2);
	Range = (5000 / (TenBitNumber + 55)) + 2;
	//4 ifs to trigger(set) enums
	if (Range < 10)//true value is 8 but used the calculated value of 10
	{
		FSM_run(channel,GoToWallLong);
	}
	else if (Range < 14)//true value of 15 but used the calculated value of 14
	{
		FSM_run(channel,GoToCenterLong);
	}
	else if (Range < 18)//true value of 20 but used the calculated value of 18
	{
		FSM_run(channel,GoToStartTurnLong);
	}
	else{
		FSM_run(channel,GoToNoWall);
	}
	channel = nextchannel;
	++nextchannel;//increment the sensor to read from
	if (nextchannel > 7)//resets the channel to zero if it is greater than 7 because we have no more sensors
	{
		Q_enqueue((void*) &LongSensorArray, &LongSensorStates);//Sends data to the brain
		nextchannel =4;
	}
}

void Wall_State_Long()
{
	LongSensorArray[channel] = Wall;
}

void Center_State_Long()
{
	LongSensorArray[channel] = Center;
}

void Start_Turn_State_Long()
{
	LongSensorArray[channel] = StartTurn;
}

void NoWall_State_Long()
{
	LongSensorArray[channel] = NoWall;
}
