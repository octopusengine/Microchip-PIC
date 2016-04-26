/******* USER CONFIG *******/
#define I2C_BUFFERSIZE  16
//#define I2C_SLAVE_ADDR    0b0101010 //2a// Sets the 7-bit slave address
#define I2C_SLAVE_ADDR  0x22 //0b0101011 //2b// dwarf
/***************************/

// BF RCinprocess_TXcomplete; UA dontupdate; SMP midsample_enable; P stopbit_notdetected; S startbit_notdetected; R_nW write_noTX; CKE tx_on_idle_to_active; D_nA lastbyte_address;
#define INIT_SSP1STAT 0x00
// SSPEN enabled; WCOL no_collision; SSPOV no_overflow; CKP lo_hold; SSPM I2CSlave_7bit;
#define INIT_SSP1CON1 0x26
// ACKSTAT received; RCEN disabled; RSEN disabled; ACKEN disabled; ACKDT acknowledge; SEN disabled; GCEN disabled; PEN disabled;
#define INIT_SSP1CON2 0x00
// BOEN disabled; AHEN disabled; SBCDE disabled; SDAHT 100nshold; ACKTIM ackseq; DHEN disabled; PCIE disabled; SCIE disabled;
#define INIT_SSP1CON3 0x00
// SSPBUF 0x00
#define INIT_SSP1BUF 0x00
// SSPMSK
#define INIT_SSP1MSK 0xFF
// SSPADD
#define INIT_SSP1ADD (I2C_SLAVE_ADDR<<1)

#define SSPBUFIDX_SLEEP 0
#define SSPBUFIDX_PR2 1
#define SSPBUFIDX_CCPR1L 2

#define TRUE 1
#define FALSE 0

extern unsigned char SSPBuffer[];

void PIC12_I2C_Slave_init(void); // I2C Initialization Routine
void PIC12_I2C_Slave_handler(void); // I2C Interrupt Handler
