/*
********************************************************************************************************* 
* 						bill validator 
* 				(c) Copyright 2012 D2NM, All rights reserved
********************************************************************************************************* 
*/


/*
********************************************************************************************************* 
* 						bill validator 
* Filename      : 
* Version       : V1.0
* Programmer(s) : TCY
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

#include "MDB.h"
#include "UART.h"

/* 
********************************************************************************************************* 
*                                               DEFINES
********************************************************************************************************* 
*/ 
//HAL defines

//UART allocation
//UART1A - UART1 
//UART3A - UART2 
//UART2A - UART3 
//UART1B - UART4 
//UART3B - UART5 
//UART2B - UART6
#define PB_DIV                 	8
#define PRESCALE               	256
#define TIME_FREQUENCY			1000
#define MILLISECOND				SYS_FREQ/PB_DIV/PRESCALE/1000


#define NOT_INIT	0
#define READY		1

#define NONE						0
#define NO_PENDING_CMD				0xFFFF

#define DESIRED_BAUDRATE    		(9600) 
#define MDB_PORT 					UART3A
#define MDB_VECTOR 					_UART_3A
	
#define NO_OF_MDB_EVENTS 			3
#define POLL_TIME					50*(MILLISECOND)
#define RETRY_TIME					5*(MILLISECOND)

//states
#define IDLE_STATE			1
#define COMM_STATE	 		2

//events
#define COMM_NEXT			1
#define MESSAGE_RECEIVED	2
#define COMM_SUCCESSFUL		3
#define RETRY				4


//addresses
#define BILL_VALIDATOR_ADDRESS	0x060    //0x030 - all values are shifted by 1 to make room for address bit
#define COIN_CHANGER_ADDRESS	0x010     //0x08
#define MODE_BIT				0x001


//bill validator - commands
#define RESET 			0x00  //0x00 up to 0x07 - all values are shifted by 1 to make room for address bit
#define SETUP 			0x02
#define SECURITY 		0x04
#define POLL 			0x06
#define BILL_TYPE 		0x08
#define ESCROW 			0x0A
#define STACKER 		0x0C
#define EXPANSION 		0x0E

//common and basic commands
#define ACK	 			0x00			
#define NAK				0x1FE //0xFF
#define RET				0x154 //0xAA

//lengths of responses
#define SETUP_LENGTH	 	15

//positions on security setups
#define SECURITY_POSITION	8


#define BILL_TYPE_BITS     0b00001111
#define BILL_STATUS_BITS   0b01110000

//poll responses
#define BILL_STACKED	   					0
#define ESCROW_POSITION						1
#define BILL_RETURNED						2
#define BILL_TO_RECYCLER					3
#define DISABLED_BILL_REJECTED 				4
#define BILL_TO_RECYCLER_MANUAL_FILL 		5
#define MANUAL_DISPENSE						6
#define TRANSFERRED_FROM_RECYCLER			7

//setup response positions
#define BILL_TYPE_POS			12
#define CURRENCY_CODE_POS		2
#define BILL_SCALE_FACTOR_POS	4
#define LKR_CODE				0x1144

#define INIT_CMD_LENGTH 5

//coin commands
#define TUBE_STATUS		0x04
#define COIN_TYPE 		0x08
#define DISPENSE_COIN	0x0A


//peripherlal bytes
#define SECURITY_HIGH_BYTE 0x00 //0
#define SECURITY_LOW_BYTE  0x00 //0

#define BILL_TYPE_ENABLE_HIGH_BYTE 			0x00
#define BILL_TYPE_ENABLE_LOW_BYTE  			0x00
#define BILL_TYPE_ESCROW_ENABLE_HIGH_BYTE 	0x00
#define BILL_TYPE_ESCROW_ENABLE_LOW_BYTE  	0x00

#define ESCROW_STATUS	0x00

//amount of bytes to be received - without address bytes (have to add 1 if address byte)
#define SETUP_BYTES 		27
#define POLL_BYTES 			16
#define EXPANSION_BYTES 	29
#define STACKER_BYTES 		2

//rember to raise error here
#define CHECK_BYTES(OF) 				\
	if(contentLength != OF##_BYTES)		\
	{									\
		mdbEnque(RETRY);				\
		return;							\
	}									\
	break;								\



/* 
********************************************************************************************************* 
*                                          GLOBAL VARIABLES 
********************************************************************************************************* 
*/ 

///////////Mdb state machine variables/////////////////////
unsigned char mdbQueue[NO_OF_MDB_EVENTS];
int queueHead;
int queueTail;
unsigned char state;

////////////Mdb communication variables///////////////////
unsigned int mdbPacketTx[36];
unsigned int mdbPacketRx[36]; 

//////tyepdef for the struct having peripheral details///////////////////
typedef struct {
   unsigned int 	address;
   unsigned int 	status;
   unsigned char 	currentInit;   //current init command processing    
   unsigned int 	timeWithoutAns;
   unsigned int 	currencyType;
} peripheralDetails;

/////////////processing variables////////////////////////
unsigned int currentPeripheral; //current peripheral number
unsigned int currentCommand;    //current executing command
unsigned int pendingCommand;    //to store the next command to be executed

const unsigned int noOfPeripherals = 2;            //peripheral amount
peripheralDetails peripherals[2];    //peripheral details

unsigned int lengthRx = 0;  //lengthRx = 0 still has not received any
unsigned int lengthTx = 0;
unsigned int lastPacket = 0;   //logic not implemented
unsigned int contentLength;

const unsigned int billsAccepted[] = {20,50,100,500};
const unsigned int lengthBillsAccepted = 4;  //length of the above array
unsigned int sendBillsAccepted = 0;   //bills accepted mask

const unsigned int initCommands[] = {RESET,POLL,SETUP,STACKER,BILL_TYPE};

/* 
********************************************************************************************************* 
*                                        FUNCTION PROTOTYPES 
********************************************************************************************************* 
*/ 

////////////////////Queue management functions////////////////////////////////
uint8 mdbEnque(uint8 eventId);
uint8 mdbDeque();
uint8 mdbGetNext(uint8 index);

////////////////////HAL functions///////////////////////////////////////////
void startRetryTimer();
void closeRetryTimer();
void startPollTimer();
void closePollTimer();
MDB_SEND_9_DATA_PROTO(MDB_PORT);


///////////////////Mdb processing functions////////////////////////////
unsigned int preparePacket(unsigned int command,unsigned int unit);
void sendMdbPacket(unsigned int command,unsigned int unit);
unsigned int generateCheckSum(unsigned int length,unsigned int * buffer);
void receivePacket(unsigned int _9bitData);
void setStatus();
void processRxPacket();
void retry();
void getNextPeripheral();
void hal_mdbInit(void);



/* 
********************************************************************************************************* 
*                                        CONFIGURATION BITS 
********************************************************************************************************* 
*/ 



/* 
********************************************************************************************************* 
*                                        DEFINES 
********************************************************************************************************* 
*/ 





unsigned int preparePacket(unsigned int command,unsigned int unit){
	
	mdbPacketTx[0] = (unsigned char)(command | unit | MODE_BIT); 
	unsigned int length = 1;
	
	currentCommand = command;
	if(unit == BILL_VALIDATOR_ADDRESS){
		switch(command){	
			case RESET: break;   //nothing to do 
			case SETUP: break;  //nothing to do
			case SECURITY: {
				mdbPacketTx[1] = SECURITY_HIGH_BYTE;
				mdbPacketTx[2] = SECURITY_LOW_BYTE;	
				length += 2;
				break;
			}	
			case POLL: break; //nothing to do
			case BILL_TYPE:{
				mdbPacketTx[1] = (sendBillsAccepted>>8)<<1;
				mdbPacketTx[2] = (sendBillsAccepted%256)<<1;
				mdbPacketTx[3] = mdbPacketTx[1];
				mdbPacketTx[4] = mdbPacketTx[2];
				length += 4;
				break;
			}	 
			case ESCROW:{
				mdbPacketTx[1] = ESCROW_STATUS;
				length += 2;
				pendingCommand = POLL;
				break;
			}	
			case STACKER: break;  //nothing to do
			case EXPANSION: break;  //see the sub command scene
			default:break;
		}
	}
	/*
	//coin commands - to be filled
	else if(unit == COIN_CHANGER_ADDRESS){
		switch(command){
			case RESET: break;
			case SETUP: break;
			case TUBE_STATUS: break;
			case POLL: break;
			case COIN_TYPE: break;
			case DISPENSE: break;
			case EXPANSION: break;
			default: break;
		}	
	}*/		
	
	return length;
}	

void sendMdbPacket(unsigned int command,unsigned int unit){
	
	unsigned int length = preparePacket(command,unit);
	unsigned int i = 0;
	for(;i<length;i++){
		hal_uartSend9bitData_MDB_PORT(mdbPacketTx[i]);
	}
	
	mdbPacketTx[i] = generateCheckSum(length,mdbPacketTx);
	hal_uartSend9bitData_MDB_PORT(mdbPacketTx[i]);
	lengthTx = length+1;	
	
}

unsigned int generateCheckSum(unsigned int length,unsigned int * buffer){
	unsigned int i = 0;
	unsigned int checkSum = *buffer;
	i++;
	for(;i<length;i++){
		checkSum += *(buffer+i);
	}
	return ((checkSum)&0xFF)<<1;	
}

void receivePacket(unsigned int _9bitData){
	
	//algo - check for ACK or NAK first - also other commands like just reset etc. if yes raise events if it is what is wanted and return
	//otherwise check for the final packet - then call to process the packet
	unsigned int last = _9bitData & MODE_BIT;

	mdbPacketRx[lengthRx] = _9bitData;
	lengthRx++;
	
	unsigned int command = _9bitData ^ MODE_BIT;
	
	
	//first part of algo
	if(currentCommand == RESET || 
	   currentCommand == SECURITY || 
	   currentCommand == BILL_TYPE || 
	   currentCommand == ESCROW ||
	   currentCommand == POLL)
	{	
		if(command == ACK){ 
			mdbEnque(COMM_SUCCESSFUL);
			lengthRx = 0;
			setStatus();
			
			return;
		}  	  
		else if(command == NAK){
			mdbEnque(RETRY);
			lengthRx = 0;
			return;
		} 
	}
	else{
		if(command == ACK){
			lengthRx = 0;
			return;
		} 		
		else if(command == NAK){
			mdbEnque(RETRY);
			lengthRx = 0;
			return;
		} 	
	}
	
	//second part of algo
	if(last){
		processRxPacket();
		contentLength = lengthRx - 1;  //-1 for the CHK byte
		lengthRx = 0;
	}
	else{
		startRetryTimer();
	}	
	
}

void setStatus(){
	peripheralDetails * cur = peripherals + currentPeripheral;
	if(cur->status == NOT_INIT){
		currentCommand = initCommands[cur->currentInit];
		if(cur->currentInit < INIT_CMD_LENGTH - 1){
			cur->currentInit = cur->currentInit + 1;
		}
		else{
			cur->status = READY;
		}
		mdbEnque(COMM_NEXT);
	}
}

void processRxPacket(){
	
	//algo - first check for check sum if not raise exception
	//	   - next check for the number of bytes expected if not raise exception
	//	   - assign values for the variables
	//	   - raise event of successful reception 
	
	//checking checksum
	if( (mdbPacketRx[contentLength]^MODE_BIT) != generateCheckSum(contentLength,mdbPacketRx) ){
		return;
	}	
	
	//checking the number of bytes received - remember to raise event in macro
	switch(currentCommand){
		
		case SETUP:{CHECK_BYTES(SETUP)}	
		case POLL:{CHECK_BYTES(POLL)}
		case EXPANSION:{CHECK_BYTES(EXPANSION)}
		case STACKER:{CHECK_BYTES(STACKER)}
		
	}
	
	//one for sure we have the correct data	- now process
	if(currentCommand == SETUP){
	
		unsigned int currencyCode = mdbPacketRx[CURRENCY_CODE_POS]<<7 + mdbPacketRx[CURRENCY_CODE_POS+1]>>1;
		unsigned int scalingFactor = mdbPacketRx[BILL_SCALE_FACTOR_POS]<<7 + mdbPacketRx[BILL_SCALE_FACTOR_POS]>>1;
		
		if(currencyCode != LKR_CODE){
			sendBillsAccepted = 0;
			return;
		}		
		
		unsigned int i;
		unsigned int j;
		for(i=BILL_TYPE_POS;i<contentLength;i++){
			for(j=0;j<lengthBillsAccepted;j++){
				if( billsAccepted[j] == (mdbPacketRx[i]>>1)*(scalingFactor) ){
					sendBillsAccepted |= 0x01<<(i-BILL_TYPE_POS);
				}				
			}
		}
		
	}
	else if(currentCommand == POLL){
		//need to raise events depending on the response specially escrow
		unsigned int i=0;
		for(i=0;i<contentLength;i++){
			unsigned int bill = (mdbPacketRx[i] & BILL_TYPE_BITS);
			unsigned int statusOfBill = (mdbPacketRx[i] & BILL_STATUS_BITS)>>4;   //status error commands not implemened
			if(statusOfBill == ESCROW_POSITION){
				pendingCommand = ESCROW;
			}
			else if(statusOfBill == BILL_STACKED){
				//give out the value to the vending machine
			}
			else if(statusOfBill == BILL_RETURNED){
			}
		}
	}
	else if(currentCommand == EXPANSION){
	}
	else if(currentCommand == STACKER){
	}
	else{
		//RAISE EXP - INVALID COMMAND
	}
}

void retry(){

	unsigned int i=0;
	for(i=0;i<lengthTx;i++){
		hal_uartSend9bitData_MDB_PORT(mdbPacketTx[i]);
	}	
	lengthRx = 0;
	
}

void getNextPeripheral(){
	if(currentPeripheral==noOfPeripherals-1){
		currentPeripheral = 0;
	}
	else{
		currentPeripheral++;
	}	
}

void mdbStateMachine(){

	uint8 eventId = mdbDeque();
	if(eventId){
		switch(state){
		
			case IDLE_STATE:{
			
				if(eventId == COMM_NEXT){
					sendMdbPacket(peripherals[currentPeripheral].address,currentCommand);
					startRetryTimer();
					state = COMM_STATE;
				}
				break;
				
			}
			case COMM_STATE:{
			
				if(eventId == MESSAGE_RECEIVED){
					sendMdbPacket(peripherals[currentPeripheral].address,ACK);
					lastPacket = 1;
				}
				else if(eventId == COMM_NEXT){
					state = IDLE_STATE;
					mdbEnque(COMM_NEXT);
				}
				else if(eventId == COMM_SUCCESSFUL){
					if(pendingCommand != NO_PENDING_CMD){
						currentCommand = pendingCommand;
						mdbEnque(COMM_NEXT);
						pendingCommand = NO_PENDING_CMD;
					}
					state = IDLE_STATE;
				}
				else if(eventId == RETRY){
					retry();
				}
				
			}
		}
	}
}



////////////////////////////////////Hardware Abstraction Layer///////////////////////////////
void hal_mdbInit(void){
	
	state = IDLE_STATE;
	UART_INIT(MDB_PORT,DESIRED_BAUDRATE,9_BITS)
	
	peripherals[0].address 				= BILL_VALIDATOR_ADDRESS;
	peripherals[0].status  				= NOT_INIT;
	peripherals[0].timeWithoutAns 		= 0; 
	peripherals[0].currencyType 		= 0;
	peripherals[0].currentInit 			= 0;
	
	peripherals[1].address 		  		= COIN_CHANGER_ADDRESS;
	peripherals[1].status  		  		= NOT_INIT;
	peripherals[1].timeWithoutAns 		= 0; 
	peripherals[1].currencyType   		= 0;
	peripherals[1].currentInit 	  		= 0;
	
	currentPeripheral = 0;
	
	startPollTimer();
		
	//legacy opening 
	//#define UBRG(baud) (((GetPeripheralClock())/4/(baud)-1)) 
	//OpenUART2(UART_EN | UART_NO_PAR_9BIT | UART_1STOPBIT | UART_MODE_SIMPLEX | UART_BRGH_FOUR, UART_RX_ENABLE | UART_TX_ENABLE, UBRG(DESIRED_BAUDRATE) );

}

void startRetryTimer(){

	OpenTimer4(T4_ON | T4_SOURCE_INT | T4_PS_1_256, RETRY_TIME);
	ConfigIntTimer4(T4_INT_ON | T4_INT_PRIOR_4);
	
}	

void closeRetryTimer(){
	
	CloseTimer4();	
	WriteTimer4(0);
	
}	

void startPollTimer(){

	OpenTimer5(T5_ON | T5_SOURCE_INT | T5_PS_1_256, POLL_TIME);
	ConfigIntTimer5(T5_INT_ON | T5_INT_PRIOR_4);
	
}	

void closePollTimer(){
	
	CloseTimer5();	
	WriteTimer5(0);
	
}	

//functions for sending 9 bit data
//function prototype - void hal_uartSend9bitData_##PORT (unsigned int no)
MDB_SEND_9_DATA(MDB_PORT)

//////////////////////MDB ISRs///////////////////////////////////////////////////

//uart isr
UART_INT(_UART_3A, ipl2){
	if(INTGetFlag(INT_SOURCE_UART_RX(MDB_PORT))){
	    INTClearFlag(INT_SOURCE_UART_RX(MDB_PORT));	    
	    if (UARTReceivedDataIsAvailable(MDB_PORT))      
		{                
			//unsigned int temp =  ReadUART2();  -  legacy 
			UART_DATA data =  UARTGetData(MDB_PORT); 
			unsigned int temp = data.__data;      
			                
			//put an event and then handle 
			closeRetryTimer();
			receivePacket(temp);
			mdbEnque(MESSAGE_RECEIVED);
		}
	}																								
	if ( INTGetFlag(INT_SOURCE_UART_TX(MDB_PORT)) )  	
	{												
		INTClearFlag(INT_SOURCE_UART_TX(MDB_PORT));	    
	}
}

//timer ISR
void __ISR(_TIMER_5_VECTOR, ipl4) PollTimerIntHandler(void)
{
	mT5ClearIntFlag();

	mdbEnque(COMM_NEXT);	
	getNextPeripheral();
	if(peripherals[currentPeripheral].status == NOT_INIT){
		currentCommand = initCommands[peripherals[currentPeripheral].currentInit];
		pendingCommand = NO_PENDING_CMD;
	}
	else{
		currentCommand = POLL;
		pendingCommand = NO_PENDING_CMD;
	}
  
}

void __ISR(_TIMER_4_VECTOR, ipl4) RetryTimerIntHandler(void)
{
	mT4ClearIntFlag();
	if(!lastPacket){
		mdbEnque(RETRY);	
	}
	else{
		mdbEnque(COMM_SUCCESSFUL);
		setStatus();
		lastPacket = 0;
	}
  
}

//////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////
//Event queue management
/////////////////////////////////////////////////////////////////////////
uint8 mdbEnque(uint8 eventId){
  if(mdbGetNext(queueTail) == queueHead){
    return FALSE;
  }
  else{
    mdbQueue[queueTail] = eventId;
    queueTail = mdbGetNext(queueTail);
    return TRUE;
  }
}

uint8 mdbDeque(){
  if(queueHead == queueTail){
    return 0;
  }
  else{
    uint8 val = mdbQueue[queueHead];
    queueHead = mdbGetNext(queueHead);
    return val;
  }
}

     
uint8 mdbGetNext(uint8 index){
    if(index == NO_OF_EVENTS - 1){
      return 0;
    }
    else{
      return ++index;
    }
}




		