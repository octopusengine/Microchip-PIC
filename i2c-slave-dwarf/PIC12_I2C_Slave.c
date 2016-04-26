#include <xc.h>
#include "PIC12_I2C_Slave.h"

unsigned char SSP_bufIndex;
unsigned char SSPBuffer[I2C_BUFFERSIZE];

void PIC12_I2C_Slave_handler(void) {
    unsigned char BUF_temp; // Working variable
    static unsigned char byte_count;
    static unsigned char *byte_ptr;
    static unsigned char registerIndex;

    SSP1IF = 0; // Clear Interrupt Flag

    BUF_temp = SSP1BUF; // Clear BF flag

    if (SSP1STATbits.S) // If Start bit was detected
    {
        if (D_nA == 0) // If received byte is my address
        {
            SSP_bufIndex = 0; // Initialize byte count
        } else // Reading or writing data
        {
            if (R_nW) // If Master is reading
            {
                if (SSP_bufIndex == 0) {
                    // This is the first byte in a Read request..
                    // Point to the beginning of our data buffer
                    byte_ptr = SSPBuffer;
                }
                SSP1BUF = *byte_ptr++; // pointer math is a little faster than SSP_bufIndex dereference
                if (ACKSTAT == 0) {
                    // Master signaled that the message is over
                }
            } else // Master is writing
            {
                if (SSP_bufIndex == 0) { // First byte in a Write command
                    registerIndex = BUF_temp;
                }
                else if (SSP_bufIndex == 1) {
                    SSPBuffer[registerIndex] = BUF_temp;
                } else
                {
                    // Extra byte(s) in the Write command, so ignore them
                }
            }
            SSP_bufIndex++;
        }
    }
    if (WCOL | SSPOV) // If Write Collision Error or Overflow
    {
        PIC12_I2C_Slave_init(); // Reset I2C module on error condition
    }
    CKP = 1; // Ensure SCL is released
}

////////////////////////////////////////////////////////////////
// Init function is called at startup from main()
//
// - SSP1CON, 7-bit Addressing, No SSPIF on start or stop detect
// - SSPMSK, Mask register set to 0x80, only MSb of 7-bit address matters
// - SSP1IE, Enable SSP interrupts 
//

void PIC12_I2C_Slave_init(void) {
    SSP1STAT = INIT_SSP1STAT;
    SSP1CON1 = INIT_SSP1CON1;
    SSP1CON2 = INIT_SSP1CON2;
    SSP1CON3 = INIT_SSP1CON3;
    SSP1BUF = INIT_SSP1BUF;
    SSP1MSK = INIT_SSP1MSK;
    SSP1ADD = INIT_SSP1ADD;
    SSP1IF = 0; // Clear the SSP Interrupt flag
    SSP1IE = 1; // Enable SSP Interrupts
}
