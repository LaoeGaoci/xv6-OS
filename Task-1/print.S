.section .text
.code32
.global print_c

print_c:
    pushw %ax
    pushw %bx
    pushw %cx
    pushw %dx
    pushw %si
    pushw %di

    movw $0xB800, %ax
    movw %ax, %es
    xorw %di, %di

.next_char:
    lodsb
    cmpb $0, %al
    je .done
    movb %al, %es:(%di)
    movb $0x07, %es:1(%di)
    addw $2, %di
    jmp .next_char

.done:
    popw %di
    popw %si
    popw %dx
    popw %cx
    popw %bx
    popw %ax
    ret
