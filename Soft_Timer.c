/*
 *	Soft_Timer.c
 *	Created: 1/11/2013 2:38:13 PM
 *  Author: Nicholas Smith, Sean Robb
 *	About:	Unsorted Registry Software timer
 */ 

#include "Soft_Timer.h"

int tablesize = 0;//tells how many threads are in the table (registry)
int tablefull = 15;//is the max amount of threads the registry can hold
Queue ticks;

void Start(void* fnctorun,int time_delay)
{
	if (tablesize == tablefull)//if there is no room in the registry table
	{
		return;
	}
	//*****************************************************************************************************
	//why did we do this?? why is there a special case because if it is uncommented then we are skipping one
	//*****************************************************************************************************
	//if (tablesize == 0)//if it is the first element in the registry
	//{
		//putting the new timer into the registry table
		++tablesize;//increase the tablesize because there is an added function
		registry_table[tablesize].action = fnctorun;//adding what function that needs to be ran into the registry table
		registry_table[tablesize].countdown = time_delay;//adding what delay until the function needs to be ran	
	//}
	//else//if it is not the first element in the registry
	//{
		////putting the new timer into the registry table
		//++tablesize;//increase the tablesize because there is an added function
		//registry_table[tablesize].action = fnctorun;//adding what function that needs to be ran into the registry table
		//registry_table[tablesize].countdown = time_delay;//adding what delay until the function needs to be ran
	//}
}

void MaintainAll()
{
	if (tablesize == 0)//if there is nothing in the registry then quit
	{
		return;
	}
	while (!(Q_isEmpty(&ticks)))//while the queue has ticks in it it should keep subtracting
	{
		Q_dequeue(&ticks);//makes one less tick in the queue
		for (int i = 1; i<=tablesize; ++i)//go through the registry and subtract 1 from all the countdowns
		{
			--registry_table[i].countdown;	
		}
		for (int k = tablesize; k>0; --k)//go through the registry looking for 0 and run those functions and adjust registry accordingly
		{//***special case*** need to make sure that the one we shift up is checked also to see if it is zero
				//to stop this process we are going to start from the bottom and go up.
			if (registry_table[k].countdown == 0)
			{
				func_ptr=registry_table[k].action;//selecting the function to run
				func_ptr();//run the function	
				if (k!=tablesize)//shift if k is not equal to table size
				{
					registry_table[k] = registry_table[tablesize];//shifts the bottom one up--should get rid of special case also
				}			
				--tablesize;//subtract from table size
			}//end of if
		}//end of for
	}//end of while
}//end of MaintainAll
