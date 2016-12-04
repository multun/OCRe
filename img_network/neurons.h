#ifndef IMG_NEURONS_H
#define IMG_NEURONS_H
#ifndef FUCKIT
#include "../neural_network/neurons.h"
#include "../neural_network/forward.h"
#include "../neural_network/backward.h"
#include "../neural_network/builder.h"
#include "../neural_network/cycle.h"
# else
#undef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 201610L

#include "../neural_network/neurons.c"
#include "../neural_network/builder.c"
#include "../neural_network/forward.c"
#include "../neural_network/backward.c"
#include "../neural_network/cycle.c"
#endif
#endif
