typedef unsigned int   uint;
typedef unsigned char  uchar;
typedef unsigned short ushort;

// ELF constants
#define ELF_MAGIC 0x464C457F  // "\x7FELF" in little endian
#define SECTSIZE 512

// ELF header
struct elfhdr {
  uint magic;  // must equal ELF_MAGIC
  uchar elf[12];
  ushort type;
  ushort machine;
  uint version;
  uint entry;
  uint phoff;
  uint shoff;
  uint flags;
  ushort ehsize;
  ushort phentsize;
  ushort phnum;
  ushort shentsize;
  ushort shnum;
  ushort shstrndx;
};

// Program section header
struct proghdr {
  uint type;
  uint off;
  uint vaddr;
  uint paddr;
  uint filesz;
  uint memsz;
  uint flags;
  uint align;
};

// I/O functions for x86 port-mapped I/O
static inline uchar inb(ushort port) {
  uchar data;
  __asm__ volatile("inb %1, %0" : "=a" (data) : "d" (port));
  return data;
}

static inline void outb(ushort port, uchar data) {
  __asm__ volatile("outb %0, %1" : : "a" (data), "d" (port));
}

static inline void insl(uint port, void *addr, int cnt) {
  __asm__ volatile(
    "cld; rep insl"
    : "=D" (addr), "=c" (cnt)
    : "d" (port), "0" (addr), "1" (cnt)
    : "memory", "cc"
  );
}

// Function declarations
void readseg(uchar*, uint, uint);
void readsect(void*, uint);
void waitdisk(void);

#define ELFHDR ((struct elfhdr*)0x10000)

void bootmain(void) {
  struct proghdr *ph;
  void (*entry)(void);

  // Read ELF header (first 1 page)
  readseg((uchar*)ELFHDR, 4096, 0);

  // Check ELF magic
  if (ELFHDR->magic != ELF_MAGIC)
    return;

  // Load each program segment
  ph = (struct proghdr*)((uchar*)ELFHDR + ELFHDR->phoff);
  for (int i = 0; i < ELFHDR->phnum; i++, ph++)
    readseg((uchar*)ph->paddr, ph->filesz, ph->off);

  // Call the entry point from the ELF header
  entry = (void (*)(void))(ELFHDR->entry);
  entry();
}

void waitdisk(void) {
  while ((inb(0x1F7) & 0xC0) != 0x40);
}

void readsect(void *dst, uint secno) {
  waitdisk();
  outb(0x1F2, 1);
  outb(0x1F3, secno & 0xFF);
  outb(0x1F4, (secno >> 8) & 0xFF);
  outb(0x1F5, (secno >> 16) & 0xFF);
  outb(0x1F6, ((secno >> 24) & 0x0F) | 0xE0);
  outb(0x1F7, 0x20);
  waitdisk();
  insl(0x1F0, dst, SECTSIZE / 4);
}

void readseg(uchar *pa, uint count, uint offset) {
  uchar *epa = pa + count;
  pa -= offset % SECTSIZE;
  offset = (offset / SECTSIZE) + 1;
  for (; pa < epa; pa += SECTSIZE, offset++)
    readsect(pa, offset);
}
