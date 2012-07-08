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
 * Filename      : keys.c
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
*                                            INCLUDE FILES 
********************************************************************************************************* 
*/ 

#include <p32xxxx.h> 
#include <plib.h>                   // include peripheral library functions
#include "global.h"
#include "keys.h"

/* 
********************************************************************************************************* 
*                                               DEFINES
********************************************************************************************************* 
*/ 


#define TEST 
#define COL1	BIT_3
#define COL2	BIT_4
#define	COL3	BIT_5
#define COL4 	BIT_13

#define ROW1	BIT_0
#define ROW2	BIT_1
#define	ROW3	BIT_2

#define ALL_SET PORTSetBits(IOPORT_B, COL1| COL2 |COL3);\
				PORTSetBits(IOPORT_C, COL4);			\
				mPORTBOpenDrainOpen(COL1| COL2 |COL3);	\
				mPORTCOpenDrainOpen(COL4);			  	

#define ALL_CLEAR 	mPORTBOpenDrainClose(COL1| COL2 |COL3);				\
					mPORTCOpenDrainClose(COL4);			   				\
					PORTSetPinsDigitalOut(IOPORT_B, COL1| COL2 |COL3);	\
					PORTSetPinsDigitalOut(IOPORT_C, COL4);				\
					PORTClearBits(IOPORT_B, COL1| COL2 |COL3);			\
					PORTClearBits(IOPORT_C, COL4);


/* 
********************************************************************************************************* 
*                                          GLOBAL VARIABLES 
********************************************************************************************************* 
*/ 
unsigned int type = 0;
unsigned int done = 1;

/* 
********************************************************************************************************* 
*                                        FUNCTION PROTOTYPES 
********************************************************************************************************* 
*/ 
void button(uint32 BIT_X);

/* 
********************************************************************************************************* 
*                                        CONFIGURATION BITS 
********************************************************************************************************* 
*/ 


	
void keypad_init(){
	
	PORTClearBits(IOPORT_B, COL1| COL2 |COL3);\
	PORTClearBits(IOPORT_C, COL4);
	
	PORTSetPinsDigitalOut(IOPORT_E, BIT_0 | BIT_1| BIT_2);
	PORTSetBits(IOPORT_E, BIT_0 | BIT_1| BIT_2);
	
	PORTSetPinsDigitalIn(IOPORT_B, BIT_0 | BIT_1| BIT_2);
	PORTSetPinsDigitalOut(IOPORT_B, BIT_3| BIT_4 |BIT_5);
	PORTSetPinsDigitalOut(IOPORT_C, BIT_13);	
	
	// Enable change notice, enable discrete pins and weak pullups
    mCNOpen(CONFIG, PINS, PULLUPS);
    // Clear change notice interrupt flag
    //ConfigIntCN(INTERRUPT);
    // Ok now to enable multi-vector interrupts
    //INTEnableSystemMultiVectoredInt();

}



/******************************************************************************
*	Change Notice Interrupt Service Routine
*
*   Note: Switch debouncing is not performed.
*   Code comes here if SW2 (CN16) PORTD.RD7 is pressed or released.
*   The user must read the IOPORT to clear the IO pin change notice mismatch
*	condition first, then clear the change notice interrupt flag.
******************************************************************************/ 
/*void __ISR(_CHANGE_NOTICE_VECTOR, ipl2) ChangeNotice_Handler(void)
{
	done =0;
	INTEnable(INT_CN,0);
	INTDisableInterrupts();
	if(!INTGetEnable(INT_CN))
	{
  		hal_sendString_UART1("INT_DIS");	
   	}
   	
   	
    // Step #1 - always clear the mismatch condition first
    unsigned int dummy = PORTReadBits(IOPORT_B,  BIT_0 | BIT_1 | BIT_2);
    
    // Step #2 - then clear the interrupt flag
    INTClearFlag(INT_CN);
    
    hal_sendString_UART1("interrupt");
   
     // Step #3 - process the switches
     
    unsigned int sw1;
    unsigned int sw2;
    unsigned int sw3; 
    
    unsigned int chk = 0;
    unsigned int but1 = 0;
    unsigned int but2 = 0;
    unsigned int but3 = 0;
    unsigned int i=0;
    
    for(i=0;i<80000;i++){
	    dummy = PORTReadBits(IOPORT_B,  BIT_0 | BIT_1 | BIT_2);
	    sw1 = dummy&BIT_0;
    	sw2 = dummy&BIT_1;
    	sw3 = dummy&BIT_2;
	    
	    if(sw1==0){
	    	but1++;
	    }
	    else if(sw2==0){
	    	but2++;
	    }
	    else if(sw3==0){
	    	but3++;
	    }
	    else{
		    
	    }
	    
    }
        
    hal_sendChar_UART1('\n');
    hal_uartWriteNumber(but1);
    hal_sendChar_UART1('\n');
    hal_uartWriteNumber(but2);
    hal_sendChar_UART1('\n');
    hal_uartWriteNumber(but3);
    hal_sendChar_UART1('\n');
    
   type = 0;
   unsigned int max = 70000;
   if(but1>max){
	   	type = 1;
   		max = but1;
   }
   else if(but2>max){
   		type = 2;
   		max = but2;
   }
   else if(but3>max){
   		type = 3;
   		max = but3;
   }
    
   	if(type==1)
   	{
	    	#ifdef TEST
				hal_sendString_UART1("row1");
			#endif	
			
			

	 }
	 else if(type==2){
		    #ifdef TEST
				hal_sendString_UART1("row2");
	    	#endif
			
			
	    	

	 }
	 else if(type==3){
		    #ifdef TEST
				hal_sendString_UART1("row3");
	    	#endif
			
	}
	else{
		hal_sendString_UART1("error\n");
	}
	
	

	
    // additional processing here...

	
 }
 
 
 
 void key_pole()
 {
	if(!done){
		if(type==1){
			ALL_SET
			PORTClearBits(IOPORT_B,COL1);
			unsigned int dummy = PORTReadBits(IOPORT_B,  ROW1);
			unsigned int chk = dummy&ROW1;
			
			if(chk==0){
				hal_sendString_UART1("but1");
			}		
			
			ALL_SET
			PORTClearBits(IOPORT_B,COL2);
			dummy = PORTReadBits(IOPORT_B,  ROW1);
			chk = dummy&ROW1;
			
			if(chk==0){
				hal_sendString_UART1("but2");
			}	
			
			ALL_SET
			PORTClearBits(IOPORT_B,COL3);
			dummy = PORTReadBits(IOPORT_B,  ROW1);
			chk = dummy&ROW1;
			
			if(chk==0){
				hal_sendString_UART1("but3");
			}	
			
			ALL_SET
			PORTClearBits(IOPORT_C,COL4);
			dummy = PORTReadBits(IOPORT_B,  ROW1);
			chk = dummy&ROW1;
			
			if(chk==0){
				hal_sendString_UART1("but4");
			}
		}	
	}
	type=0;
	done=1;
	
	PORTClearBits(IOPORT_B, BIT_3| BIT_4 |BIT_5);
	PORTClearBits(IOPORT_C, BIT_13);
	
	INTEnable(INT_CN,1);
  	INTEnableInterrupts();
	
}*/


void keypad_pole()
{
	
	unsigned int dummy = PORTReadBits(IOPORT_B,  BIT_0 | BIT_1 | BIT_2);

	 // process the switches
     
    unsigned int sw1;
    unsigned int sw2;
    unsigned int sw3; 
    
    unsigned int chk = 0;
    unsigned int but1 = 0;
    unsigned int but2 = 0;
    unsigned int but3 = 0;
    unsigned int i=0;
    
    for(i=0;i<8000;i++){
	    dummy = PORTReadBits(IOPORT_B,  BIT_0 | BIT_1 | BIT_2);
	    sw1 = dummy&BIT_0;
    	sw2 = dummy&BIT_1;
    	sw3 = dummy&BIT_2;
	    
	    if(sw1==0){
	    	but1++;
	    }
	    else if(sw2==0){
	    	but2++;
	    }
	    else if(sw3==0){
	    	but3++;
	    }
	    else{
		    
	    }
    }
	 
	type = 0;
    unsigned int max = 7000;
    if(but1>max){
	   	type = 1;
   		max = but1;
    }
    else if(but2>max){
   		type = 2;
   		max = but2;
    }
    else if(but3>max){
   		type = 3;
   		max = but3;
    }
   
   	if(type==1&&done)
   	{
	   	done=0;
   		
    	#ifdef TEST
			hal_sendString_UART1("row1");
		#endif	
		
		select_key(ROW1);	
		
	 }
	 else if(type==2&&done){
		done=0;
	    #ifdef TEST
			hal_sendString_UART1("row2");
    	#endif	
    		
		select_key(ROW2);
		
	}
	 else if(type==3&&done){
		done=0;
	    #ifdef TEST
			hal_sendString_UART1("row3");
    	#endif		
    	
		select_key(ROW3);
		
	}
	else if(type==0){
		done=1;
	}	
	ALL_CLEAR 
}


