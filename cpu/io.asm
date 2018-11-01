;
; io.asm - cpu ports input output routines
; Copyright (C) 2018 - Leonard LIMON, LIC
;

[global inb]
[global inw]
[global outb]
[global outw]

inb:
    mov     dx, [esp+4]
    in      al, dx
    ret

inw:
    mov     dx, [esp+4]
    in      ax, dx
    ret

outb:
    mov     dx, [esp+4]
    mov     al, [esp+8]
    out     dx, al
    ret

outw:
    mov     dx, [esp+4]
    mov     ax, [esp+8]
    out     dx, ax
    ret
