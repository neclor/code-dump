PROCESSOR 16F1789

; CONFIG1
  CONFIG  FOSC = INTOSC         ; Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
  CONFIG  WDTE = OFF            ; Watchdog Timer Enable (WDT disabled)
  CONFIG  PWRTE = OFF           ; Power-up Timer Enable (PWRT disabled)
  CONFIG  MCLRE = ON            ; MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
  CONFIG  CP = OFF              ; Flash Program Memory Code Protection (Program memory code protection is disabled)
  CONFIG  CPD = OFF             ; Data Memory Code Protection (Data memory code protection is disabled)
  CONFIG  BOREN = ON            ; Brown-out Reset Enable (Brown-out Reset enabled)
  CONFIG  CLKOUTEN = OFF        ; Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
  CONFIG  IESO = ON             ; Internal/External Switchover (Internal/External Switchover mode is enabled)
  CONFIG  FCMEN = ON            ; Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

; CONFIG2
  CONFIG  WRT = OFF             ; Flash Memory Self-Write Protection (Write protection off)
  CONFIG  VCAPEN = OFF          ; Voltage Regulator Capacitor Enable bit (Vcap functionality is disabled on RA6.)
  CONFIG  PLLEN = ON            ; PLL Enable (4x PLL enabled)
  CONFIG  STVREN = ON           ; Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
  CONFIG  BORV = LO             ; Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
  CONFIG  LPBOR = OFF           ; Low Power Brown-Out Reset Enable Bit (Low power brown-out is disabled)
  CONFIG  DEBUG = OFF           ; In-Circuit Debugger Mode (In-Circuit Debugger disabled, ICSPCLK and ICSPDAT are general purpose I/O pins)
  CONFIG  LVP = ON             ; Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

#define SLAVE_ADDRESS 0x42 	; edit this line

#include <xc.inc>

PSECT udata_shr
byte_sent:
	DS 	1
message:
	DS	1


PSECT reset_vec, class = CODE, delta = 2
reset_vec:
        goto    start

PSECT isr_vec, class = CODE, delta = 2
isr_vec:
	banksel PIR1
	btfsc	PIR1, 3
	call 	interrupt_I2C
	banksel INTCON
	btfsc   INTCON, 2
	call 	interrupt_timer0
	retfie
interrupt_I2C:
	bcf 	PIR1, 3
	bsf 	byte_sent, 0
	return
interrupt_timer0:
	banksel INTCON
	bcf 	INTCON, 2 	; clear interrupt flag
	banksel PORTA
	movf	PORTA, 0        ; load PORTA into W
	xorlw 	1		; toggle bit 0
	movwf	PORTA		; place result on PORTA again
	movwf	message		; copy result to send it
	return
		
PSECT code

start:  banksel OSCCON
        movlw   0xf8
        movwf   OSCCON
	
	banksel ANSELA		
        clrf    ANSELA
        banksel TRISA
        clrf    TRISA
        banksel PORTA
	clrf 	PORTA

	;;I2C	
	banksel SSP1STAT
	bsf 	SSP1STAT, 7	;slewrate control disbled
	bsf 	SSP1STAT, 6	;compliant with SMBus
	banksel SSP1CON1
	movlw	00111000B	;read only|read only|enable serial port|enable clock|master mode
	movwf	SSP1CON1
	banksel SSP1CON2
	bcf 	SSP1CON2, 7	;general call interrupt disbled
	bcf	SSP1CON2, 5	;acknowledge
	bsf	SSP1CON2, 4	;enable acknowledge
	bcf 	SSP1CON2, 0	;start condition idle
	banksel SSP1CON3
	movlw 	00001000B	;read only|disble stop detection interrupt|disble start
				;detection interrupt|ignored|min hold time 300 ns|
				;slave mode only|slave mode only|slave mode only
	banksel SSP1MSK		;not relevant
	banksel SSP1ADD
	movlw	0x8F		;set baud rate to twice minimum
	movwf	SSP1ADD
	banksel TRISC
	bsf 	TRISC,	4	;set SDA as input
	bsf 	TRISC, 	3	;set SCL as input
	;; timer0
	banksel OPTION_REG
	movlw 	01000111B	;weakpullup enabled individually|rising edge|internal clock|low-to-high|prescaler on|1:256
	movwf   OPTION_REG

	
	;; interrupts
	banksel INTCON
	bsf 	INTCON, 5	; enable timer0 overflow interrupt
	bsf 	INTCON, 6	; enable peripheral interrupts
	banksel	INTCON
	bsf 	INTCON, 7	;enable general interrupts
	
	clrf	message
	call 	initiate_transaction_master_sender
	banksel PIE1
	bsf 	PIE1, 3		;enable interrupts from MSSP
	
	movlw	1
	movwf	byte_sent 	;to enter the loop for the first time
loop:
	btfss	byte_sent, 0
	goto 	loop
	movf 	message, 0
	call 	send_byte
	goto loop

send_byte:
	bcf	byte_sent, 0
	banksel SSP1BUF
	movwf	SSP1BUF
	return
	
start_condition:
	banksel SSP1CON2
	bsf 	SSP1CON2, 0
	return
	
stop_condition:
	banksel SSP1CON2
	bsf 	SSP1CON2, 3
	return
	
initiate_transaction_master_sender:
	call	start_condition
wait_start_complete:
	banksel PIR1
	btfss	PIR1, 3
	goto	wait_start_complete
	movlw 	SLAVE_ADDRESS * 2
	call	send_byte
	return
	
end reset_vec
