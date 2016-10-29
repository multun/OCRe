#include <stdlib.h>

double normalized_random(void)
{
  int rnd = rand();
  return ((double)rnd / (double)RAND_MAX);
}
