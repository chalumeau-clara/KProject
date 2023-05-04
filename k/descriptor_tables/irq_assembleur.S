
;.section .text

.altmacro

.macro IRQ_HAS_NOT_ERR_CODE num
.align 16
.global irq_\num

irq_\num:
    pushl $0x80 # Remplacer le code d'erreur
    pushl $(\num + 32) # Numéro d'interruption
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


# pic
IRQ_HAS_NOT_ERR_CODE 0
IRQ_HAS_NOT_ERR_CODE 1
IRQ_HAS_NOT_ERR_CODE 2
IRQ_HAS_NOT_ERR_CODE 3
IRQ_HAS_NOT_ERR_CODE 4
IRQ_HAS_NOT_ERR_CODE 5
IRQ_HAS_NOT_ERR_CODE 6
IRQ_HAS_NOT_ERR_CODE 7
IRQ_HAS_NOT_ERR_CODE 8
IRQ_HAS_NOT_ERR_CODE 9
IRQ_HAS_NOT_ERR_CODE 10
IRQ_HAS_NOT_ERR_CODE 11
IRQ_HAS_NOT_ERR_CODE 12
IRQ_HAS_NOT_ERR_CODE 13
IRQ_HAS_NOT_ERR_CODE 14
IRQ_HAS_NOT_ERR_CODE 15
