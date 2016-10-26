#include <math.h>
#include "../tdefs.h"
#include "../error.h"
#include "neurons.h"


double __attribute__((hot, const)) sigmoid_f(double in)
{
  return 1.0/(1.0 + exp(-in));
}

double __attribute__((hot, const)) sigmoid_fd(double in, double x)
{
  UNUSED(x);
  return in * (1.0 - in);
}


const t_nrn_cls sigmoid = {
    .activation	= sigmoid_f,
    .activation_d = sigmoid_fd,
};


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
