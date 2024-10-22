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
  #define BOTAO_C0 PORTC, 0
  #define BOTAO_C1 PORTC, 1

  ; SAIDAS
  #define LED_B0_ON  bsf PORTB, 0
  #define LED_B0_OFF bcf PORTB, 0
  #define LED_B1_ON  bsf PORTB, 1
  #define LED_B1_OFF bcf PORTB, 1
  #define LED_B2_ON  bsf PORTB, 2
  #define LED_B2_OFF bcf PORTB, 2
  #define LED_B3_ON  bsf PORTB, 3
  #define LED_B3_OFF bcf PORTB, 3

  ; RESET
  PSECT code, abs
  ORG	0x00
  goto INICIO	 
  
  ; INTERRUPÇÂO
  PSECT code, abs
  ORG 0x04
  retfie
  
  ; DELAY
  DELAY:
	movlw   0x08
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
  movlw 11110000B
  movwf TRISB
  movlw 0xFF
  movwf TRISC
  movlw 0xFF
  movwf TRISD
  movlw 0xFF
  movwf TRISE
  bank0

  LOOP:
    movlw 0x00
    movwf PORTB
    btfss BOTAO_C0
    goto LOOP1
    btfss BOTAO_C1
    goto LOOP2
    goto LOOP
    LOOP1:
	movlw 00000001B
	movwf PORTB
	rlf PORTB,f
	call DELAY
	rrf PORTB,f
	call DELAY
	goto LOOP
    LOOP2:
	movlw 00001000B
	movwf PORTB
	rrf PORTB,f
	call DELAY
	rlf PORTB,f
	call DELAY
	goto LOOP
        
 end



