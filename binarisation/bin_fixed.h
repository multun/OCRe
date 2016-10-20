#ifndef BIN_FIXED_H
# define BIN_FIXED_H

#include "../bitmap/img.h"

typedef unsigned char t_bin_fixed_opts;

t_bw_img *bin_fixed(const t_color_img *orig_img, void *options);

#endif
