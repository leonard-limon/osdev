;
; a20.inc - routines to handle A20 line
; Copyright (C) 2018 - Leonard LIMON, LIC
;

[BITS 16]

; ==============================================================================
; a20 check: check if a20 line is enabled
; return:
;   - 0 if disabled
;   - 1 if enabled
; ==============================================================================

a20_ck:
    pushf
    push    ds
    push    es
    push    di
    push    si

    cli

    xor     ax, ax
    mov     es, ax

    not     ax                      ; ax = 0xFFFF
    mov     ds, ax

    mov     di, 0x0500
    mov     si, 0x0510

    mov     al, byte [es:di]
    push    ax

    mov     al, byte [ds:si]
    push    ax

    mov     byte [es:di], 0x00
    mov     byte [ds:si], 0xFF

    cmp     byte [es:di], 0xFF

    pop     ax
    mov     byte [ds:si], al

    pop     ax
    mov     byte [es:di], al

    mov     ax, 0
    je      a20_ck_exit

    mov     ax, 1

a20_ck_exit:
    pop     si
    pop     di
    pop     es
    pop     ds
    popf

    ret

; ==============================================================================
; enable a20 line with keyboard controller
; ==============================================================================

enable_A20:
        cli

        call    a20wait
        mov     al,0xAD
        out     0x64,al

        call    a20wait
        mov     al,0xD0
        out     0x64,al

        call    a20wait2
        in      al,0x60
        push    eax

        call    a20wait
        mov     al,0xD1
        out     0x64,al

        call    a20wait
        pop     eax
        or      al,2
        out     0x60,al

        call    a20wait
        mov     al,0xAE
        out     0x64,al

        call    a20wait
        sti
        ret

a20wait:
        in      al,0x64
        test    al,2
        jnz     a20wait
        ret


a20wait2:
        in      al,0x64
        test    al,1
        jz      a20wait2
        ret
