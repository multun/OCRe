#include "neurons.h"
#include "../error.h"

static inline double neuron_delta(const t_layer *layer,
				  double error,
				  double in,
				  double out)
{
  return error * layer->class.activation_d(out, in);
}

double compute_error(const t_network *net, const double *target)
{
  t_layer *o_layer = net->layers + (net->layers_count - 1);
  double *output = o_layer->out;
  double error = 0.0;
  for(size_t i = 0; i < o_layer->size; i++)
  {
    double er = target[i] - output[i];
#ifdef DEBUG
    printf("%f - %f = %f\n", target[i], output[i], er);
#endif
    o_layer->delta[i] = neuron_delta(o_layer,
				     er,
				     o_layer->in[i],
				     o_layer->out[i]);

#ifdef DEBUG
    printf("o_delta: %f\n", o_layer->delta[i]);
#endif
    error += 0.5 * er * er;
  }
  return error;
}



static inline void backward_layer(const t_layer *layer)
{
  const t_layer *n_layer = layer + 1;
  const size_t segment_size = LAYER_NEURON_WSIZE(layer);
  size_t woff = 0;
  double *weights   = layer->weights;
  double *weights_d = layer->weights_delta;

  for(size_t i = 0; i < layer->size; i++)
  {
    double sum = 0.0;
    for(size_t j = 0; j < n_layer->size; j++)
    {
      double wdelta = n_layer->delta[j];
      weights_d[woff + j] += wdelta * layer->out[i];
      sum += wdelta * weights[woff + j];
    }

    layer->delta[i] = neuron_delta(layer,
				   sum,
				   layer->in[i],
				   layer->out[i]);
    woff += segment_size;
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
