/*
 *	Maze_Robot_20.c
 *	Created: 1/16/2013 8:39:01 PM
 *  Author: Nicholas Smith, Sean Robb
 */ 

//360 sensor is hooked up to PA0
//90 sensor is hooked up to PA1
//180 sensor is hooked up to PA2
//270 sensor is hooked up to PA3
//45 sensor is hooked up to PA4
//135 sensor is hooked up to PA5
//225 sensor is hooked up to PA6
//315 sensor is hooked up to PA7

#include "Maze_Robot_20.h"
#include "Soft_Timer.h"
#include "CombinedSensors.h"
#include "Motor_Driver.h"
#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TIMER0_COMP_vect)
{
	//equals 1/4 a millisecond
	Q_enqueue((void*) 1, &ticks);
}

int main(void)
{
	InitializeAll();
	
	//Start(&Brain,100000);//run the brain function first to get the first scheduling done--has to start in 5ms for the sensors
	Start(&Brain,20);
	Q_enqueue((void*) 360,&Motor);//enqueue the angle we want the motor to run
    while(1)
    {
		MaintainAll();
    }
}

void InitializeAll()
{
	DDRC = 0xFF;//for motors
	DDRD = 0xB0;//Output for the three pwm pins (PD4, PD5, PD7)	
	//for PWM********************************************************************
	//for OCRn 250 is max!!!!****************
	//for 8 bit timer -
	//Motors B and D which correspond to sensors 5 and 7
	TCCR2 = 0x62;//Phase Correct, Divide by 8, Clear on upcounting set on downcounting
	OCR2 = 191;//initialize high so their would be no breaking
	//for 16 bit timer -
	// OCR1A - Motor A which corresponds to sensor 4
	// OCR1B - Motor C which corresponds to sensor 6
	TCCR1A = 0xA1;//8 bit phase correct, Clear on upcounting set on downcounting
	TCCR1B = 0x02;//Divide by 8
	OCR1A = 191;//initialize high so their would be no breaking
	OCR1B = 191;//initialize high so their would be no breaking
	//***************************************************************************
	//for motor
	FSM_register(777, front, &Brain_State_Machine, &Direction_Table);
	//PORTC=0x55;//all motors stoped because all brake lines are high
	PORTC = 0x88;//send 360 to initialize
	//pg82		   |------CTC Mode-------|Normal port operation|------------clk/8------------|
	TCCR0=(1<<FOC0)|(1<<WGM01)|(0<<WGM00)|(0<<COM01)|(0<<COM00)|(0<<CS02)|(1<<CS01)|(0<<CS00);
	OCR0=249;							//system clock is 8mhz
	TIMSK=TIMSK|(1<<OCIE0);				//Set interrupts
	sei();
	
	/*
	*				fclk_I/O			8MHz = .125 us	6MHz = 0.166666 us	4MHz = .25 us	1MHz = 1 us
	*	fOCn=	-----------------
	*			2 (N) (1 + OCRn)
	*/	
   
	FSM_register(132, ninety, &Motor_State_Machine, &State_Location_Table);//register the state machine for motors
	//for sensors
	ADMUX = 0xE0;//Internal 2.56V, left adjust,  has result
	ADCSRA = 0xC6; //on, start conversion, auto triggering off, divide by 64
	//have to register a state machine for each sensor
	//short sensors
	FSM_register(0, NoWall, &Short_Sensor_State_Machine, &Short_Sensor_Table);
	FSM_register(1, NoWall, &Short_Sensor_State_Machine, &Short_Sensor_Table);
	FSM_register(2, NoWall, &Short_Sensor_State_Machine, &Short_Sensor_Table);
	FSM_register(3, NoWall, &Short_Sensor_State_Machine, &Short_Sensor_Table);
	//long sensors
	FSM_register(4, Center, &Long_Sensor_State_Machine, &Long_Sensor_Table);
	FSM_register(5, Center, &Long_Sensor_State_Machine, &Long_Sensor_Table);
	FSM_register(6, Center, &Long_Sensor_State_Machine, &Long_Sensor_Table);
	FSM_register(7, Center, &Long_Sensor_State_Machine, &Long_Sensor_Table);
}