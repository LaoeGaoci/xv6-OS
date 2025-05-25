extern void print_c(const char *s);
// Allocate a real stack and switch to it, first
// doing some setup required for memory allocator to work.
int main(void)
{
  static char message[] = "Hello from ASM print!";
  print_c(message); // 直接调用汇编函数
  while (1);
}
