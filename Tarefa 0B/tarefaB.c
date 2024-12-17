/* 
 * File:   tarefaB.c
 * Author: jamerson
 *
 * Created on 17 de Dezembro de 2024, 14:32
 */
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

#define _XTAL_FREQ 20000000
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define tecla_0 PORTBbits.RB0
#define tecla_1 PORTBbits.RB1
#define tecla_2 PORTBbits.RB2
#define tecla_3 PORTBbits.RB3

#define RS PORTEbits.RE0
#define EN PORTEbits.RE1
    
void config_ports();

unsigned char ler_teclado();

void lcd_data(char data);
void lcd_command(unsigned char cmd);
void lcd_initialise();
void lcd_string(const unsigned char *str,unsigned char num);
void lcd_gravar();

void escreverEEPROM(unsigned char endereco, unsigned char dado) {
    // Espera o sistema de escrita na EEPROM ficar disponível
    while (EECON1bits.WR); // Aguarda se a memória estiver em processo de escrita
    
    // Define o endereço de onde escrever
    EEADR = endereco; // Endereço de memória
    EEDATA = dado;    // Dado a ser gravado
    
    // Inicia o processo de escrita
    EECON1bits.WREN = 1;    // Habilita a gravação na EEPROM
    INTCONbits.GIE = 0;     // Desabilita interrupções globais
    EECON2 = 0x55;          // Sequência necessária para escrita
    EECON2 = 0xAA;
    EECON1bits.WR = 1;      // Inicia a escrita
    INTCONbits.GIE = 1;     // Habilita interrupções globais
    EECON1bits.WREN = 0;    // Desabilita a gravação na EEPROM
    EECON1bits.WR = 0;
}

unsigned char lerEEPROM(unsigned char endereco) {
    EEADR = endereco;      // Endereço de memória
    EECON1bits.RD = 1;     // Inicia o processo de leitura
    return EEDATA;         // Retorna o dado lido
}

void config_ports()
{
    TRISA = 0xFF;
    TRISB = 0x0F;
    TRISCbits.TRISC2 = 0;
    TRISD = 0x00;
    TRISE = 0x00;
}

void lcd_data(char data)
{
    PORTD = data;
    RS = 1;
    //RW = 0;
    EN = 1;
    __delay_ms(5);
    EN = 0;
}

void lcd_command(unsigned char cmd)
{
    PORTD = cmd;
    RS = 0;
    //RW = 0;
    EN = 1;
    __delay_ms(5);
    EN = 0;
}

void lcd_initialise()
{
    lcd_command(0x38);//Function Set 0011 1000 (D5=1) (DL=D4=1 - 8 bits) (D3=N=1 - 2 linhas) (D2=F=0 - 5x10 tipo da fonte)
    lcd_command(0x06);//Entry Mode Set 0000 0110 (D2=1) (D1=I/D=1 - Cursor move para direita) (D0=SH=0 mensagem se desloca) 
    lcd_command(0x0C);//Display ON/OFF 0000 1100 (D3=1) (D2=D=1 Liga Display) (D1=C=0 Desliga Cursor) (D0=B=0 Cursor n o pisca)
    lcd_command(0x01);//Display Clear 0000 0001
    lcd_command(0x80);
    __delay_ms(500);
}

void lcd_string(const unsigned char *str,unsigned char num)  
{
    unsigned char i;
    for(i=0;i<num;i++)
    {
      lcd_data(str[i]);  
        
    }
}

void lcd_gravar()
{
    lcd_command(0x01);
    lcd_command(0x80);
    lcd_string("VALOR ", 6); //"--- VALOR  a ---"
    lcd_command(0xC0);
    lcd_string("SALVO", 5);
    __delay_ms(500);
    lcd_command(0x01);
    lcd_command(0x80);
    lcd_string("Dig. Tecla: ", 12); 
}

unsigned char ler_teclado()
{
    unsigned char tecla = 0;
    
    do
    {
        PORTB = 0xEF; // 1110
        
        if (tecla_0 == 0)
        {
            tecla = 0x45; // E
            while (tecla_0 == 0);
            break;
        }
        else if (tecla_1 == 0)
        {
            tecla = 0x30; // 0
            while (tecla_1 == 0);
            break;
        }
        else if (tecla_2 == 0)
        {
            tecla = 0x46; // F
            while (tecla_2 == 0);
            break;
        }
        else if (tecla_3 == 0)
        {
            tecla = 0x44; // D
            while (tecla_3 == 0);
            break;
        }

        PORTB = 0xDF; // 1101
        
        if (tecla_0 == 0)
        {
            tecla = 0x37; // 7
            while (tecla_0 == 0);
            break;
        }
        else if (tecla_1 == 0)
        {
            tecla = 0x38; // 8
            while (tecla_1 == 0);
            break;
        }
        else if (tecla_2 == 0)
        {
            tecla = 0x39; // 9
            while (tecla_2 == 0);
            break;
        }
        else if (tecla_3 == 0)
        {
            tecla = 0x43; // C
            while (tecla_3 == 0);
            break;
        }

        PORTB = 0xBF; // 1011
        
        if (tecla_0 == 0)
        {
            tecla = 0x34; // 4
            while (tecla_0 == 0);
            break;
        }
        else if (tecla_1 == 0)
        {
            tecla = 0x35; // 5
            while (tecla_1 == 0);
            break;
        }
        else if (tecla_2 == 0)
        {
            tecla = 0x36; // 6
            while (tecla_2 == 0);
            break;
        }
        else if (tecla_3 == 0)
        {
            tecla = 0x42; // B
            while (tecla_3 == 0);
            break;
        }

        PORTB = 0x7F; // 1111
        
        if (tecla_0 == 0)
        {
            tecla = 0x31; // 1
            while (tecla_0 == 0);
            break;
        }
        else if (tecla_1 == 0)
        {
            tecla = 0x32; // 2
            while (tecla_1 == 0);
            break;
        }
        else if (tecla_2 == 0)
        {
            tecla = 0x33; // 3
            while (tecla_2 == 0);
            break;
        }
        else if (tecla_3 == 0)
        {
            tecla = 0x41; // A
            while (tecla_3 == 0);
            break;
        }

    } while (tecla == 0);  // Repete até que uma tecla seja pressionada
    
    return tecla;  // Retorna a tecla pressionada
}

void main(void) 
{      
    config_ports();
    lcd_initialise();
    unsigned char buffer[2];
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    lcd_command(0x80);
    lcd_string("Dig. Tecla: ", 12);
    
    while(1)
    {
        buffer[0] = ler_teclado();
        if(buffer[0] == 0x46) // F
        {
            //gravar();
            lcd_gravar();
            escreverEEPROM(0x10, buffer[1]);
        } else if (buffer[0] == 0x45) // E
        {
            lcd_command(0xC0);
            lcd_data(lerEEPROM(0x10));
            __delay_ms(500);
            lcd_command(0x01);
            lcd_command(0x80);
            lcd_string("Dig. Tecla: ", 12);
            buffer[1] = buffer[0];
        }else
        {
            buffer[1] = buffer[0];
            lcd_data(buffer[0]);
            lcd_command(0x8C);
        }
    }
    return;
}