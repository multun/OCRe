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
} t_layer;

//DECL_NAMED_VECTOR(t_layer*, layer);

typedef struct s_network
{
  size_t	lweights_size;
  size_t	layers_count;
  double	*weights;
  t_layer	*layers;
} t_network;


#endif
