#ifndef HISTOGRAM_H
# define HISTOGRAM_H

#include <stdlib.h>
#include "../tdefs.h"

typedef struct s_histogram
{
  size_t count;
  size_t cells[256];
} t_histogram;

void fill_histogram(t_bw_img *image, t_histogram *hist);
size_t histogram_average(const t_histogram *hist);
size_t histogram_ratio(float ratio, const t_histogram *hist);
uchar hist_min(t_histogram *hist);

#endif
