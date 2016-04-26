#include <xc.h>

#define PIC12_ADC_NUMCHANS 4

#define ADC_FULLSCALE 0xFF

#define ADC_CONV_DLY_MSEC 1

extern volatile unsigned char PIC12_ADC_channelData[];

void PIC12_ADC_init(void);

unsigned char PIC12_ADC_read(unsigned char chan);
