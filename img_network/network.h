#ifndef NETWORK_H
#define NETWORK_H

#include "../tdefs.h"

static const uint img_width =
#include "img_width"
  ;


#define NETWORK(NAME)				\
  {						\
  .name = NAME,					\
    .layers_count = 3,				\
    .layers = (t_layer*)&(t_layer[]) {		\
      (t_layer){				\
	.size = img_width * img_width,		\
	.class = sigmoid,			\
      },					\
      (t_layer){				\
	.size = 30,				\
	.class = sigmoid,			\
      },					\
      (t_layer){				\
	.size = 1,				\
	.class = sigmoid,			\
      }						\
    }						\
  }

#endif
