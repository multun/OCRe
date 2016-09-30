#include <stdlib.h>
#include "binarise.h"
#include "../bitmap/img.h"

void bw_img_treshold(unsigned char treshold, t_bw_img *img)
{
  unsigned int width = img->width;
  unsigned int height = img->height;

  for(unsigned int x = 0; x < width; x++)
    for(unsigned int y = 0; y < height; y++)
    {
      unsigned int offset = y * width + x;
      img->pixels[offset] = (img->pixels[offset] > treshold) ? 255 : 0;
    }
}

t_bw_img *bin_treshold(unsigned char treshold, const t_color_img *orig_img)
{
  t_bw_img *bw_img = greyscale(intensity, orig_img);
  bw_img_treshold(treshold, bw_img);
  return bw_img;
}
