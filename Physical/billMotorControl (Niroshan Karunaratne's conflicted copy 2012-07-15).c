//motor controlling file
/*
********************************************************************************************************* 
 *					 	Bill validator 
 *
 * (c) Copyright 2012 UOM, All rights reserved
********************************************************************************************************* 
*/


/*
********************************************************************************************************* 
 * 						Vending machine controller 
 *
 * Filename      : billMotorControl.c
 * Version       : V1.0
 * Programmer(s) : TCY
 *
 * Note(s)		 :
 *	hardware used - timer 1 - encoder counter (theta*count = angle)
 *					timer 2 - to get the time (time)
 *					combination is used to get (angular speed = angle / time)
 *					timer 3 - for PWM generation
 *
 *
********************************************************************************************************* 
*/



 
/* 
********************************************************************************************************* 
*                                            INCLUDE FILES 
********************************************************************************************************* 
*/ 

#include "billMotorControl.h"


/* 
********************************************************************************************************* 
*                                               DEFINES
********************************************************************************************************* 
*/ 

#define PB_DIV                 8
#define PRESCALE               256
#define MOTOR_RIGHT			   PORTEbits.RE5	
#define MOTOR_LEFT			   PORTEbits.RE6

#define ENCODER_COUNT			100   //dummy change this
#define TIME_FREQUENCY			1000
#define MILLISECOND				SYS_FREQ/PB_DIV/PRESCALE/TIME_FREQUENCY

#define DISTANCE_PER_COUNT		100 //dummy change this

/* 
********************************************************************************************************* 
*                                          GLOBAL VARIABLES 
********************************************************************************************************* 
*/ 

unsigned int encoderCount = 0;
unsigned int time = 0;

/* 
********************************************************************************************************* 
*                                        FUNCTION PROTOTYPES 
********************************************************************************************************* 
*/ 


/* 
********************************************************************************************************* 
*                                        CONFIGURATION BITS 
********************************************************************************************************* 
*/ 



//functions	

void intialize(){
	encoderCount = 0;
	time = 0;
}	


void motorsInit(){
	//set motor I/O pin directions
	PORTClearBits(IOPORT_E, BIT_5 | BIT_6);
	PORTSetPinsDigitalOut(IOPORT_E, BIT_5 | BIT_6);
	//initialize PWM - config/primary - OC1R/secondary - OC1RS
	//OpenOC1(OC_ON | OC_TIMER_MODE16 | OC_TIMER3_SRC | OC_PWM_FAULT_PIN_DISABLE, 0,0);
	//OpenTimer3(T3_ON | T3_PS_1_64, PWM_PERIOD);
	//intialize timers
	encoderInit();
	//set initial motor dir and speed
	motorDir(STOP);
	//initialize variables
	intialize();
	
}		

void motorDir(unsigned char dir){
	if(dir==CLOCKWISE){
		MOTOR_RIGHT = 1;
		MOTOR_LEFT = 0;
	}
	else if(dir==ANTICLOCKWISE){
		MOTOR_RIGHT = 0;
		MOTOR_LEFT = 1;
	}
	else if(dir==STOP){
		MOTOR_RIGHT = 1;
		MOTOR_LEFT = 1;
	}			
}

void motorChangeSpeed(unsigned char speed){   //Not Implemented
	//SetDCOC0PWM(0xff);   //OCR1S register
}

double motorGetSpeed(){   //Not Implemented
	//this is the basic idea - you can refine this
	/*double totalTime = time + (double)(ReadTimer1())/(double)(MILLISECOND);
	totalTime /= 1000.0; //time in seconds
	unsigned int totalCount = encoderCount + ReadTimer2();
	double conveyorTravel = totalCount*DISTANCE_PER_COUNT;
	return conveyorTravel/totalTime;*/
	return 0.0;
}





//functions involving getting the speed

void encoderInit(){
	
	//change parameters in test
	//timer1 for the encoder count 1:1
	OpenTimer1(T1_ON | T1_SOURCE_EXT | T1_PS_1_1, ENCODER_COUNT);
	ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_2);
	//timer2 to get the time
	OpenTimer2(T1_ON | T2_SOURCE_EXT | T2_PS_1_256, MILLISECOND);
	ConfigIntTimer1(T2_INT_ON | T2_INT_PRIOR_2);
	
}



void __ISR(_TIMER_1_VECTOR, ipl2) EncoderIntHandler(void)
{
	mT1ClearIntFlag();
	//for counting
	encoderCount += ENCODER_COUNT;
  
}


void __ISR(_TIMER_2_VECTOR, ipl2) timeIntHandler(void)
{
  	mT2ClearIntFlag();
  	time += 1;
}

void __ISR(_TIMER_4_VECTOR, ipl2) pidIntHandler(void){
	
}	


void doPID(){
	
}




//NOTES - 
/*
The timer period is determined by the value in the PR period register. To initialize the timer period,
a user may write to the PR period register directly at any time while the timer is disabled,
ON bit = 0, or during a timer match Interrupt Service Routine (ISR) while the timer is enabled,
ON bit = 1. In all other cases, writing to the period register while the timer is enabled is not
recommended*/

		