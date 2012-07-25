#include <p32xxxx.h>
#include "Compiler.h"
#include "GenericTypeDefs.h"
#include "TimeDelay.h"
#include "skpic32_glcd.h"


void Disp_Init(){


PORTClearBits(IOPORT_C , BIT_3|BIT_2|BIT_1);
PORTClearBits(IOPORT_E , BIT_2|BIT_3|BIT_4|BIT_5|BIT_6|BIT_7);
PORTClearBits(IOPORT_G , BIT_12|BIT_13|BIT_14|BIT_15);
PORTSetPinsDigitalOut(IOPORT_C , BIT_3|BIT_2|BIT_1);
PORTSetPinsDigitalOut(IOPORT_E , BIT_2|BIT_3|BIT_4|BIT_5|BIT_6|BIT_7);
PORTSetPinsDigitalOut(IOPORT_G , BIT_12|BIT_13|BIT_14|BIT_15);
mPORTCOpenDrainOpen(BIT_3|BIT_2|BIT_1);
mPORTEOpenDrainOpen(BIT_2|BIT_3|BIT_4|BIT_5|BIT_6|BIT_7);
mPORTGOpenDrainOpen(BIT_12|BIT_13|BIT_14|BIT_15);

PORTCbits.RC2=0;	//WR
PORTGbits.RG12=1; 	//PSB 

}


/*******************************************************************************
* PUBLIC FUNCTION: Disp_GLCDInit
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Initialize and clear the LCD display.
* Blinking cursor is turned off.
* 
*******************************************************************************/
void Disp_GLCDInit()
{
	RST = 0;						// Resets the screen.
	DelayMs(1);
	RST = 1;
	DelayMs(50);					// Short delay after resetting.

	Disp_GLCDCommand(0b00100000);// 4-bit mode.
    DelayMs(1);
	Disp_GLCDCommand(0b00100000);// 4-bit mode again.
    DelayMs(1);
    Disp_GLCDCommand(0b00001100);// Cursor and blinking cursor disabled.
    DelayMs(1);;
    Disp_GLCDCommand(0b00000001);// Clears screen.
    DelayMs(10);
    Disp_GLCDCommand(0b00000110);// Cursor moves right, no display shift. 
    Disp_GLCDCommand(0b00000010);// Returns to home. Cursor moves to starting point.
    
    //Start screen
    Disp_GLCDWriteText(2, 0,"WELCOME");

}

/*******************************************************************************
* PUBLIC FUNCTION: Disp_GLCDCommand
*
* PARAMETERS:
* ~ command byte
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Send command byte to the GLCD controller.
* Please refer to the datasheet for the list of commands.
*******************************************************************************/
void Disp_GLCDCommand(unsigned char command)
{
	RS = 0;
    //Delay10us(2);

    E = 1;
	Delay10us(2);
	// Move higher nibble first
	DB4 = (command & (1 << 4)) >> 4;
    DB5 = (command & (1 << 5)) >> 5;
    DB6 = (command & (1 << 6)) >> 6;
    DB7 = (command & (1 << 7)) >> 7;
	Delay10us(2);
	E = 0;

	E = 1;
	Delay10us(2); 
	// Then move the lower nibble
    DB4 = (command & (1 << 0)) >> 0;
    DB5 = (command & (1 << 1)) >> 1;
    DB6 = (command & (1 << 2)) >> 2;
    DB7 = (command & (1 << 3)) >> 3;
	Delay10us(2); 
    E = 0;

	DelayMs(1);					// Please increase this delay if the screen is not showing things correctly.
	
}

/*******************************************************************************
* PUBLIC FUNCTION: Disp_GLCDData
*
* PARAMETERS:
* ~ command byte
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Send data byte to the GLCD controller.
* 
*******************************************************************************/
void Disp_GLCDData(unsigned char data)
{
	RS = 1;
    //Delay10us(2);

    // Move higher nibble first
    E = 1;
	Delay10us(2);
	DB4 = (data & (1 << 4)) >> 4;
    DB5 = (data & (1 << 5)) >> 5;
    DB6 = (data & (1 << 6)) >> 6;
    DB7 = (data & (1 << 7)) >> 7;
	Delay10us(2);
	E = 0;

    // Then move the lower nibble
    E = 1;
	Delay10us(2);
	DB4 = (data & (1 << 0)) >> 0;
    DB5 = (data & (1 << 1)) >> 1;
    DB6 = (data & (1 << 2)) >> 2;
    DB7 = (data & (1 << 3)) >> 3;
	Delay10us(2);  
	E = 0;

	DelayMs(1);					// Please increase this delay if the screen is not showing things correctly.
}

/*******************************************************************************
* PUBLIC FUNCTION: Disp_GLCDWrite
*
* PARAMETERS:
* ~ character byte
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Place a character to the GLCD controller on the specified row and column.
* Due to the design of the ST7920 controller (to accomodate Mandarin and Cyrillic), you must place the text on the column
* according to the numbers above the diagram below:
* 
* |--0--|--1--|--2--|...	  ...|--7--|
* +--+--+--+--+--+---------------------+
* |H |e |l |l |o |  ...                | <- row 0 (address 0x80)
* +--+--+--+--+--+---------------------+
* |T |h |i |s |  |i ...				   | <- row 1 (address 0x90)
* +--+--+--+--+--+---------------------+
* |* |* |* |* |* |* ...				   | <- row 2 (address 0x88)
* +--+--+--+--+--+---------------------+
* |- |- |- |- |- |- ...				   | <- row 3 (address 0x98)
* +--+--+--+--+--+---------------------+ 
*
* Example:
* Writing 'a' onto the 1st column, and 1st row:
* |--0--|--1--|--2--|...	  ...|--7--|
* +--+--+--+--+--+---------------------+
* |  |  |  |  |  |  ...                | <- row 0 (address 0x80)
* +--+--+--+--+--+---------------------+
* |  |  |a |  |  |  ...				   | <- row 1 (address 0x90)
* +--+--+--+--+--+---------------------+
* |  |  |  |  |  |  ...				   | <- row 2 (address 0x88)
* +--+--+--+--+--+---------------------+
* |  |  |  |  |  |  ...				   | <- row 3 (address 0x98)
* +--+--+--+--+--+---------------------+ 
* 
*
*******************************************************************************/
void Disp_GLCDWrite(unsigned char col, unsigned char row, unsigned char inpChr)
{
	switch (row) {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0x90;
            break;
        case 2:
            col |= 0x88;
            break;
        case 3:
            col |= 0x98;
            break;
        default:
            col |= 0x80;
            break;
    }

    Disp_GLCDCommand(col);
	Disp_GLCDData(inpChr);
}	

/*******************************************************************************
* PUBLIC FUNCTION: Disp_GLCDWriteText
*
* PARAMETERS:
* ~ row and column, string
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Place a string to the GLCD controller on the specified row and column.
* Due to the design of the ST7920 controller (to accomodate Mandarin and Cyrillic), you must place the text on the column
* according to the numbers above the diagram below:
*
* |--0--|--1--|--2--|...	  ...|--7--|
* +--+--+--+--+--+---------------------+
* |H |e |l |l |o |  ...                | <- row 0 (address 0x80)
* +--+--+--+--+--+---------------------+
* |T |h |i |s |  |i ...				   | <- row 1 (address 0x90)
* +--+--+--+--+--+---------------------+
* |* |* |* |* |* |* ...				   | <- row 2 (address 0x88)
* +--+--+--+--+--+---------------------+
* |- |- |- |- |- |- ...				   | <- row 3 (address 0x98)
* +--+--+--+--+--+---------------------+ 
*
* Example:
* Writing "abc" onto the 1st column, and 1st row:
* |--0--|--1--|--2--|...	  ...|--7--|
* +--+--+--+--+--+---------------------+
* |  |  |  |  |  |  ...                | <- row 0 (address 0x80)
* +--+--+--+--+--+---------------------+
* |  |  |a |b |c |  ...				   | <- row 1 (address 0x90)
* +--+--+--+--+--+---------------------+
* |  |  |  |  |  |  ...				   | <- row 2 (address 0x88)
* +--+--+--+--+--+---------------------+
* |  |  |  |  |  |  ...				   | <- row 3 (address 0x98)
* +--+--+--+--+--+---------------------+ 
* 
* String printing method courtesy of Lucio di Jasio, from the book "Programming 32-bit Microcontrollers in C - Exploring the PIC32"
*
*******************************************************************************/
void Disp_GLCDWriteText(unsigned char col, unsigned char row, char* string)
{
    switch (row) {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0x90;
            break;
        case 2:
            col |= 0x88;
            break;
        case 3:
            col |= 0x98;
            break;
        default:
            col |= 0x80;
            break;
    }

    Disp_GLCDCommand(col);

    while (*string)
        Disp_GLCDData(*string++);
}


void Disp_GLCDClearDisp(){

Disp_GLCDCommand(0x01);

}



/*******************************************************************************
* PUBLIC FUNCTION: Disp_GLCDEnableGraphics
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Enables Graphics Mode on the GLCD.
* Make sure the screen is void of text before using this function.
*******************************************************************************/
void Disp_GLCDEnableGraphics()
{
	Disp_GLCDCommand(0x20);		
	DelayMs(1);
	Disp_GLCDCommand(0x24);			// Switch to extended instruction mode.	
	DelayMs(1);
	Disp_GLCDCommand(0x26);			// Enable graphics mode.
	DelayMs(1);
}

/*******************************************************************************
* PUBLIC FUNCTION: Disp_GLCDDisableGraphics
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Disable Graphics Mode on the GLCD and return to text mode.
*******************************************************************************/
void Disp_GLCDDisableGraphics()
{
	Disp_GLCDCommand(0x20);			// Graphics and extended instruction mode turned off.
	DelayMs(1);
}

/*******************************************************************************
* PUBLIC FUNCTION: Disp_GLCDFillScreenGraphic
*
* PARAMETERS:
* ~ a constant pointer to a graphics array (1024 bytes total)
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Fills the screen with the graphics described in a 1024-byte array
* 
*******************************************************************************/
void Disp_GLCDFillScreenGraphic(const unsigned char* graphic)
{
	unsigned char x, y;
	for(y = 0; y < 64; y++)
	{
		if(y < 32)
		{
			for(x = 0; x < 8; x++)							// Draws top half of the screen.
			{												// In extended instruction mode, vertical and horizontal coordinates must be specified before sending data in.
				Disp_GLCDCommand(0x80 | y);				// Vertical coordinate of the screen is specified first. (0-31)
    			Disp_GLCDCommand(0x80 | x);				// Then horizontal coordinate of the screen is specified. (0-8)
				Disp_GLCDData(graphic[2*x + 16*y]);		// Data to the upper byte is sent to the coordinate.
				Disp_GLCDData(graphic[2*x+1 + 16*y]);	// Data to the lower byte is sent to the coordinate.
			}
		}
		else
		{
			for(x = 0; x < 8; x++)							// Draws bottom half of the screen.
			{												// Actions performed as same as the upper half screen.
				Disp_GLCDCommand(0x80 | (y-32));			// Vertical coordinate must be scaled back to 0-31 as it is dealing with another half of the screen.
    			Disp_GLCDCommand(0x88 | x);
				Disp_GLCDData(graphic[2*x + 16*y]);
				Disp_GLCDData(graphic[2*x+1 + 16*y]);
			}
		}
		
	}
}

/*******************************************************************************
* PUBLIC FUNCTION: Disp_GLCDClearGraphics
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Clear the screen by filling the screen with zero bytes.
* Enable Graphics Mode first by calling this function Disp_GLCDClearGraphics()
* before calling this function.
*******************************************************************************/
void Disp_GLCDClearGraphics()
{
	// This function performs similar to the Disp_GLCDFillScreenGraphic but
	// only zeros are sent into the screen instead of data from an array.
	unsigned char x, y;
	for(y = 0; y < 64; y++)
	{
		if(y < 32)
		{
			Disp_GLCDCommand(0x80 | y);
    		Disp_GLCDCommand(0x80);
		}
		else
		{
			Disp_GLCDCommand(0x80 | (y-32));
    		Disp_GLCDCommand(0x88);
		}
		for(x = 0; x < 8; x++)
		{
			Disp_GLCDData(0x00);
			Disp_GLCDData(0x00);
		}
	}
}






