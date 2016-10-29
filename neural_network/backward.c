#include "neurons.h"
#include "../error.h"

double compute_error(const t_network *net, const double *target)
{
  t_layer *o_layer = net->layers + (net->layers_count - 1);
  double *output = o_layer->out;
  double error = 0.0;
  for(size_t i = 0; i < o_layer->size; i++)
  {
    double er = target[i] - output[i];
    o_layer->delta[i] = er;// * o_layer->class->activation_d(o_layer->out[i],
//							  o_layer->in[i]);
    error += er;//0.5 * er * er;
  }
  return error;
}

static inline void backward_layer(const t_layer *layer)
{
  const t_layer *n_layer = layer + 1;
  const size_t segment_size = LAYER_NEURON_WSIZE(layer);
  double *weights   = layer->weights;
  double *weights_d = layer->weights_delta;

  //printf("updating deltas on %p\n", layer->delta);
  for(size_t i = 0; i < layer->size; i++)
  {
    double sum = 0.0;
    for(size_t j = 0; j < n_layer->size; j++)
    {
      double wdelta = weights[j] * n_layer->delta[j];
      //double wdelta = n_layer->delta[j] * layer->out[i];
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
  net->backprop_count += 1;
  for(int i = (int)net->layers_count - 2; i >= 0; i--)
  {
    //printf("backward on layer %d\n", i);
    backward_layer(layers + i);
  }
}
