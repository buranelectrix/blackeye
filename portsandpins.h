#ifndef __PORTS_AND_PINS__
#define __PORTS_AND_PINS__


volatile uint8_t *ports[] = {
        &PORTG, &PORTL, &PORTL, &PORTA, 
        &PORTA, &PORTA, &PORTC, &PORTB, 
        &PORTB, &PORTL, &PORTL, &PORTL, 
        &PORTC, &PORTA, &PORTA, &PORTG, 
        &PORTB, &PORTL, &PORTB, &PORTD, 
        &PORTL, &PORTA, &PORTA, &PORTL, 
        &PORTC, &PORTA, &PORTG, &PORTC, 
        &PORTC, &PORTC, &PORTC 
    };
const uint8_t pins[] = {
        PG1, PL7, PL5, PA3, 
        PA1, PA7, PC5, PB3, 
        PB1, PL6, PL2, PL4, 
        PC6, PA5, PA0, PG2, 
        PB0, PL1, PB2, PD7, 
        PL0, PA6, PA2, PL3, 
        PC7, PA4, PG0, PC0, 
        PC4, PC2, PC3
     };

void set_column(uint8_t col_num) {
    static uint8_t last_col_num = 0;
    *ports[last_col_num] |= (1 << pins[last_col_num]);
    *ports[col_num] &= ~(1 << pins[col_num]);
    last_col_num = col_num;
}



#endif

