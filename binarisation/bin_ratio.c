#include <stdlib.h>
#include "binarise.h"
#include "../error.h"
#include "histogram.h"
#include "bin_treshold.h"
#include "../bitmap/img.h"
#include "../tdefs.h"
#include "bin_ratio.h"

t_bw_img *bin_ratio(const t_color_img *orig_img, void *options)
{
  t_histogram hist;
  t_bin_ratio_opts *opts = (t_bin_ratio_opts*)options;
  t_bw_img *bw_img = greyscale(intensity, orig_img);
  fill_histogram(bw_img, &hist);
  uchar median = (uchar)histogram_ratio(opts->hist_ratio, &hist);
  bw_img_treshold((uchar)((float)median * opts->post_ratio), bw_img);
  return bw_img;
}
