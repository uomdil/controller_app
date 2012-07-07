/*
********************************************************************************************************* 
 *					 	Global header file 
 *
 * (c) Copyright 2012 D2NM, All rights reserved
********************************************************************************************************* 
*/


/*
********************************************************************************************************* 
 * 						Vending machine controller 
 *
 * Filename      : global.h
 * Version       : V1.0
 * Programmer(s) : DIL
 *
 * Note(s)		 :
 *
 *
********************************************************************************************************* 
*/


/* 
********************************************************************************************************* 
*                                            MODULE
********************************************************************************************************* 
*/ 

#ifndef GLOBAL
#define GLOBAL
   
/* 
********************************************************************************************************* 
*                                            INCLUDE FILES 
********************************************************************************************************* 
*/ 


//#include <p32xxxx.h>		// include chip specific header file
#include <plib.h>           // include peripheral library functions


/* 
********************************************************************************************************* 
*                                               DEFINES
********************************************************************************************************* 
*/ 

//clocks
#define SYS_FREQ 					(80000000)
#define	GetSystemClock() 			(80000000ul)
#define	GetPeripheralClock()		(GetSystemClock()/(1 << OSCCONbits.PBDIV))
#define	GetInstructionClock()		(GetSystemClock())


//define state IDs of the controller

#define START				0
#define DIAGNOSTIC			1
#define INIT				2
#define UPDATE_VARS			3
#define WAIT_MONEY			4
#define WAIT_PRODUCT		5
#define WAIT_AMOUNT			6
#define BALANCE				7
#define DISPENSE			8
#define RETURN_MONEY		9
#define SYSTEM_LOCK			10
#define NFC_PAY				11
#define GSM_CONTROL			12



//define event IDs for controller

#define DIAGNOSE			0
#define INIT_VARS			1
#define UPDATE_DATA			2
#define FIN					3
#define MORE_DATA			4
#define CASH_IN				5
#define COIN_IN				6
#define PRODUCT_NO			7
#define OK					8
#define CANCEL				9
#define ENTER_NO			10
#define TIME_OUT			11
#define WRONG				12
#define ERROR				13
#define SEND_ERROR			14
#define NFC_IN				15
#define NFC_GET_CONFIRM		16
#define NFC_SET_CONFIRM		17

#define NO_OF_EVENTS	4

#define SHOW_MESSAGES ;



//define true and false

#ifndef false
	#define false	0
#endif
#ifndef true
	#define true 	1
#endif
#ifndef FALSE
	#define FALSE	0
#endif
#ifndef TRUE
	#define TRUE	1
#endif
#ifndef False
	#define False	0
#endif
#ifndef True
	#define True 	1
#endif



/* 
********************************************************************************************************* 
*                                               DATA TYPES 
********************************************************************************************************* 
*/ 


typedef unsigned char uint8;
typedef signed char int8;
typedef unsigned short int uint16;
typedef signed short int int16;
typedef unsigned int uint32;
typedef signed int int32;
typedef unsigned char bool;


/* 
********************************************************************************************************* 
*                                               EXTERNS 
********************************************************************************************************* 
*/

extern uint32 key;

/* 
********************************************************************************************************* 
*                                          GLOBAL VARIABLES 
********************************************************************************************************* 
*/ 


/* 
********************************************************************************************************* 
*                                              MACROS 
********************************************************************************************************* 
*/ 

/* 
********************************************************************************************************* 
*                                        FUNCTION PROTOTYPES 
********************************************************************************************************* 
*/ 

uint8 enque(uint8 eventId);

/* 
********************************************************************************************************* 
*                                        CONFIGURATION BITS 
********************************************************************************************************* 
*/ 


/* 
********************************************************************************************************* 
*                                           MODULE END 
********************************************************************************************************* 
*/

#endif
