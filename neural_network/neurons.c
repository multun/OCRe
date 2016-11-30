#include <math.h>
#include "../tdefs.h"
#include "../error.h"
#include "neurons.h"

#ifndef FUCKIT
#include "random.h"
#else
#include "random.c"
#endif


nfloat __attribute__((hot, const)) sigmoid_f(nfloat in)
{
  return CF(1.0)/(CF(1.0) + EXP(-in));
}

nfloat __attribute__((hot, const)) sigmoid_fd(nfloat out, nfloat x)
{
  UNUSED(x);
  return out * (CF(1.0) - out);
}


const t_nrn_cls sigmoid = {
    .activation	= sigmoid_f,
    .activation_d = sigmoid_fd,
};

nfloat __attribute__((hot, const)) identity_f(nfloat in)
{
  return in;
}

nfloat __attribute__((hot, const)) identity_fd(nfloat in, nfloat x)
{
  UNUSED(x);
  UNUSED(in);
  return 1.0;
}

const t_nrn_cls identity = {
    .activation	= identity_f,
    .activation_d = identity_fd,
};

void apply_delta(t_network *net, nfloat ratio)
{
  ratio *= 1/(nfloat)net->backprop_count;
  net->backprop_count = 0;
  for(size_t l = 0; l < net->layers_count - 1; l++)
  {
    t_layer *layer = &net->layers[l];
    for(size_t i = 0; i < LAYER_WSIZE(layer); i++)
    {
      layer->weights[i] += layer->weights_delta[i] * ratio;
      layer->weights_delta[i] = 0.0;
    }
  }
}

static void fill_random(nfloat *array, size_t size, nfloat min, nfloat max)
{
  for(size_t i = 0; i < size; i++)
    array[i] = normalized_random(min, max);
}

void fill_constant(nfloat *array, size_t size, nfloat constant)
{
  for(size_t i = 0; i < size; i++)
    array[i] = constant;
}

void random_weights(t_network *net, nfloat min, nfloat max)
{
  for(size_t layer_i = 0; layer_i < net->layers_count; layer_i++)
  {
    t_layer *layer = &net->layers[layer_i];
    fill_random(layer->delta, layer->size, min, max);
    if(layer->weights)
    {
      size_t weights_count = LAYER_WSIZE(layer);
      fill_random(layer->weights, weights_count, min, max);
      fill_constant(layer->weights_delta, weights_count, 0.0);
    }
  }
}



void print_weights_deltas(t_network *net)
{
  for(size_t layer_i = 0; layer_i < net->layers_count; layer_i++)
  {
    t_layer *layer = &net->layers[layer_i];
    if(layer->weights)
    {
      //size_t weights_count = LAYER_WSIZE(layer);
      size_t offset = (layer+1)->size;
      nfloat *weights = layer->weights_delta;
      printf("layer %lu's deltas\n", layer_i);
      for(size_t i = 0; i < layer->size; i++)
      {
	for(size_t con = 0; con < offset; con++)
	  printf("%3f ", weights[con]);
        puts("");
	weights += offset;
      }
    }
  }
}

void layer_print_weights(t_layer *layer)
{
  if(layer->weights)
  {
    //size_t weights_count = LAYER_WSIZE(layer);
    size_t offset = LAYER_NEURON_WSIZE(layer);
    nfloat *weights = layer->weights;
    for(size_t i = 0; i < layer->size; i++)
    {
      printf("%4f\n", *weights);
      for(size_t con = 1; con < offset; con++)
	printf("%4f ", weights[con]);
      puts("");
      weights += offset;
    }
  }
}

void print_weights(t_network *net)
{
  for(size_t layer_i = 0; layer_i < net->layers_count; layer_i++)
  {
    t_layer *layer = &net->layers[layer_i];
    layer_print_weights(layer);
  }
}

void print_nfloat_array(nfloat *ar, size_t size)
{
  for(size_t i = 0; i < size; i++)
    printf("%3f\t", ar[i]);
  puts("");
}


void print_net(t_network *net)
{
  for(size_t layer_i = 0; layer_i < net->layers_count; layer_i++)
  {
    t_layer *layer = &net->layers[layer_i];
    printf("######## layer %lu ########\n", layer_i);
    puts("=> IN");
    print_nfloat_array(layer->in, layer->size);
    puts("=> OUT");
    print_nfloat_array(layer->out, layer->size);
    print_weights(net);
  }
}
// static neural network example
/*
void lol(void)
{
  const t_network net = {
    .layers_count = 3,
    .layers = (t_layer*)&(t_layer[]) {
      (t_layer){
	.size = 2,
      },
      (t_layer){
	.size = 10,
	.class = sigmoid,
      },
      (t_layer){
	.size = 2,
	.class = sigmoid,
      }
    }
  };
  printf("%p\n", (const void*)&net);
}
*/
