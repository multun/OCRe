#ifndef SEGMENT_H_
#define SEGMENT_H_

#include "../base_structs/vector.h"
#include "../bitmap/bmp.h"
#include "../bitmap/img.h"
#include "../tdefs.h"
#include "../error.h"
#include "../gtk/vectors.h"

typedef struct s_shape

{
  uint Xmin;
  uint Ymin;
  uint Xmax;
  uint Ymax;
  uint size;
} t_shape;

DECL_NAMED_VECTOR(t_shape*, shape);

t_sub_bw_img_vect *char_segmentation(t_sub_bw_img *img);

#endif
