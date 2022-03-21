.global _hardware_init, _adc_init, _adc_read
.global _lcd_port, _enable_keypad_column, _elevator_down
.global _elevator_up, elevator_stop, _heating_system
.global _cooling_system, _stable_temperature, _fire_alarm_buzzer
.global _fire_alarm_warning_lights, _fire_alarm_warning_lights_invert
.extern _usart_init
.extern _lcd_init

_hardware_init:
    ;	   FEDCBA9876543210
    MOV #0b0000000000000011, W0 ;TRISB
    MOV #0b0001111000000000, W1 ;TRISD
    MOV #0b0000000001110100, W2 ;TRISF
    MOV #0b0000000000000000, W3 ;LATB
    MOV #0b1110000111000000, W4 ;LATD
    MOV #0b0001000100000000, W5 ;LATF
    MOV W0, TRISB
    MOV W1, TRISD
    MOV W2, TRISF
    MOV W3, LATB
    MOV W4, LATD
    MOV W5, LATF
    CALL _adc_init
    CALL _usart_init
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

_heating_system:
    BTSC W0, #0
    GOTO heat_on
    GOTO heat_off
    heat_on:
	BCLR LATD, #13
	RETURN
    heat_off:
	BSET LATD, #13
	RETURN

_cooling_system:
    BTSC W0, #0
    GOTO cool_on
    GOTO cool_off
    cool_on:
	BCLR LATD, #15
	RETURN
    cool_off:
	BSET LATD, #15
	RETURN

_stable_temperature:
    BTSC W0, #0
    GOTO stable_on
    GOTO stable_off
    stable_on:
	BCLR LATD, #14
	RETURN
    stable_off:
	BSET LATD, #14
	RETURN

_fire_alarm_buzzer:
    BTSC W0, #0
    GOTO buzzer_on
    GOTO buzzer_off
    buzzer_on:
	BSET LATF, #7
	RETURN
    buzzer_off:
	BCLR LATF, #7
	RETURN
	
_fire_alarm_warning_lights:
    BSET LATF, #12
    BTSC W0, #0
    GOTO alights_on
    GOTO alights_off
    alights_on:
	BCLR LATF, #8
	RETURN
    alights_off:
	BSET LATF, #8
	RETURN

_fire_alarm_warning_lights_invert:
    
    BTSC LATF, #8
    GOTO lighta_on
    GOTO lighta_off
    lighta_on:
	BCLR LATF, #8
	GOTO next_cmp
    lighta_off:
	BSET LATF, #8
	GOTO next_cmp
    next_cmp:
    BTSC LATF, #12
    GOTO lightb_on
    GOTO lightb_off
    lightb_on:
	BCLR LATF, #12
	RETURN
    lightb_off:
	BSET LATF, #12
	RETURN

.end
