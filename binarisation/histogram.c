#include <string.h>
#include "../error.h"
#include "../bitmap/img.h"
#include "histogram.h"
#include "../tdefs.h"

void fill_histogram(t_bw_img *image, t_histogram *hist)
{
  //UNCHECKED
  memset(hist, 0, sizeof(t_histogram));
  hist->count = image->width * image->height;
  for(size_t i = 0; i < image->width * image->height; i++)
    hist->cells[image->pixels[i]]++;
}

size_t histogram_ratio(float ratio, const t_histogram *hist)
{
  size_t ncount = 0;
  size_t treshold = (size_t)((long double)hist->count * ratio);
  for (size_t i = 0; i < sizeof(hist->cells); i++)
  {
    ncount += hist->cells[i];
    if (ncount > treshold)
      return i;
  }
  FAIL0("invalid histogram");
}

size_t histogram_average(const t_histogram *hist)
{
  size_t total = 0;
  for (size_t i = 0; i < sizeof(hist->cells); i++)
    total += hist->cells[i] * i;
  return total / hist->count;
}

uchar hist_min(t_histogram *hist)
{
  return (uchar)histogram_ratio(0.1f, hist);

  /*
  for(size_t i = 0; i < 256; i++)
    if (hist->cells[i])
      return (uchar)i;
      return (uchar)255;*/
}
