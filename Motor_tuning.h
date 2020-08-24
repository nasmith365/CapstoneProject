/*
 *	Motor_tuning.h
 *	Created: 3/17/2013 12:54:42 PM
 *  Author: Nicholas Smith, Sean Robb
 *	About:
 */ 

#ifndef MOTOR_TUNING_H_
#define MOTOR_TUNING_H_

#include "CombinedSensors.h"
#include "Queue.h"

void Motor_Tuning();
//Functions for Centering
void Speed_AC_Slow_BD();
void Speed_BD_Slow_AC();
void Speed_All_Up();
//functions for twisting
void Speed_Up_A();
void Speed_Up_C();

extern Queue SensorDistances;//Has all the distances of each sensor
extern Queue CurrentDirection;

#endif /* MOTOR_TUNING_H_ */