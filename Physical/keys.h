/*
********************************************************************************************************* 
 *					 	keypad
 *
 * (c) Copyright 2012 D2NM, All rights reserved
********************************************************************************************************* 
*/


/*
********************************************************************************************************* 
 * 						keypad
 *
 * Filename      : keys.h
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

#ifndef KEYS
#define KEYS
 
 
 
/* 
********************************************************************************************************* 
*                                            INCLUDE FILES 
********************************************************************************************************* 
*/ 


#include <p32xxxx.h> 			// include chip specific header file
#include <plib.h>                   // include peripheral library function
#include "global.h"


/* 
********************************************************************************************************* 
*                                               DEFINES
********************************************************************************************************* 
*/ 


//setting for change notice interrupts 
#define CONFIG          (CN_OFF)
#define PINS            (CN2_ENABLE | CN3_ENABLE | CN4_ENABLE)
#define PULLUPS         (CN2_PULLUP_ENABLE | CN3_PULLUP_ENABLE | CN4_PULLUP_ENABLE)
#define INTERRUPT       (CHANGE_INT_ON | CHANGE_INT_PRI_2)

/* 
********************************************************************************************************* 
*                                               DATA TYPES 
********************************************************************************************************* 
*/ 




/* 
********************************************************************************************************* 
*                                               EXTERNS 
********************************************************************************************************* 
*/ 


/* 
********************************************************************************************************* 
*                                          GLOBAL VARIABLES 
********************************************************************************************************* 
*/ 

uint32 key;



/* 
********************************************************************************************************* 
*                                              MACROS 
********************************************************************************************************* 
*/ 

    
#define select_key(ROWX)	ALL_SET												\
							PORTClearBits(IOPORT_B,COL1);						\
							unsigned int dummy = PORTReadBits(IOPORT_B, ROWX);	\
							unsigned int chk = dummy&ROWX;						\
							if(chk==0){											\
								enque(ENTER_NO);								\
								hal_sendString_UART1("but1");					\
								switch(ROWX){									\
									case ROW1:key=0;break;						\
									case ROW2:key=4;break;						\
									case ROW3:key=8;break;						\
								}												\
							}													\
							ALL_SET												\
							PORTClearBits(IOPORT_B,COL2);						\
							dummy = PORTReadBits(IOPORT_B,  ROWX);				\
							chk = dummy&ROWX;									\
							if(chk==0){											\
								enque(ENTER_NO);								\
								hal_sendString_UART1("but2");					\
								switch(ROWX){									\
									case ROW1:key=1;break;						\
									case ROW2:key=5;break;						\
									case ROW3:key=9;break;						\
								}												\
							}													\
							ALL_SET												\
							PORTClearBits(IOPORT_B,COL3);						\
							dummy = PORTReadBits(IOPORT_B,  ROWX);				\
							chk = dummy&ROWX;									\
							if(chk==0){											\
								enque(ENTER_NO);								\
								hal_sendString_UART1("but3");					\
								switch(ROWX){									\
									case ROW1:key=2;break;						\
									case ROW2:key=6;break;						\
									case ROW3:key=10;break;						\
								}												\
							}													\
							ALL_SET												\
							PORTClearBits(IOPORT_C,COL4);						\
							dummy = PORTReadBits(IOPORT_B,  ROWX);				\
							chk = dummy&ROWX;									\
							if(chk==0){											\
								enque(ENTER_NO);								\
								hal_sendString_UART1("but4");					\
								switch(ROWX){									\
									case ROW1:key=3;break;						\
									case ROW2:key=7;break;						\
									case ROW3:key=11;break;						\
								}												\
							}


#define check_key() switch(key){			\
				case 0:count++;break;		\
				case 1:count++;break;		\
				case 2:count++;break;		\
				case 3:count++;break;		\
				case 4:count++;break;		\
				case 5:count++;break;		\
				case 6:count++;break;		\
				case 7:count++;break;		\
				case 8:count++;break;		\
				case 9:count++;break;		\
				case 10:enque(OK);break;	\
				case 11:enque(CANCEL);break;\
			}





/* 
********************************************************************************************************* 
*                                        FUNCTION PROTOTYPES 
********************************************************************************************************* 
*/ 

void keypad_init();
void keypad_pole();

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
