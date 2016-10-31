#ifndef NEURONS_H
#define NEURONS_H

#include "../base_structs/vector.h"

typedef double (*act_f)(double);
typedef double (*act_fd)(double, double);


typedef struct s_nrn_cls
{
  act_f		activation;
  act_fd	activation_d;
} t_nrn_cls;


typedef struct s_layer
{
  t_nrn_cls	class;
  size_t	size;
  double	*in;
  double	*out;
  double	*delta;
  double	*weights;
  double	*weights_delta;
} t_layer;

#define LAYER_NEURON_WSIZE(layer) (((layer) + 1)->size)
#define LAYER_WSIZE(layer) ((layer)->size * LAYER_NEURON_WSIZE(layer))

typedef struct s_network
{
  const char	*name;
  size_t	backprop_count;
  size_t	layers_count;
  t_layer	*layers;
} t_network;

extern const t_nrn_cls sigmoid;
extern const t_nrn_cls identity;

void random_weights(t_network *net);
void apply_delta(t_network *net, double ratio);
void print_weights_deltas(t_network *net);
void layer_print_weights(t_layer *layer);
void print_weights(t_network *net);
void print_double_array(double *ar, size_t size);
void print_net(t_network *net);


#endif
