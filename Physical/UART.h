#ifndef MDBUART
#define MDBUART



#define UART_INIT(PORT,BAUD,NO_BITS)  													     	 \
		UARTConfigure(PORT, UART_ENABLE_PINS_TX_RX_ONLY); 									     \
		UARTSetFifoMode(PORT, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);   \
		UARTSetLineControl(PORT, UART_DATA_SIZE_##NO_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);\
		UARTSetDataRate(PORT, GetPeripheralClock(), BAUD);									     \
		UARTEnable(PORT, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));			     \
		INTEnable(INT_SOURCE_UART_RX(PORT), INT_ENABLED);                                        \
		INTSetVectorPriority(INT_VECTOR_UART(PORT), INT_PRIORITY_LEVEL_2);                       \
		INTSetVectorSubPriority(INT_VECTOR_UART(PORT), INT_SUB_PRIORITY_LEVEL_0);                \


#define UART_INT(PORT,PRIORITY)  void __ISR( PORT##_VECTOR , PRIORITY) PORT##IntHandler(void)

#define UART_SEND_CHAR_PROTO(PORT) void hal_sendChar_##PORT (const char character)

#define UART_SEND_CHAR(PORT) 								\
		void hal_sendChar_##PORT (const char character) 	\
		{ 													\
			while(!UARTTransmitterIsReady(PORT))			\
            ;												\
        	UARTSendDataByte(PORT, character);				\
        	while(!UARTTransmissionHasCompleted(PORT))		\
            ;												\
		}													\

#define UART_SEND_STRING_PROTO(PORT) void hal_sendString_##PORT (const char *string)

#define UART_SEND_STRING(PORT) 									\
		void hal_sendString_##PORT (const char *string)			\
		{														\
   			 while(*string != '\0')								\
    		{													\
        		while(!UARTTransmitterIsReady(PORT))			\
            	;												\
        		UARTSendDataByte(PORT, *string);				\
        		string++;										\
        		while(!UARTTransmissionHasCompleted(PORT))		\
            	;												\
    		}													\
		}														\
		
#define UART_INT_TEST(PORT)							\
	if(INTGetFlag(INT_SOURCE_UART_RX(PORT)))		\
	{												\
	    INTClearFlag(INT_SOURCE_UART_RX(PORT));	    \
	    if (UARTReceivedDataIsAvailable(PORT))      \
		{                                           \
			hal_sendChar_##PORT (UARTGetDataByte(PORT)); \
		}											\
	}												\
	if ( INTGetFlag(INT_SOURCE_UART_TX(PORT)) )  	\
	{												\
		INTClearFlag(INT_SOURCE_UART_TX(PORT));	    \
	}												\
	

#define MDB_SEND_9_DATA(PORT)							\
	void hal_uartSend9bitData_##PORT(unsigned int no){	\
		UART_DATA data = {.__data = no};				\
      	while(!UARTTransmitterIsReady(PORT))			\
        ;												\
      	UARTSendData(PORT,data);						\
      	while(!UARTTransmissionHasCompleted(PORT))		\
        ;												\
	}													\
	
#define MDB_SEND_9_DATA_PROTO(PORT)  void hal_uartSend9bitData_##PORT (unsigned int no)
	

	/*
	//	    while(!DataRdyUART2());						\
	//		INTEnable(INT_SOURCE_UART_RX(PORT), INT_DISABLED); \
	//		INTEnable(INT_SOURCE_UART_RX(PORT), INT_ENABLED);  \
	
	 // Added this today // 
         while(!DataRdyUART2()); // Wait for data in the UARTRx. 
         data = (char)ReadUART2(); // Read data from Rx. 
  
         // Echo what we just received 
         PutCharacter(data); */


//prototypes

void hal_allUARTInit(void);
UART_SEND_CHAR_PROTO(UART1);
UART_SEND_STRING_PROTO(UART1);


void hal_uartWriteNumber(unsigned int no);



#endif
