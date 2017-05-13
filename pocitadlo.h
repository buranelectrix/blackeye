#ifndef __MYCOUNTER__
#define __MYCOUNTER__

typedef struct {
    uint8_t jednotky;
    uint8_t desitky;
    uint8_t stovky;
    uint8_t tisice;
    uint8_t destis;
    uint8_t statis;
 } My_counter;

inline static void counter_reset(My_counter* c) {
    c->jednotky = 0;
    c->desitky = 0;
    c->stovky = 0;
    c->tisice = 0;
    c->destis = 0;
    c->statis = 0;
};

inline static void counter_advance(My_counter* c) {
    c->jednotky += 1;
    if (c->jednotky == 10) {
        c->jednotky = 0;
        c->desitky += 1;
        if (c->desitky == 10) {
            c->desitky = 0;
            c->stovky += 1;
            if (c->stovky == 10) {
                c->stovky = 0;
                c->tisice += 1;
                if (c->tisice == 10) {
                    c->tisice = 0;
                    c->destis += 1;
                    if (c->destis == 10) {
                        c->destis = 0;
                        c->statis += 1;
                        if (c->statis == 10) {
                            c->statis = 0;
                        }
                    }
                }
            }
        }
    }
}    




#endif
