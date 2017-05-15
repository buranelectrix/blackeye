// display 01

#include "Wire.h"
#include <avr/interrupt.h>
#include "portsandpins.h"
#include "font.h"
#define F_CPU 16000000L
#include <util/delay.h>
#include <RTClib.h>
#include <avr/pgmspace.h>
#include "timetable.h"

RTC_DS1307 DS1307;

uint8_t current_date[] = {0,0};
uint8_t current_time[] = {0,0,0};
uint8_t vychod_time[] = {0,0};
uint8_t vychod_time_next[] = {0,0};
uint8_t zapad_time[] = {0,0};

volatile uint8_t is_night = 0;

volatile uint8_t display_buffer[12][31] = {
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0},
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0},
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0},
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0},
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0},
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0},
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0},
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0},
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0},
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0},
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0},
    {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0}
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



void print_current_time(void) {
    DateTime datumCas = DS1307.now();
    Serial.print(datumCas.hour());
    Serial.print(':');
    Serial.print(datumCas.minute());
    Serial.print(':');
    Serial.print(datumCas.second());
    Serial.println();
}

void print_time_to_vychod(uint8_t h, uint8_t m, uint8_t s) {
    Serial.print("time to vychod: ");
    Serial.print(h);
    Serial.print(":");
    Serial.print(m);
    Serial.print(":");
    Serial.print(s);
    Serial.println();
}

void display_current_time(void) {
    DateTime datumCas = DS1307.now();
    display_digit(0,  datumCas.hour()/10);
    display_digit(5,  datumCas.hour()%10);
    display_digit(10, datumCas.minute()/10);
    display_digit(15, datumCas.minute()%10);
    display_digit(20, datumCas.second()/10);
    display_digit(25, datumCas.second()%10);
}

void display_time_to_vychod(uint8_t h, uint8_t m, uint8_t s) {
    display_digit(0,  h/10);
    display_digit(5,  h%10);
    display_digit(10, m/10);
    display_digit(15, m%10);
    display_digit(20, s/10);
    display_digit(25, s%10);
}

void get_vychod_time(uint8_t datum[]) {
    uint16_t record;
    for (record = 0; record < CASY_SIZE; record ++) {
        uint8_t table_month = pgm_read_byte( &(casy[record][0]) );
        if (table_month == datum[0]) {
            uint8_t table_day = pgm_read_byte( &(casy[record][1]) );
            if (table_day == datum[1]) {
                vychod_time[0] = pgm_read_byte( &(casy[record][2]));
                vychod_time[1] = pgm_read_byte( &(casy[record][3]));

                uint16_t next_record = record + 1;
                if (next_record >= CASY_SIZE) {
                    next_record = 0;
                }
                vychod_time_next[0] = pgm_read_byte( &(casy[next_record][2]));
                vychod_time_next[1] = pgm_read_byte( &(casy[next_record][3]));
            }
        }
    }
}

void get_zapad_time(uint8_t datum[]) {
    uint16_t record;
    for (record = 0; record < CASY_SIZE; record ++) {
        uint8_t table_month = pgm_read_byte( &(casy[record][0]) );
        if (table_month == datum[0]) {
            uint8_t table_day = pgm_read_byte( &(casy[record][1]) );
            if (table_day == datum[1]) {
                zapad_time[0] = pgm_read_byte( &(casy[record][4]));
                zapad_time[1] = pgm_read_byte( &(casy[record][5]));
            }
        }
    }
}

void get_current_date() {
    DateTime datumCas = DS1307.now();
    current_date[0] = datumCas.month();
    current_date[1] = datumCas.day();
    current_time[0] = datumCas.hour();
    current_time[1] = datumCas.minute();
    current_time[2] = datumCas.second();

}

uint16_t get_current_timestamp() {
    DateTime datumCas = DS1307.now();
    current_time[0] = datumCas.hour();
    current_time[1] = datumCas.minute();
    return (datumCas.hour() * 100) + datumCas.minute();
}








int main(void) {

    setup();


    DS1307.begin();
    DS1307.isrunning();
    //příklad: 26.4.2016 9:10:11
    // DS1307.adjust(DateTime(2017, 5, 15, 1, 21, 10));


    get_current_date();


    while(1) {

        get_current_date();
        // get_vychod_zapad_time(current_date);
        uint16_t current_timestamp = get_current_timestamp();

        get_vychod_time(current_date);
        get_zapad_time(current_date);

        uint16_t vychod_timestamp_next = (vychod_time_next[0] * 100) + vychod_time_next[1];
        uint16_t vychod_timestamp = (vychod_time[0] * 100) + vychod_time[1];
        uint16_t zapad_timestamp = (zapad_time[0] * 100) + zapad_time[1];
     
        // print_current_time();

        // Serial.print(zapad_timestamp);
        // Serial.print(" ");
        // Serial.print(current_timestamp);
        // Serial.println();

        if ((current_timestamp < vychod_timestamp) || (current_timestamp >= zapad_timestamp)) {
            // je noc
            is_night = 1;
            // Serial.print("Je noc.");
            // Serial.println();

            uint8_t v_hour   = 0;
            uint8_t v_minute = 0;
            uint8_t c_hour   = current_time[0];
            uint8_t c_minute = current_time[1];
            uint8_t c_second = current_time[2];

            uint8_t h_to_vychod = 0;
            uint8_t m_to_vychod = 0;
            uint8_t s_to_vychod = 0;


            if (current_timestamp < vychod_timestamp) {
                //je mezi pulnoci a vychodem slunce
                // ted 2:55   vych 7:34  
                v_hour   = vychod_time[0];
                v_minute = vychod_time[1];
                if (v_minute <= c_minute) {
                    h_to_vychod = v_hour - c_hour - 1;
                    m_to_vychod = (60 + v_minute) - c_minute - 1;
                    s_to_vychod = 59 - c_second;
                }
                else {
                    h_to_vychod = v_hour - c_hour;   
                    m_to_vychod = v_minute - c_minute - 1;
                    s_to_vychod = 59 - c_second;
                }
            }


            if (current_timestamp >= zapad_timestamp) {
                //je mezi zapadem slunce a pulnoci
                // ted 22:55   vych 7:34  
                v_hour   = vychod_time_next[0];
                v_minute = vychod_time_next[1];
                if (v_minute <= c_minute) {
                    h_to_vychod = (v_hour + 24) - c_hour - 1;
                    m_to_vychod = (60 + v_minute) - c_minute - 1;
                    s_to_vychod = 59 - c_second;
                }
                else {
                    h_to_vychod = (v_hour + 24) - c_hour;   
                    m_to_vychod = v_minute - c_minute - 1;
                    s_to_vychod = 59 - c_second;
                }
            }


            display_time_to_vychod(h_to_vychod, m_to_vychod, s_to_vychod);
            // print_time_to_vychod(h_to_vychod, m_to_vychod, s_to_vychod);



        }
        else {
            // je den
            is_night = 0;
            // Serial.print("Je den.");
            // Serial.println();
        }
         

        _delay_ms(100);
        print_current_time();

    }
}






ISR(TIMER0_COMPA_vect) {
    static uint8_t column = 0;
    uint8_t i;

    uint8_t tmpF = 0;
    for (i = 0; i < 8; i ++) {
        tmpF |= (display_buffer[i][column] << i);
    }

    uint8_t tmpK = 0;
    for (i = 0; i < 4; i ++) {
        tmpK |= (display_buffer[i+8][column] << i);
    }

    if (is_night) {
        PORTF = tmpF;
        PORTK = tmpK;
    }
    else {
        PORTF = 0x00;
        PORTK = 0x00;   
    }


    set_column(column);
    column += 1;
    if (column > 30) {
        column = 0;
    }
}


