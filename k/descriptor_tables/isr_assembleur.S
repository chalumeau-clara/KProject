
.section .text

.extern generic_c_handler
.align 4
.macro ISR_WITHOUT_ERR_CODE num
    .global isr_\num
    isr_\num:
        pushl $80 # Remplacer le code d'erreur
        pushl $\num # Numéro d'interruption
        jmp isr
.endm

.macro ISR_WITH_ERR_CODE num
    .global isr_\num
    isr_\num:
        pushl $\num
        jmp isr
.endm

isr:
#<save registers>
    pushal
    pushl %esp
    call generic_c_handler
    addl $4, %esp
    #<restore registers>
    addl $8, %esp
    popal
    iret  # interrupt return
         # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
         # restores the saved flags into the EFLAGS register

ISR_WITHOUT_ERR_CODE 0
ISR_WITHOUT_ERR_CODE 1
ISR_WITHOUT_ERR_CODE 2
ISR_WITHOUT_ERR_CODE 3
ISR_WITHOUT_ERR_CODE 4
ISR_WITHOUT_ERR_CODE 5
ISR_WITHOUT_ERR_CODE 6
ISR_WITHOUT_ERR_CODE 7
ISR_WITH_ERR_CODE 8
ISR_WITHOUT_ERR_CODE 9
ISR_WITH_ERR_CODE 10
ISR_WITH_ERR_CODE 11
ISR_WITH_ERR_CODE 12
ISR_WITH_ERR_CODE 13
ISR_WITH_ERR_CODE 14
ISR_WITH_ERR_CODE 15
ISR_WITHOUT_ERR_CODE 16
ISR_WITH_ERR_CODE 17
ISR_WITHOUT_ERR_CODE 18
ISR_WITHOUT_ERR_CODE 19
ISR_WITHOUT_ERR_CODE 20
ISR_WITHOUT_ERR_CODE 21
ISR_WITHOUT_ERR_CODE 22
ISR_WITHOUT_ERR_CODE 23
ISR_WITHOUT_ERR_CODE 24
ISR_WITHOUT_ERR_CODE 25
ISR_WITHOUT_ERR_CODE 26
ISR_WITHOUT_ERR_CODE 27
ISR_WITHOUT_ERR_CODE 28
ISR_WITHOUT_ERR_CODE 29
ISR_WITHOUT_ERR_CODE 30
ISR_WITHOUT_ERR_CODE 31
ISR_WITHOUT_ERR_CODE 127
