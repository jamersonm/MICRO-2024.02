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

#define tecla PORTBbits.RB0
#define C0 PORTCbits.RC0
unsigned char segment[] = {0x02, 0x9f, 0x25, 0x0d, 0x99, 0x49, 0x41, 0x1f, 0x01, 0x19, 0x11, 0xc1, 0x63, 0x85, 0x61, 0x71};
unsigned char i = 0;
char direction = 1; 

void main(void)
{
    TRISA = 0x00;   
    TRISB = 0xFF;   
    TRISC = 0XEE;  
    TRISD = 0x00;   
    TRISE = 0XEF;
    PORTAbits.RA1 = 0;
    
    while(1)
    {
        
        if (tecla == 0 ) 
        {
            PORTC = 0x10;
            __delay_ms(100); 
            PORTC = 0x00;
            if (tecla == 0)  
            {
                direction = -direction;
            }
        }
        
        PORTD = segment[i];
        __delay_ms(1000);
       
        if (direction == 1) 
        {
            i = (i + 1) % 16;
        } else if (i == 0)
        {
            i = 15; 
        }
        else 
        {
            i = i - 1;
        }
    }
}