#include <stdio.h>
#include "bitmap/img.h"
#include "bitmap/bmp.h"
#include "binarisation/binarise.h"
#include "memtools.h"

int main(void)
{
  t_color_img *img = load_bmp("bitmap/tests/test_art.bmp");

  t_bw_img *bw_img = binarise(SAUVOLA, img);

  for(unsigned int y = 0; y < img->height; y++)
  {
    for(unsigned int x = 0; x < img->width; x++)
      printf((bw_img->pixels[y*img->width + x]) ? "#" : ".");
    printf("\n");
  }

  free_bw_img(bw_img);
  free_color_img(img);
  return 0;
}
