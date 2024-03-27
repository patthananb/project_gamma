.global _init_timer
.global _init_ADC

.equ TCCR1B_reg, 0x81    ; Timer/Counter1 Control Register B
.equ OCR1A_reg, 0x88     ; Output Compare Register 1 A
.equ TIMSK1_reg, 0x6F    ; Timer/Counter1 Interrupt Mask Register
.equ ADMUX_reg, 0x7C     ; ADC Multiplexer Selection Register
.equ ADCSRA_reg, 0x7A    ; ADC Control and Status Register A
.equ ADCSRB_reg, 0x7B    ; ADC Control and Status Register B

.equ WGM12_bit, 3        ; Bit 3: WGM12 (Waveform Generation Mode 12)
.equ OCIE1A_bit, 1       ; Bit 1: OCIE1A (Output Compare A Match Interrupt Enable)
.equ CS11_bit, 1         ; Bit 1: CS11 (Clock Select bit 1)
.equ CS10_bit, 0         ; Bit 0: CS10 (Clock Select bit 0)
.equ REFS0_bit, 6        ; Bit 6: REFS0 (Reference Selection bit 0)
.equ ADEN_bit, 7         ; Bit 7: ADEN (ADC Enable)
.equ ADIE_bit, 3         ; Bit 3: ADIE (ADC Interrupt Enable)
.equ ADATE_bit, 5        ; Bit 5: ADATE (ADC Auto Trigger Enable)
.equ ADPS2_bit, 2        ; Bit 2: ADPS2 (ADC Prescaler Select bit 2)
.equ ADPS1_bit, 1        ; Bit 1: ADPS1 (ADC Prescaler Select bit 1)
.equ ADPS0_bit, 0        ; Bit 0: ADPS0 (ADC Prescaler Select bit 0)

.equ prescaler_64, 0x03  ; Prescaler value for 64
.equ prescaler_128, 0x07 ; Prescaler value for 128

.section .text

_init_timer:
    ; Configure Timer1 for CTC mode (WGM12 = 1)
    ldi r16, (1 << WGM12_bit)
    sts TCCR1B_reg, r16
    
    ; Set compare value for 1ms at 16MHz clock (prescaler 64)
    ldi r16, 249    ; 16,000,000 / 64 / 1000 - 1 = 249
    sts OCR1A_reg, r16
    
    ; Enable Timer1 Compare A Match interrupt (OCIE1A = 1)
    ldi r16, (1 << OCIE1A_bit)
    sts TIMSK1_reg, r16
    
    ; Start Timer1 with prescaler 64 (CS11 = 1, CS10 = 0)
    ldi r16, (1 << CS11_bit)
    sts TCCR1B_reg, r16
    
    ret

_init_ADC:
    ; Set internal reference voltage to Vcc (AVcc)
    ldi r16, (1 << REFS0_bit)
    sts ADMUX_reg, r16
    
    ; Enable ADC, enable ADC interrupt, set ADC Auto Trigger Source to Timer/Counter1 Compare Match A,
    ; and set ADC prescaler to 128 (ADEN = 1, ADIE = 1, ADATE = 1, ADPS2:0 = 1)
    ldi r16, (1 << ADEN_bit) | (1 << ADIE_bit) | (1 << ADATE_bit) | prescaler_128
    sts ADCSRA_reg, r16
    
    ret
