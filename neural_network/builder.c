#include "neurons.h"
#include "../tdefs.h"
#include <stdbool.h>

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
