#ifndef NEURONS_H
#define NEURONS_H

#include "../base_structs/vector.h"

#define EXP expf
#define CF(val) val##f

typedef float nfloat;

typedef nfloat (*act_f)(nfloat);
typedef nfloat (*act_fd)(nfloat, nfloat);


typedef struct s_nrn_cls
{
  act_f		activation;
  act_fd	activation_d;
} t_nrn_cls;


typedef struct s_layer
{
  t_nrn_cls	class;
  size_t	size;
  nfloat	*in;
  nfloat	*out;
  nfloat	*delta;
  nfloat	*weights;
  nfloat	*weights_delta;
} t_layer;

#define LAYER_NEURON_WSIZE(layer) (((layer) - 1)->size + 1)
#define LAYER_WSIZE(layer) ((layer)->size * (LAYER_NEURON_WSIZE(layer)))
#define WEIGHT(w, l, i, j) (*((w) + (j) * ((l)->size + 1) + (i) + 1))
#define BIAS(w, l, j) (*((w) + (j) * ((l)->size + 1)))

#define NET_IN(net) ((net)->layers[0].out)
#define NET_OUT(net) ((net)->layers[(net)->layers_count - 1].out)

typedef struct s_network
{
  const char	*name;
  size_t	backprop_count;
  size_t	layers_count;
  t_layer	*layers;
} t_network;

extern const t_nrn_cls sigmoid;
extern const t_nrn_cls identity;

void random_weights(t_network *net,
		    nfloat min, nfloat max,
		    nfloat bmin, nfloat bmax);

void apply_delta(t_network *net, nfloat ratio);
void print_weights_deltas(t_network *net);
void layer_print_weights(t_layer *layer);
void print_weights(t_network *net);
void print_nfloat_array(nfloat *ar, size_t size);
void print_net(t_network *net);

void fill_constant(nfloat *array, size_t size, nfloat constant);


#endif
