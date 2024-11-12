/* 
 * File:   tarefa5b.c
 * Author: jamerson
 *
 * Created on 12 de Novembro de 2024, 16:05
 */

#include <xc.h>
#define _XTAL_FREQ 20000000

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits
#pragma config CP = OFF         // Flash Program Memory Code Protection bit

#define tecla PORTCbits.RC0
unsigned char segment[] = {0x09, 0x03, 0x06, 0x0C};
unsigned char i = 0;
char dir = 1; 
unsigned char botao_anterior = 1;
unsigned char cond_ini = 0;

void main(void)
{
    TRISA = 0x00;   
    TRISB = 0xFF;   
    TRISC = 0XFF;  
    TRISD = 0x00;   
    TRISE = 0XEF;
    PORTAbits.RA1 = 0;
    
    while(1)
    {
        if(cond_ini == 1)
        {
            PORTD = segment[i];
            __delay_ms(100);
        }
        
        if (tecla == 0 && botao_anterior == 1) 
        {
            __delay_ms(100); 
            if (tecla == 0)  
            {
                dir = -dir;
                cond_ini = 1;
            }
        }
        botao_anterior = tecla;
       
        if (dir == 1) 
        {
            i = (i + 1) % 4;
        } else if (i == 0)
        {
            i = 3; 
        }
        else 
        {
            i = i - 1;
        }
    }
}