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

#define RS PORTEbits.RE0
#define EN PORTEbits.RE1

#define tecla_1 PORTCbits.RC1

void lcd_data(unsigned char data)
{
    PORTD = data;
    RS = 1;
    EN = 1;
    __delay_ms(5);
    EN = 0;
}

void lcd_command(unsigned char cmd)
{
    PORTD = cmd;
    RS = 0;
    EN = 1;
    __delay_ms(5);
    EN = 0;
}

void lcd_string(const unsigned char *str, unsigned char num)
{
    unsigned char i;
    for(i = 0; i < num; i++)
    {
        lcd_data(str[i]);
    }
}

void lcd_initialise()
{
    lcd_command(0x38);
    lcd_command(0x06);
    lcd_command(0x0C);
    lcd_command(0x01);
}

void setup_timer1()
{
    T1CON = 0x87;
    TMR1H = 0xFF;
    TMR1L = 0xF0;
}

unsigned int read_timer1()
{
    return ((TMR1H << 8) | TMR1L);
}

void main(void)
{
    TRISB = 0xFF;
    TRISC = 0XFE;
    TRISE = 0x00;
    TRISD = 0x00;
    
    lcd_initialise();
    setup_timer1();

    while(1)
    {
        lcd_command(0x80);
        lcd_string("CLIQUES:", 8);

        lcd_command(0xC0);
        char buffer[10];
        sprintf(buffer, "%05u", read_timer1());
        lcd_string(buffer, 5);

        if(tecla_1 == 1)
        {
            TMR1L = 0x00;
            while(tecla_1 == 1);
        }
    }
}
