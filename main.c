#include <stdio.h>
#include "libbitmap/img.h"
#include "libbitmap/bmp.h"
#include "binarisation/binarise.h"
#include "memtools.h"

int main(void)
{
  t_img *img = load_bmp("libbitmap/tests/test_art.bmp");

  t_bw_img *bw_img = binarise(FIXED, img);

  for(unsigned int y = 0; y < img->height; y++)
  {
    for(unsigned int x = 0; x < img->width; x++)
      printf((bw_img->pixels[y*img->width + x]) ? "#" : ".");
    printf("\n");
  }

  free_img(img);
  return 0;
}
