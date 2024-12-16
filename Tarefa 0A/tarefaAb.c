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

#define UM_SEG 10
#define BOTAO_B0 RB0
#define BOTAO_B1 RB1
#define MOTOR RC0

int cont = 0;
int t2 = 0;
int t4 = 0;

void __interrupt() isr(void)
{
    if(PIR1bits.TMR1IF == 1)
    {
        PIR1bits.TMR1IF = 0;
        cont++;
        if(t2 == 1 && cont == 2*UM_SEG)
        {
            MOTOR = 1;
            cont = 0;
            t2 = 0;
            T1CONbits.TMR1ON = 0;
        }
        if(t4 == 1 && cont == 4*UM_SEG)
        {
            MOTOR = 1;
            cont = 0;
            t4 = 0;
            T1CONbits.TMR1ON = 0;
        }
    }
}

void setup_timer1()
{
    TMR1H = 0;  // Zera o Timer1 (parte alta)
    TMR1L = 0;  // Zera o Timer1 (parte baixa)
    TMR1CS = 0; // Usa o clock interno
    T1CKPS0 = 1;  // Prescaler de 1:8
    T1CKPS1 = 1;  // Prescaler de 1:8
    TMR1IE = 1;  // Habilita a interrupção do Timer1
    GIE = 1;     // Habilita as interrupções globais
    PEIE = 1;    // Habilita as interrupções periféricas
    TMR1ON = 1;  // Liga o Timer1
}

void main(void) {
    
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISCbits.TRISC0 = 0;
    
    PORTC = 0x01;
    
    T1CON = 0b00110000;
    PIR1bits.TMR1IF = 0; 
    PIE1bits.TMR1IE = 1; 
    INTCONbits.GIE = 1; 
    INTCONbits.PEIE = 1; 
    
    //setup_timer1();
    
    while(1)
    {
        if(BOTAO_B0 == 0 && t2 == 0)
        {
            t2 = 1;
            T1CONbits.TMR1ON = 1;
            MOTOR = 0;
            while(t2 == 1);
        }
        if(BOTAO_B1 == 0 && t4 == 0)
        {
            t4 = 1;
            T1CONbits.TMR1ON = 1;
            MOTOR = 0;
            while(t4 == 1);
        }
    }
}

