#include <xc.h>

// Configura��es do microcontrolador
#pragma config FOSC = HS    // Oscilador de alta velocidade
#pragma config WDTE = OFF   // Watchdog Timer desativado
#pragma config PWRTE = OFF  // Power-up Timer desativado
#pragma config CP = OFF     // Prote��o de c�digo desativada

#define _XTAL_FREQ 16000000  // Frequ�ncia do oscilador (16 MHz)

void configurar_ADC() {
    ADCON0 = 0b10011001;  // Ativa ADC, seleciona canal AN3 (RA3), FOSC/8
    ADCON1 = 0b10000010;  // Resultado justificado � direita, Vref+ = VDD, Vref- = VSS
}

unsigned int ler_ADC(unsigned char canal) {
    ADCON0 &= 0xC5;            // Limpa bits de sele��o de canal
    ADCON0 |= (canal << 3);    // Seleciona o canal desejado
    __delay_ms(2);             // Tempo de aquisi��o
    ADCON0bits.GO_nDONE = 1;   // Inicia a convers�o
    while (ADCON0bits.GO_nDONE); // Aguarda a convers�o terminar
    return (((unsigned int)ADRESH << 8) + ADRESL); // Retorna resultado de 10 bits
}

void configurar_PWM() {
    CCP1CON = 0b00001100; // Configura CCP1 em modo PWM
    PR2 = 0xFF;           // Define o per�odo do PWM
    T2CON = 0x04;         // Ativa Timer2 com prescaler 1:1
    CCPR1L = 0x00;        // Duty cycle inicial em zero
}

void ajustar_duty_cycle(unsigned int duty) {
    // Ajusta o duty cycle do PWM (valor entre 0 e 1023)
    CCPR1L = (unsigned char)(duty >> 2); // 8 bits mais significativos
    // Ajuste dos 2 bits menos significativos
    CCP1CONbits.CCP1X = (duty >> 1) & 0x01; // Bit menos significativo
    CCP1CONbits.CCP1Y = duty & 0x01;        // Segundo bit menos significativo
}

void main() {
    unsigned int valor_ADC;
    unsigned int duty_cycle;

    // Configura��es de entrada e sa�da
    TRISA = 0xFF;             // Porta A como entrada (RA3 para ADC)
    TRISCbits.TRISC2 = 0;     // RC2 como sa�da (PWM para o LED)

    configurar_ADC();
    configurar_PWM();

    while (1) {
        valor_ADC = ler_ADC(3);       // L� o valor do ADC no canal AN3 (RA3)
        duty_cycle = valor_ADC;       // Usa o valor do ADC para ajustar o duty cycle
        ajustar_duty_cycle(duty_cycle);
        __delay_ms(10);               // Pequeno atraso para estabilidade
    }
}
