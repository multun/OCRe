#ifndef BIN_SAUVOLA_H
#define BIN_SAUVOLA_H

#include "../tdefs.h"
#include "../bitmap/img.h"

typedef struct s_bin_sauvola_opts
{
  uint	window;
  float	k;
} t_bin_sauvola_opts;

t_bw_img *bin_sauvola(const t_color_img *orig_img, void *options);

#endif
