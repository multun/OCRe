#include "neurons.h"
#include "../error.h"

static inline nfloat neuron_delta(const t_layer *layer,
				  nfloat error,
				  nfloat in,
				  nfloat out)
{
  return error * layer->class.activation_d(out, in);
}

nfloat compute_error(const t_network *net, const nfloat *target)
{
  t_layer *o_layer = net->layers + (net->layers_count - 1);
  nfloat *output = o_layer->out;
  nfloat error = 0.0;
  for(size_t i = 0; i < o_layer->size; i++)
  {
    nfloat er = target[i] - output[i];
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
    error += CF(0.5) * er * er;
  }
  return error;
}



static inline void backward_layer(const t_layer *layer)
{
  const t_layer *n_layer = layer + 1;
  const size_t segment_size = LAYER_NEURON_WSIZE(layer);
  size_t woff = 0;
  nfloat *weights   = layer->weights;
  nfloat *weights_d = layer->weights_delta;

  for(size_t i = 0; i < layer->size; i++)
  {
    nfloat sum = 0.0;
    for(size_t j = 0; j < n_layer->size; j++)
    {
      nfloat wdelta = n_layer->delta[j];
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
