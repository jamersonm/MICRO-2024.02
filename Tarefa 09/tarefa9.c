#include <xc.h>
#include <stdio.h>
#define _XTAL_FREQ 20000000

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = OFF
#pragma config LVP = ON
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

int qtd_estouro = 0;
unsigned int estado_led = 0;

void configurar_Timer0() {
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS = 0b111;
    
    TMR0 = 0;
    
    INTCONbits.T0IE = 1;
    INTCONbits.T0IF = 0;
    INTCONbits.GIE = 1;
}

void inverter_led() {
    if(estado_led % 2 == 0) {
        PORTD = 0x01;
    } else {
        PORTD = 0x00;
    }
    estado_led++;
    if(estado_led == 65530) {
        estado_led = 0;
    }
}

void __interrupt() isr(void) {
    if (INTCONbits.T0IF) {
        TMR0 = 0;
        qtd_estouro++;
        
        if (qtd_estouro >= 77) {
            inverter_led();
            qtd_estouro = 0;
        }
        
        INTCONbits.T0IF = 0;
    }
}

void main(void) {
    TRISD = 0xFE;
    PORTD = 0x00;
    
    configurar_Timer0();
    
    while(1) {
    }
    return;
}
