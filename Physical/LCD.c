/*
********************************************************************************************************* 
 *					 	LCD 
 *
 * (c) Copyright 2012 D2NM, All rights reserved
********************************************************************************************************* 
*/


/*
********************************************************************************************************* 
 * 						Vending machine controller 
 *
 * Filename      : LCD.c
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
#include "LCD.h"
#include <plib.h>                   // include peripheral library functions
#include "global.h"


/* 
********************************************************************************************************* 
*                                               DEFINES
********************************************************************************************************* 
*/ 




/* 
********************************************************************************************************* 
*                                              MACROS 
********************************************************************************************************* 
*/ 

				

							
#define Input_LCD_pins() 	mPORTCOpenDrainOpen(BIT_3|BIT_2|BIT_1);\
							mPORTEOpenDrainOpen(BIT_2|BIT_3|BIT_4|BIT_5|BIT_6|BIT_7);\
							mPORTGOpenDrainOpen(BIT_12|BIT_13|BIT_14|BIT_15);\
							PORTSetPinsDigitalIn(IOPORT_C , BIT_3|BIT_2|BIT_1);\
							PORTSetPinsDigitalIn(IOPORT_E , BIT_2|BIT_3|BIT_4|BIT_5|BIT_6|BIT_7);\
							PORTSetPinsDigitalIn(IOPORT_G , BIT_12|BIT_13|BIT_14|BIT_15);
								
					
/* 
********************************************************************************************************* 
*                                          GLOBAL VARIABLES 
********************************************************************************************************* 
*/ 




void LCDInit(){

PORTClearBits(IOPORT_C , BIT_3|BIT_2|BIT_1);
PORTClearBits(IOPORT_E , BIT_2|BIT_3|BIT_4|BIT_5|BIT_6|BIT_7);
PORTClearBits(IOPORT_G , BIT_12|BIT_13|BIT_14|BIT_15);
PORTSetPinsDigitalOut(IOPORT_C , BIT_3|BIT_2|BIT_1);
PORTSetPinsDigitalOut(IOPORT_E , BIT_2|BIT_3|BIT_4|BIT_5|BIT_6|BIT_7);
PORTSetPinsDigitalOut(IOPORT_G , BIT_12|BIT_13|BIT_14|BIT_15);
mPORTCOpenDrainOpen(BIT_3|BIT_2|BIT_1);
mPORTEOpenDrainOpen(BIT_2|BIT_3|BIT_4|BIT_5|BIT_6|BIT_7);
mPORTGOpenDrainOpen(BIT_12|BIT_13|BIT_14|BIT_15);
					
					
}


void start_LCD(){

RS=0;
RW=0;
E=0;
DB0=0;
DB1=0;
DB2=0;
DB3=0;
DB4=0;
DB5=0;
DB6=0;
DB7=0;
PSB=1;
RESET=1;

}

void enable(){
E=1;
}

void disable(){
E=0;
}

void select_mode(uint8 mode){

RS=mode;

}


void ShortDelay(                       // Short Delay 
  uint32 DelayCount)                   // Delay Time (CoreTimer Ticks) 
{ 
  UINT32 StartTime;                    // Start Time 
  StartTime = ReadCoreTimer();         // Get CoreTimer value for StartTime 
  while ( (UINT32)(ReadCoreTimer() - StartTime) < DelayCount ) {}; 
} 



/*********************************************************************
Basic Instruction Set

Use Instruction_Mode() before using bellow fuctions

Example:

Instruction_Mode()
Display_Clear();
	
**********************************************************************/

//Display Clear

void Display_Clear(){
	start_LCD();
	DB0=1;
	enable();
	ShortDelay(8000);
	disable();
	ShortDelay(8000);
}


//Return Home
//Cursor moves to origin
void Return_Home(){
	start_LCD();
	DB1=1;
	enable();
	ShortDelay(8000);
	disable();
	ShortDelay(8000);
}


//Entry Mode Set
//Set the cursor movement and display shift direction when doing write or read operation. 
void Entry_Mode(uint8 cursor,uint8 shift){ 

	start_LCD();
	DB2=1;
	if(cursor == RIGHT) 	//cursor moves right
		DB1=1;
	else if(cursor == LEFT) //cursor moves left
		DB1=0;
	if(shift == ON) 		//Entire display shift by 1 
		DB0=1;
	enable();
	ShortDelay(8000);
	disable();
	ShortDelay(8000);

}


//Display Control

void Display(uint8 state){
	start_LCD();
	DB3=1;
	DB2=state;
	enable();
	ShortDelay(8000);
	disable();
	ShortDelay(8000);
}


void Cursor(uint8 state){
	start_LCD();
	DB3=1;
	DB1=state;
	enable();
	ShortDelay(8000);
	disable();
	ShortDelay(8000);
}


void Character_Blink(uint8 state){

	DB3=1;
	if(state==ON){
		DB0=1;
	}

}


//Cursor Display Control
//configures the cursor moving direction or the display shifting direction.
void CursorDisplay_Shift(uint8 cursor,uint8 shift){ 

	DB4=1;
	if(cursor == LEFT); 	//cursor moves left
	else if(cursor == RIGHT) //cursor moves right
		DB2=1;
	if(shift == LEFT){ 		//Entire display shift left by 1 
		DB3=1;
		DB2=0;
	}
	else if(shift == RIGHT){//Entire display shift left by 1 
		DB3=1;
		DB2=1;
	}	

}


//function set
//In same instruction cannot alter DL and RE at once. Make sure that change DL first then RE. 

//DL
void MPU_bus(uint8 bus){
	start_LCD();
	DB5=1;
	DB4=bus;			// 1- 8-bit MPU bus interface,0- 4-bit MPU bus interface  
 	enable();
	ShortDelay(8000);
	disable();
	ShortDelay(8000);	
}

//RE
void Extended_instruct(uint8 extd){
	start_LCD();
	DB5=1;
	DB2=extd; 					//1- extended instruction set, 0- basic instruction set 
	enable();
	ShortDelay(8000);
	disable();
	ShortDelay(8000);
	
}


//CGRAM Address
//Set CGRAM address into address counter (AC)  
//AC range is 00H…3FH
//Make sure that in extended instruction SR=0 (scroll address or RAM address select)
void CGRAM_adress(uint8 address){

	DB6=1;
	DB0=address&BIT_0;
	DB1=address&BIT_1;
	DB2=address&BIT_2;
	DB3=address&BIT_3;
	DB4=address&BIT_4;
	DB5=address&BIT_5;

}

//DDRAM Address
//Set DDRAM address into address counter (AC). 
//First line AC range is 80H…8FH 
//Second line AC range is 90H…9FH 
//Third line AC range is A0H…AFH 
//Fourth line AC range is B0H…BFH 
void DDRAM_adress(uint8 address){

	DB7=1;
	DB0=address&BIT_0;
	DB1=address&BIT_1;
	DB2=address&BIT_2;
	DB3=address&BIT_3;
	DB4=address&BIT_4;
	DB5=address&BIT_5;
	DB6=address&BIT_6;

}

//Read Busy Flag (BF) and Address
//Read busy flag (BF) can check whether the internal operation is finished or not. At the same time, the value of 
//address counter (AC) is also read. When BF = “1”, further instruction(s) will not be accepted until BF = “0”. 
void BusyFlag(uint8 address){
	Input_LCD_pins()
	PORTSetBits(IOPORT_C , RW);
	PORTClearBits(IOPORT_C, RS);
	PORTSetPinsDigitalOut(IOPORT_C , RW|RS);
	mPORTCOpenDrainOpen(RW|RS);
	RW=1;
	DB7=1;
	DB0=address&BIT_0;
	DB1=address&BIT_1;
	DB2=address&BIT_2;
	DB3=address&BIT_3;
	DB4=address&BIT_4;
	DB5=address&BIT_5;
	DB6=address&BIT_6;

}





/*********************************************************************
END of basic Instruction set
**********************************************************************/

unsigned char X,Y;


void lcdwc(unsigned char com)
{
	ShortDelay(8000);
	RS=0;
	RW=0;
    DB0=com&BIT_0;
	DB1=com&BIT_1;
	DB2=com&BIT_2;
	DB3=com&BIT_3;
	DB4=com&BIT_4;
	DB5=com&BIT_5;
	DB6=com&BIT_6;
	DB7=com&BIT_7;   
	enable();
	ShortDelay(8000);
	disable();
	ShortDelay(8000);
}


void lcdwd(unsigned char dat)
{
    RS=1;
    RW=0;
    DB0=dat&BIT_0;
	DB1=dat&BIT_1;
	DB2=dat&BIT_2;
	DB3=dat&BIT_3;
	DB4=dat&BIT_4;
	DB5=dat&BIT_5;
	DB6=dat&BIT_6;
	DB7=dat&BIT_7;   
	enable();
	ShortDelay(8000);
	disable();
	ShortDelay(8000);
}

void lcdpos(void)
{
   X&=0x07;     
   Y&=0x03;	  
   if(Y==0x00)
      lcdwc(X|0x80);
   else if(Y==0x01)
      lcdwc((X+0x40)|0x80);
}


unsigned int CGTAB[9][8]={

0x08,0x0f,0x12,0x0f,0x0a,0x1f,0x02,0x02,   
0x0f,0x09,0x0f,0x09,0x0f,0x09,0x11,0x00,   
0x1f,0x11,0x11,0x1f,0x11,0x11,0x1f,0x00,   
0x11,0x0a,0x04,0x1f,0x04,0x1f,0x04,0x00,   
0x0e,0x00,0x1f,0x0a,0x0a,0x0a,0x13,0x00,   
0x18,0x18,0x07,0x08,0x08,0x08,0x07,0x00,   
0x04,0x0a,0x15,0x04,0x04,0x04,0x04,0x00,   
0x17,0x15,0x15,0x15,0x15,0x15,0x17,0x00,   
0x04,0x04,0x04,0x04,0x15,0x0a,0x04,0x00,   
};



CGRAM()
  {
   uint8 dat;
   unsigned char i,j;
   CGRAM_adress(0x40);    


   for(i=0;i<9;i++)
     {
     for(j=0;j<8;j++)
        {
        dat=CGTAB[i][j]; 
        lcdwd(dat); 
        }
     }
  }



void lcdfill(unsigned char n)
{
   for(Y=0;Y<2;Y++)
      for(X=0;X<8;X++)
      {  lcdpos();      
         lcdwd(n);  }
}

