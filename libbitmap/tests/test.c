#include <stdio.h>
#include "../img.h"
#include "../bmp.h"

int main()
{
  t_img *ret = load_bmp("test.bmp");
  printf("h %d\n", ret->height);
  printf("w %d\n", ret->width);
  printf("pixaddr %p\n", ret->pixels);
  printf("ret %p\n", ret);

  free_bmp(ret);
  return 0;
}
