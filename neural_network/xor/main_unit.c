#include "../neurons.h"
#include "../forward.h"
#include "../backward.h"
#include "../builder.h"

typedef struct s_testcase
{
  double	inputs[2];
  double	result;
} t_testcase;

t_testcase tests[] =
{
  {{-1., -1.}, -1.},
  {{-1.,  1.},  1.},
  {{ 1., -1.},  1.},
  {{ 1.,  1.}, -1.},
};

double weights0[] =
{
  //  0.35,
  0.1, // 00 -> 10
  0.4, // 01 -> 10

//  0.35,
  0.8, // 00 -> 11
  0.6, // 01 -> 11
};

double weights1[] =
{
//  0.6,
  0.3, // 10 -> 20
  0.9, // 10 -> 21


//  0.6,
  0.50, // 11 -> 20
  0.55, // 11 -> 21
};

int main()
{
  t_network net = {
    .layers_count = 3,
    .layers = (t_layer*)&(t_layer[]) {
      (t_layer){
	.size = 2,
	.class = identity,
      },
      (t_layer){
	.size = 2,
	.class = sigmoid,
      },
      (t_layer){
	.size = 1,
	.class = sigmoid,
      }
    }
  };

  alloc_network(&net);
  net.layers[0].weights = weights0;
  net.layers[1].weights = weights1;

  puts("=> initial weights");
  print_weights(&net);

  forward_init(&net, (double[]){0.35,0.9});
  forward(&net);
  double target = 0.5;
  double er = compute_error(&net, &target);
  printf("error: %f\n", er);
  backward(&net);
  print_weights_deltas(&net);
  apply_delta(&net, 1.);


  forward_init(&net, (double[]){0.35,0.9});
  forward(&net);
  compute_error(&net, &target);

}
