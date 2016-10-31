#include "../bitmap/img.h"
#include "../gtk/render.h"
#include "../boxing/bounding_box.h"
#include "../boxing/morpho.h"
#include "../base_structs/vector.h"
#include "../gtk/vectors.h"
#include "segmentation.h"

t_sub_bw_img_vect_vect_vect *line_segment(t_sub_bw_img_vect_vect *input_blocks)
{
  t_sub_bw_img_vect_vect_vect *rrv = VECT_ALLOC(sub_bw_img_vect_vect,
						VECT_GET_SIZE(input_blocks));
  for (unsigned int b = 0; b < VECT_GET_SIZE(input_blocks); b++)
  {
    t_sub_bw_img_vect *input_lines = VECT_GET(input_blocks, b);
    t_sub_bw_img_vect_vect *rv = VECT_ALLOC(sub_bw_img_vect, 16);
    for (unsigned int i = 0; i < VECT_GET_SIZE(input_lines); i++)
    {
      t_sub_bw_img_vect *seg = segmentation(VECT_GET(input_lines, i));
      for (unsigned int j = 0; j < VECT_GET_SIZE(seg); j++)
	t_sub_bw_img *ch = VECT_GET(seg, j);
      VECT_PUSH(rv, seg);
    }
    VECT_PUSH(rrv, rv);
  }
  return rrv;
}
