/*
 *	State_Machine.c
 *	Created: 1/13/2013 1:57:42 PM
 *  Author: Dr. Carroll,
 *			Modified by: Nicholas Smith, Sean Robb
 *	About:	
 */ 

  #include "State_Machine.h"
  #include "Binary_Search.h"


static struct {int FSM_ID; int state; FSM_model_t *model;} all_machines[10];
static int all_machines_counter = 0;

static struct {int FSM_ID; ListOfStates *reg;}TableOfContents[10]; 
int Amount_of_States = 4;

/*********************************************************************/
/* Register main and all INSTANCES of submachines. This process 
allocates a unique state variable and binds it to the given model.
You are asked to assign each instance an id integer.
*/

bool FSM_register(int FSM_ID, int initial_state, FSM_model_t *model, ListOfStates *reg)
{
	int LocationIndex = 0; //Where we are in the location table (Little one the search is going to use)
	int TransitionIndex = 0;//Where we are in the transition table that we made		
//*******************************************************************************************
//Doc's Code
	if (all_machines_counter > 15)
		return false;

	all_machines[all_machines_counter].FSM_ID = FSM_ID;
	all_machines[all_machines_counter].state = initial_state;	
	all_machines[all_machines_counter].model = model;
	
//*******************************************************************************************
//Our Code	
	TableOfContents[all_machines_counter].FSM_ID = FSM_ID;//gives the corresponding id to table of contents from state machine
	TableOfContents[all_machines_counter].reg = reg;//links the id to the ListOfStates array
	//Putting The first state into the Location table and where it starts
	reg[LocationIndex].StateOptions = model->transition[TransitionIndex].current;
	reg[LocationIndex].handle = TransitionIndex;
	do//the do - while loop goes through the complete transition table and fills out the location table
	{
		if (reg[LocationIndex].StateOptions != model->transition[TransitionIndex].current)
		{
			++LocationIndex;//incrementing where we are in the location table
			//Putting the first of each new state into the Location table and where it starts
			reg[LocationIndex].StateOptions = model->transition[TransitionIndex].current;
			reg[LocationIndex].handle = TransitionIndex;
		}
		++TransitionIndex;//increments where we are in the transition table to go through all the states
	}
	while (TransitionIndex < model->size);
//*******************************************************************************************	
	all_machines_counter++;
	return true;
	/*successful registration*/
}

/*********************************************************************/
/* 	Given an id number (FSM_ID), look up the registered model.
	Model includes current state and ptr-to-State-Transition-Table.
	Look up current state & input to determine next state and output.
	Change state value & execute output action using the FSM_run
	second parameter as an arg to the action. This allows you to pass
	in a message (typically provided from an intertask-queue) to the 
	FSM output action. 
*/

int FSM_run(int FSM_ID, int input)
{
	int retVal = 0;
	ListOfStates *LoS;//**********************************
	/* Start with FALSE flag */
	FSM_model_t *fsm = 0;
	int *statePtr;
	//finds which state machine to use -- finds the right id
	for (int i = 0; i<all_machines_counter; i++)
	{
		if (FSM_ID == all_machines[i].FSM_ID) 
		{
			fsm = all_machines[i].model;
			LoS = TableOfContents[i].reg;//**********************************
			statePtr = &(all_machines[i].state);
			break;
		}
	}
	
	if (fsm == 0) 
		return false;
	/* ID not found */
	//*******************************************************************************************
	//Our Code
	int start = BinarySearch(LoS, (fsm->size/fsm->NumberPerGroup),*statePtr);
	if (start == -1)//it is not in the array
	{
		return false;
	}
	
	int rows =start + fsm->NumberPerGroup;
//*******************************************************************************************	
	for (; start<rows; ++start)
	{
		FSM_transition_t *arc = (fsm->transition+start);
		if ((message_match(input, arc-> trigger)))//**********************************
		{
			retVal = (arc->output)(input);
			/* Output actions usually return a nonzero int, but if the 
			action is a state sub-machine, AND that submachine can't 
			match the given input, it should set its state to INITIAL
			or leave it as DEEP HISTORY, and return 0. */
			if (retVal != 0) 
			{
				*statePtr = arc->next;
				return retVal;
			}
		/* else, keep looking! */
		}
	}
return retVal;
}	
/*********************************************************************/	
bool message_match(int a, int b)
{	
	if (a == b)
 		return true;
	else if ((a == MSG_ANY) || (b== MSG_ANY)) 
		return true;
	else 
		return false;
}
/*********************************************************************/
int submachineMiss(int input)
{
	return 0;
}
/*********************************************************************/