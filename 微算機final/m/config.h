#ifndef CONFIG
#define    CONFIG

// PIC16F628A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config OSC = INTIO67 // Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
//#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRT = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
//#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
#pragma config WDT = OFF

#define _XTAL_FREQ 4000000

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <stdio.h>
#include <math.h>
//#include "lcd.h"


#endif    /* CONFIG */