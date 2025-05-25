.text
.globl multiboot_header
multiboot_header:
  .long 0x1BADB002     # magic
  .long 0              # flags
  .long -(0x1BADB002)  # checksum

.globl _start
_start = entry

.globl entry
entry:
  # 设置栈指针
  movl $(stack + 4096), %esp

  # 跳转到 main 函数
  call main

halt:
  jmp halt

.comm stack, 4096