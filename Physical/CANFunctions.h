/**********************************************************************
* © 2007 Microchip Technology Inc.
*
* FileName:        CANFunctions.h	 
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

#ifndef _CAN_FUNCTIONS_H_
#define _CAN_FUNCTIONS_H_

#include "CANGenericTypeDefs.h"
#include <p32xxxx.h>
#include <sys/kmem.h>
#include "plib.h"

#define SYSTEM_FREQ 80000000
#define CAN_BUS_SPEED 250000

#define CAN1_BRPVAL	0x7			/* CAN speed select - 0x7 for 500kbps, 0xF for 250Kbps, 0x3 for 1Mbps 		*/

extern BYTE CAN1MessageFifoArea[2 * 8 * 16];	/* This is the CAN1 FIFO message area.	*/

/****************************************************************************
  Function:
    void CAN1Init(void);

  Description:
	This function initializes CAN1 module. It sets up speed, FIFOs,
	filters and interrupts. FIFO0 is set up for TX with 8 message 
	buffers. FIFO1 is set up for RX with 8 message buffers. Filter 0
	is set with Mask 0 for EID 0x8004001. Only RXNEMPTY interrupt and
	RBIF interrupt is enabled.	

  ***************************************************************************/
void CAN1Init(void);

/****************************************************************************
  Function:
    void CAN1RxMsgProcess(void);

  Description:
	This function checks if a message is available to be read in
	CAN1 FIFO1. If a message is available, the function will check
	byte 0 (boolean flag) of the CAN message payload and will switch 
	LED6 accordingly.

  ***************************************************************************/
void CAN1RxMsgProcess(void);

/****************************************************************************
  Function:
    void CAN1TxSendLEDMsg(void);

  Description:
	This function will send a CAN message with EID 0x8004002. In the payload,
	it sends one byte which is a toggle state for LED5. FIFO0 is used for
	sending the message.

  ***************************************************************************/
void CAN1TxSendLEDMsg(void);

#endif
