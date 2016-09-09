#ifndef HISTOGRAM_H
# define HISTOGRAM_H

typedef struct s_histogram
{
  unsigned int count;
  unsigned int cells[256];
} t_histogram;

void fill_histogram(t_bw_img *image, t_histogram *hist);
unsigned int histogram_median(const t_histogram *hist);
unsigned int histogram_average(const t_histogram *hist);
unsigned int histogram_ratio(float ratio, const t_histogram *hist);

#endif
