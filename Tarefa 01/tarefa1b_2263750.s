PROCESSOR 16F877
#include <xc.inc>
  ; CONFIG
  CONFIG FOSC  = HS       
  CONFIG WDTE  = OFF         
  CONFIG PWRTE = OFF         
  CONFIG CP    = OFF           
  CONFIG BOREN = OFF          
  CONFIG LVP   = OFF         
  CONFIG CPD   = OFF   
  CONFIG WRT   = OFF 

  ; BANCOS
  bank0 MACRO 
    bcf STATUS, 5 ;
    bcf STATUS, 6 ;
    endm

  bank1 MACRO  
    bsf STATUS, 5 ;
    bcf STATUS, 6 ;
    endm

  bank2 MACRO  
    bcf STATUS, 5 ;
    bsf STATUS, 6 ;
    endm

  ; VARIÀVEIS DE TEMPO
  PSECT udata
    TEMP1:DS 1
    TEMP2:DS 1
    TEMP3:DS 1

  ; ENTRADAS
  #define BOTAO_A0 PORTA, 0
  #define BOTAO_A1 PORTA, 1
  #define BOTAO_A2 PORTA, 2
  #define BOTAO_A3 PORTA, 3
  #define BOTAO_A4 PORTA, 4

  ; SAIDAS
  #define LED_B0_ON  bsf PORTB, 0
  #define LED_B0_OFF bcf PORTB, 0
  #define LED_B1_ON  bsf PORTB, 1
  #define LED_B1_OFF bcf PORTB, 1
  #define LED_B2_ON  bsf PORTB, 2
  #define LED_B2_OFF bcf PORTB, 2
  #define LED_B3_ON  bsf PORTB, 3
  #define LED_B3_OFF bcf PORTB, 3
  #define LED_B4_ON  bsf PORTB, 4
  #define LED_B4_OFF bcf PORTB, 4

  ; RESET
  PSECT code, abs
  ORG	0x00
  goto INICIO	  
  
  ; DELAY
  DELAY:
	movlw   0x05
	movwf   TEMP3
	movlw  	0xFF	
	movwf  	TEMP2
  delay_loop:
	movlw  	0xFF			
	movwf  	TEMP1	
	decfsz	TEMP1
	goto    $-1
        decfsz	TEMP2
	goto    delay_loop
	decfsz  TEMP3
	goto    delay_loop
return

  ; INICIO
  INICIO:
  clrf	PORTA
  clrf	PORTB	
  clrf	PORTC
  clrf	PORTD
  clrf	PORTE

  ; BANCO CONFIG
  ; 1 - entrada
  ; 0 - saida
  bank1
  movlw 0xFF
  movwf TRISA
  movlw 11100000B
  movwf TRISB
  movlw 0xFF
  movwf TRISC
  movlw 0xFF
  movwf TRISD
  movlw 0xFF
  movwf TRISE
  bank0

  LOOP0:
    btfss BOTAO_A0
	goto pisca_B0
    LED_B0_OFF
  goto LOOP1
  pisca_B0:
    LED_B0_ON
    call DELAY
    LED_B0_OFF
    call DELAY
 LOOP1:
    btfss BOTAO_A1
	goto pisca_B1
    LED_B1_OFF
 goto LOOP2
  pisca_B1:
    LED_B1_ON
    call DELAY
    LED_B1_OFF
    call DELAY
 LOOP2:
    btfss BOTAO_A2
	goto pisca_B2
    LED_B2_OFF
 goto LOOP3
  pisca_B2:
    LED_B2_ON
    call DELAY
    LED_B2_OFF
    call DELAY
 LOOP3:
    btfss BOTAO_A3
	goto pisca_B3
    LED_B3_OFF
 goto LOOP4
  pisca_B3:
    LED_B3_ON
    call DELAY
    LED_B3_OFF
    call DELAY
 LOOP4:
    btfss BOTAO_A4
	goto pisca_B4
    LED_B4_OFF
  goto LOOP0
  pisca_B4:
    LED_B4_ON
    call DELAY
    LED_B4_OFF
    call DELAY

 end


