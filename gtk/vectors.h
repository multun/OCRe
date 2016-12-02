#ifndef GTK_VECTORS_H
#define GTK_VECTORS_H

#include "../base_structs/vector.h"
#include "../bitmap/img.h"

DECL_NAMED_VECTOR(t_l_color_img*, l_color_img);
DECL_NAMED_VECTOR(t_l_bw_img*, l_bw_img);

DECL_NAMED_VECTOR(t_sub_color_img*, sub_color_img);
DECL_NAMED_VECTOR(t_sub_bw_img*, sub_bw_img);
DECL_NAMED_VECTOR(t_sub_bw_img_vect*, sub_bw_img_vect);
DECL_NAMED_VECTOR(t_sub_bw_img_vect_vect*, sub_bw_img_vect_vect);

#endif
