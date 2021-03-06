/*
********************************************************************************************************* 
 *					 	Vending machine controller 
 *
 * (c) Copyright 2012 D2NM, All rights reserved
********************************************************************************************************* 
*/


/*
********************************************************************************************************* 
 * 						Vending machine controller 
 *
 * Filename      : main.c
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
//#include "billMotorControl.h"
#include "Flash_Controller.h"
#include "skpic32_glcd.h"


/* 
********************************************************************************************************* 
*                                               DEFINES
********************************************************************************************************* 
*/ 

#define PRODUCT 1	//use keys for product select
#define AMOUNT 2	//use keys to select number of products


/* 
********************************************************************************************************* 
*                                          GLOBAL VARIABLES 
********************************************************************************************************* 
*/ 


//queue management
uint8 state;
uint8 g_EventQueue[NO_OF_EVENTS];
int queueHead;
int queueTail;

uint8 count=0;  					//count pressing keys

uint8 keyuse;						//keypad mode: either entering product no or 

uint32 product_no=0;				//to store entered product no
uint32 amount=0;					//to store entered prosuct amount



/* 
********************************************************************************************************* 
*                                        FUNCTION PROTOTYPES 
********************************************************************************************************* 
*/ 

void board_init();
void stateMachine(uint8 eventId);
void changeState(uint8 nextStateId);
void onStateExit(uint8 stateId);
uint8 deque();
void onStateEntry(uint8 stateId);
uint8 getNext(uint8 index);


/* 
********************************************************************************************************* 
*                                        CONFIGURATION BITS 
********************************************************************************************************* 
*/ 

#pragma config FNOSC 	= FRCPLL       	// Internal Fast RC oscillator (8 MHz) w/ PLL
#pragma config FPLLIDIV = DIV_2    		// Divide FRC before PLL (now 4 MHz)
#pragma config FPLLMUL  = MUL_20     	// PLL Multiply (now 80 MHz)
                                    
#pragma config FPLLODIV = DIV_1         // PLL Output Divider
#pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
#pragma config FWDTEN   = OFF           // Watchdog Timer
#pragma config WDTPS    = PS1           // Watchdog Timer Postscale
#pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
#pragma config OSCIOFNC = OFF           // CLKO Enable
#pragma config POSCMOD  = XT            // Primary Oscillator
#pragma config IESO     = OFF           // Internal/External Switch-over
#pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable
#pragma config CP       = OFF           // Code Protect
#pragma config BWP      = OFF           // Boot Flash Write Protect
#pragma config PWP      = OFF           // Program Flash Write Protect
#pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select
#pragma config DEBUG    = OFF           // Debugger nabled for Starter Kit





/* 
********************************************************************************************************* 
*                                        DEFINES 
********************************************************************************************************* 
*/ 


#define DEBUG 

int main(void)
{
	board_init();												//Initialize the circuit board configurations
  	changeState(START);											//Initialize the state
  	enque(DIAGNOSTIC);											//first do diagnostic
  	//motorsInit();
  	//motorDir(CLOCKWISE);
  	
  	#ifdef SHOW_MESSAGES
	    hal_sendString_UART1("Machine started");
	    hal_sendChar_UART1('\n');
    #endif
  	
  	//INTEnableInterrupts();
  	//INTEnableSystemMultiVectoredInt();
  	
  	
  	SKPIC32_Init();
	SKPIC32_GLCDInit();
	SKPIC32_GLCDWriteText(0, 0, "Hello World!");
	SKPIC32_GLCDWriteText(0, 1, "This is SKPIC32!");
	SKPIC32_GLCDWriteText(0, 2, "1234567890");
	SKPIC32_GLCDWrite(0, 3, 0x31);
	DelayMs(2000);
	

  	while(1){
    	uint8 nextEvent = deque();								//get next event
    	if(nextEvent)
	    {
     		stateMachine(nextEvent);							//enter the event functions
    	}
  
    	if(keyuse ==(uint8)PRODUCT)								//check key press for product selection
     	{	    
     			keypad_pole();
     	}
     	else if(keyuse ==(uint8)AMOUNT)							//check key press for amount selection
     	{
     			keypad_pole();	
     	}	
  	}	
  	
}


void board_init(){
	DDPCONbits.JTAGEN = 0;										//disable JTAG
	DBINIT();     												// Initialize the IO channel
  	hal_allUARTInit();											//Initialize all UARTs
  	keypad_init();												//Initialize keypad
}



void stateMachine(uint8 eventId){
  switch(state){
    case START :
    	if(eventId == (uint8)DIAGNOSTIC){
	    	enque(INIT_VARS);
    	    changeState(DIAGNOSTIC);
    	}          	
     break;
    
    case DIAGNOSTIC:
      	if(eventId == (uint8)INIT_VARS){
	      	enque(CASH_IN);  			//for test only
	      	enque(PRODUCT_NO); 			//for test only
    	    changeState(INIT);  
    	}    
    	else if(eventId == (uint8)ERROR){	    	
	    	enque(SEND_ERROR);
    	    changeState(SYSTEM_LOCK);
    	}      
     break;
    
    case INIT:
      	if(eventId == (uint8)CASH_IN){
    	    changeState(WAIT_MONEY);  
    	}    
    	else if(eventId == (uint8)COIN_IN){
    	    changeState(WAIT_MONEY);
    	}
    	else if(eventId == (uint8)NFC_IN){	    	
	    	enque( NFC_GET_CONFIRM);
    	    changeState(NFC_PAY);
    	}
    	else if(eventId == (uint8)UPDATE_DATA){
    	    changeState(UPDATE_VARS);
    	}
    
     break;
    
    case UPDATE_VARS :
      	if(eventId == (uint8)FIN){
    	    changeState(INIT);  
    	}    
    	else if(eventId == (uint8)MORE_DATA){
	    	
    	}
   
     break;
    
    case WAIT_MONEY:
    	if(eventId == (uint8)PRODUCT_NO){
    	    changeState(WAIT_PRODUCT);  
    	}    
    	else if(eventId == (uint8)COIN_IN){
    	}
    	else if(eventId == (uint8)NFC_IN){	    	
    	}
    	else if(eventId == (uint8)CANCEL){
	    	enque(INIT_VARS);
	    	changeState(RETURN_MONEY);
    	}
    	else if(eventId == (uint8)TIME_OUT){
	    	enque(INIT_VARS);	    	
	    	changeState(RETURN_MONEY);
    	}
    	
     break;
    
    case WAIT_PRODUCT:
      	if(eventId == (uint8)OK){
	      	#ifdef DEBUG
				hal_sendString_UART1("Got product");
				hal_sendChar_UART1('\n');
			#endif
			SKPIC32_GLCDClearDisp();
    	    changeState(WAIT_AMOUNT);  
    	}    
    	else if(eventId == (uint8)ENTER_NO){
	    	check_key()
	    	if(count<3 && key<10){
		 		product_no=product_no*10+key;
		 		//write to LCD
		 		SKPIC32_GLCDClearDisp();
		 		DelayMs(20);
		 		char* num=(unsigned char)product_no;
		 		SKPIC32_GLCDWriteText(count,0,num);
		 		DelayMs(20);
		 		#ifdef DEBUG
					hal_sendString_UART1("product no = ");
					hal_uartWriteNumber(product_no);
					hal_sendChar_UART1('\n');
				#endif
		 	}
    	}
    	else if(eventId == (uint8)CANCEL){
	    	product_no=0;
	    	enque(INIT_VARS);
	    	changeState(RETURN_MONEY);
    	}
    	else if(eventId == (uint8)WRONG){
	    	product_no=0;
	    	changeState(WAIT_MONEY);
    	}
      
     break;
    
    case WAIT_AMOUNT:
    	if(eventId == (uint8)OK){
	    	#ifdef DEBUG
				hal_sendString_UART1("Got amount");
				hal_sendChar_UART1('\n');
			#endif
			SKPIC32_GLCDClearDisp();
    	    changeState(BALANCE);  
    	}    
    	else if(eventId == (uint8)ENTER_NO){
	    	check_key()
	    	if(count<3 && key<10){	
		 		amount=amount*10+key;
		 		//write to LCD
		 		SKPIC32_GLCDClearDisp();
		 		DelayMs(20);
		 		char* num=amount;
		 		SKPIC32_GLCDWriteText(count,0,num);
		 		DelayMs(20);
		 		#ifdef DEBUG
					hal_sendString_UART1("amount = ");
					hal_uartWriteNumber(amount);
					hal_sendChar_UART1('\n');
				#endif
		 	}
    	}
    	else if(eventId == (uint8)CANCEL){
	    	amount=0;
	    	changeState(WAIT_AMOUNT);
    	}
    
     break;
    
    case BALANCE:
    	if(eventId == (uint8)OK){
    	    changeState(DISPENSE);  
    	}    
    	else if(eventId == (uint8)CANCEL){
	    	enque(INIT_VARS);
	    	changeState(RETURN_MONEY);
    	}
    
     break;
      
    case DISPENSE:
      	if(eventId == (uint8)INIT_VARS){
    	    changeState(INIT);  
    	}    
    	else if(eventId == (uint8)CANCEL){
	    	enque(INIT_VARS);
	    	changeState(RETURN_MONEY);
    	}
    	else if(eventId == (uint8)ERROR){
	    	enque(ERROR);	    	
	    	changeState(RETURN_MONEY);
    	}
    	
    	
     break;
    
    case RETURN_MONEY:
    	if(eventId == (uint8)INIT_VARS){
    	    changeState(INIT);  
    	}    
    	else if(eventId == (uint8)ERROR){
	    	enque(SEND_ERROR);
    	    changeState(SYSTEM_LOCK);
    	}
      
     break;
      
    case SYSTEM_LOCK:
    	if(eventId == (uint8)SEND_ERROR){
    	    changeState(GSM_CONTROL);  
    	} 
      
     break;
      
    case NFC_PAY:
    	if(eventId == (uint8)NFC_GET_CONFIRM){
	    	enque(NFC_SET_CONFIRM);
    	    changeState(GSM_CONTROL);  
    	}    
    	else if(eventId == (uint8)PRODUCT_NO){
	    	changeState(WAIT_PRODUCT); 
    	}
      
     break;
      
    case GSM_CONTROL:
    	if(eventId == (uint8)NFC_SET_CONFIRM){
    	    changeState(NFC_PAY);  
    	}
      
     break;
        
    default:
    
     break;
    
  }
}




void changeState(uint8 nextStateId){
  if(nextStateId != state){
    onStateExit(state);
  }
  state = nextStateId;
  onStateEntry(nextStateId);
}

void onStateEntry(uint8 stateId){
  switch(stateId){
    case START :
    
          	
      break;
    
    case DIAGNOSTIC:
      
	
    break;
    
    case INIT:
      
 
    
    break;
    
    case UPDATE_VARS :
      
   
    break;
    
    case WAIT_MONEY:
    
    
    break;
    
    case WAIT_PRODUCT:
    count=0; 
    keyuse=PRODUCT;     
      
    break;
    
    case WAIT_AMOUNT:
    keyuse==AMOUNT; 
    count=0; 
    
    break;
    
    case BALANCE:
    
    break;
      
    case DISPENSE:
      
      break;
    
    case RETURN_MONEY:
      
      break;
      
    case SYSTEM_LOCK:
      
      break;
      
    case NFC_PAY:
      
      break;
      
    case GSM_CONTROL:
      
      break;
    
    default:
    
      break;
    
  }
}

void onStateExit(uint8 stateId){
  
  switch(stateId){
    case START :
    
          	
      break;
    
    case DIAGNOSTIC:
      
	
      break;
    
    case INIT:
      
 
    
      break;
    
    case UPDATE_VARS :
      
   
      break;
    
    case WAIT_MONEY:
      
    
      break;
    
    case WAIT_PRODUCT:
	count=0; 
      
    break;
    
    case WAIT_AMOUNT:
    keyuse=0;
	count=0;
    
    break;
    
    case BALANCE:
    
      break;
      
    case DISPENSE:
      
      break;
    
    case RETURN_MONEY:
      
      break;
      
    case SYSTEM_LOCK:
      
      break;
      
    case NFC_PAY:
      
      break;
      
    case GSM_CONTROL:
      
      break;
    
    default:
    
      break;
   
  }
  
}



//////////////////////////////////////////////////////////////////////////////////
//Event queue management

uint8 enque(uint8 eventId){
  if(getNext(queueTail) == queueHead){
    return FALSE;
  }
  else{
    g_EventQueue[queueTail] = eventId;
    queueTail = getNext(queueTail);
    return TRUE;
  }
}

uint8 deque(){
  if(queueHead == queueTail){
    return 0;
  }
  else{
    uint8 val = g_EventQueue[queueHead];
    queueHead = getNext(queueHead);
    return val;
  }
}

     
uint8 getNext(uint8 index){
    if(index == NO_OF_EVENTS - 1){
      return 0;
    }
    else{
      return ++index;
    }
}





