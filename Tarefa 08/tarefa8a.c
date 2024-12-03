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
    for (i = 0; i < num; i++)
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

void adc_config()
{
    ADCON0 = 0b10011001;
    ADCON1 = 0b10000010;
}

int adc_read()
{
    ADCON0 &= 0xC5;
    ADCON0 |= (3 << 3);
    ADCON0bits.GO_nDONE = 1;
    while (ADCON0bits.GO_nDONE);
    return ((ADRESH << 8) + ADRESL);
}

void lcd_print(float read)
{
    unsigned char buffer[4];
    unsigned int int_read = (unsigned int)(read * 100);
    
    buffer[0] = (unsigned char)(int_read / 100) + '0';
    buffer[1] = (unsigned char)((int_read % 100) / 10) + '0';
    buffer[2] = (unsigned char)(int_read % 10) + '0';
    
    lcd_command(0x88);
    lcd_string(buffer, 1);
    lcd_command(0x06);
    lcd_string(",", 1);
    lcd_command(0x06);
    lcd_string(&buffer[1], 2);
    lcd_command(0x06);
    lcd_string(" V", 2);
}

void main(void)
{
    TRISD = 0x00;
    TRISE = 0x00;
    float result;
    int read;
    
    lcd_initialise();
    lcd_command(0x80);
    lcd_string("Tensao: ", 8);
    
    adc_config();
    
    while (1)
    {
        read = adc_read();
        result = (read * 5.0) / 1023.0;
        lcd_print(result);
        __delay_ms(100);
    }
}
