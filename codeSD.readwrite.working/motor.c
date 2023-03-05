#define F_CPU 16000000UL
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "75.h"
#include "i2cmaster.h"
#include "lcd.h"
#include "usart.h" // inculdes all necesarry files for the screen and the 

int i = 0;


int main(void) { 
    uart_init(); // open the communication to the microcontroller
	io_redirect(); // redirect input and output to the communication

    while(1){
// 200 steps per revolution
//1.8degree per step
        DDRB = 0b11111111;
        PORTB =0b00101111;
        // max = _delay_ms(0.06);
        _delay_ms(1);
        PORTB =0b00111111;
    }
}