#include "PIC12_WDT.h"

void PIC12_WDT_init(void) {
    WDTCONbits.WDTPS = 10; // Timeout period = 1 second
    SWDTEN = 1; // Enable WDT
}

void PIC12_WDT_kick(void) {
    CLRWDT(); // Clear WDT
}
