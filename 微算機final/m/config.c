#include "config.h"
#include <pic18f4520.h>

int degree = 32;
int start = 0;
int C2H5OH=0;
int count = 0;
int halted = 1;
void initialize_motor(void){
    TRISBbits.RB4 = 0;
    TRISBbits.RB5 = 0;
    TRISCbits.RC4 = 0;
    TRISCbits.RC5 = 0;
    TRISCbits.RC6 = 0;
    TRISCbits.RC7 = 0;
    
    LATCbits.LATC4 = 0;
    LATCbits.LATC5 = 0;
    LATCbits.LATC6 = 0;
    LATCbits.LATC7 = 0;
    
    LATBbits.LATB4 = 0;
    LATBbits.LATB5 = 0;
    return;
}
void __interrupt(high_priority) ADC_ISR (){
    unsigned int temp = 0;
    temp = temp | ADRESH;
    temp = temp * 256;
    temp = temp |ADRESL;
    unsigned int i = 0;
    while(temp > 64){
        temp -= 64;
        i++;
    }
    if(i>1)
    {
        LATDbits.LATD1 = 0; 
        LATDbits.LATD2 = 1; 
        C2H5OH = 1;
        ADCON0bits.ADON = 0;
        //__delay_ms(400);
        LATBbits.LATB4 = 1;
        LATBbits.LATB5 = 1;
        halted = 0;
    }
    else{
        LATDbits.LATD1 = 1; 
        C2H5OH = 0;
    }
    PIR1bits.ADIF = 0;
    ADCON0bits.GODONE = 1;
    __delay_us(4);
}



void main(void) {
    // Turn comparator off and enable pins for digital I/O functions
    ADCON1bits.PCFG=0b1110;  //AN0 is analog 
    //T1CONbits.RD16 = 1;
    TMR1L = 0;  
    initialize_motor();
    // Ultrasonic sensor pins
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC3 = 0;
    TRISAbits.TRISA2 = 0;  // TRIG pin
    TRISAbits.TRISA3 = 1;  // ECHO pin
    TRISAbits.RA0 = 1;
    TRISD = 0;
    TRISDbits.RD4 = 1;
    TRISDbits.RD5 = 1;
    TRISDbits.RD6 = 1;
    TRISDbits.RD7 = 1;
    TRISCbits.RC3 = 0;
    RCONbits.IPEN = 1;
    T1CONbits.T1CKPS = 0b11;
    // Select internal clock (FOSC/4)
    //T1CONbits.TMR1CS = 0;
    T2CON = 0b00001010; // prescaler = 1:16
    OSCCONbits.IRCF = 0b100; //1MHZ
    CCP1CONbits.CCP1M = 0b1100;
    PR2 = 0x9b;    
    ADCON0bits.CHS = 0b0000;
    ADCON0bits.ADON = 1;
    ADCON2bits.ADFM = 1; //right justified
    ADCON2bits.ADCS = 0b000;   //FOSC/2 1TAD = 2usec   
    ADCON2bits.ACQT = 0b101;   //4TAD> 2.4usec
      
    PIE1bits.ADIE = 1;   //Enable A/D interrupt
    PIR1bits.ADIF = 0;    //AD conversion is not complete
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;   //enable global interrupt
    IPR1bits.ADIP = 1;
    
    ADCON0bits.GODONE = 1; //Enable AD converter module
    __delay_us(4);


    int timer;
    LATD = 0;
    while(1) {
        if(C2H5OH == 1){
            if(start != 0){
                T2CONbits.TMR2ON = 1;
                degree = 144;
                CCP1CONbits.DC1B = degree & 0b11;
                CCPR1L = degree >> 2;
                __delay_ms(100);
                degree = 32;
                CCP1CONbits.DC1B = degree & 0b11;
                CCPR1L = degree >> 2;
                start = 0;
            }
            TMR1L = 0;   
            PORTAbits.RA2 = 1;
            __delay_us(10);
            PORTAbits.RA2 = 0;
            while(PORTAbits.RA3 == 0);
            T1CONbits.TMR1ON = 1;   // Enable TIMER1 module 
            while(PORTAbits.RA3);
            T1CONbits.TMR1ON = 0;   // Disable TIMER1 module
            timer =TMR1L;
            if(timer > 25 && halted == 0){
                LATDbits.LATD0 = 0;
                start = 0;
                count = 0;
            }else if(timer > 20 && halted == 1){
                LATDbits.LATD0 = 0;
                LATBbits.LATB4 = 1;
                LATBbits.LATB5 = 1;
                halted = 0;
                count = 0;
                start = 0;
            }
            else if(count > 3){
                __delay_ms(200);
                LATDbits.LATD0 = 1;
                LATBbits.LATB4 = 0;
                LATBbits.LATB5 = 0;
                __delay_ms(100);
                start = 1;
                halted = 1;
            }else{
                count++;
            }
            if(PORTDbits.RD4 == 1){   //green back
                LATCbits.LATC4 = 0;
                LATCbits.LATC5 = 1;
                LATCbits.LATC6 = 0;
                LATCbits.LATC7 = 1;
                LATCbits.LC3 = 0;
            }else if(PORTDbits.RD5 == 1){ //yellow right
                LATCbits.LATC4 = 1;
                LATCbits.LATC5 = 0;
                LATCbits.LATC6 = 0;
                LATCbits.LATC7 = 1;
                LATCbits.LC3 = 0;
            }else if(PORTDbits.RD6 == 1){  //red left
                LATCbits.LATC4 = 0;
                LATCbits.LATC5 = 1;
                LATCbits.LATC6 = 1;
                LATCbits.LATC7 = 0;
                LATCbits.LC3 = 1;
            }else if(PORTDbits.RD7 == 1){   //blue straight
                LATCbits.LATC4 = 1;
                LATCbits.LATC5 = 0;
                LATCbits.LATC6 = 1;
                LATCbits.LATC7 = 0;
                LATCbits.LC3 = 0;
            }else{
                LATCbits.LATC4 = 0;
                LATCbits.LATC5 = 0;
                LATCbits.LATC6 = 0;
                LATCbits.LATC7 = 0;
                LATCbits.LC3 = 0;
            }
            __delay_ms(10);
        }
    }
    return;
}