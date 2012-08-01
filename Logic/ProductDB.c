/*
********************************************************************************************************* 
 *					 	Vending machine controller 
 *
 * (c) Copyright 2012 D2NM, All rights reserved
********************************************************************************************************* 
*/


/*
********************************************************************************************************* 
 * 						Product database
 *
 * Filename      : ProductDB.c
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
#include "ProductDB.h"
#include "Flash_Controller.h"

/* 
********************************************************************************************************* 
*                                               DEFINES
********************************************************************************************************* 
*/ 



/* 
********************************************************************************************************* 
*                                          GLOBAL VARIABLES 
********************************************************************************************************* 
*/ 

uint8 traySize=0;
uint8 noOftrays=0;




void InitDB()
{
	uint32* tmp  = read_flash_page( sizeof(tbl)/32, 0);
	uint16 i=0;
	uint16 count=WORD_SIZE+4;
	
	#ifdef DEBUG
		hal_sendString_UART1("Database downloading");
		hal_sendChar_UART1('\n');
	#endif

	for(i=0;i<sizeof(tbl)*count/32;i++){
		uint8 j=i%count;
		uint8 k=i/count;
		if(j<WORD_SIZE)
		tbl[k].name[j]=*tmp;
		else if(j==WORD_SIZE)
		tbl[k].number=*tmp;
		else if(j==WORD_SIZE+1)
		tbl[k].amount=*tmp;
		else if(j==WORD_SIZE+2)
		tbl[k].valDec=*tmp;
		else if(j==WORD_SIZE+3)
		tbl[k].valCent=*tmp;
			
		tmp++;
	}
}


void addData(uint8 trayNum,uint8 num,char name[],uint32 number,uint32 amount,uint32 valDec,uint32 valCent)
{
	uint8 idx=(trayNum-1)*traySize+num;
	
	if(idx>0 && idx <sizeof(tbl)/32){
		uint8 i=0;
		for(i=0;i<sizeof(name);i++)
		tbl[idx].name[i]=(uint32)name[i];
		
		tbl[idx].number=number;
		tbl[idx].amount=amount;
		tbl[idx].valDec=valDec;
		tbl[idx].valCent=valCent;
	}
}


Table getTable(uint8 trayNum,uint8 num)
{
	if(trayNum>0 && trayNum<=noOftrays)
		return tbl[(trayNum-1)*traySize+num];
	else
		return tbl[0];
		
}

void flashDB()
{
	erase_flash_page(0);
	write_flash_page((uint32)tbl, sizeof(tbl)/32 , 0);
}

void setTraySize(uint8 size)
{
	traySize = size;
}

void setNoOftrays(uint8 num)
{
	noOftrays = num;
}


