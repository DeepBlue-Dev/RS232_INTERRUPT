/*
 * RS232_interrupt.c
 *
 * Created: 5/11/2021 13:49:28
 * Author : blue
 */ 
#define  F_CPU 3686400L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void update_portc(void);
void sendChar(unsigned char byte);

unsigned char recv_buffer = 0;
unsigned char previous_pina_state = 0;

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
		
		if(previous_pina_state != PINA){
			PORTC ^= (PINA & 0xFF);//	extract the bits that are 1 and toggle those outputs
			sendChar(PORTC);
			while(PINA & 0xFF);	//	wait till we release the buttons, we dont want to send it again
			previous_pina_state = PINA;
			_delay_ms(5);	//	stupid bouncing
		}
		
    }
}

void update_portc(void){
	PORTC = PINA;
}

void sendChar(unsigned char byte){
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = byte;
}

ISR(USART0_RX_vect){
	recv_buffer = UDR0;
	PORTC ^= (2 << (recv_buffer)) / 2;
}


