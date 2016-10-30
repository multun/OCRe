
#include "neurons.h"

#include <stdio.h>

static inline double forward_neuron(const t_layer *layer,
				    double *weights,
				    double *in)
{
  *in = 0.0;//*(weights++); // initialize with the bias
  const t_layer *p_layer = layer - 1;
  double *p_layer_out= p_layer->out;
  for(size_t i = 0; i < p_layer->size ;i++)
  {
//    printf(" => neuron %lu\n", i);
    //printf("adding weight %f\n", *weights);
    //printf("previous layer output is %f\n", p_layer_out[i]);
    *in += p_layer_out[i] * *weights;
    //printf("adding %f * %f\n", p_layer_out[i], *weights);
    weights += LAYER_NEURON_WSIZE(p_layer);
  }

  //printf("neuron in: %f\n", *in);
  double out = layer->class.activation(*in);
  //printf("neuron out: %f\n", out);
  return out;
}

static inline void forward_layer(const t_layer *layer)
{
  const t_layer *p_layer = layer - 1;
  double *weights = p_layer->weights;
  //printf("p_layer: %p layer: %p. Input is %p\n", p_layer, layer, layer->in);
  for(size_t i = 0; i < layer->size; i++)
  {
//    printf(" => neuron %lu\n", i);
    layer->out[i] = forward_neuron(layer,
				   &weights[i],
				   layer->in + i);
  }
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
  t_layer *layers = net->layers;
  //print_layer(layers, 0);
  for(size_t i = 1; i < net->layers_count; i++)
  {
//    printf("=> layer %lu\n", i);
    t_layer *layer = layers + i;
    //print_layer(layer, i);
    forward_layer(layer);
  }
}

void forward_init(const t_network *net, double *inputs)
{
  t_layer *f_layer = &net->layers[0];
  double *net_in = f_layer->out;
  for(size_t s = 0; s < f_layer->size; s++)
    *(net_in++) = *(inputs++);
}
