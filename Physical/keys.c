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


#define TEST ;

/* 
********************************************************************************************************* 
*                                          GLOBAL VARIABLES 
********************************************************************************************************* 
*/ 

uint32 dummy;
uint8 state=0x00;

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
	#ifdef TEST
		PORTSetPinsDigitalOut(IOPORT_E, BIT_0 | BIT_1|BIT_2 );
		PORTSetBits(IOPORT_E, BIT_0 | BIT_1|BIT_2);
	#endif
	
	PORTClearBits(IOPORT_B, BIT_3| BIT_4 |BIT_5);
	PORTClearBits(IOPORT_C, BIT_13);
	

	PORTSetPinsDigitalIn(IOPORT_B, BIT_0 | BIT_1| BIT_2);
	PORTSetPinsDigitalOut(IOPORT_B, BIT_3| BIT_4 |BIT_5);
	PORTSetPinsDigitalOut(IOPORT_C, BIT_13);	
	
	// Enable change notice, enable discrete pins and weak pullups
    mCNOpen(CONFIG, PINS, PULLUPS);
    // Clear change notice interrupt flag
    ConfigIntCN(INTERRUPT);
    // Ok now to enable multi-vector interrupts
    INTEnableSystemMultiVectoredInt();

}



/******************************************************************************
*	Change Notice Interrupt Service Routine
*
*   Note: Switch debouncing is not performed.
*   Code comes here if SW2 (CN16) PORTD.RD7 is pressed or released.
*   The user must read the IOPORT to clear the IO pin change notice mismatch
*	condition first, then clear the change notice interrupt flag.
******************************************************************************/ 
void __ISR(_CHANGE_NOTICE_VECTOR, ipl2) ChangeNotice_Handler(void)
{
	
	INTEnable(INT_CN,INT_DISABLED);
	
    // Step #1 - always clear the mismatch condition first
    dummy = PORTReadBits(IOPORT_B,  BIT_0 | BIT_1 | BIT_2);
             
    // Step #2 - then clear the interrupt flag
    mCNClearIntFlag();
    
   
     // Step #3 - process the switches
    
	/*mPORTBOpenDrainClose(BIT_3);       // Disable open drain (High impedance) 
	mPORTBOpenDrainClose(BIT_4);       // Disable open drain (High impedance)     
    mPORTBOpenDrainClose(BIT_5);       // Disable open drain (High impedance)
    mPORTCOpenDrainClose(BIT_13);      // Disable open drain (High impedance) 
     */
    
         
    unsigned int sw1 = dummy&BIT_0;
    unsigned int sw2 = dummy&BIT_1;
    unsigned int sw3 = dummy&BIT_2;
    unsigned int but1 = 0;
    unsigned int but2 = 0;
    unsigned int but3 = 0;
    unsigned int i=0;
    
    for(i=0;i<30;i++){
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
    
    PORTSetBits(IOPORT_B, BIT_3| BIT_4 |BIT_5);
	PORTSetBits(IOPORT_C, BIT_13);

    	if(but1>5)
	    	{
		    	#ifdef TEST
				PORTClearBits(IOPORT_E, BIT_0);
				PORTSetBits(IOPORT_E, BIT_1|BIT_2);
				#endif	
				//TRISBbits.TRISB3=0;                         // Direction: output 
				PORTBbits.RB3=0;  
				dummy = PORTReadBits(IOPORT_B,  BIT_0);                           // state: off 
				if(dummy&BIT_0==0)
				{
					PORTEbits.RE0=0 ;
					PORTEbits.RE1=1;
					PORTEbits.RE2=1;
				}
				else
				{ 	
					PORTBbits.RB3=1;
					//TRISBbits.TRISB3=1;
					//mPORTBOpenDrainClose(BIT_3);       // Disable open drain (High impedance) 
					//mPORTBOpenDrainOpen(BIT_4);     // Enable Open drain	
					//TRISBbits.TRISB4=0;                         // Direction: output 
					PORTBbits.RB4=0;                             // state: off 
					dummy = PORTReadBits(IOPORT_B,  BIT_0); 
					if(dummy&BIT_0==0)
					{
						PORTEbits.RE0=1 ;
						PORTEbits.RE1=0;
						PORTEbits.RE2=1;
					}
					else 
					{
						PORTBbits.RB4=1;
						//TRISBbits.TRISB4=1;
						//mPORTBOpenDrainClose(BIT_4);       // Disable open drain (High impedance)
						//mPORTBOpenDrainOpen(BIT_5);     // Enable Open drain 
						//TRISBbits.TRISB5=0;                         // Direction: output 
						PORTBbits.RB5=0;                             // state: off 
						dummy = PORTReadBits(IOPORT_B,  BIT_0); 
						if(dummy&BIT_0==0)
						{
							PORTEbits.RE0=0 ;
							PORTEbits.RE1=0;
							PORTEbits.RE2=1; 
						}	
						else 
						{
							PORTBbits.RB5=1;
							//TRISBbits.TRISB5=1;
							//mPORTBOpenDrainClose(BIT_5);       // Disable open drain (High impedance)
							//mPORTCOpenDrainOpen(BIT_13);     // Enable Open drain
							//TRISCbits.TRISC13=0;                         // Direction: output 
							PORTCbits.RC13=0;                             // state: off 
							dummy = PORTReadBits(IOPORT_B,  BIT_0); 
							if(dummy&BIT_0==0)
							{
								PORTEbits.RE0=1 ;
								PORTEbits.RE1=1;
								PORTEbits.RE2=0; 
							}
							PORTCbits.RC13=1;  
							//TRISCbits.TRISC13=1; 
							//mPORTCOpenDrainClose(BIT_13);       // Disable open drain (High impedance) 
						}
					}	
				}	
					
		   	 }
		   	 
		/* else if(but2>5){
			    #ifdef TEST
				PORTClearBits(IOPORT_E, BIT_1);
				PORTSetBits(IOPORT_E, BIT_0|BIT_2);
		    	#endif
		    	
		    	//button(BIT_1);
		    }
		 else if(but3>5){
			    #ifdef TEST
				PORTClearBits(IOPORT_E, BIT_2);
				PORTSetBits(IOPORT_E, BIT_1|BIT_0);
		    	#endif
		    	
		    	//button(BIT_2);
		    }*/

  
   	   
  	INTEnable(INT_CN,INT_ENABLED);
    // additional processing here...

 }

void button(uint32 BIT_X){
	
	
	LATBbits.LATB3=0;  
	dummy = PORTReadBits(IOPORT_B,  BIT_X);                           // state: off 
	if(dummy&BIT_X==0)
	{
		PORTClearBits(IOPORT_E, BIT_1);
		PORTSetBits(IOPORT_E, BIT_0|BIT_2);
	}
	else
	{ 	
		PORTClearBits(IOPORT_E, BIT_1|BIT_2);
		mPORTBOpenDrainClose(BIT_3);       // Disable open drain (High impedance) 	
		LATBbits.LATB4=0;                             // state: off 
		if(dummy&BIT_X==0)
		{
			PORTClearBits(IOPORT_E, BIT_2);
			PORTSetBits(IOPORT_E, BIT_1|BIT_0);
		}
		else 
		{
			mPORTBOpenDrainClose(BIT_4);       // Disable open drain (High impedance) 
			LATBbits.LATB5=0;                             // state: off 
			if(dummy&BIT_X==0)
			{

				 
			}	
			else 
			{
				mPORTBOpenDrainClose(BIT_5);       // Disable open drain (High impedance)
				LATCbits.LATC13=0;                             // state: off 
				if(dummy&BIT_X==0)
				{
					
				}
				mPORTCOpenDrainClose(BIT_13);       // Disable open drain (High impedance) 
			}
		}
		
	}	
		
}	
