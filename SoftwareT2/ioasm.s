.global _hardware_init, _adc_init, _adc_read, _lcd_port, _enable_keypad_column, _elevator_down, _elevator_up, elevator_stop
.extern _usart_init
.extern _lcd_init
    
_hardware_init:
    MOV #0x1E00, W0
    MOV #0x0074, W1
    MOV #0x0003, W2
    MOV W0, TRISD
    MOV W1, TRISF
    MOV W2, TRISB
    CALL _adc_init
    CALL _usart_init
    CALL _lcd_init
    RETURN

_adc_init:
    ;	   FEDCBA9876543210
    MOV #0b0000000011100000, W0 ;AD1CON1
    MOV #0b0000010000000100, W1 ;AD1CON2
    MOV #0b0000000100000010, W2 ;AD1CON3
    MOV #0b0000000000000011, W3 ;AD1CSSL
    MOV #0b1111111111111100, W4 ;AD1PCFG
    MOV W0, AD1CON1
    MOV W1, AD1CON2
    MOV W2, AD1CON3
    MOV W3, AD1CSSL
    MOV W4, AD1PCFG
    BCLR IFS0, #13
    BSET AD1CON1, #15
    RETURN

_adc_read:
    BSET AD1CON1, #2
    AD1IF_loop:
	BTSS IFS0, #13
	GOTO AD1IF_loop
    BCLR AD1CON1, #2
    BCLR IFS0, #13
    MOV ADC1BUF0, W1
    MOV ADC1BUF1, W2
    SUB W1, W2, W0
    RETURN
    
_lcd_port:
    BCLR LATD, #0
    BCLR LATD, #1
    BCLR LATD, #2
    BCLR LATD, #3
    BTSC W0, #0
    BSET LATD, #0
    BTSC W0, #1
    BSET LATD, #1
    BTSC W0, #2
    BSET LATD, #2
    BTSC W0, #3
    BSET LATD, #3
    RETURN
    
_enable_keypad_column:
    SUB W0, #1
    BRA Z, first_column
    SUB W0, #1
    BRA Z, second_column
    GOTO last_column
    first_column:
        BSET LATD, #8
        BCLR LATD, #7
        BCLR LATD, #6
	RETURN
    second_column:	
        BCLR LATD, #8
        BSET LATD, #7
        BCLR LATD, #6
	RETURN
    last_column:
	BCLR LATD, #8
	BCLR LATD, #7
	BSET LATD, #6
	RETURN
    
_elevator_down:
    BCLR LATF, #0
    BSET LATF, #1
    RETURN
    
_elevator_up:
    BSET LATF, #0
    BCLR LATF, #1    
    RETURN
    
_elevator_stop:
    BCLR LATF, #0
    BCLR LATF, #1
    RETURN
    
.end
