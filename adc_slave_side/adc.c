#ifndef F_CPU 
	#define F_CPU 16000000
#endif

#define FOSC 16000000
#define BAUD 57600

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint32_t myPow(size_t i){

	if(i == 0) return 1;
	else return 10*myPow(i-1);

}

typedef struct{
	uint8_t *buf;
	size_t front;
	size_t rear;
	size_t cnt;
} uartBuf;

void initBuf(uartBuf *buf, size_t cnt){
	if(cnt < 1) return;
	
	buf->front = 0;
	buf->rear = -1;
	buf->cnt = cnt;
	buf->buf = (uint8_t *)malloc(sizeof(uint8_t)*buf->cnt);
}

size_t sizeBuf(uartBuf *buf){
	if((buf->front == 0) && (buf->rear == -1)) return 0;
	
	else{
		return (buf->rear >= buf->front)?(buf->rear-buf->front+1):(buf->cnt-buf->front+buf->rear+1);
	}
}

void enq(uartBuf *buf, uint8_t data){
	if(sizeBuf(buf) >= buf->cnt) return;
	
	else {
		buf->rear = (buf->rear + 1) % buf->cnt;
		buf->buf[buf->rear] = data;
	}
}

void enqStr(uartBuf *buf, const uint8_t* str){
	size_t i;
	while(*(str+i*sizeof(uint8_t)) != '\0'){
		enq(buf, *(str+i*sizeof(uint8_t)));
		i++;
	}
}

uint8_t deq(uartBuf *buf){
	if(sizeBuf(buf) < 1) return -1;
	
	else{
		uint8_t res = buf->buf[buf->front];

		if (sizeBuf(buf) == 1){
			buf->front = 0;
			buf->rear = -1;
		}

		else{
			buf->front = (buf->front + 1) % buf->cnt;
		}
		
		return res;
	}
}

void flushBuf(uartBuf *buf){
	while(deq(buf) != -1);
}

void freeBuf(uartBuf *buf){
	free(buf->buf);
}

void double2buf(const double val, const size_t ipl, const size_t fpl, uartBuf *buf){
	
	uint32_t ipart = (uint32_t) val;  
	double   fpart = val - (double) ipart;

	size_t i;
	for(i=ipl; i>0; i--){
		enq(buf, (uint8_t)((ipart % myPow(i)) / myPow(i-1)) + 48);
	}

	enq(buf, 46);
	
	fpart = fpart * myPow(fpl);
	ipart = ((uint32_t)(fpart * 10) % 10 >= 5) ? ((uint32_t)fpart + 1):((uint32_t)fpart);
	for(i=fpl; i>0; i--){
		enq(buf, (uint8_t)((ipart % myPow(i)) / myPow(i-1)) + 48);
	}

}

void int2buf(const uint16_t val, uartBuf *buf){
	
	size_t i, j;
	int16_t res = -1;

	j = 0;
	while (res != val){
		j++;
		res = val % myPow(j);
	}

	for(i=j; i>0; i--){
		enq(buf, (uint8_t)((val % myPow(i)) / myPow(i-1)) + 48);
	}
}

void adc2buf(const uint16_t adcValue, const size_t ipl, const size_t fpl, uartBuf *buf){

	double val = adcValue * 4.921 / 1024;

	enqStr(buf, "(");
	int2buf(adcValue, buf);
       	enqStr(buf, ") ");	

	enqStr(buf, "Out: ");	
	double2buf(val, 1, fpl, buf);
	enqStr(buf, "V\t");

	enqStr(buf, "@ ");
	if( (val + 0.3*12)/0.2 < 18.1 ) enqStr(buf, "Off\n\n");
	else{
		double2buf((val + 0.3*12)/0.2, ipl, 2, buf); 
		enqStr(buf, "V\n\n");
	}
}

static volatile uartBuf *buffer;
static volatile uint16_t adcHigh;
static volatile uint16_t adcLow;

void main(void){

	// Initialize USART0
	uint8_t ubrr = FOSC/16/BAUD-1;
	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)ubrr;

	UCSR0B = (1 << TXEN0);
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);

	buffer = (uartBuf *)malloc(sizeof(uartBuf));	
	initBuf(buffer, 40);

	// Initialize ADC
	ADCSRA = (1 << ADPS1) | (1 << ADPS0);

	ADMUX  =  (1 << REFS0);
	ADMUX  &=  ~(1 << ADLAR);

	ADCSRA |= (1 << ADFR);
	ADCSRA |= (1 << ADEN);
	ADCSRA |= (1 << ADIE);
	sei();

	ADCSRA |= (1 << ADSC);

	while(1);

	freeBuf(buffer);
	free(buffer);
	
}

ISR(ADC_vect){
	adcLow = (uint16_t) ADCL;
	adcHigh = (uint16_t) ADCH << 8;

	adc2buf(adcLow + adcHigh, 2, 3, buffer);
	while(sizeBuf(buffer) != 0){
		while ( !(UCSR0A & (1 << UDRE0)) );
		UDR0 = deq(buffer);	
	}
}
