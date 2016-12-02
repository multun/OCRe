#include "neurons.h"
#include "../error.h"
#include <stdbool.h>

static inline nfloat neuron_delta(const t_layer *layer,
				  nfloat error,
				  nfloat in,
				  nfloat out)
{
  return error * layer->class.activation_d(out, in);
}

nfloat compute_error(const t_network *net, const nfloat *target)
{
  t_layer *o_l = net->layers + (net->layers_count - 1);
  nfloat *output = o_l->out;
  nfloat error = CF(0.0);
  for(size_t i = 0; i < o_l->size; i++)
  {
    nfloat er = target[i] - output[i];
    o_l->delta[i] = neuron_delta(o_l, er, o_l->in[i], o_l->out[i]);
    error += CF(0.5) * er * er;
  }
  return error;
}

static inline void backward_layer(const t_layer *l, bool is_first)
{
  const t_layer *nl = l + 1;
  nfloat *w   = nl->weights;
  nfloat *w_d = nl->weights_delta;

  for(size_t j = 0; j < nl->size; j++)
    BIAS(w_d, l, j) += nl->delta[j];

  for(size_t i = 0; i < l->size; i++)
  {
    nfloat sum = 0.0;
    for(size_t j = 0; j < nl->size; j++)
    {
      nfloat wdelta = nl->delta[j];
      WEIGHT(w_d, l, i, j) += wdelta * l->out[i];
      sum += wdelta * WEIGHT(w, l, i, j);
    }
    if(!is_first)
      l->delta[i] = neuron_delta(l, sum, l->in[i], l->out[i]);
  }
}

void backward(t_network *net)
{
  t_layer *layers = net->layers;
  net->backprop_count += 1;
  for(int i = (int)net->layers_count - 2; i >= 0; i--)
    backward_layer(layers + i, i == 0);
}
