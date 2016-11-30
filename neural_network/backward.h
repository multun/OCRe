#ifndef NEURAL_BACKWARD_H
#define NEURAL_BACKWARD_H

#include "neurons.h"

void backward(t_network *net);
nfloat compute_error(t_network *net, nfloat *target);

#endif
