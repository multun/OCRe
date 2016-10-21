#include "bitmap/img.h"
#include "gtk/render.h"

t_l_color_img_vect *test_block_segmentation(t_color_img *block)
{
  t_l_color_img_vect *rv = VECT_ALLOC(l_color_img, 2);
  for (unsigned int i = 0; i < 2; i++)
  {
    t_l_color_img *linked = alloc_l_color_img(block,
					      i * 5,
					      i * 5,
					      4,
					      4);
    VECT_PUSH(rv, linked);
  }
  return rv;
}
