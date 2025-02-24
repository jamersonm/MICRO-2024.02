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

void UART_Init(void);
void UART_SendChar(char data);
void UART_SendString(const char *str);
volatile char uart_received_data[17]; 
volatile uint8_t uart_index = 0;
volatile uint8_t uart_data_ready = 0;

unsigned char leportb;

float result;
int read;
int count = 0;
int controle_led = 0;

void lcd_nibble(unsigned char data) {
    PORTD = (PORTD & 0x0F) | (data & 0xF0); 
    EN = 1; 
    __delay_us(10); 
    EN = 0;
    __delay_us(10);  
}

void lcd_command(unsigned char cmd) {
    RS = 0;  
    lcd_nibble(cmd);  
    lcd_nibble(cmd << 4);  
    __delay_ms(2);
}

void lcd_data(unsigned char data) {
    RS = 1; 
    lcd_nibble(data); 
    lcd_nibble(data << 4); 
    __delay_us(100); 
}

void lcd_string(const unsigned char *str, unsigned char num) {
    unsigned char i;
    for (i = 0; i < num; i++) {
        lcd_data(str[i]); 
    }
}

void lcd_initialise() {
    __delay_ms(20);  
    lcd_nibble(0x30);  
    __delay_ms(5);
    lcd_nibble(0x30); 
    __delay_us(100);
    lcd_nibble(0x30);  
    __delay_us(100);
    lcd_nibble(0x20);  
    __delay_us(100);

    lcd_command(0x28); 
    lcd_command(0x06);  
    lcd_command(0x0C); 
    lcd_command(0x01); 
    __delay_ms(2); 
}

/*void lcd_data(unsigned char data)
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
}*/

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

void __interrupt() isr(void)
{
    if(PIR1bits.TMR2IF == 1) // Timer
    {
        count++;
        PIR1bits.TMR2IF = 0;
        if(count == 76*3) //76 = 1s
        {
            PORTBbits.RB1 = ~PORTBbits.RB1;
            UART_SendString("ok!");
            //PIE1bits.TMR2IE = 0;
            //count = 0;
        }
    }  
    if(INTCONbits.INTF == 1) // borda de subida acontece
    {
        read = adc_read();
        result = (read * 5.0) / 1023.0;
        lcd_print(result);
        //__delay_ms(100);
        count = 0;
        PORTBbits.RB1 = 1;
        PIE1bits.TMR2IE = 1;
        INTCONbits.INTF = 0;
    }
    /*if(INTCONbits.RBIF == 1) //mudança de estado
    {
        PORTDbits.RD2 = ~PORTDbits.RD2;
        leportb = PORTB;
        INTCONbits.RBIF = 0;
    }*/
}

void main(void)
{
    TRISD = 0x00;
    TRISE = 0x00;
    TRISB = 0x01;
    PORTBbits.RB1 = 0;
    
    lcd_initialise();
    lcd_command(0x80);
    lcd_string("Tensao: ", 8);
    
    adc_config();
    UART_Init();
    
    T2CON = 0b01111111;
    OPTION_REG = 0b01001000;
    INTCON = 0b11011000; 
    PR2 = 0xFF;
    PIE1bits.TMR2IE = 0;
    PIR1bits.TMR2IF = 0;
    
    while (1)
    {
        /*read = adc_read();
        result = (read * 5.0) / 1023.0;
        lcd_print(result);
        __delay_ms(100);*/
    }
}

void UART_Init(void)
{
    TRISC6 = 1;  
    TRISC7 = 1;  
    
    SPBRG = 31;  
    TXSTAbits.SYNC = 0;  
    TXSTAbits.BRGH = 1; 
    RCSTAbits.SPEN = 1;  
    TXSTAbits.TXEN = 1; 
    RCSTAbits.CREN = 1; 
    PIE1bits.RCIE = 1;  
}

void UART_SendChar(char data)
{
    while(!TXIF);
    TXREG = data; 
}

void UART_SendString(const char *str)
{
    while(*str)
    {
        UART_SendChar(*str++);
    }
}