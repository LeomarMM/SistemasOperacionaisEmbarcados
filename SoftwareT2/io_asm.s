.global _hardware_init, _elevator_down, _elevator_up, elevator_stop
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
