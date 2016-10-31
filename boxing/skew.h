#ifndef SKEW_H_
#define SKEW_H_

#include "../bitmap/img.h"
#include "../error.h"
#include "../tdefs.h"
#include <stdio.h>
#include "../base_structs/vector.h"
#include "bounding_box.h"
#include <math.h>

t_bw_img *rotate_img(t_bw_img *input, double angle);

t_box_vect *filter_box_list(t_box_vect *box_list, t_bw_img *input_img);

t_box_vect *get_rows(t_box_vect *box_list);

#endif
