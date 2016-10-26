#include "neurons.h"
#include "../error.h"

static inline void init_backward(t_network *net)
{
  UNUSED(net);
}


static inline void backward_layer(t_layer *layer)
{
  t_layer *n_layer = layer + 1;
  size_t segment_size = n_layer->size + 1;
  double *weights   = layer->weights;
  double *weights_d = layer->weights_delta;

  for(size_t i = 0; i < layer->size; i++)
  {
    double sum = 0.0;
    for(size_t j = 0; j < n_layer->size; j++)
    {
      double wdelta = weights[j] * n_layer->delta[j];
      weights_d[j] += wdelta;
      sum += wdelta;
    }

    layer->delta[i] = sum * layer->class.activation_d(layer->out[i],
						      layer->in[i]);

    weights   += segment_size;
    weights_d += segment_size;
  }
}

void backward(t_network *net)
{
  t_layer *layers = net->layers;
  init_backward(net);
  for(size_t i = net->layers_count - 1; i > 0; i--)
    backward_layer(layers + i);
}


double set_get_net_error(t_network *net, double *target)
{
  t_layer *o_layer = net->layers + (net->layers_count - 1);
  double *output = o_layer->out;
  double error = 0.0;
  for(size_t i = 0; i < o_layer->size; i++)
  {
    double er = target[i] - output[i];
    er = 0.5 * er * er;
    o_layer->delta[i] = er;
    error += er;
  }
  return error;
}
