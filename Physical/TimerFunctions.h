/**********************************************************************
* © 2007 Microchip Technology Inc.
*
* FileName:        TimerFunctions.h
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

#ifndef _TIMER_FUNCTIONS_H_
#define _TIMER_FUNCTIONS_H_

#include "CANGenericTypeDefs.h"
#include <p32xxxx.h>

#define TIMER1_PERIOD 	0x1F40	/* For a 0.1 millisecond interval	*/
#define ONE_SECOND_MSEC	0x2710	/* For a 1 second timeout in terms 	*/
								/* of 0.1 milliseconds	*/	

/****************************************************************************
  Function:
    void IsOneSecondUp(void);

  Description:
    This function will return true if one second has expired
	since the last time the function returned true.

  Example:
  	if(IsOneSecondUp() == TRUE)
	{
		foo();
	}
  ***************************************************************************/

BOOL IsOneSecondUp(void);

/****************************************************************************
  Function:
    void Timer1Init(void);

  Description:
    This function will intialize and start Timer 1 for period
	specified by the TIMER1_PERIOD macro. It will enable
	timer interrupt and will intialize the internal one
	second counter. 

  ***************************************************************************/

void Timer1Init(void);


#endif

