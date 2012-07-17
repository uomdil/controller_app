/**********************************************************************
* © 2009 Microchip Technology Inc.
*
* FileName:        TimerFunctions.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       PIC32
* Compiler:        MPLAB® C32 
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all ownership and 
* intellectual property rights in the code accompanying this message and in all 
* derivatives hereto.  You may use this code, and any derivatives created by 
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes 
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A 
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S 
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER 
* IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), 
* STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, 
* PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF 
* ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN 
* ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
* THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO 
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify, test, 
* certify, or support the code.
************************************************************************/

#include "TimerFunctions.h"

static BOOL oneSecondUp;
static UINT32 msecCounter;


void Timer1Init(void)
{
	/* This function will intialize the Timer 1
	 * for basic timer operation. It will enable
	 * timer interrupt. The one second flag is
	 * initialized and the millisecond counter is 
	 * initialized. */

	T1CON = 0x0;			/* Basic Timer Operation				*/
	PR1 = TIMER1_PERIOD;	/* Timer period in TimerFunctions.h 	*/

	IFS0CLR = 0x10;			/* Clear interrupt flag and enable		*/
	IEC0SET = 0x10;			/* timer 1 interrupt. 					*/
	IPC1bits.T1IP = 4;		/* Timer 1 priority is 4				*/

	oneSecondUp = FALSE;	/* Intialize the one second flag		*/
	msecCounter = 0;		/* and the millisecond counter.			*/

	T1CONSET = 0x8000;		/* Start the timer.						*/

}

BOOL IsOneSecondUp(void)
{
	/* This function will return TRUE if
	 * a second has expired since the last
	 * time the function had returned
	 * TRUE. If not then function returns
	 * FALSE. */

	BOOL result;

	if (oneSecondUp == TRUE)
	{
		/* If a second has expired
		 * then return true and reset 
		 * the one second flag. */

		result = TRUE;	
		oneSecondUp = FALSE;
	}
	else
	{
		result = FALSE;
	}
	
	return(result);
}

void __attribute__((vector(4), interrupt(ipl4), nomips16)) Timer1InterruptHandler(void)
{
	/* This is the Timer 1 ISR */
	
	IFS0CLR = 0x10; 	/* Clear the Interrupt Flag	*/
	
	msecCounter ++;		/* Increment illisecond counter.	*/ 
	
	if(msecCounter == ONE_SECOND_MSEC)
	{
		/* This means that one second
		 * has expired since the last time
		 * msecCounter was 0. */

		oneSecondUp = TRUE;	/* Indicate that one second is up	*/
		msecCounter = 0;	/* Reset the millisecond counter.	*/
	}


}
