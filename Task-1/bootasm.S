.code16
.global start

start:
    cli                     # Disable interrupts
    xor %ax, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %ss
    mov $0x7C00, %sp        # Set up stack
    mov $0x0E, %ah          # BIOS teletype output function
    mov $'X', %al           # Character to print
    int $0x10     
    call bootmain           # Call the C function (linked within same binary)

hang:
    jmp hang                # Infinite loop if bootmain returns

.size start, . - start
