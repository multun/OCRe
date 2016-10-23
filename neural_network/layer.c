#include "neurons.h"

static inline void forward_neuron(t_layer *layer,
		    double *weights,
		    double *in,
		    double *out)
{
  *in = weights[0]; // initialize with the bias
  t_layer *p_layer = layer - 1;
  double *p_layer_out= p_layer->out;
  for(size_t i = 0; i < p_layer->size ;i++)
    *in += p_layer_out[i] * weights[i+1];
  *out = layer->class.activation(*in);
}

static inline void forward_layer(t_layer *layer,
				 double *weights)
{
  t_layer *p_layer = layer - 1;
  size_t segment_size = p_layer->size + 1;
  for(size_t i = 0; i < layer->size; i++)
  {
    forward_neuron(layer,
		   weights,
		   (&layer->in[i]),
		   (&layer->out[i]));
    weights += segment_size;
  }
}

void forward(t_network *net)
{
  double *weights = net->weights;
  t_layer *layers = net->layers;
  size_t offset = net->lweights_size;
  for(size_t i = 1; i < net->layers_count; i++)
  {
    forward_layer(layers + i, weights);
    weights += offset;
  }
}

double get_net_error(t_network *net, double *target)
{
  t_layer *o_layer = net->layers + (net->layers_count - 1);
  double *output = o_layer->out;
  double error = 0.0;
  for(size_t i = 0; i < o_layer->size; i++)
  {
    double er = target[i] - output[i];
    error += 0.5 * er * er;
  }
  return error;
}
