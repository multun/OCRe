#include "neurons.h"

static inline double forward_neuron(t_layer *layer,
				    double *weights,
				    double *in)
{
  *in = weights[0]; // initialize with the bias
  t_layer *p_layer = layer - 1;
  double *p_layer_out= p_layer->out;
  for(size_t i = 0; i < p_layer->size ;i++)
    *in += p_layer_out[i] * weights[i+1];
  return layer->class.activation(*in);
}

static inline void forward_layer(t_layer *layer)
{
  t_layer *p_layer = layer - 1;
  double *weights = p_layer->weights;
  size_t segment_size = p_layer->size + 1;
  for(size_t i = 0; i < layer->size; i++)
  {
    layer->out[i] = forward_neuron(layer,
				   weights,
				   (&layer->in[i]));
    weights += segment_size;
  }
}

void forward(t_network *net)
{
  t_layer *layers = net->layers;
  for(size_t i = 1; i < net->layers_count; i++)
    forward_layer(layers + i);
}
