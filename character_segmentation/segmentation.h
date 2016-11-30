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

  int label;
} t_shape;

DECL_NAMED_VECTOR(t_shape*, shape);

void taint(t_sub_bw_img *img, uint x, uint y, int label, int mat[][]);
void update_shape(t_shape *shapes, uint x, uint y);
void add_shape(t_sub_bw_img *img,uint x,uint y,t_shape_vect *shapes,int mat[][]);

t_sub_bw_img_vect *char_segmentation(t_sub_bw_img *img);

#endif
