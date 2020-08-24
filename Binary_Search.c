/*
 *	Binary_Search.c
 *	Created: 1/15/2013 1:59:11 PM
 *  Author: Nicholas Smith, Sean Robb
 *	About: This function takes in a pointer to an array of ListOfStates, the size of that array, and what state it is looking for and 
 *		   searches through the array using a binary search and returns the index of the location of what it was looking for
 */

#include "Binary_Search.h"

int BinarySearch(ListOfStates* array, int size, int LookState)
{
	int min=0;//initially the min is zero
	int max=size;//max is set to the size of the array which is sent to this function
	int mid = (max + min) /2;//mid will be the middle of the array
	
while (min < max)//if min is greater than max it means that the value is not in the array
{
	if (array[mid].StateOptions == LookState)//if the mid is what we are looking for
	{
		return array[mid].handle;//return the location of the start of the array
	}
	else if (array[mid].StateOptions > LookState)//if the mid value is greater then what we are looking for
	{
		max = mid;//since we checked the min the max becomes one less than the mid
		min = min;//min will stay the same
		mid = (max + min) /2;//caluculate the new mid
	}
	else //mid<LookState //if the mid value is less then what we are looking for
	{
		min = mid +1;//since we checked the mid the min becomes one more than the mid
		max = max;//max stays the same
		mid = (max + min) /2;//calculate the new mid
	}
}
	return -1;//if it is not in the array then it will return -1 to say that it is not in the array
}
