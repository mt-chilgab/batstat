#!/bin/bash

MCU="atmega128a"
AVRCC="avr-gcc"
AVROBJCOPY="avr-objcopy"
AVRDUDE="/mnt/c/users/grant/desktop/avrdude-v7.0-windows-windows-x64/avrdude.exe"
PORT="COM4"

if [ $(echo $1 | grep "c") ]; then		
	LIST=$(ls | grep "adc\.[hex|o]")
	for i in $LIST;
	do
		[[ -f "./$i" ]] && rm "./$i"
	done
fi

if [ $(echo $1 | grep "h") ]; then
	$AVRCC -O3 -mmcu=$MCU -o adc.o adc.c
	$AVROBJCOPY -j .text -j .data -O ihex adc.o adc.hex
fi

if [ $(echo $1 | grep "u") ]; then
	taskkill.exe /F /IM realterm.exe
	$AVRDUDE -b 115200 -p m128 -c STK500 -P $PORT -U flash:w:adc.hex:i
	"/mnt/c/Program Files (x86)/BEL/Realterm/realterm.exe"
fi


