#include "../boxing/bounding_box.h"
#include "../boxing/morpho.h"
#include "../base_structs/vector.h"
#include "../gtk/vectors.h"
#include "wrapper.h"

t_sub_bw_img_vect *line_extraction(t_sub_bw_img *img)
{
  return line_subdivision(img);
}
