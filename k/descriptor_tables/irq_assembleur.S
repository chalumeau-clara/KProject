
;.section .text

;.align 16


.macro IRQ num, nb_byte
.global irq_\num
.align 16
irq_\num:
    pushl $0 # Remplacer le code d'erreur
    pushl $\nb_byte
    jmp irq
.endm


irq:
    #;<save registers>
    pusha #AX, CX, DX, BX, SP, BP, SI, DI
    push %ds
    push %es
    push %fs
    push %gs
    mov $0x10, %ax #; load the kernel data segment descriptor
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    pushl %esp
    call irq_handler
    add $4, %esp
    #;<restore registers>
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa

    add $8, %esp
    iret

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47