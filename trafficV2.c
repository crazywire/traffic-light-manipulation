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
unsigned short time_count = 0;
unsigned short fblink =0;
enum light_color{
	green,
	orange,
	red, 
	} traffic_light;


ISR(TIMER0_COMPA_vect){
	if (time_count > 16000)
	{
		time_count =0;
		fblink =0;
	}
	
	time_count++;
	if (time_count >= 13000)
	{
		fblink++;
	}
	
}


void schedule(){
	
	if (time_count <= 8000)
	{
		traffic_light = red;
		PORTD = (1<<PD2);
		
	}else if (time_count <= 10000)
	{
		traffic_light = orange;
		PORTD = (1<<PD1);
	}else if (time_count <= 16000)
	{
		traffic_light = green;
		PORTD = (1<<PD0);
		
		if (time_count < 13000)
		{
			PORTD |= (1<<PD3);
		}
       
	}
	
	if (fblink >=500)
	{
		PORTD ^= (1<<PD3);
		fblink=0;
	}
	
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
		
		if (PINB & (1<<PB0))
		{
			time_count=0;
			TCNT0 =0;
			traffic_light =red;
		}
    }
}

