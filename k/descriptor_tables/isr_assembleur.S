
;.section .text

.altmacro

.macro ISR_HAS_NOT_ERR_CODE num
.align 16
.global isr_\num

isr_\num:
    pushl $0x80 # Remplacer le code d'erreur
    pushl $\num # Numéro d'interruption
    jmp isr
.endm

.macro ISR_HAS_ERR_CODE num
.align 16
.global isr_\num

isr_\num:
        pushl $\num
        jmp isr
.endm



isr:
    #;<save registers>
    pusha #AX, CX, DX, BX, SP, BP, SI, DI
    push %ds
    push %es
    push %fs
    push %gs
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

    pushl %esp
    call generic_c_handler
    add $4, %esp
    #;<restore registers>
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa

    add $8, %esp
    iret

ISR_HAS_NOT_ERR_CODE 0
ISR_HAS_NOT_ERR_CODE 1
ISR_HAS_NOT_ERR_CODE 2
ISR_HAS_NOT_ERR_CODE 3
ISR_HAS_NOT_ERR_CODE 4
ISR_HAS_NOT_ERR_CODE 5
ISR_HAS_NOT_ERR_CODE 6
ISR_HAS_NOT_ERR_CODE 7
ISR_HAS_ERR_CODE 8
ISR_HAS_NOT_ERR_CODE 9
ISR_HAS_ERR_CODE 10
ISR_HAS_ERR_CODE 11
ISR_HAS_ERR_CODE 12
ISR_HAS_ERR_CODE 13
ISR_HAS_ERR_CODE 14
ISR_HAS_ERR_CODE 15
ISR_HAS_NOT_ERR_CODE 16
ISR_HAS_ERR_CODE 17
ISR_HAS_NOT_ERR_CODE 18
ISR_HAS_NOT_ERR_CODE 19
ISR_HAS_NOT_ERR_CODE 20
ISR_HAS_NOT_ERR_CODE 21
ISR_HAS_NOT_ERR_CODE 22
ISR_HAS_NOT_ERR_CODE 23
ISR_HAS_NOT_ERR_CODE 24
ISR_HAS_NOT_ERR_CODE 25
ISR_HAS_NOT_ERR_CODE 26
ISR_HAS_NOT_ERR_CODE 27
ISR_HAS_NOT_ERR_CODE 28
ISR_HAS_NOT_ERR_CODE 29
ISR_HAS_NOT_ERR_CODE 30
ISR_HAS_NOT_ERR_CODE 31



# Syscall
ISR_HAS_NOT_ERR_CODE 128