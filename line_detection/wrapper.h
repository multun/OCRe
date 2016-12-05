#ifndef LINE_SUBDIVISION_H
#define LINE_SUBDIVISION_H

#include "line_detection.h"
#include "../bitmap/img.h"
#include "../gtk/render.h"
#include "../gtk/vectors.h"

t_sub_bw_img_vect_vect *detect_lines_wrapped(t_sub_bw_img_vect *img);

#endif
