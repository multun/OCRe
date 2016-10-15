#include "../error.h"
#include "../bitmap/img.h"

#include "binarise.h"

#include "bin_fixed.h"
#include "bin_ratio.h"
#include "bin_wolf.h"
#include "bin_sauvola.h"

t_binarisator binarisators[] = {
  bin_fixed,
  bin_ratio,
  bin_wolf,
  bin_sauvola,
};

unsigned char intensity(t_color_pix pix)
{
  return (unsigned char)(((uint)pix.r + (uint)pix.g + (uint)pix.b)/3);
}

t_bw_img *binarise(t_bintype bintype, const t_color_img *orig_img, void *options)
{
  if (bintype > sizeof(binarisators))
    FAIL0("no such binarisator");
  return binarisators[bintype](orig_img, options);
}
