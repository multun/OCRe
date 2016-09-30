#include <stdlib.h>
#include "img.h"

t_color_img* alloc_img(unsigned int width, unsigned int height)
{
  t_color_img *ret	= malloc(sizeof(t_color_img) + sizeof(t_color_pix) * width * height);
  ret->width	= width;
  ret->height	= height;
  return ret;
}

t_bw_img* alloc_bw_img(unsigned int width, unsigned int height)
{
  t_bw_img *ret	= malloc(sizeof(t_bw_img) + sizeof(t_bw_pix) * width * height);
  ret->width	= width;
  ret->height	= height;
  return ret;
}

t_color_img	*img_alloc_twin(t_color_img *img)
{
  return alloc_img(img->width, img->height);
}

t_bw_img	*bw_img_alloc_twin(t_bw_img *img)
{
  return alloc_bw_img(img->width, img->height);
}


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

//UNCHECKED
void free_img(t_color_img* image)
{
  free(image); // UNCHECKED
}

void free_bw_img(t_bw_img* image)
{
  free(image); // UNCHECKED
}
