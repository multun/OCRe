#include "../boxing/bounding_box.h"
#include "../boxing/morpho.h"
#include "../base_structs/vector.h"
#include "../gtk/vectors.h"
#include "wrapper.h"

t_sub_bw_img_vect_vect *detect_lines_wrapped(t_sub_bw_img_vect *blocks)
{
  t_sub_bw_img_vect_vect *rv = VECT_ALLOC(sub_bw_img_vect,
					  VECT_GET_SIZE(blocks));
  for(size_t i = 0; i < VECT_GET_SIZE(blocks); i++)
    VECT_PUSH(rv, detect_lines(VECT_GET(blocks, i)));
  return rv;
}
