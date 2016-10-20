#ifndef BIN_RATIO_H
# define BIN_RATIO_H

#include "../bitmap/img.h"

typedef struct s_bin_ratio_opts
{
  float	hist_ratio;
  float	post_ratio;
} t_bin_ratio_opts;

t_bw_img *bin_ratio(const t_color_img *orig_img, void *options);

#endif
