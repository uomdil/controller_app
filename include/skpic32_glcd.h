// Simple GLCD Library for Cytron Disp.
// GLCD is using ST7920 controller.

/*#define DB4 LATEbits.LATE0
#define DB5 LATEbits.LATE1
#define DB6 LATEbits.LATE2
#define DB7 LATEbits.LATE3
#define E	LATEbits.LATE4
#define RS  LATEbits.LATE5
#define RST LATEbits.LATE7
*/

#define DB4		LATEbits.LATE4
#define DB5		LATEbits.LATE3
#define DB6		LATEbits.LATE2
#define DB7		LATGbits.LATG13
#define RS 		LATCbits.LATC3
#define RST		LATGbits.LATG14
#define E  		LATCbits.LATC1


void Disp_Init();
void Disp_GLCDClearDisp();
void Disp_GLCDData(unsigned char data);
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
void Disp_GLCDInit();

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
void Disp_GLCDCommand(unsigned char);

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
void Disp_GLCDData(unsigned char);

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
* Send a character to the GLCD controller on the specified row and column.
* 
*******************************************************************************/
void Disp_GLCDWrite(unsigned char, unsigned char, unsigned char);

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
* Place a string in the specified row and column of the screen.
* +--+--+--+--+--+---------------------+
* |0 |1 |2 |3 |4 |5 ...              15| <- row 0 (address 0x80)
* +--+--+--+--+--+---------------------+
* |0 |1 |2 |3 |4 |5 ...				 15| <- row 1 (address 0x90)
* +--+--+--+--+--+---------------------+
* |0 |1 |2 |3 |4 |5 ...				 15| <- row 2 (address 0x88)
* +--+--+--+--+--+---------------------+
* |0 |1 |2 |3 |4 |5 ...				 15| <- row 3 (address 0x98)
* +--+--+--+--+--+---------------------+ 
* 
* Up to 16 characters can be printed on each row by using Latin alphabets.
* String printing method courtesy of Lucio di Jasio, from the book "Programming 32-bit Microcontrollers in C - Exploring the PIC32"
*
*******************************************************************************/
void Disp_GLCDWriteText(unsigned char, unsigned char, char*); 

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
void Disp_GLCDEnableGraphics();


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
void Disp_GLCDDisableGraphics();

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
void Disp_GLCDFillScreenGraphic(const unsigned char*);

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
void Disp_GLCDClearGraphics();
