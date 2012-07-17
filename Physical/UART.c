#include "global.h"
#include "UART.h"
#include "LCD.h"

#define DESIRED_BAUDRATE_1A   		(9600)      //The desired BaudRate 1A
#define DESIRED_BAUDRATE_1B    		(9600)      //The desired BaudRate 1B
#define DESIRED_BAUDRATE_2A    		(9600)      //The desired BaudRate 2A
#define DESIRED_BAUDRATE_2B    		(9600)      //The desired BaudRate 2B
#define DESIRED_BAUDRATE_3A    		(9600)      //The desired BaudRate 3A
#define DESIRED_BAUDRATE_3B    		(9600)      //The desired BaudRate 3B
#define DESIRED_BAUDRATE    		(9600) 

#define DEBUG_PORT					UART3A


void hal_allUARTInit(void){

	UART_INIT(UART1,DESIRED_BAUDRATE_1A,8_BITS)
	UART_INIT(UART2,DESIRED_BAUDRATE_1B,8_BITS)
	UART_INIT(UART3,DESIRED_BAUDRATE_2A,8_BITS)
	UART_INIT(UART4,DESIRED_BAUDRATE_2B,8_BITS)
	UART_INIT(UART5,DESIRED_BAUDRATE_3A,8_BITS)
	UART_INIT(UART6,DESIRED_BAUDRATE_3B,8_BITS)

}

//function prototype - void hal_sendString_##PORT (const char *string)
UART_SEND_STRING(UART1)
UART_SEND_STRING(UART2)
UART_SEND_STRING(UART3)
UART_SEND_STRING(UART4)
UART_SEND_STRING(UART5)
UART_SEND_STRING(UART6)


//function prototype - void hal_sendChar_##PORT (const char character)
UART_SEND_CHAR(UART1)
UART_SEND_CHAR(UART2)
UART_SEND_CHAR(UART3)
UART_SEND_CHAR(UART4)
UART_SEND_CHAR(UART5)
UART_SEND_CHAR(UART6)

//interrupt service routines
UART_INT(_UART_1, ipl2){
	//UART_INT_TEST(UART1)
	if(INTGetFlag(INT_SOURCE_UART_RX(UART1)))		
	{												
	    INTClearFlag(INT_SOURCE_UART_RX(UART1));	    
	    if (UARTReceivedDataIsAvailable(UART1))      
		{                                           
			uint8 x=UARTGetDataByte(UART1);
			
			switch(x){
				case 4:RS=~RS;hal_sendChar_UART1 (x);break;
				case 5:RW=~RW;hal_sendChar_UART1 (x);break;
				case 6:E=~E;hal_sendChar_UART1 (x);break;
				case 7:DB0=~DB0;hal_sendChar_UART1 (x);break; 
			 	case 8:DB1=~DB1;hal_sendChar_UART1 (x);break;
			 	case 9:DB2=~DB2;hal_sendChar_UART1 (x);break;
			 	case 10:mPORTGToggleBits(BIT_15);hal_sendChar_UART1 (x);break;
			 	case 11:DB4=~DB4;hal_sendChar_UART1 (x);break;
			 	case 12:DB5=~DB5;hal_sendChar_UART1 (x);break;
			 	case 13:DB6=~DB6;hal_sendChar_UART1 (x);break;
			 	case 14:DB7=~DB7;hal_sendChar_UART1 (x);break;
			 	case 15:PSB=~PSB;hal_sendChar_UART1 (x);break;
			 	case 17:RESET=~RESET;hal_sendChar_UART1 (x);break;
			}
		}											
	}												
	if ( INTGetFlag(INT_SOURCE_UART_TX(UART1)) )  	
	{												
		INTClearFlag(INT_SOURCE_UART_TX(UART1));	    
	}
}	
UART_INT(_UART_2, ipl2){
	UART_INT_TEST(UART2)
}
UART_INT(_UART_3, ipl2){
	UART_INT_TEST(UART3)
}
UART_INT(_UART_4, ipl2){
	UART_INT_TEST(UART4)
}
UART_INT(_UART_5, ipl2){
	UART_INT_TEST(UART5)
}
UART_INT(_UART_6, ipl2){
	UART_INT_TEST(UART6)
}


//only for debug port only - UART3A
void hal_uartWriteNumber(unsigned int no){	
	char val[10];

	unsigned int i=0;
	while(no>0){
		unsigned int dec = no%10;
		no = no/10;
		i++;
		val[i]=dec+'0';
	}

	for(;i>0;i--){
		hal_sendChar_UART1(val[i]);
	}		
}




/*
// Is this an RX interrupt?
	if(INTGetFlag(INT_SOURCE_UART_RX(UART3A)))
	{
		// Clear the RX interrupt Flag
	    INTClearFlag(INT_SOURCE_UART_RX(UART3A));

		// Echo what we just received.
		//PutCharacter(UARTGetDataByte(UART3A));
		hal_sendChar_UART3A(UARTGetDataByte(UART3A));

	}

	// We don't care about TX interrupt
	if ( INTGetFlag(INT_SOURCE_UART_TX(UART3A)) )
	{
		INTClearFlag(INT_SOURCE_UART_TX(UART3A));
	}
*/
