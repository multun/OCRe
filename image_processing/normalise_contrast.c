#include "normalise_contrast.h"
#include "../binarisation/histogram.h"
#include "../math_macros.h"

static void adjust_contrast_sub(t_bw_img *img)
{
  t_histogram hist;
  fill_histogram(img, &hist);
  uchar min = hist_min(&hist);

  if(min == 0 || min == 255)
    return;

  float ns = (float)(255-min);
  for(uint y = 0; y < img->height; y++)
    for(uint x = 0; x < img->width; x++)
      AT(img, x, y) = (uchar)(((float)(MAX(0,AT(img, x, y) - min)) / ns) * 255);
}

void adjust_contrast(t_bw_img *img)
{
  adjust_contrast_sub(img);
  invert(img);
  adjust_contrast_sub(img);
  invert(img);
}


void invert(t_bw_img *img)
{
  for(uint y = 0; y < img->height; y++)
    for(uint x = 0; x < img->width; x++)
      AT(img, x, y) = (t_bw_pix)(255 - AT(img, x, y));
}
