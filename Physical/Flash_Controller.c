/*
********************************************************************************************************* 
 *					 	Flash Controller 
 *
 * (c) Copyright 2012 D2NM, All rights reserved
********************************************************************************************************* 
*/


/*
********************************************************************************************************* 
 * 						Vending machine controller 
 *
 * Filename      : Flash_Controller.c
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
#include "Flash_Controller.h"
#include <plib.h>                   // include peripheral library functions
#include "global.h"


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

unsigned int pagebuff[2048];
unsigned int databuff[128];


/* 
********************************************************************************************************* 
*                                        CONFIGURATION BITS 
********************************************************************************************************* 
*/ 



void flash_row_data(uint32* data, uint32 length){
uint32 x =0;

	for(x =0; x < sizeof(databuff)/4; x++){
		if(length>0){
			databuff[x] = *data;
			data++;
			length--;
		}
		else
			databuff[x] = 0;		
	}
	#ifdef DEBUG
		hal_sendString_UART1("row data aquired");
		hal_sendChar_UART1('\n');
	#endif
}


void flash_page_data(uint32* data, uint32 length){
uint32 x =0;
	for(x =0; x < sizeof(pagebuff)/4; x++){
		if(length>0){
			pagebuff[x] = *data;
			data++;
			length--;
		}
		else
			pagebuff[x] = 0;		
	}
	#ifdef DEBUG
		hal_sendString_UART1("page data aquired");
		hal_sendChar_UART1('\n');
	#endif
}
		
	
void erase_flash_page(uint32 page){	  //number from 0,1,2...		

 	NVMErasePage((void *)(NVM_PROGRAM_PAGE+page*NVM_PAGE_SIZE));  //erase the required page
 	#ifdef DEBUG
 		hal_sendString_UART1("page erased ");
		hal_sendChar_UART1('\n');
	#endif
}


void write_flash_word(uint32 data, uint32 address){

	NVMWriteWord((void*)(address), data);

	// Verify data matches
    if(*(int *)(address) != data)
	{
		//indicate an error
		#ifdef DEBUG
			hal_sendString_UART1("word write error ");
			hal_sendChar_UART1('\n');
		#endif
	}
}
 	
 	
void write_flash_row(uint32* data, uint32 length, uint32 page, uint32 row){
	
	flash_row_data(data,length);
	// Write 128 words 
	NVMWriteRow((void *)(NVM_PROGRAM_PAGE+page*NVM_PAGE_SIZE+row*NVM_ROW_SIZE), (void*)databuff);

	// Verify data matches
	if(memcmp(databuff, (void *)(NVM_PROGRAM_PAGE+page*NVM_PAGE_SIZE+row*NVM_ROW_SIZE), sizeof(databuff)))
	{
		// indicate an error
		#ifdef DEBUG
			hal_sendString_UART1("data write error ");
			hal_sendChar_UART1('\n');
		#endif
	}
}	



void write_flash_page(uint32* data, uint32 length, uint32 number){	

	flash_page_data(data, length);
	
	// Write the pagebuff data to NVM
	NVMProgram((void *)(NVM_PROGRAM_PAGE + number*NVM_PAGE_SIZE), (const void *)pagebuff, sizeof(pagebuff), (void*) pagebuff);

	// Verify data matches
	if(memcmp(pagebuff, (void *)(NVM_PROGRAM_PAGE + number*NVM_PAGE_SIZE), sizeof(pagebuff)))
	{
		// indicate an error
		#ifdef DEBUG
			hal_sendString_UART1("page write error ");
			hal_sendChar_UART1('\n');
		#endif
	}
}



uint32 read_flash_word(uint32 address){

	uint32 word;
	memcpy(&word, (void *)(address), sizeof(word)); 
	
	#ifdef DEBUG
		hal_sendString_UART1("reading word");
		hal_sendChar_UART1('\n');
	#endif
	
	return word;
}

uint32* read_flash_row(uint32 length, uint32 page, uint32 row){

	uint32 i=0;
	memcpy(databuff, (void *)(NVM_PROGRAM_PAGE+page*NVM_PAGE_SIZE+row*NVM_ROW_SIZE), sizeof(databuff)); 
	
	#ifdef DEBUG
		hal_sendString_UART1("reading row");
		hal_sendChar_UART1('\n');
		/*for(i=0;i<length ;i++){
			hal_uartWriteNumber(databuff[i]);
			hal_uartWriteNumber(i);
			hal_sendChar_UART1('\n');	
		}*/
	#endif
	
	return databuff;
}

uint32* read_flash_page(uint32 length, uint32 page){

	uint32 i=0;
	memcpy(pagebuff, (void *)(NVM_PROGRAM_PAGE+page*NVM_PAGE_SIZE), sizeof(pagebuff)); 
	
	#ifdef DEBUG
		hal_sendString_UART1("reading page");
		hal_sendChar_UART1('\n');
		/*for(i=0;i<length ;i++){
			hal_uartWriteNumber(pagebuff[i]);
			hal_uartWriteNumber(i);
			hal_sendChar_UART1('\n');	
		}*/
	#endif
	
	return pagebuff;	
}

