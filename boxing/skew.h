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

#endif
