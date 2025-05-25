
#include "types.h"
# include "syscall.h"

// Task-1 print函数
extern int sys_print(void);


static int (*syscalls[])(void) = {
    [SYS_print] sys_print, // 加入print
};
