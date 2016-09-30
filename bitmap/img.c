#include <stdlib.h>
#include "img.h"

DEFAULT_IMG_TYPES_APPLY(DEFINE_IMG_TOOLS, )

t_bw_img *greyscale(unsigned char (*intensity)(t_color_pix), const t_color_img *orig_img)
{
  unsigned int width = orig_img->width;
  unsigned int height = orig_img->height;
  t_bw_img* img = alloc_bw_img(width, height);

  for(unsigned int x = 0; x < width; x++)
    for(unsigned int y = 0; y < height; y++)
    {
      unsigned int offset = y * width + x;
      img->pixels[offset] = intensity(orig_img->pixels[offset]);
    }
  return img;
}
