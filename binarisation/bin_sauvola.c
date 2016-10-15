#include <stdlib.h>
#include "binarise.h"
#include "../error.h"
#include "../bitmap/img.h"
#include "../tdefs.h"
#include "../stats.h"

#include "bin_sauvola.h"

int sauvola_treshold(
    t_bw_img *inpImg,
    uint x,
    uint y,
    uint window,
    float coef)
{
  // (k*(dev/128-1)+1) * mean
  int pix_dev = img_deviation(inpImg, x, y);
  double normalized_dev = coef * (((float)pix_dev / 128) - 1) + 1;
  uint mean = img_mean(inpImg, x, y, window);
  return (int)((double)mean * normalized_dev);
}

t_bw_img *bin_sauvola(const t_color_img *orig_img, void *options)
{
  t_bin_sauvola_opts *opts = (t_bin_sauvola_opts*)options;
  t_bw_img *bw_img = greyscale(intensity, orig_img);
  t_bw_img *ret_img = alloc_bw_img_twin(bw_img);
  float	k = opts->k;
  uint window = opts->window;

  for (uint y = 0; y < bw_img->height; y++)
    for (uint x = 0; x < bw_img->width; x++)
    {
      uchar val = AT(bw_img, x, y) < sauvola_treshold(bw_img, x, y, window, k);
      AT(ret_img, x, y) = (val == 0 ? 255: 0);
    }
  free_bw_img(bw_img);
  return ret_img;
}
