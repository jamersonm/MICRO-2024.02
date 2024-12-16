/* 
 * File:   tarefaAa.c
 * Author: jamerson
 *
 * Created on 10 de Dezembro de 2024, 14:49
 */
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

#define _XTAL_FREQ 20000000
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = OFF
#pragma config LVP = ON
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

int count = 0;

unsigned char leportb;

void __interrupt() isr(void)
{
    if(PIR1bits.TMR2IF == 1)
    {
        count++;
        PIR1bits.TMR2IF = 0;
        if(count == 76)
        {
            PORTDbits.RD0 = ~PORTDbits.RD0;
            count = 0;
        }
    }  
    if(INTCONbits.INTF == 1)
    {
        PORTDbits.RD1 = ~PORTDbits.RD1;
        INTCONbits.INTF = 0;
    }
    if(INTCONbits.RBIF == 1)
    {
        PORTDbits.RD2 = ~PORTDbits.RD2;
        leportb = PORTB;
        INTCONbits.RBIF = 0;
    }
}

void main(void) {
    TRISB = 0x81;
    TRISD = 0xF8; //1111 1000
    
    PORTD = 0xFF;
    
    T2CON = 0b01111111;
    OPTION_REG = 0b01001000;
    INTCON = 0b11011000; 
    PR2 = 0xFF;
    PIE1bits.TMR2IE = 1;
    PIR1bits.TMR2IF = 0;
    while(1)
    {
        
    }
}

