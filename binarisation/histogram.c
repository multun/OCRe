#include <string.h>
#include "../error.h"
#include "../libbitmap/img.h"
#include "histogram.h"

void fill_histogram(t_bw_img *image, t_histogram *hist)
{
  hist->count = image->width * image->height;
  memset(hist, 0, sizeof(unsigned int) * 256); //UNCHECKED
  for(unsigned int i = 0; i < image->width * image->height; i++)
    hist->cells[image->pixels[i]]++;
}

// TODO: do a sister function with quartiles / ratios
unsigned int histogram_median(const t_histogram *hist)
{
  unsigned int ncount = 0;
  for (unsigned int i = 0; i < sizeof(hist->cells); i++)
  {
    ncount += hist->cells[i];
    if (ncount > hist->count / 2)
      return i;
  }
  FAIL0("invalid histogram");
}

unsigned int histogram_average(const t_histogram *hist)
{
  unsigned int total = 0;
  for (unsigned int i = 0; i < sizeof(hist->cells); i++)
    total += hist->cells[i] * i;
  return total / hist->count;
}
