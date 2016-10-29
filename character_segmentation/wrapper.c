#include "../bitmap/img.h"
#include "../gtk/render.h"
#include "../boxing/bounding_box.h"
#include "../boxing/morpho.h"
#include "../base_structs/vector.h"
#include "../gtk/vectors.h"
#include "segmentation.h"

t_sub_bw_img_vect_vect *line_segment(t_sub_bw_img_vect *input_lines)
{
  t_sub_bw_img_vect_vect *rv = VECT_ALLOC(sub_bw_img_vect, 16);
  for (unsigned int i = 0; i < VECT_GET_SIZE(input_lines); i++)
  {
    t_sub_bw_img_vect *seg = segmentation(VECT_GET(input_lines, i));
    for (unsigned int j = 0; j < VECT_GET_SIZE(seg); j++)
    {
      t_sub_bw_img *ch = VECT_GET(seg, j);
      printf("f: %p\n"
	     "w: %u\n"
	     "h: %u\n"
	     "ox: %u\n"
	     "oy: %u\n",
	     (void*)ch->father,
	     ch->width,
	     ch->height,
	     ch->xoff,
	     ch->yoff);

    }
    VECT_PUSH(rv, seg);
  }
  return rv;
}
