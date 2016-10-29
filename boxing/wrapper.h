#ifndef TEST_L_IMGS_H
#define TEST_L_IMGS_H

#include "../bitmap/img.h"
#include "../gtk/render.h"

t_sub_bw_img_vect *block_segment(t_bw_img *input_img);

t_sub_bw_img_vect *temp_line_segment(t_bw_img *input_img);
#endif
