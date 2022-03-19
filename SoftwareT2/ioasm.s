.global _hardware_init, _enable_keypad_column, _elevator_down, _elevator_up, elevator_stop
.extern _adc_init
.extern _usart_init
.extern _lcd_init
    
_hardware_init:
    MOV #0x1E00, W0
    MOV #0x0074, W1
    MOV W0, TRISD
    MOV W1, TRISF
    CALL _adc_init
    CALL _usart_init
    CALL _lcd_init
    return

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
	return
    second_column:	
        BCLR LATD, #8
        BSET LATD, #7
        BCLR LATD, #6
	return
    last_column:
	BCLR LATD, #8
	BCLR LATD, #7
	BSET LATD, #6
	return
    
_elevator_down:
    BCLR LATF, #0
    BSET LATF, #1
    return
    
_elevator_up:
    BSET LATF, #0
    BCLR LATF, #1    
    return
    
_elevator_stop:
    BCLR LATF, #0
    BCLR LATF, #1
    return
    
.end
