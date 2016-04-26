/*
 * File:   mojeFce1840-01.h
 * Author: Uživatel
 *
 * Created on 13. duben 2014, 13:03
 */

#ifndef MOJEFCE1840_01_H
#define	MOJEFCE1840_01_H


#define testbit(var,bit) ((var) & (1<<(bit)))
#define setbit(var,bit) ((var) |=(1<<(bit))
#define clrbit(var,bit) ((var) &=~(1<<(bit)))

#ifdef	__cplusplus
extern "C" {
#endif

unsigned int rem,temp_num,i;
static char retnum[21];

///////////////////////////////////////////////////////////////////////////////
void set4mhz()
{

//	BSF OSCCON,IRCF2
//	BSF OSCCON,IRCF1
//	BCF OSCCON,IRCF0
    //1101 = 4MHz HF str. 61
    OSCCON = 0x68;                  //internal oscillator, 4MHz
}

void set8mhz()
{
    //IRCF3=1; IRCF2=1; IRCF1=1; IRCF0=0;
    OSCCONbits.IRCF3 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;
    //OSCCONbits.
}

void set32mhz()
{
    //IRCF3=1; IRCF2=1; IRCF1=1; IRCF0=0;
    OSCCONbits.IRCF3 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;
}








/////////////////////////////////////////////////////////////////////////////////////////
//     this is delay routine
//     input: required Delay duration in mili Seconds
//    output: none
/////////////////////////////////////////////////////////////////////////////////////////
void delay_ms(unsigned int duration) // delay in miliseconds for 4.0MHZ crystal
{
    unsigned int i;
    for(;duration!=0;duration--)
    {
        for(i=0;i<=50;i++)
        {
            #asm
                nop
                nop
                nop
            #endasm
            ;
        }
            #asm
                nop
                nop
            #endasm
        ;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
//     this is beep routine
//     input: none
//    output: none
/////////////////////////////////////////////////////////////////////////////////////////
void beep(int co)
{
  int p;
  for(p=0;p<30; p++)
  {
    setbit(PORTA,co);
    //PORTA = sGPIO;
    __delay_ms(1);
    clrbit(PORTA,co);
    //PORTA = sGPIO;
    __delay_ms(1);
  }
  ;
}


void beep2(int co)
{
  int p;
  for(p=0;p<50; p++)
  {
    setbit(PORTA,co);
    //PORTA = sGPIO;
    __delay_us(500);
    clrbit(PORTA,co);
    //PORTA = sGPIO;
    __delay_us(500);
  }
  ;
}


void blik(int co)
{
    setbit(PORTA,co);
    __delay_ms(300);
    clrbit(PORTA,co);
    __delay_ms(500);
}



//----------------------------------------------------------------------------
// Low priority interrupt routine

/*
#pragma code
#pragma interrupt InterruptHandlerLow

void InterruptHandlerLow ()
{
  if (PIR1bits.RCIF==1)//is interrupt occured by EUSART receive?,
                        //then RCREG is full we have new data (cleared when RCREG is read)
  {
    if(RCSTA&0x06) //more efficient way than following commented method to check for reception error
    //if(RCSTAbits.FERR==1 || RCSTAbits.OERR==1 )
    {
      RCSTAbits.CREN=0;    //Overrun error (can be cleared by clearing bit CREN)
      cUART_char=RCREG;    //clear Framing error
      RCSTAbits.CREN=1;
    }
    else
    {
       cUART_char = RCREG; // read new data into variable
       cUART_data_flg = 1; // new data received. so enable flg
    }
  }
}
*/

//------------------------------------------------------------------------------
//http://www.electro-tech-online.com/threads/how-to-do-a-serial-port-streaming-in-pic-xc8-compiler.138639/
void uart_init()
{
    APFCONbits.TXCKSEL  = 1; //1 = TX/CK function is on RA4 / 0 na RA0

    TXSTAbits.TXEN =1;
    TXSTAbits.SYNC=0;//assynchronous mode
    RCSTAbits.SPEN = 1;
    //TRISAbits.TRISA5=1; //Make UART RX pin input
    //TRISAbits.TRISA4=0; //Make UART TX pin output
    //srt. 269

    TXSTAbits.TX9=0; //8 bit reception
    TXSTAbits.TRMT=1;//reset the buffer
    TXSTAbits.CSRC=0;// Master mode  (clock generated internally from BRG)
    TXSTAbits.BRGH=0;//baud rate high bits
    BAUDCONbits.BRG16=0;//1 = 16-bit Baud Rate Generator ? SPBRGH and SPBRG
    RCSTAbits.RX9=0;//8 bit reception
    RCSTAbits.CREN = 0; //1 = enable the reciever

    SPBRGH  =0;
    SPBRG   = 25;
    //str 281 > 2400BD
}


void uart_init2400()
{
    uart_init();
}

void uart_init9600()
{
    uart_init();
    TXSTAbits.BRGH=1;//baud rate high bits
    if (_XTAL_FREQ == 4000000)    { SPBRG   = 25;    }
    if (_XTAL_FREQ == 8000000)    { SPBRG   = 51;    }
    if (_XTAL_FREQ == 32000000)   { SPBRG   = 207; }
    //nebo 0 / 51
    //str 281 >
}

void uart_init19200()
{
    uart_init();
    if (_XTAL_FREQ == 8000000)
    {
    TXSTAbits.BRGH=1;//baud rate high bits
    SPBRG   = 25;
    }
    //str 281 >
}

void uart_init57600()
{
    uart_init();
    if (_XTAL_FREQ == 8000000)
    {
    TXSTAbits.BRGH=1;//baud rate high bits
    SPBRG   = 8;
    }

    if (_XTAL_FREQ == 32000000)
    {
    TXSTAbits.BRGH=1;//baud rate high bits
    SPBRG   = 34;
    }

    //str 281 >
}


void uart_init600()
{
    uart_init();
    if (_XTAL_FREQ == 4000000)
    {   
    SPBRG   = 103;
    }

    if (_XTAL_FREQ == 8000000)
    {
    SPBRG   = 206;
    }


}

void uart_init300()
{
    uart_init();
    SPBRG   = 207;
}

void uart_init_fast() //pro 32Mhz > 57,6K
{//Enables Serial port
    APFCONbits.TXCKSEL  = 1; //1 = TX/CK function is on RA4

    TXSTAbits.TXEN =1;
    TXSTAbits.SYNC=0;//assynchronous mode
    RCSTAbits.SPEN = 1;
    //srt. 269

    TXSTAbits.TX9=0; //8 bit reception
    TXSTAbits.TRMT=1;//reset the buffer
    TXSTAbits.CSRC=0;// Master mode  (clock generated internally from BRG)
    TXSTAbits.BRGH=1;//baud rate high bits

    BAUDCONbits.BRG16=0;//1 = 16-bit Baud Rate Generator ? SPBRGH and SPBRG

    RCSTAbits.RX9=0;//8 bit reception
    RCSTAbits.CREN = 0; //1 = enable the reciever

    SPBRGH  =0;
    SPBRG   = 34;
    //str 281 > 57,6kBD
    TRISA  = 0b100000; // out = 0 (in =1)
}


//////////////////////////
//Function for sending char
///////////////////////////
//Requires  : <Xc.h>,Uartinit
//Returns  : None
//Parameters: None
///////////////////////////
void uart_char(char data)
{
    //TXSTAbits.TXEN=0;// disable transmission
    TXREG = data;           // load txreg with data
    //TXSTAbits.TXEN=1;    // enable transmission
    while(!TXSTAbits.TRMT);
}

///////////////////////////
//Function for reciveing from serial
///////////////////////////
//Requires  : <Xc.h>,Uartinit
//Returns  : None
//Parameters: None
///////////////////////////
char uart_read()
{
    while(!RCIF);
    RCIF=0;
    return RCREG;
}
///////////////////////////
//Function for sending number
///////////////////////////
//Requires  : <Xc.h>,Uartinit
//Returns  : None
//Parameters: None
///////////////////////////
void uart_1numb(int num)
{
    uart_char(48+num);
}

void uart_3numb(int num3)
{
    int i100= (num3/100);
    int i10 = (num3-i100*100)/10;
    int i1 = num3-i100*100-i10*10;
    uart_1numb(i100);
    uart_1numb(i10);
    uart_1numb(i1);
}

void uart_4numb(int num4)
{
    num4  = num4 * 2;
//tisíce stovky desítky a jednotky
    uart_char(num4/1000 + 48);
    uart_char((num4/100)%10 + 48);
    uart_char((num4/10)%10 + 48);
    uart_char(num4%10 + 48);
}


void uart_numb(int num)
{

temp_num=num;
if(num==0)
{
    uart_char(48);
}
else
{
while(temp_num!=0)
{
  temp_num=temp_num/10;
  rem=rem++;
}
//  sprintf( retnum, "%ul", num );
  for(i=0;i<=(rem-1);i++)
  {
  uart_1numb(retnum[i]);
  }
  rem=0;
}


}

///////////////////////////
//Function for new line,uart char
///////////////////////////
//Requires  : <Xc.h>,Uartinit
//Returns  : None
//Parameters: None
///////////////////////////
void uart_newline()
{
  uart_char(0x0A);
  uart_char(0x0D);

}
///////////////////////////
//Function for sending string
///////////////////////////
//Requires  : <Xc.h>,Uartinit
//Returns  : None
//Parameters: None
///////////////////////////

void uart_string(char *str)
//take address vaue of the string in pointer *str
{
    int i=0;
    while(str[i]!='\0')
    // loop will go on till the NULL charaters is soon in string
    {
        uart_char(str[i]);
        // sending data on CD byte by byte
        i++;
    }
    return;
}
///////////////////////////
//Function for dumping string
//until 0xff is recieved
///////////////////////////
//Requires  : <Xc.h>,Uartinit
//Returns  : None
//Parameters: None
///////////////////////////

void uart_dump(char *str)
//take address vaue of the string in pionter *str
{

    //for(static int i=0;i<=UART_ARRAY;i++)
    for(static int i=0;i<=123;i++)
    {
        uart_newline();
        uart_numb(i);
        uart_char(':');
        uart_numb(str[i]);
        // sending data on CD byte by byte
    }

    return;
}


///////////////////////////
//Function for sending string
///////////////////////////
//Requires  : <Xc.h>,Uartinit
//Returns  : None
//Parameters: None
///////////////////////////

void uart_stringln(char *str)
//take address vaue of the string in pionter *str
{
    uart_newline();
//    int i=0;
    while(str[i]!='\0')
    // loop will go on till the NULL charaters is seen in string
    {
        uart_char(str[i]);
        // sending data on CD byte by byte
        i++;
    }

}

//-----------------------------------------------------------------------------
void initAnalog(int ch)
{
    
    setbit(TRISA,ch);
    ANSELA |= 0b01110001;
    

    //if (_XTAL_FREQ == 4000000)  { ADCON1   = ADCON1bits.ADCS = 0b100;;    }
    if (_XTAL_FREQ == 32000000) { ADCON1   = ADCON1bits.ADCS = 0b010;;    }
    //str 128

   
    //ADCON0bits.ADON=0;

    //ADCON0bits.ADCS=0b11; //clock = INTOSC/4
    //ADCON0bits.ANS=0b10; //AN0, AN2 analog
    //;00 = AN0   ;bit 3-2
    //;01 = AN1
    //;10 = AN2
    //;11 = AN3
 //str 133

    //ADCON0bits.CHS=0b00; //selet chanel AN0
    //ADCON0bits.ADON=1; //turn ADC on (raedy for sampling)


ADCON0bits.ADON = 1; //enable

switch(ch)
    {
     case 0:
     //ADCON0 = 0b10000001;

         ADCON0bits.CHS = 0b0000;
     break;

     case 1:
         ADCON0bits.CHS = 0b0001;
     break;

     case 2:
         ADCON0bits.CHS = 0b0010;
         break;

     case 3:
         ADCON0bits.CHS = 0b0011;
         break;

        //default: //
    }
}


void initAnalog0()
{
    /*
    ANS<3:0>: Analog Select bits
    Analog select between analog or digital function on pins AN<3:0>, respectively.
    1 = Analog input. Pin is assigned as analog input(1).
    0 = Digital I/O. Pin is assigned to port or special function.
    */
    //ANSELbits.ANS=1;
    setbit(TRISA,0);
    ANSELA |= 0b01110001;
    /*
#asm
    //BANKSEL TRISIO ;
    //BSF TRISIO,0 		;Set GP0 to input
    BANKSEL ANSELA ;
    MOVLW 0b01110001            ;ADC Frc clock,
    IORWF ANSELA 		;and GP0 as analog
    //BANKSEL ADCON0 ;
    //MOVLW 0b10000001            ;Right justify, AN0 - ok
    /////MOVLW B'10001101' 	;Right justify, AN3 - ok
    //MOVWF ADCON0 		;Vdd Vref, AN0, On
#endasm
     */
    ADCON0 = 0b10000001;

    //if (_XTAL_FREQ == 4000000)  { ADCON1   = ADCON1bits.ADCS = 0b100;;    }
    if (_XTAL_FREQ == 32000000) { ADCON1   = ADCON1bits.ADCS = 0b010;;    }
    //str 128

    /*
    ADCON: ADC Enable bit
    1 = ADC is enabled
    0 = ADC is disabled and consumes no operating current
    */
    //ADCON0bits.ADON=0;

    //ADCON0bits.ADCS=0b11; //clock = INTOSC/4
    //ADCON0bits.ANS=0b10; //AN0, AN2 analog
    //;00 = AN0   ;bit 3-2
    //;01 = AN1
    //;10 = AN2
    //;11 = AN3

    //ADCON0bits.CHS=0b00; //selet chanel AN0
    //ADCON0bits.ADON=1; //turn ADC on (raedy for sampling)
}

void initAnalog3(void)
{
    setbit(TRISA,3);
    ANSELA |= 0b01110000;
    ADCON0 = 0b10001101;
    //ADCON1 = 0x00;   // Reference voltage is Vdd
    //ADCON1 = 0;
/*
     //U ref 2.048V
    FVRCONbits.FVREN = 1; // FVR enabled
    //FVRCONbits.ADFVR0 = 1;
    //FVRCONbits.ADFVR1 = 0;
    FVRCONbits.ADFVR = 0b10;
    while(!FVRRDY); //po?ká na init VREF
    //str. 124
*/
}

void setUref2V(void)
{
     //U ref 2.048V
    FVRCONbits.FVREN = 1; // FVR enabled
    //FVRCONbits.ADFVR0 = 1;
    //FVRCONbits.ADFVR1 = 0;
    FVRCONbits.ADFVR = 0b10;
    while(!FVRRDY); //po?ká na init VREF
    //str. 124
}


//----------------------
void zobraz_4094(int byte8, int STR, int DAT, int CLK) //zobrazí Byte 1/strobe0 2/data1 3/clock2
{
    for(int i = 0; i<9; i++)
    {
       //clrbit(PORTA,STR); //strobe off
       //__delay_ms(1);
       setbit(PORTA,CLK); //clk on
        //__delay_ms(1);
        if (!testbit(byte8,i)) {setbit(PORTA,DAT);}
        else {clrbit(PORTA,DAT);}
        //__delay_ms(1);
        clrbit(PORTA,CLK); //clk off
    }
    setbit(PORTA,STR); //strobe on
    __delay_ms(1);
    clrbit(PORTA,STR); //strobe off
}

//==============================================================================




#ifdef	__cplusplus
}
#endif

#endif	/* MOJEFCE1840_01_H */

