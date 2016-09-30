#include <stdlib.h>
#include "binarise.h"
#include "../error.h"
#include "histogram.h"
#include "bin_treshold.h"
#include "../bitmap/bmp.h"

t_bw_img *bin_median(const t_color_img *orig_img)
{
  t_histogram hist;
  t_bw_img *bw_img = greyscale(intensity, orig_img);
  fill_histogram(bw_img, &hist);
  unsigned char median = (unsigned char)histogram_median(&hist);
  bw_img_treshold(median, bw_img);
  return bw_img;
}
