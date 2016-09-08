#include <stdio.h>

void printmem(void* addr, unsigned int count, unsigned int step)
{
  char* caddr = (char*)addr;
  unsigned int i = 0;
  while (i < count)
  {
    if (i && i % step == 0)
      printf("\n");
    printf("%02x ",(unsigned char)caddr[i]);
    i++;
  }
  printf("\n");
}
