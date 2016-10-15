#include <stdlib.h>
#include "binarise.h"
#include "bin_treshold.h"
#include "bin_fixed.h"

t_bw_img *bin_fixed(const t_color_img *orig_img, void *options)
{
  t_bin_fixed_opts *treshold = (t_bin_fixed_opts*)options;
  return bin_treshold(*treshold,orig_img);
}
