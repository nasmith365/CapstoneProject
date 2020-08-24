/*
 *	State_Machine.h
 *	Created: 1/13/2013 1:57:35 PM
 *  Author: Dr. Carroll, 
 *			Modified by: Nicholas Smith, Sean Robb
 *	About:	
 */ 

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include <stdbool.h>

#define MSG_ANY (-1)

typedef struct {
	int current; 
	int trigger; 
	int next; 
	int (*output)(int trigger);} FSM_transition_t;

typedef struct {
	int size;//size of the model
	int NumberPerGroup;//how many states a state can go to
	FSM_transition_t *transition;} FSM_model_t;

typedef struct {int StateOptions; int handle;}ListOfStates;//holds where each state starts in our transistion table

int FSM_run(int FSM_id_number, int input);
bool FSM_register(int FSM_id_number, int initial_state, FSM_model_t *fsm, ListOfStates *reg);
bool message_match(int msg1, int msgs2);
int submachineMiss(int ignored);

#endif /* STATE_MACHINE_H_ */