ENTRY(_start)
SECTIONS {
  . = 0x100000; /* Kernel加载到1MB */
  .text : { *(.text) }
  .data : { *(.data) }
  .bss  : { *(.bss COMMON) }
}
