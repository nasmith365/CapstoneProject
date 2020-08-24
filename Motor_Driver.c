/*
 *	Motor_Driver.c
 *	Created: 1/13/2013 1:44:02 PM
 *  Author: Nicholas Smith, Sean Robb
 *	About:	This sets up our motor driver allowing us to send this function a degree to run
 *			in and then it will set our motors to what to run.
 */ 
#include <avr/io.h>
#include "Motor_Driver.h"


//outputs for motors
/*					PortC
Key: D= Direction Pin
	 B = Braking Pin
direction
its moving	7	6	5	4		3	2	1	0
	90		1	0	0	0		0	0	1	0		
	360		1	0	0	0		1	0	0	0
	270		0	0	1	0		1	0	0	0	
	180		0	0	1	0		0	0	1	0	
	Pin		D	B	D	B		D	B	D	B
	45		1	0	0	0		0	1	0	1
	315		0	1	0	1		1	0	0	0
	225		0	0	1	0		0	1	0	1
	135		0	1	0	1		0	0	1	0
Which wheels get what
evens all on one side, odd on the other
				PC0	PC1
			PC6			PC4
			PC7			PC5
				PC2 PC3
*/
//so we do not have to write this ugly statement multiple times
//checks to see if an edge is within 1 us of the current time
#define timeFromEdge 2
#define edge (((OCR2 - TCNT2) < timeFromEdge) || ((OCR2 - TCNT2) > -timeFromEdge) || ((OCR1A - TCNT1) < timeFromEdge) || ((OCR1A - TCNT1) > -timeFromEdge) || ((OCR1B - TCNT1) < timeFromEdge) || ((OCR1B - TCNT1) > -timeFromEdge))

Queue Motor;
Queue Centering;

FSM_transition_t Motor_Transitions[] =
{
	//Transitions if in our 360 state
{three_sixty, 45, fourty_five, Fourty_Five_State},
{three_sixty, 90, inverse_360, Inverse_360},
{three_sixty, 315, three_fifteen, Three_Fifteen_State},
{three_sixty, 270, inverse_360, Inverse_360},
	//Transitions if in our 45 state
{fourty_five, 90, ninety, Ninety_State},
{fourty_five, 135, one_thirty_five,One_Thirty_Five_State},
{fourty_five, 360, three_sixty, Three_Sixty_State},
{fourty_five, 315, three_fifteen, Three_Fifteen_State},
	//Transitions if in our 90 state
{ninety, 135, one_thirty_five,One_Thirty_Five_State},
{ninety, 180, inverse_90,Inverse_90},
{ninety, 45, fourty_five, Fourty_Five_State},
{ninety, 360, inverse_90, Inverse_90},
	//Transitions if in our 135 state
{one_thirty_five, 90, ninety, Ninety_State},
{one_thirty_five, 45, fourty_five, Fourty_Five_State},
{one_thirty_five, 180, one_eighty,One_Eighty_State},
{one_thirty_five, 225, two_twenty_five, Two_Twenty_Five_State},
	//Transitions if in our 180 state
{one_eighty, 135, one_thirty_five,One_Thirty_Five_State},
{one_eighty, 90, inverse_180, Inverse_180},
{one_eighty, 225, two_twenty_five, Two_Twenty_Five_State},
{one_eighty, 270, inverse_180, Inverse_180},
	//Transitions if in our 225 state
{two_twenty_five, 180, one_eighty,One_Eighty_State},
{two_twenty_five, 135, one_thirty_five,One_Thirty_Five_State},
{two_twenty_five, 270, two_seventy, Two_Seventy_State},
{two_twenty_five, 315, three_fifteen, Three_Fifteen_State},
	//Transitions if in our 270 state
{two_seventy, 135, one_thirty_five,One_Thirty_Five_State},
{two_seventy, 180, inverse_270,Inverse_270},
{two_seventy, 315, three_fifteen, Three_Fifteen_State},
{two_seventy, 360, inverse_270, Inverse_270},
	//Transitions if in our 315 state
{three_fifteen, 270, two_seventy, Two_Seventy_State},
{three_fifteen, 225, two_twenty_five, Two_Twenty_Five_State},
{three_fifteen, 360, three_sixty, Three_Sixty_State},
{three_fifteen, 45, fourty_five, Fourty_Five_State},
	//Braking Transitions
	//for Inverse 360
{inverse_360, 90, ninety,Ninety_State},
{inverse_360, 270, two_seventy,Two_Seventy_State},
{inverse_360, 90, ninety,Ninety_State},
{inverse_360, 270, two_seventy,Two_Seventy_State},
	//for Inverse 90
{inverse_90, 360, three_sixty,Three_Sixty_State},
{inverse_90, 180, one_eighty,One_Eighty_State},
{inverse_90, 360, three_sixty,Three_Sixty_State},
{inverse_90, 180, one_eighty,One_Eighty_State},
	//For inverse 180
{inverse_180, 90, ninety,Ninety_State},
{inverse_180, 270, two_seventy,Two_Seventy_State},
{inverse_180, 90, ninety,Ninety_State},
{inverse_180, 270, two_seventy,Two_Seventy_State},
	//for inverse 270
{inverse_270, 360, three_sixty,Three_Sixty_State},
{inverse_270, 180, one_eighty,One_Eighty_State},
{inverse_270, 360, three_sixty,Three_Sixty_State},
{inverse_270, 180, one_eighty,One_Eighty_State}
};

FSM_model_t Motor_State_Machine = {.size = 48, .NumberPerGroup = 4,Motor_Transitions};//Makes the State Machine and attaches Transitions

int Three_Sixty_State()
{
	while(edge){;}
	PORTC=0x88;
	return 360;
}

int Fourty_Five_State()
{
	while(edge){;}
	PORTC = 0x85;
	return 45;
}

int Ninety_State()
{
	while(edge){;}
	PORTC=0x82;
	return 90;
}

int One_Thirty_Five_State()
{
	while(edge){;}
	PORTC = 0x52;
	return 135;
}

int One_Eighty_State()
{
	while(edge){;}
	PORTC=0x22;
	return 180;
}

int Two_Twenty_Five_State()
{
	while(edge){;}
	PORTC = 0x25;
	return 225;
}

int Two_Seventy_State()
{
	while(edge){;}
	PORTC=0x28;
	return 270;
}

int Three_Fifteen_State()
{
	while(edge){;}
	PORTC = 0x58;
	return 315;
}

void Motor_Driver()
{
	int AngleToRun;
	if (Q_isEmpty(&Motor))//Is to check to see if the queue is empty and if it is then just exit
	{
		return;
	}
	AngleToRun = (int) Q_dequeue(&Motor);
	if (!(Q_isEmpty(&Centering)))
	{
		AngleToRun = (int) Q_dequeue(&Centering);
	}
	FSM_run(132, AngleToRun);//Dequeues from the motor queue what state to run
}

int Inverse_360()
{
	while(edge){;}
	PORTC=0x22;
	return 360;
}

int Inverse_90()
{
	while(edge){;}
	PORTC=0x28;
	return 90;
}

int Inverse_180()
{
	while(edge){;}
	PORTC=0x88;
	return 180;
}

int Inverse_270()
{
	while(edge){;}
	PORTC=0x82;
	return 270;
}
