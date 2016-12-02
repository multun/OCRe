#ifndef NOISE_REDUCTION_H
#define NOISE_REDUCTION_H

#include <stdlib.h>

#include "../bitmap/img.h"

size_t ibw(size_t a, size_t b, size_t c);
t_bw_img* reduce_noise(t_bw_img *img);

#endif
