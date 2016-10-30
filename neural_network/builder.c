#include "neurons.h"
#include "../tdefs.h"
#include <stdbool.h>

static void fill_constant(double *array, size_t size, double constant)
{
  for(size_t i = 0; i < size; i++)
    array[i] = constant;
}

static inline void alloc_layer(t_layer *layer, bool has_weights)
{
  size_t layer_size = sizeof(*layer->out) * layer->size;
  layer->in	= malloc(layer_size);

  layer->out	= malloc(layer_size);
  layer->delta	= malloc(layer_size);
  if (has_weights)
  {
    size_t w_size = sizeof(double) * LAYER_WSIZE(layer);
    layer->weights	 = malloc(w_size);
    layer->weights_delta = malloc(w_size);
    fill_constant(layer->weights_delta, LAYER_WSIZE(layer), 0.0);
  }
  else
  {
    layer->weights	 = NULL;
    layer->weights_delta = NULL;
  }
}

void alloc_network(const t_network *net)
{
  for(size_t i = 0; i < net->layers_count; i++)
    alloc_layer(&net->layers[i], i < (net->layers_count - 1));
}

static inline void free_layer(t_layer *layer, bool has_weights)
{
  free(layer->in);
  free(layer->out);
  free(layer->delta);
  if (has_weights)
  {
    free(layer->weights);
    free(layer->weights_delta);
  }
}

void free_network(const t_network *net)
{
  for(size_t i = 0; i < net->layers_count; i++)
    free_layer(&net->layers[i], i < (net->layers_count - 1));
}
