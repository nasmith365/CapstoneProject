/*
 *	Brain.c
 *	Created: 1/20/2013 12:33:26 PM
 *  Author: Nicholas Smith, Sean Robb
 *	About:  	   
 */

#include "Brain.h"

Queue SensorStates;//move to brain.cpp

FSM_transition_t Direction_Transitions[] =
{
	//Transitions if in our front state
	{front, FrontToRight, right, Right_State},
	{front, FrontToLeft, left, Left_State},
	//Transitions if in our back state
	{back, BackToRight, right, Right_State},
	{back, BackToLeft, left, Left_State},
	//Transitions if in our right state	
	{right, RightToFront, front, Front_State},
	{right, RightToBack, back, Back_State},
	//Transitions if in our left state	
	{left, LeftToFront, front, Front_State},
	{left, LeftToBack, back, Back_State}
};

FSM_model_t Brain_State_Machine = {.size = 8, .NumberPerGroup = 2, Direction_Transitions};//Makes the State Machine and attaches Transitions

static int angle = 360;//start initially at 360 degrees, keep track of angle we are currently moving at
static int Current_State = front;//keeps track of the current state

void Brain()//the main for this class
{
	int size = 8;//the amount of sensors data we will be taking in 
	int SensorData[size];//the array with all the sensor states in it
	
	//Schedule the first batch of sensors which should be the short senors
	Start(&CombinedSensor,6);
	Start(&CombinedSensor,12);
	Start(&CombinedSensor,18);
	Start(&CombinedSensor,24);
	//Schedule the second batch of sensors which should be the long sensors
	Start(&CombinedSensor,30);
	Start(&CombinedSensor,36);
	Start(&CombinedSensor,42);
	Start(&CombinedSensor,48);
	
	Start(&Brain, 49);//Schedule the brain
	Start(&Motor_Driver, 50);//Schedule the Motor Driver
	if (Q_isEmpty(&SensorStates))//if there is no data from the sensors, we are blind and dont want to tell the motors what to do so it exits
	{
		return;
	}
	int *ptr;
	ptr = (int) Q_dequeue(&SensorStates);
	for (int i = 0; i<size/2; ++i)//takes our array that we enqueued and makes it our sensor data --short sensors
	{
		SensorData[i] = *(ptr+i);
	}
	ptr = (int) Q_dequeue(&SensorStates);
	for (int i = 4; i<size; ++i)//takes our array that we enqueued and makes it our sensor data --long sensors
	{
		SensorData[i] = *(ptr+(i));
	}
	/* what order the sensors are enqueued
			  _____
			 /7 0 4\
		    |3     1|
			 \6_2_5/
	*/
	//SensorData[0] = 0 --ShortSensor
	//SensorData[1] = 1 --ShortSensor
	//SensorData[2] = 2 --ShortSensor
	//SensorData[3] = 3 --ShortSensor
	//SensorData[4] = 4 --LongSensor
	//SensorData[5] = 5 --LongSensor
	//SensorData[6] = 6 --LongSensor
	//SensorData[7] = 7 --LongSensor
	switch(Current_State)
	{
		case front:// Sensor 0 is front
			if (SensorData[4] == StartTurn)
			{
				angle=45;
			}
			else if (SensorData[7] == StartTurn)
			{
				angle=315;
			}
			if (SensorData[0] == Wall)//change to start turning
			{
				if ((SensorData[3] == NoWall))//left turn
				{
					FSM_run(777, FrontToLeft);
				}
				else if (SensorData[1] == NoWall)//right turn
				{
					FSM_run(777, FrontToRight);	
				}
				else if ((SensorData[3] == Wall) && (SensorData[1] == Wall))//finish
				{
					//never schedule the motors to run again and turn off all motors
					//aka brake then while(1){;}--kind of
				}
			}
		break;
		
		case back:// Sensor 2 is front
			if (SensorData[5] == StartTurn)
			{
				angle=135;
			}
			else if (SensorData[6] == StartTurn)
			{
				angle=225;
			}
			if (SensorData[2] == Wall)
			{
				if ((SensorData[1] == NoWall))//left turn
				{
					FSM_run(777, BackToRight);//mirrored because directions are states not relative
				}
				else if (SensorData[3] == NoWall)//right turn
				{
					FSM_run(777, BackToLeft);//mirrored because directions are states not relative
				}
				else if ((SensorData[1] == Wall) && (SensorData[3] == Wall))//finish
				{
					//never schedule the motors to run again and turn off all motors
				}
			}	
		break;
		
		case right:// Sensor 1 is front
			if (SensorData[4] == StartTurn)
			{
				angle=45;
			}
			else if (SensorData[5] == StartTurn)
			{
				angle=135;
			}
			if (SensorData[1] == Wall)
			{
				if ((SensorData[0] == NoWall))//left turn
				{
					FSM_run(777, RightToFront);
				}
				else if (SensorData[2] == NoWall)//right turn
				{
					FSM_run(777, RightToBack);
				}
				else if ((SensorData[0] == Wall) && (SensorData[2] == Wall))//finish
				{
					//never schedule the motors to run again and turn off all motors
				}
			}	
		break;
		
		case left:// Sensor 3 is front
		if (SensorData[6] == StartTurn)
		{
			angle=225;
		}
		else if (SensorData[7] == StartTurn)
		{
			angle=315;
		}
		if (SensorData[3] == Wall)
		{
			if ((SensorData[2] == NoWall))//left turn
			{
				FSM_run(777, LeftToBack);
			}
			else if (SensorData[0] == NoWall)//right turn
			{
				FSM_run(777, LeftToFront);
			}
			else if ((SensorData[0] == Wall) && (SensorData[2] == Wall))//finish
			{
				//never schedule the motors to run again and turn off all motors
			}
		}
		break;
		default: break;//Bad News (Bears)!!!!
	}	
	//****************************************************************************************************************
	//this is where we schedule our center and twists functions--look at pictures to see what twist and center is
	//****************************************************************************************************************
	Q_enqueue((void*) angle,&Motor);//enqueue the angle we want the motor to run
	if ((SensorData[4] == Center) && (SensorData[5] == Center) && (SensorData[6] == Center) && (SensorData[7] == Center))
	{
		Q_enqueue((void*) angle, &CurrentDirection);
		Start(&Motor_Tuning,1);
	}
	else
	{
		Q_resetPtrs(&SensorDistances);
		Q_resetPtrs(&CurrentDirection);//Should be empty but just in case
	}
}

int Front_State()
{
	Current_State = front;
	angle = 360;
	
	return 360;
}

int Back_State()
{
	Current_State = back;	
	angle = 180;
	
	return 180;
}

int Right_State()
{
	Current_State = right;	
	angle = 90;
	
	return 90;
}

int Left_State()
{
	Current_State = left;	
	angle = 270;
	
	return 270;
}
