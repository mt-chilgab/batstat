#ifndef F_CPU 
	#define F_CPU 16000000
#endif

#define FOSC 16000000
#define BAUD 57600

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

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

void adc2char(const uint16_t adcValue, const size_t num, uartBuf *buf, uint8_t *str){

	double val = (((double)adcValue * 5.0) / 1023 ) * pow(10, num);
	if ( (uint32_t)(val * 10) % 10 >= 5 ) val += 1;

	enqStr(buf, str);
	enqStr(buf, ": ");	
	enq(buf, (uint8_t)((uint32_t)(val / pow(10, num)) % 10 + 48));
	enq(buf, 46);
	
	size_t i;
	for(i=1; i<num+1; i++){
		enq(buf, (uint8_t)((uint32_t)(val / pow(10, num-i)) % 10 + 48));
	}
	
	if( str == "Out1" ) enqStr(buf, "V\t");
	else enqStr(buf, "V\n\n");
}

static volatile uartBuf *buffer;
static volatile uint16_t adcHigh;
static volatile uint16_t adcLow;
static volatile uint8_t *str;

void main(void){

	// Initialize USART0
	uint8_t ubrr = FOSC/16/BAUD-1;
	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)ubrr;

	UCSR0B = (1 << TXEN0);
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);

	buffer = (uartBuf *)malloc(sizeof(uartBuf));	
	initBuf(buffer, 20);

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

	if(ADMUX & (1 << MUX0)){
		str = "Out1";
	}
	else{
		str = "Out2";
	}

	adc2char(adcLow + adcHigh, 3, buffer, str);
	while(sizeBuf(buffer) != 0){
		while ( !(UCSR0A & (1 << UDRE0)) );
		UDR0 = deq(buffer);	
	}

	ADMUX ^= (1 << MUX0);
	
	_delay_ms(200);
}
