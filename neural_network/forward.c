#include <stdio.h>
#include "neurons.h"

static inline nfloat forward_neuron(const t_layer *layer,
				    nfloat *weights,
				    nfloat *in)
{
  const t_layer *p_layer = layer - 1;
  *in = *(weights++);

  for(size_t i = 0; i < p_layer->size; i++)
    *in += p_layer->out[i] * weights[i];

  return layer->class.activation(*in);
}

static inline void forward_layer(const t_layer *layer)
{
  const size_t seg_size = (layer - 1)->size + 1;
  for(size_t i = 0; i < layer->size; i++)
    layer->out[i] = forward_neuron(layer,
				   layer->weights + i * seg_size,
				   layer->in + i);
}

void forward(t_network *net)
{
  for(size_t i = 1; i < net->layers_count; i++)
    forward_layer(net->layers + i);
}

void forward_init(const t_network *net, nfloat *inputs)
{
  t_layer *f_layer = &net->layers[0];
  nfloat *net_in = f_layer->out;
  for(size_t s = 0; s < f_layer->size; s++)
    *(net_in++) = *(inputs++);
}
