#ifndef NET_POOL_H
#define NET_POOL_H

#include "../base_structs/vector.h"
#include "../neural_network/neurons.h"

typedef struct s_w_network
{
  char *target;
  t_network *net;
} t_w_network;

DECL_NAMED_VECTOR(t_w_network*, w_network);
typedef t_w_network_vect t_net_pool;


#endif
