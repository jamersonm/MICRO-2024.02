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
    TEMP_SEQ1:DS 1
    TEMP_SEQ2:DS 1
    
  ; SAIDAS
  #define LED_A0_ON  bcf PORTA, 0
  #define LED_A0_OFF bsf PORTA, 0

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
  movlw 0xFE
  movwf TRISA
  movlw 0xFF
  movwf TRISB
  movlw 0xFF
  movwf TRISC
  movlw 0x00
  movwf TRISD
  movlw 11101111B
  movwf TRISE
  bank0

  movlw 0xFF
  movwf PORTD
  
  LOOP:
    
    clrf TEMP_SEQ1               ; clear nas variáveis de repetição
    clrf TEMP_SEQ2 
    
    btfsc PORTB, 0               ; identifica botao
    goto LOOP
    call DELAY
    btfsc PORTB, 0
    goto LOOP
    goto LIGA_PARES              ; início sequencia 1
    goto LOOP
        
  LIGA_PARES:
    movlw 10101010B
    movwf PORTD
    call DELAY
    movlw 0xFF
    movwf PORTD
    call DELAY
  LIGA_IMPARES:
    movlw 01010101B
    movwf PORTD
    call DELAY
    movlw 0xFF
    movwf PORTD
    call DELAY
    
  movlw 2
  movwf TEMP_SEQ2 ; Repetir a subida dos leds 2 vezes
  LIGA_SEQ1:
    movlw 8
    movwf TEMP_SEQ1 ; Deslocar 8 vezes
    SEQ_LOOP1: 
    rrf PORTD, f
    bcf STATUS, 0
    call DELAY
    decfsz TEMP_SEQ1
    goto SEQ_LOOP1
  movlw 0xFF
  movwf PORTD
  call DELAY
  decfsz TEMP_SEQ2
  goto LIGA_SEQ1
  goto LOOP_BOTAO
  
  LOOP_BOTAO:
    btfsc PORTB, 0               ; identifica botao
    goto LOOP_BOTAO
    call DELAY
    btfsc PORTB, 0
    goto LOOP_BOTAO
    goto LIGA_INFERIORES         ; início sequencia 2
    goto LOOP_BOTAO
  
  LIGA_INFERIORES:
    movlw 0x0F
    movwf PORTD
    call DELAY
    movlw 0xFF
    movwf PORTD
    call DELAY
  LIGA_SUPERIORES:
    movlw 0xF0
    movwf PORTD
    call DELAY
    movlw 0xFF
    movwf PORTD
    call DELAY
    
  LIGA_SEQ2:
    movlw 8
    movwf TEMP_SEQ1 ; Deslocar 8 vezes pra cima
    SEQ_LOOP2: 
    rrf PORTD, f
    call DELAY
    decfsz TEMP_SEQ1
    goto SEQ_LOOP2
    movlw 8
    movwf TEMP_SEQ1 ; Deslocar 8 vezes pra baixo
    SEQ_LOOP22:
    rlf PORTD, f
    call DELAY
    decfsz TEMP_SEQ1
    goto SEQ_LOOP22
  
  movlw 0xFF
  movwf PORTD
  call DELAY
  
  goto LOOP ; repetir
 end






