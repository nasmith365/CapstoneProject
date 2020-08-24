/*
 *	Sensor.c
 *	Created: 1/22/2013 2:43:39 PM
 *  Author: Nicholas Smith, Sean Robb
 *	About:
 */ 
//need to make so it has 2 states --nowall and wall (wall will be small (less than 10%))
//						--wall state will change which way is "front"
//						--nowall will allow the robot to change if the long sensor is in the correct range
#include "ShortSensor.h"
#include <avr/io.h>


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
static int ShortSensorArray[4];

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
	Range = (2914 / (TenBitNumber + 5)) - 1;
	//4 ifs to trigger(set) enums************
	if (Range < 15)//probably need to change
	{
		FSM_run(channel,GoToWall);
	}
	else
	{
		FSM_run(channel,GoToNoWall);
	}
	channel = nextchannel;
	++nextchannel;//increment the sensor to read from
	if (nextchannel > 3)//resets the channel to zero if it is greater than 7 because we have no more sensors
	{
		Q_enqueue((void*) &ShortSensorArray, &ShortSensorStates);//Sends data to the brain
		nextchannel = 0;
	}
}

void Wall_State()
{
	ShortSensorArray[channel] = Wall;
}

void NoWall_State()
{
	ShortSensorArray[channel] = NoWall;
}