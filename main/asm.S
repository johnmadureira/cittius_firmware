; Arquivo: init_buttons.S
.global init_buttons

init_buttons:
    ; Configura bits 7 a 4 do PORTD como entrada com pull-up (pinos digitais 47, 38, 49, 48)

    ; DDRD &= 0x0F (bits 7-4 como entrada)
    lds r16, 0x2A       ; 0x2A = DDRD
    andi r16, 0x0F
    sts 0x2A, r16

    ; PORTD |= 0xF0 (pull-up ativado nos bits 7-4)
    lds r16, 0x2B       ; 0x2B = PORTD
    ori r16, 0xF0
    sts 0x2B, r16

    ; Configura pino 26 (PORTA bit 4) como saída
    lds r16, 0x21       ; 0x21 = DDRA
    ori r16, (1 << 4)   ; seta bit 4 como saída
    sts 0x21, r16

    ret
