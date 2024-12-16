#include <xc.h>

// Configuração do PIC
#pragma config FOSC = XT        // Oscilador XT
#pragma config WDTE = OFF       // Watchdog Timer desativado
#pragma config PWRTE = ON       // Power-up Timer ativado
#pragma config BOREN = ON       // Brown-out Reset ativado
#pragma config LVP = OFF        // Low Voltage Programming desativada
#pragma config CPD = OFF        // Código não protegido em EEPROM
#pragma config WRT = OFF        // Escrita protegida desativada
#pragma config CP = OFF         // Código não protegido

#define _XTAL_FREQ 20000000     // Frequência do oscilador (20 MHz)

// Definições de pinos
#define BUTTON1 RB0
#define BUTTON2 RB1
#define MOTOR RD0

// Variáveis globais
volatile unsigned char motor_active = 0; // Indica se o motor está ativo
volatile unsigned int timer_cycles = 0; // Número de ciclos restantes

void init_config() {
    // Configuração dos pinos
    TRISB0 = 1;  // RB0 como entrada (botão 1)
    TRISB1 = 1;  // RB1 como entrada (botão 2)
    TRISD0 = 0;  // RD0 como saída (motor)

    // Configuração do Timer1
    T1CKPS0 = 1; // Prescaler 1:8
    T1CKPS1 = 1;
    TMR1CS = 0;  // Uso do clock interno (FOSC/4)
    TMR1ON = 0;  // Timer desligado inicialmente

    // Habilitação de interrupções
    TMR1IE = 1;  // Habilita interrupção do Timer1
    PEIE = 1;    // Habilita interrupções periféricas
    GIE = 1;     // Habilita interrupções globais
}

void start_timer(unsigned int duration_ms) {
    TMR1ON = 0;                    // Desliga o Timer1 para configuração
    TMR1 = 65536 - 62500;          // Configura para 104.86 ms
    timer_cycles = duration_ms / 104; // Calcula o número de ciclos necessários
    TMR1ON = 1;                    // Liga o Timer1
    motor_active = 1;              // Indica que o motor está ativo
    MOTOR = 1;                     // Liga o motor
}

void __interrupt() isr() {
    if (TMR1IF) { // Interrupção do Timer1
        TMR1IF = 0; // Limpa a flag de interrupção
        if (timer_cycles > 0) {
            timer_cycles--;
            TMR1 = 65536 - 62500; // Recarrega o Timer1 para o próximo ciclo
        } else {
            TMR1ON = 0;  // Desliga o Timer1
            MOTOR = 0;   // Desliga o motor
            motor_active = 0; // Indica que o motor está inativo
        }
    }
}

void main() {
    init_config();

    while (1) {
        if (!motor_active) { // Apenas processa se o motor não estiver ativo
            if (BUTTON1 == 0) { // Botão 1 pressionado
                __delay_ms(50); // Debounce
                if (BUTTON1 == 0) {
                    start_timer(2000); // Liga o motor por 2 segundos
                }
            }
            if (BUTTON2 == 0) { // Botão 2 pressionado
                __delay_ms(50); // Debounce
                if (BUTTON2 == 0) {
                    start_timer(4000); // Liga o motor por 4 segundos
                }
            }
        }
    }
}