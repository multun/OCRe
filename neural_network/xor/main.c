#include "../neurons.h"
#include "../forward.h"
#include "../backward.h"
#include "../builder.h"
#include <stdio.h>


#define ERROR_TRESHOLD 0.001

#define STR(x) #x
#define XSTR(x) STR(x)

typedef struct s_testcase
{
  double	inputs[2];
  double	result;
} t_testcase;

t_testcase tests[] =
{
  {{0., 0.}, 0.},
  {{0., 1.}, 1.},
  {{ 1., 0.},  1.},
  {{ 1.,  1.}, 0.},
};

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
  srand(42);
  if(load_network(&net))
    random_weights(&net);

  const size_t test_count = sizeof(tests)/sizeof(tests[0]);

  double learning_rate = 0.06;
  for(size_t epoch = 0;;epoch++)
  {
    double error = 0.0;
    for(size_t test_i = 0; test_i < test_count; test_i++)
    {
      t_testcase *test = &tests[test_i];
      forward_init(&net, &test->inputs[0]);
      forward(&net);
      double er = compute_error(&net, &(test->result));
      error += er;
      backward(&net);
      apply_delta(&net, learning_rate);
    }
    error /= (double)test_count;

    if(epoch % 1000 == 0)
      printf("%lu\t error: %f\n", epoch, error);
    if (error < ERROR_TRESHOLD)
    {
      puts("error below " XSTR(ERROR_TRESHOLD));
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
