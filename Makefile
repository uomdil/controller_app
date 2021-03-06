# MPLAB IDE generated this makefile for use with GNU make.
# Project: controller.mcp
# Date: Fri Aug 03 15:18:12 2012

AS = pic32-as.exe
CC = pic32-gcc.exe
LD = pic32-ld.exe
AR = pic32-ar.exe
HX = pic32-bin2hex.exe
RM = rm

controller.hex : controller.elf
	$(HX) "controller.elf"

controller.elf : GSM.o main.o NFC.o keys.o UART.o Flash_Controller.o LCD.o skpic32_glcd.o TimeDelay.o ProductDB.o
	$(CC) -mprocessor=32MX795F512L "GSM.o" "main.o" "NFC.o" "keys.o" "UART.o" "Flash_Controller.o" "LCD.o" "skpic32_glcd.o" "TimeDelay.o" "ProductDB.o" -o"controller.elf" -Wl,-L"C:\Program Files (x86)\Microchip\MPLAB C32 Suite\lib",-L"C:\Program Files (x86)\Microchip\MPLAB C32 Suite\pic32mx\lib",--defsym=__MPLAB_BUILD=1,-Map="controller.map"

GSM.o : Logic/GSM.c include/global.h
	$(CC) -mprocessor=32MX795F512L -x c -c "Logic\GSM.c" -o"GSM.o" -MMD -MF"GSM.d" -I"Common" -I"physical" -I"include" -g

main.o : Logic/main.c include/global.h physical/keys.h include/global.h physical/Flash_Controller.h include/skpic32_glcd.h include/ProductDB.h include/main.h include/global.h
	$(CC) -mprocessor=32MX795F512L -x c -c "Logic\main.c" -o"main.o" -MMD -MF"main.d" -I"Common" -I"physical" -I"include" -g

NFC.o : Logic/NFC.c include/global.h
	$(CC) -mprocessor=32MX795F512L -x c -c "Logic\NFC.c" -o"NFC.o" -MMD -MF"NFC.d" -I"Common" -I"physical" -I"include" -g

keys.o : Physical/keys.c include/global.h Physical/keys.h include/global.h
	$(CC) -mprocessor=32MX795F512L -x c -c "Physical\keys.c" -o"keys.o" -MMD -MF"keys.d" -I"Common" -I"physical" -I"include" -g

UART.o : Physical/UART.c include/global.h Physical/UART.h Physical/LCD.h include/global.h include/ProductDB.h include/main.h include/global.h
	$(CC) -mprocessor=32MX795F512L -x c -c "Physical\UART.c" -o"UART.o" -MMD -MF"UART.d" -I"Common" -I"physical" -I"include" -g

Flash_Controller.o : Physical/Flash_Controller.c Physical/Flash_Controller.h include/global.h include/global.h
	$(CC) -mprocessor=32MX795F512L -x c -c "Physical\Flash_Controller.c" -o"Flash_Controller.o" -MMD -MF"Flash_Controller.d" -I"Common" -I"physical" -I"include" -g

LCD.o : Physical/LCD.c Physical/LCD.h include/global.h include/global.h
	$(CC) -mprocessor=32MX795F512L -x c -c "Physical\LCD.c" -o"LCD.o" -MMD -MF"LCD.d" -I"Common" -I"physical" -I"include" -g

skpic32_glcd.o : Common/skpic32_glcd.c include/Compiler.h include/TimeDelay.h include/GenericTypeDefs.h include/skpic32_glcd.h Common/graphics.h
	$(CC) -mprocessor=32MX795F512L -x c -c "Common\skpic32_glcd.c" -o"skpic32_glcd.o" -MMD -MF"skpic32_glcd.d" -I"Common" -I"physical" -I"include" -g

TimeDelay.o : Common/TimeDelay.c include/Compiler.h include/global.h include/TimeDelay.h include/GenericTypeDefs.h
	$(CC) -mprocessor=32MX795F512L -x c -c "Common\TimeDelay.c" -o"TimeDelay.o" -MMD -MF"TimeDelay.d" -I"Common" -I"physical" -I"include" -g

ProductDB.o : Logic/ProductDB.c include/global.h include/ProductDB.h include/main.h include/global.h physical/Flash_Controller.h include/global.h
	$(CC) -mprocessor=32MX795F512L -x c -c "Logic\ProductDB.c" -o"ProductDB.o" -MMD -MF"ProductDB.d" -I"Common" -I"physical" -I"include" -g

clean : 
	$(RM) "GSM.o" "main.o" "NFC.o" "keys.o" "UART.o" "Flash_Controller.o" "LCD.o" "skpic32_glcd.o" "TimeDelay.o" "ProductDB.o" "controller.elf" "controller.hex"

