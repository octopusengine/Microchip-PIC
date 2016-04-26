#include "PIC12_TMR2.h"

void PIC12_TMR2_init(void) {
    T2CONbits.T2CKPS = 1;   // Timer prescale value = 4
    // Calculate what PR2 should be to achieve 6kHz
    // Period = {[PR2 + 1] * 4 * Tosc * T2CKPS}
    // (1/6000) = {[PR2 + 1] * 4 * (1 / 8 MHz) * 4}
    PR2 = 0x52;
    TMR2ON = 1; // Enable Timer2
}
