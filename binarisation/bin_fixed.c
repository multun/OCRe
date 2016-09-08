#include <stdlib.h>
#include "binarise.h"
#include "../error.h"
#include "../libbitmap/bmp.h"

unsigned char intensity(t_img_pix pix)
{
  return (pix.r + pix.g + pix.b)/3;
}

t_bw_img *bin_fixed(const t_img *orig_img)
{
  unsigned int width = orig_img->width;
  unsigned int height = orig_img->height;
  t_bw_img* img = alloc_bw_img(width, height);

  for(unsigned int x = 0; x < width; x++)
    for(unsigned int y = 0; y < height; y++)
    {
      unsigned int offset = y * width + x;
      img->pixels[offset] = (intensity(orig_img->pixels[offset]) > 127) ? 255 : 0;
    }
  return img;
}
