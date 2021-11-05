/*
 * RS232_interrupt.c
 *
 * Created: 5/11/2021 13:49:28
 * Author : blue
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

void update_portc(void);

int main(void)
{
	//	all pins on portc as output
	DDRC = 0xFF;
	//	all pins on porta as input
	DDRA = 0x00;
	//	-- USART SETUP --	//
	//	set RX(PD0) as input, TX(PD1) as output and high
	DDRD &= ~(1 << DDRD0);
	DDRD |= (1 << DDRD1);
	PORTD |= (1 << PORTD1);
	//	normal speed
	UCSR0A &= ~(1 << U2X0);
	//	set baud rate to 9600 (23 for the register)
	UBRR0 = 23;
	//	mode select: asynchronous USART
	UCSR0C &= ~((1 << UMSEL00) | (1 << UMSEL01));
	//	No parity
	UCSR0C &= ~((1 << UPM00) | (1 << UPM01));
	//	1 stop bit
	UCSR0C &= ~(1 << USBS0);
	//	8 databits
	UCSR0B &= ~(1 << UCSZ02);
	UCSR0C |=((1 << UCSZ00) | (1 << UCSZ01));
	//	we want to trigger an interrupt when the recv buffer is full
	UCSR0B |= (1 << RXCIE0);
	//	enable receive and transmit port
	UCSR0B |= ((1 << RXEN0) | (1 << TXEN0));
	//	enable GIE
	sei();
	
   
    while (1) 
    {
    }
}

void update_portc(void){
	PORTC = PINA;
}


