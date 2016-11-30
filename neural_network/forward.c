#include <stdio.h>
#include "neurons.h"

static inline nfloat forward_neuron(const t_layer *layer,
				    nfloat *weights,
				    nfloat *in)
{
  const t_layer *p_layer = layer - 1;
  const size_t weights_off = LAYER_NEURON_WSIZE(p_layer);

  *in = 0;//*weights;
  //weights += weights_off;

  for(size_t i = 0; i < p_layer->size ;i++, weights += weights_off)
    *in += p_layer->out[i] * *weights;

  return layer->class.activation(*in);
}

static inline void forward_layer(const t_layer *layer)
{
  const t_layer *p_layer = layer - 1;
  nfloat *weights = p_layer->weights;
  for(size_t i = 0; i < layer->size; i++)
    layer->out[i] = forward_neuron(layer,
				   &weights[i],
				   layer->in + i);
}


void print_layer(const t_layer *layer, size_t i)
{
    printf("layer %p(%lu):\n"
	   "\t->in %p\n"
	   "\t->out %p\n"
	   "\t->delta %p\n"
	   "\t->weights %p\n"
	   "\t->weights_delta %p\n",
	   (const void*)layer,
	   i,
	   (const void*)layer->in,
	   (const void*)layer->out,
	   (const void*)layer->delta,
	   (const void*)layer->weights,
	   (const void*)layer->weights_delta);
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
