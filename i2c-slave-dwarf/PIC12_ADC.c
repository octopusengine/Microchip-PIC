#include "bitmasking.h"
#include "PIC12_ADC.h"
#include "PIC12_TMR0.h"

// Declared volatile as to not be optimized out by the compiler
// volatile can be removed when the value is actually referenced in code
volatile unsigned char PIC12_ADC_channelData[PIC12_ADC_NUMCHANS];

/*==============================================================================
  FUNCTION:     PIC12_ADC_read()
  DESCRIPTION:  Get ADC result of the specified channel 
  PARAMETERS:   Channel number
  RETURNS:      Higher 8-bits of the result of the 10-bit conversion
  REQUIREMENTS: none
==============================================================================*/
unsigned char PIC12_ADC_read(unsigned char chan) {
    ADCON0 &= ~(BIT5 | BIT4 | BIT3 | BIT2); // Clear the channel select bits
    ADCON0 |= (chan << 2); // Set the channel select bits
    ADON = 1; // Turn ON ADC
    ADIF = 0; // Reset A/D conversion flag
    ADGO = 1; // Start an A-to-D Conversion
    while (ADGO); // Wait for conversion to complete *DANGEROUS*
    //TMR0_counter = 0; // Use TMR0 software counter to implement
    //while (TMR0_counter < ADC_CONV_DLY_MSEC); // the conversion delay
    ADON = 0; // Turn OFF ADC (no current draw from ADC block)

    return (ADRESH); // Return higher 8 bits of the ADC result
}

/*==============================================================================
  FUNCTION:     PIC12_ADC_init()
  DESCRIPTION:  Initialize ADC 
  PARAMETERS:   none
  RETURNS:      nothing
  REQUIREMENTS: none
==============================================================================*/
void PIC12_ADC_init(void) {
    ADCON0 = 0x00; // Clear ADCON0 to default settings
    ADCON1 = 0x00; // Clear ADCON1 to default settings
}
