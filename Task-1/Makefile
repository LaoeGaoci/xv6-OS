# 编译和链接参数
CC = gcc
LD = ld
AS = nasm
OBJCOPY = objcopy
CFLAGS = -m32 -fno-pic -fno-stack-protector -nostdinc -fno-builtin \
         -fno-omit-frame-pointer -fno-strict-aliasing -Wall -MD -ggdb -I. -Os
LDFLAGS = -m elf_i386

# 源文件目标
OBJS = main.o print.o console.o syscall.o sysproc.o usys.o

all: xv6.img

bootblock: bootasm.S bootmain.c
	$(CC) $(CFLAGS) -c bootmain.c -o bootmain.o
	$(CC) $(CFLAGS) -c bootasm.S -o bootasm.o
	$(LD) $(LDFLAGS) -N -e start -Ttext 0x7C00 -o bootblock.elf bootasm.o bootmain.o
	$(OBJCOPY) -S -O binary -j .text bootblock.elf bootblock
	perl sign.pl bootblock

# 编译 .c 文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 编译 .S 文件（AT&T 汇编）
%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

# 编译 .asm 文件（如果你还保留了 nasm 格式）
%.o: %.s
	$(AS) -f elf $< -o $@

# 只保留这一个 kernel 目标
kernel: $(OBJS) entry.o kernel.ld
	$(LD) $(LDFLAGS) -T kernel.ld -o kernel entry.o $(OBJS)

xv6.img: bootblock kernel
	rm -f xv6.img
	dd if=/dev/zero of=xv6.img bs=512 count=10000
	dd if=bootblock of=xv6.img conv=notrunc
	dd if=kernel of=xv6.img seek=1 conv=notrunc

clean:
	rm -f *.o *.d *.elf bootblock kernel xv6.img

.PHONY: all clean xv6.img bootblock kernel
