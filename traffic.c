/*
 * traffic light.c
 *
 * Created: 2018-02-05 6:32:30 PM
 * Author : Abdul
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

const float period = 1e-3;
const float fclk = 14.7456e6/64;
const short fblink = 
unsigned short time_count = 0;

enum light_color{
	green,
	orange,
	red, 
	} traffic_light;

ISR(TIMER0_COMPA_vect){
	if (time_count > 16000)
	{
		time_count =0;
	}
	time_count++;
	
}

void schedule(){
	
	if (time_count <= 8000)
	{
		traffic_light = red;
		
	}else if (time_count <= 10000)
	{
		traffic_light = orange;
		
	}else if (time_count <= 16000)
	{
		traffic_light = green;
	}
}

void Delay(unsigned int Delay) { //  Delay = Approx 1mS * Delay
	int i;
	for(i = 0; i < Delay; i++) {
		TCCR1A = 0;
		TCCR1B = (1<<CS12) | (1<<CS10);
		OCR1A = 13;            //Approx 1mS
		TCNT1 = 0;
		while (!(TIFR1 & (1<<OCF1A)));
		TIFR1 = (1 << OCF1A);
	}
	TCCR1B = 0;      // Turn Timer Off
}

void pedestrian(){
	
	PORTD |= (1<<PD3);
	Delay(4000);
	
	short i = 0;
	while ( i <= 8)
	{
		PORTD ^= (1<<PD3);
		Delay(500);
		i++;
	}
	
	TCNT0 =0;
	time_count =0;
}

int main(void)
{
	DDRD = 0xFF;
	DDRB = 0x00;
    
	PORTD = 0;
	traffic_light = red;
	
	TIMSK0 |= (1<<OCIE0A);
	TCCR0A = 0;
	TCCR0B = (1<<CS01)|(1<<CS00);
	OCR0A = (int)(period*fclk -1);
	TCNT0 = 0;
	sei();
	
    while (1) 
    {
		schedule();
		
		if (traffic_light == red)
		{
			PORTD = (1<<PD2); //turn on red light only
			
		}
		
		if (traffic_light == orange)
		{
			PORTD = (1<<PD1); //turn on orange light only
			
		}
		
		if (traffic_light == green)
		{
			PORTD = (1<<PD0); //turn on green light only
			
			pedestrian();
			 // use to control the pedestrian light
		}
		
		//use PB0 to reset traffic light
		if (PINB & (1<<PB0))
		{
			TCNT0 =0;
			time_count = 0;
			traffic_light = red;
		}
		
    }
}

