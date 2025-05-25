#include "console.h"

int sys_print(void)
{
  char *str = "Hello from syscall!";;
  print_str(str); 
  return 0;
}
