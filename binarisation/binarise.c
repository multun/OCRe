#include "../error.h"
#include "../libbitmap/img.h"

#include "binarise.h"

#include "bin_fixed.h"
#include "bin_median.h"
#include "bin_wolf.h"

t_binarisator binarisators[] = {
  bin_fixed,
  bin_median,
  bin_wolf,
};

unsigned char intensity(t_img_pix pix)
{
  return (pix.r + pix.g + pix.b)/3;
}

t_bw_img *binarise(t_bintype bintype, const t_img *orig_img)
{
  if (bintype > sizeof(binarisators))
    FAIL0("no such binarisator");
  return binarisators[bintype](orig_img);
}