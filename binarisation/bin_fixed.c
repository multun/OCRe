#include <stdlib.h>
#include "binarise.h"
#include "bin_treshold.h"

t_bw_img *bin_fixed(const t_color_img *orig_img)
{
  return bin_treshold(127,orig_img);
}
