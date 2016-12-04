#ifndef FUCKIT
#include "neurons.h"
#include "forward.h"
#include "backward.h"
#include "builder.h"
#endif

#include "cycle.h"

nfloat cycle(t_network *net, nfloat *inputs, nfloat *expected)
{
  forward_init(net, inputs);
  forward(net);
  nfloat er = compute_error(net, expected);
  backward(net);
  return er;
}
