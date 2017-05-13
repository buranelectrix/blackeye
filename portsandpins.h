#ifndef __PORTS_AND_PINS__
#define __PORTS_AND_PINS__


volatile uint8_t *ports[] = {
        &PORTG, &PORTG, &PORTG, 
        &PORTL, &PORTL, &PORTL, &PORTL, &PORTL, &PORTL, &PORTL, &PORTL,
        &PORTB, &PORTB, &PORTB, &PORTB,
        &PORTA, &PORTA, &PORTA, &PORTA, &PORTA, &PORTA, &PORTA, &PORTA,
        &PORTC, &PORTC, &PORTC, &PORTC, &PORTC, &PORTC, &PORTC, &PORTC 
    };
const uint8_t pins[] = {
        PG2, PG1, PG0, 
        PL7, PL6, PL5, PL4, PL3, PL2, PL1, PL0,
        PB3, PB2, PB1, PB0,
        PA7, PA6, PA5, PA4, PA3, PA2, PA1, PA0,
        PC7, PC6, PC5, PC4, PC3, PC2, PC1, PC0
     };

void set_column(uint8_t col_num) {
    static uint8_t last_col_num = 0;
    *ports[last_col_num] |= (1 << pins[last_col_num]);
    *ports[col_num] &= ~(1 << pins[col_num]);
    last_col_num = col_num;
}



#endif