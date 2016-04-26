#include "PIC12_ECCP.h"

void PIC12_ECCP_init(void) {
    CCP1CONbits.P1M = 0; // Single Output; P1A modulated
    CCP1CONbits.DC1B = 0x03; // The two LSB's of the 10-bit duty cycle
    CCP1CONbits.CCP1M = PWM_P1AP1C_AH; // PWM mode: P1A,P1C active high
    CCPR1L = (PR2 >> 1); // PWM duty cycle = 50%
}
