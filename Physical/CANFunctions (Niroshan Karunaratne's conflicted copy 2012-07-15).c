/**********************************************************************
* © 2007 Microchip Technology Inc.
*
* FileName:        CANFunctions.c
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
#include "CANFunctions.h"


/* isCAN1MsgReceived is true if CAN1 FIFO1 received
 * a message. This flag is updated in the CAN1 ISR. */
static volatile BOOL isCAN1MsgReceived = FALSE;




void CAN1Init(void)
{
    CAN_BIT_CONFIG canBitConfig;
   
	/* This function will intialize
	 * CAN1 module. */

	/* Step 1: Switch the CAN module
	 * ON and switch it to Configuration
	 * mode. Wait till the mode switch is
     * complete. */

	CANEnableModule(CAN1,TRUE);

	CANSetOperatingMode(CAN1, CAN_CONFIGURATION);
	while(CANGetOperatingMode(CAN1) != CAN_CONFIGURATION);			


	/* Step 2: Configure the Clock.The
	 * CAN_BIT_CONFIG data structure is used
	 * for this purpose. The propagation segment, 
	 * phase segment 1 and phase segment 2
	 * are configured to have 3TQ. SYSTEM_FREQ
     * and CAN_BUS_SPEED are defined in  */
	
	canBitConfig.phaseSeg2Tq            = CAN_BIT_3TQ;
	canBitConfig.phaseSeg1Tq            = CAN_BIT_3TQ;
	canBitConfig.propagationSegTq       = CAN_BIT_3TQ;
	canBitConfig.phaseSeg2TimeSelect    = TRUE;
	canBitConfig.sample3Time            = TRUE;
    canBitConfig.syncJumpWidth          = CAN_BIT_2TQ;

    CANSetSpeed(CAN1,&canBitConfig,SYSTEM_FREQ,CAN_BUS_SPEED);

	/* Step 3: Assign the buffer area to the
     * CAN module.
     */ 

	CANAssignMemoryBuffer(CAN1,CAN1MessageFifoArea,(2 * 8 * 16));	

	/* Step 4: Configure channel 0 for TX and size of
     * 8 message buffers with RTR disabled and low medium
     * priority. Configure channel 1 for RX and size
     * of 8 message buffers and receive the full message.
     */

    CANConfigureChannelForTx(CAN1, CAN_CHANNEL0, 8, CAN_TX_RTR_DISABLED, CAN_LOW_MEDIUM_PRIORITY);
    CANConfigureChannelForRx(CAN1, CAN_CHANNEL1, 8, CAN_RX_FULL_RECEIVE);
	
	/* Step 5: Configure filters and mask. Configure
     * filter 0 to accept EID messages with ID 0x8004001.
     * Configure filter mask 0 to compare all the ID
     * bits and to filter by the ID type specified in
     * the filter configuration.Messages accepted by
     * filter 0 should be stored in channel 1. */
	
    CANConfigureFilter      (CAN1, CAN_FILTER0, 0x8004001, CAN_EID);    
    CANConfigureFilterMask  (CAN1, CAN_FILTER_MASK0, 0x3FFFFFFF, CAN_EID, CAN_FILTER_MASK_IDE_TYPE);
    CANLinkFilterToChannel  (CAN1, CAN_FILTER0, CAN_FILTER_MASK0, CAN_CHANNEL1); 
    CANEnableFilter         (CAN1, CAN_FILTER0, TRUE);
	
	/* Step 6: Enable interrupt and events. Enable the receive
     * channel not empty  event (channel event) and the receive
     * channel event (module event).
     * The interrrupt peripheral library is used to enable
     * the CAN interrupt to the CPU. */

    CANEnableChannelEvent(CAN1, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);
    CANEnableModuleEvent (CAN1, CAN_RX_EVENT, TRUE);

    /* These functions are from interrupt peripheral
     * library. */
    
    INTSetVectorPriority(INT_CAN_1_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_CAN_1_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_CAN1, INT_ENABLED);

	/* Step 7: Switch the CAN mode
	 * to normal mode. */

	CANSetOperatingMode(CAN1, CAN_NORMAL_OPERATION);
	while(CANGetOperatingMode(CAN1) != CAN_NORMAL_OPERATION);	

}


void CAN1RxMsgProcess(void)
{
	/* This function will check if a CAN
	 * message is available in CAN1 channel 1.
     * If so , the message is read. Byte 0 of
     * the received message will indicate if 
	 * LED6 should be switched ON or OFF. */

	CANRxMessageBuffer * message;

	if(isCAN1MsgReceived == FALSE)
	{
		/* CAN1 did not receive any message so
		 * exit the function. Note that the
		 * isCAN1MsgReceived flag is updated 
		 * by the CAN1 ISR. */

		return;
	}
	
	/* Message was received. Reset message received flag to catch
	 * the next message and read the message. Note that
     * you could also check the CANGetRxMessage function
     * return value for null to check if a message has 
     * been received. */

	isCAN1MsgReceived = FALSE;	
	
	message = (CANRxMessageBuffer *)CANGetRxMessage(CAN1,CAN_CHANNEL1);

	/* Check byte 0 of the data payload.
	 * If it is 0 then switch off LED6 else
	 * switch it on. */

	if(message->data[0] == 0)
	{
	//	LED6 = EXPLORER16_LED_OFF;
	}
	else
	{
	//	LED6 = EXPLORER16_LED_ON;
	}

	/* Call the CANUpdateChannel() function to let
     * CAN 1 module know that the message processing
     * is done. Enable the receive channale not empty event
     * so that the CAN module generates an interrupt when
     * the event occurs the next time.*/


    CANUpdateChannel(CAN1, CAN_CHANNEL1);
	CANEnableChannelEvent(CAN1, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);

}



void CAN1TxSendLEDMsg(void)
{
	/* This function will send a message to
	 * CAN2 with EID 0x8004002. The data payload
	 * size is 1 byte. The value of the LED5Status
	 * will be toggled and then sent as
	 * the payload. CAN1 Channel 0 is used to
     * send the message.*/

	CANTxMessageBuffer * message;

	/* Toggle LED5 status	*/

	LED5Status ^= 0x1;

    /* Get a pointer to the next buffer in the channel
     * check if the returned value is null. */

	message = CANGetTxMessageBuffer(CAN1,CAN_CHANNEL0);

    if(message != NULL)
    {
        /* Form a Extended ID CAN message.
         * Start by clearing the buffer. */

        message->messageWord[0] = 0;
        message->messageWord[1] = 0;
        message->messageWord[2] = 0;
        message->messageWord[3] = 0;

        message->msgSID.SID	= 0x200;		/* Send message to CAN2.				*/
        message->msgEID.EID	= 0x4002;		/* Send message to CAN2.				*/
        message->msgEID.IDE	= 1;			/* IDE = 1 means Extended ID message.	*/
        message->msgEID.RTR	= 0;			/* Not an RTR message.              	*/
        message->msgEID.DLC	= 1;			/* Send one byte of data.				*/
        message->data[0] 	= LED5Status;	/* This is the payload.					*/

        /* This function lets the CAN module
         * know that the message processing is done
         * and message is ready to be processed. */

        CANUpdateChannel(CAN1,CAN_CHANNEL0);

        /* Direct the CAN module to flush the
         * TX channel. This will send any pending
         * message in the TX channel. */

        CANFlushTxChannel(CAN1,CAN_CHANNEL0);	
    }

}


void __attribute__((vector(46), interrupt(ipl4), nomips16)) CAN1InterruptHandler(void)
{
	/* This is the CAN1 Interrupt Handler.
	 * Note that there are many source events in the
	 * CAN1 module for this interrupt. These
	 * events are enabled by the  CANEnableModuleEvent()
     * function. In this example, only the RX_EVENT
	 * is enabled. */


	/* Check if the source of the interrupt is
	 * RX_EVENT. This is redundant since only this
     * event is enabled in this example but
     * this shows one scheme for handling events. */

	if((CANGetModuleEvent(CAN1) & CAN_RX_EVENT) != 0)
	{
		
		/* Within this, you can check which channel caused the 
		 * event by using the CANGetModuleEvent() function
         * which returns a code representing the highest priority
         * pending event. */ 
		
		if(CANGetPendingEventCode(CAN1) == CAN_CHANNEL1_EVENT)
		{
			/* This means that channel 1 caused the event.
			 * The CAN_RX_CHANNEL_NOT_EMPTY event is persistent. You
			 * could either read the channel in the ISR
			 * to clear the event condition or as done 
			 * here, disable the event source, and set
			 * an application flag to indicate that a message
			 * has been received. The event can be
			 * enabled by the application when it has processed
			 * one message.
			 *
			 * Note that leaving the event enabled would
			 * cause the CPU to keep executing the ISR since
			 * the CAN_RX_CHANNEL_NOT_EMPTY event is persistent (unless
			 * the not empty condition is cleared.) 
			 * */
			
            CANEnableChannelEvent(CAN1, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
			isCAN1MsgReceived = TRUE;	
		}
	}

	/* The CAN1 Interrupt flag is  cleared at the end of the 
	* interrupt routine. This is because the interrupt source
    * that could have caused this interrupt  to occur 
    * (CAN_RX_CHANNEL_NOT_EMPTY) is disabled. Attempting to 
	* clear the CAN1 interrupt flag when the the CAN_RX_CHANNEL_NOT_EMPTY
    * interrupt is enabled will not have any effect because the 
	* base event is still present. */ 
	
	INTClearFlag(INT_CAN1);

}
