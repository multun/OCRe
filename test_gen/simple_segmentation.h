#ifndef SEGMENT_H_
#define SEGMENT_H_

#include "../base_structs/vector.h"
#include "../bitmap/bmp.h"
#include "../bitmap/img.h"
#include "../tdefs.h"
#include "../error.h"
#include "../gtk/vectors.h"

char is_white_column(t_sub_bw_img *img, uint column);
char is_white_line(t_sub_bw_img *img, uint line);
void char_vseg(t_sub_bw_img *lsub);

t_sub_bw_img_vect *simple_char_segmentation(t_sub_bw_img *img);

#endif
