#include <stdlib.h>
#include "img.h"

#define ALLOC_IMAGE_DEFINE(TYPE)					\
  ALLOC_IMAGE_DECLARE(TYPE)						\
  {									\
    t_ ## TYPE *ret	= malloc(					\
      sizeof(t_ ## TYPE ) + sizeof(ret->pixels[0]) * width * height);	\
    ret->width	= width;						\
    ret->height	= height;						\
    return ret;								\
  }

#define FREE_IMAGE_DEFINE(TYPE)						\
  FREE_IMAGE_DECLARE(TYPE)						\
  {									\
    free(img);								\
  }

#define ALLOC_IMAGE_TWIN_DEFINE(TYPE)					\
  ALLOC_IMAGE_TWIN_DECLARE(TYPE)					\
  {									\
    return alloc_ ## TYPE (img->width, img->height);			\
  }

#define DEFINE_IMG_TOOLS(TYPE)			\
  ALLOC_IMAGE_DEFINE(TYPE)			\
  FREE_IMAGE_DEFINE(TYPE)			\
  ALLOC_IMAGE_TWIN_DEFINE(TYPE)			\

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
