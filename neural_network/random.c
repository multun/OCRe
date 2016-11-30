#include <stdlib.h>
#include "neurons.h"

nfloat normalized_random(nfloat mi, nfloat ma)
{
  int rnd = rand();
  nfloat scale = ma - mi;
  return mi + ((nfloat)rnd * scale / (nfloat)RAND_MAX);
}
