// display 01

#include "Wire.h"
#include <avr/interrupt.h>
#include "portsandpins.h"
#include "font.h"
#define F_CPU 16000000L
#include <util/delay.h>
#include "pocitadlo.h"
#include <RTClib.h>

RTC_DS1307 DS1307;

volatile uint8_t display_buffer[12][31] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};


void setup () {

    Serial.begin(9600);

    // rows -  0 input, 1 output
    // ddrf rows 0..7
    DDRF  = 0xff; // set as output
    PORTF = 0xff; // set low
    // ddrk rows 8..11
    DDRK  = 0xff; // set as output
    PORTK = 0xff; // set low

    // columns -  0 input, 1 output
    DDRA  = 0xff; // output
    PORTA = 0xff; // set high
    DDRB  = 0xff; // output
    PORTB = 0xff; // set high
    DDRC  = 0xff; // output
    PORTC = 0xff; // set high
    DDRL  = 0xff; // output
    PORTL = 0xff; // set high
    DDRG  = 0xff; // output
    PORTG = 0xff; // set high
    DDRD  = 0xff; // output
    PORTD = 0xff; // set high

    // TIMER 0 - 8bit - interrupt
    TCCR0A = 0;              // reset register
    TCCR0A |= (1 << WGM01);  // CTC
    TCCR0B = 0;
    TCCR0B |= (1 << CS02);   // prescaler 256
    TIMSK0 = (1 << OCIE0A);  // output compare enable
    OCR0A = 35;

    sei(); 
}





void display_digit(uint8_t position, uint8_t digit) {

    uint8_t i;
    uint8_t j;
    uint8_t tmp;

    for (i = 0; i < 12; i++) {
        tmp = cisla[digit][i] & 0b00001111;

        for (j = 0; j < 4; j ++) {
            if (tmp & (1 << (3-j))) {
                display_buffer[i][position + j] = 1;
            }
            else {
                display_buffer[i][position + j] = 0;   
            }
        }
    }
}








int main(void) {

    setup();

    // My_counter my_counter;
    // counter_reset(&my_counter);

    DS1307.begin();
    DS1307.isrunning();

    while(1) {

        DateTime datumCas = DS1307.now();

        uint8_t hour = datumCas.hour();

        display_digit(0,  datumCas.hour()/10);
        display_digit(5,  datumCas.hour()%10);
        display_digit(11, datumCas.minute()/10);
        display_digit(16, datumCas.minute()%10);
        display_digit(22, datumCas.second()/10);
        display_digit(27, datumCas.second()%10);
        // counter_advance(&my_counter);

        _delay_ms(100);

    }
}











ISR(TIMER0_COMPA_vect) {
    static uint8_t column = 0;
    uint8_t i;
    uint8_t tmp;

    tmp = 0;
    for (i = 0; i < 8; i ++) {
        tmp |= (display_buffer[i][column] << i);
    }
    PORTF = tmp;
    
    tmp = 0;
    for (i = 0; i < 4; i ++) {
        tmp |= (display_buffer[i+8][column] << i);
    }
    PORTK = tmp;

    set_column(column);
    column += 1;
    if (column > 30) {
        column = 0;
    }
}


