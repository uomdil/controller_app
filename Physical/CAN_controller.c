/**********************************************************************
* © 2009 Microchip Technology Inc.
*
* FileName:        main.c
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

#include <p32xxxx.h>
#include <peripheral\system.h>

#include "CANGenericTypeDefs.h"
#include "CANFunctions.h"


/* This code example runs on a Explorer 16 board
 * with a 8Mhz crystal and the ECAN LIN Pictail
 * plus board. The CAN1 and CAN2 connectors on the
 * ECAN LIN PICtail plus board are connected to
 * each other to form a 2 node CAN bus.*/ 

/* This code example shows how to transmit and receive
 * extended ID messages using the PIC32 CAN modules and
 * PIC32 CAN Peripheral library. In this case CAN1 sends
 * a Extended ID message to CAN1 and CAN2 replies back
 * with a Extended ID message.
 *
 * A timer is setup for 0.1 millisecond. This is used
 * to obtain a 1 second timer. Every second CAN1 will
 * send a message to CAN2 to toggle LED5. CAN2 will
 * then send a message to CAN1 to toggle LED6. So 
 * both LEDs will toggle every second.*/

/* Configuration Bit settings
 * SYSCLK = 80 MHz (((8MHz Crystal/ FPLLIDIV) * FPLLMUL) / FPLLODIV)
 * PBCLK = 80 MHz
 * Primary Osc w/PLL (XT+,HS+,EC+PLL)
 * WDT OFF
 * Other options are don't care */


#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1

#define SYS_FREQ 				(80000000L)

/* Each CAN modulec uses 2 Channels (Channel 0
 * and Channel 1). Each channel is configured
 * to be 8 messages deep. Channel 0 is configured
 * for Transmit and Channel 1 is configured for
 * receive. CAN module configuration code
 * is in CANFunctions.c */ 

/* Note the size of each Channel area.
 * It is 2 (Channels) * 8 (Messages Buffers) 
 * 16 (bytes/per message buffer) bytes. Each 
 * CAN module should have its own message 
 * area. */

BYTE CAN1MessageFifoArea[2 * 8 * 16];
BYTE CAN2MessageFifoArea[2 * 8 * 16];

int main(void)
{
    /* Configure system for maximum performance
     * and enable multi vector interrupts. */
     
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableInterrupts();
    
	/* Intialize the Explorer16 LED ports
	 * CAN module and start the timer. Make
	 * all analog ports digital. */
	 
	AD1PCFGSET = 0xFFFFFFFF;

	Explorer16Init();   /* Function is defined in Explorer16.c      */
 	CAN1Init();		    /* Function is defined in CANFunctions.c 	*/
	CAN2Init();		    /* Function is defined in CANFunctions.c 	*/
	Timer1Init();	    /* Function is defined in TimerFunctions.c	*/

	while(1)
	{
		/* In this loop, we wait till we have
		 * 1 second tick. When a 1 second 
		 * tick is obtained, CAN1 will send a
		 * message to CAN2. When CAN2 receives
		 * this message it toggles LED5. It then
		 * sends a message to CAN1. When CAN1 
         * receives this message it toggles 
		 * LED6. */

		if(IsOneSecondUp() == TRUE)
		{
			/* If one second is up then CAN1 sends
			 * a message to CAN2 to toggle LED5.
			 *  */

		    CAN1TxSendLEDMsg();	/* Function is defined in CANFunctions.c	*/	
		}    
		
		/* CAN2RxMsgProcess will check if CAN2
		 * has received a message from CAN1 and
		 * will toggle LED5. It will send a message
		 * to CAN1 to toggle LED6. */

		CAN2RxMsgProcess();

		/* CAN1RxMsgProcess() will check if CAN1
		 * has received a message from CAN2 and
		 * will toggle LED6. */

		CAN1RxMsgProcess();
	}

    
}

