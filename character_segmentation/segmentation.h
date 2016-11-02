#ifndef SEGMENT_H_
#define SEGMENT_H_

#include "../base_structs/vector.h"
#include "../bitmap/bmp.h"
#include "../bitmap/img.h"
#include "../tdefs.h"
#include "../error.h"
#include "../gtk/vectors.h"

char isWhiteColumn (t_sub_bw_img *img, uint column);
t_sub_bw_img_vect *char_segmentation(t_sub_bw_img *img);
t_sub_bw_img *imgToSmallerImg(t_sub_bw_img *img,
			      int firstColumn,
			      int lastColumn);

#endif
