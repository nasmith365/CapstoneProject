/*
 *	Motor_tuning.c
 *	Created: 3/17/2013 12:53:03 PM
 *  Author: Nicholas Smith, Sean Robb
 *	About:
 */ 

/*
Motor Labels
	 _____
	/D   A\
   |	   |
    \C___B/
Sensor Labels
	 _______
    /7  0  4\
   |3       1|
    \6__2__5/
*/

//for OCRn 250 is max!!!!****************
#include "Motor_tuning.h"
#include <avr/io.h>

Queue SensorDistances;
Queue CurrentDirection;

void Motor_Tuning()
{
	//dequeues the pointer to the distances from the queue and makes a new array of the sensors distance for use
	int size = 8;
	int DistanceArray[size];
	int direction;
	int *ptr;
	if ((Q_isEmpty(&SensorDistances)) || (Q_isEmpty(&CurrentDirection)))//Is to check to see if the queue is empty and if it is then just exit
	{
		return;
	}		
	ptr = (int) Q_dequeue(&SensorDistances);
	direction = (int) Q_dequeue(&CurrentDirection);
	for (int i = 0; i<size; ++i)//takes our array that we enqueued and makes it our sensor data
	{
		DistanceArray[i] = *(ptr+i);
	}
	//************************************************************************************************
	//**************************************Centering Start*******************************************
	switch (direction)
	{
	case 360:
		if (DistanceArray[3] > DistanceArray[1])
		{
			Speed_AC_Slow_BD();
		}
		else if (DistanceArray[3] < DistanceArray[1])
		{
			Speed_BD_Slow_AC();
		}
		break;
	case 90:
		if (DistanceArray[0] > DistanceArray[2])
		{
			Speed_BD_Slow_AC();
		}
		else if (DistanceArray[0] < DistanceArray[2])
		{
			Speed_AC_Slow_BD();
		}
		break;
	case 180:
		if (DistanceArray[3] > DistanceArray[1])
		{
			Speed_BD_Slow_AC();
		}
		else if (DistanceArray[3] < DistanceArray[1])
		{
			Speed_AC_Slow_BD();
		}
		break;
	case 270:
		if (DistanceArray[0] > DistanceArray[2])
		{
			Speed_AC_Slow_BD();
		}
		else if (DistanceArray[0] < DistanceArray[2])
		{
			Speed_BD_Slow_AC();
		}
		break;
	default:
		break;
	}
	//***************************************Centering End********************************************
	//************************************************************************************************
	//**************************************Twisting Start********************************************
	switch (direction)
	{
		case 360:
			if (DistanceArray[4] > DistanceArray[5])
			{
				Speed_Up_C();
			}
			else if (DistanceArray[4] < DistanceArray[5])
			{
				Speed_Up_A();
			}
			break;
		case 90:
			if (DistanceArray[4] > DistanceArray[7])
			{
				Speed_Up_C();
			}
			else if (DistanceArray[4] < DistanceArray[7])
			{
				Speed_Up_A();
			}
			break;
		case 180:
			if (DistanceArray[6] > DistanceArray[7])
			{
				Speed_Up_A();
			}
			else if (DistanceArray[6] < DistanceArray[7])
			{
				Speed_Up_C();
			}
			break;
		case 270:
			if (DistanceArray[5] > DistanceArray[6])
			{
				Speed_Up_C();
			}
			else if (DistanceArray[5] < DistanceArray[6])
			{
				Speed_Up_A();
			}
			break;
		default:
			break;
	}	
}

//Speed up motor pair A & C
//			or
//Slow down motor pair B & D
void Speed_AC_Slow_BD()
{
	if ((OCR1A == 255) || (OCR1B == 255))
	{
		--OCR2;
	}
	else
	{
		++OCR1A;
		++OCR1B;
	}
}

//Speed up motor pair B & D
//			or
//Slow down motor pair A & C
void Speed_BD_Slow_AC()
{
	if (OCR2 == 255)
	{
		--OCR1A;
		--OCR1B;
	}
	else
	{
		++OCR2;
	}
}

//Speed up, go faster
void Speed_All_Up()
{
	if ((OCR2 == 255) || (OCR1A == 255) || (OCR1B == 255))
	{
		return;
	}
	else
	{
		++OCR1A;
		++OCR1B;
		++OCR2;
	}

}

void Speed_Up_A()
{
	if (OCR1A!=255)
	{
		++OCR1A;
	}
	else
	{
		--OCR1B;
	}
}

void Speed_Up_C()
{
	if (OCR1B!=255)
	{
		++OCR1B;
	}
	else
	{
		--OCR1A;
	}
}