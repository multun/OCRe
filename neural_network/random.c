#include <stdlib.h>

double normalized_random(double mi, double ma)
{
  int rnd = rand();
  double scale = ma - mi;
  return mi + ((double)rnd * scale / (double)RAND_MAX);
}
