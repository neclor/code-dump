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

#include <xc.inc>


PSECT reset_vec, class = CODE, delta = 2
reset_vec:
        goto    start

PSECT isr_vec, class = CODE, delta = 2

	banksel	PORTA
	bsf	PORTA, 0
	bcf	PORTA, 0
	banksel IOCAF
	clrf 	IOCAF
	
	retfie

PSECT code


	
start:  banksel OSCCON
        movlw   0xf8
        movwf   OSCCON
	
        banksel ANSELA		; set porta to digital I/O
        clrf    ANSELA
	
        banksel TRISA		; set porta to input (1) or output (0)
        clrf    TRISA
	bsf	TRISA, 1
	
	banksel PORTA
	clrf 	PORTA
	

	banksel INLVLA 		; schmitt trigger input on porta
	movlw 	0xFF
	movwf 	INLVLA

	banksel IOCAP 		; choose rising egde on porta
	movlw 	0xFF
	movwf	IOCAP
	
	banksel OPTION_REG
	movlw 	01000111B
	movwf   OPTION_REG

	banksel INTCON
	movlw 	10001000B
	movwf 	INTCON


	
	
loop:
	goto loop
	
	
	end	reset_vec
