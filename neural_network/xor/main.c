
#ifndef FUCKIT
#include "../neurons.h"
#include "../forward.h"
#include "../backward.h"
#include "../builder.h"
# else
#define _XOPEN_SOURCE 500
#define _POSIX_C_SOURCE 201610L

#include "../neurons.c"
#include "../builder.c"
#include "../forward.c"
#include "../backward.c"
#endif

#include <stdio.h>
#include <signal.h>
#include <assert.h>
#include "../../tdefs.h"

#define ERROR_TRESHOLD 0.001
#define LEARNING_RATE  0.05

typedef struct s_testcase
{
  double	inputs[2];
  double	result;
} t_testcase;


#define XOR_0 0.
#define XOR_1 1.

t_testcase tests[] =
{
  {{XOR_0, XOR_0}, XOR_0},
  {{XOR_0, XOR_1}, XOR_1},
  {{XOR_1, XOR_0}, XOR_1},
  {{XOR_1, XOR_1}, XOR_0},
};


void shuffle(uint *array, size_t n)
{
  if (n > 1)
    for (size_t i = 0; i < n - 1; i++)
    {
      size_t j = i + (size_t)rand() / (RAND_MAX / (n - i) + 1);
      uint t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
}

char running = 1;

static void catch_function(int signo)
{
  switch(signo)
  {
  case SIGTERM:
  case SIGINT:
    puts("caught signal");
    running = 0;
    break;
  default:
    raise(signo);
  }
}

int main(void)
{
  t_network net = {
    .name = "xor_data",
    .layers_count = 3,
    .layers = (t_layer*)&(t_layer[]) {
      (t_layer){
	.size = 2,
	.class = sigmoid,
      },
      (t_layer){
	.size = 10,
	.class = sigmoid,
      },
      (t_layer){
	.size = 1,
	.class = sigmoid,
      }
    }
  };

  assert(signal(SIGINT, catch_function) != SIG_ERR);

  srand(42);
  if(load_network(&net))
    random_weights(&net, -1., 1.);

  const size_t test_count = sizeof(tests)/sizeof(tests[0]);

  double learning_rate = LEARNING_RATE;
  uint indexes[4] = {0,1,2,3};
  for(size_t epoch = 0;;epoch++)
  {
    double error = 0.0;
    shuffle(indexes, 4);
    for(size_t test_i = 0; test_i < test_count; test_i++)
    {
      t_testcase *test = &tests[indexes[test_i]];
      forward_init(&net, &test->inputs[0]);
      forward(&net);
      double er = compute_error(&net, &(test->result));
      error += er;
      backward(&net);
      apply_delta(&net, learning_rate);
    }
    error /= (double)test_count;

    if(epoch % 10000 == 0)
      printf("%lu\t error: %f\n", epoch, error);

    if (!running)
      break;

    if (error < ERROR_TRESHOLD)
    {
#define STR(x) #x
#define XSTR(x) STR(x)
      printf("%lu\t error below " XSTR(ERROR_TRESHOLD)"\n", epoch);
      break;
    }
  }

  for(size_t test_i = 0; test_i < test_count; test_i++)
  {
    t_testcase *test = &tests[test_i];
    printf("inputs: %f %f\n", test->inputs[0], test->inputs[1]);
    forward_init(&net, &test->inputs[0]);
    forward(&net);
    printf("output: %f\n", *net.layers[2].out);
  }
  free_network(&net);
}
