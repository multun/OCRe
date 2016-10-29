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
  {{-0.5, -0.5}, -0.5},
  {{-0.5,  0.5},  0.5},
  {{ 0.5, -0.5},  0.5},
  {{ 0.5,  0.5}, -0.5},
};

double weights0[] =
{
  0.7, // 00 -> 10
  0.1, // 00 -> 11
  0.2, // 01 -> 10
  0.3, // 01 -> 11
};

double weights1[] =
{
  0.4, // 10 -> 20
  -0.5, // 11 -> 20
};


// from http://stackoverflow.com/questions/6127503/shuffle-array-in-c
static void shuffle(void *array, size_t n, size_t size) {
  char tmp[size];
  char *arr = array;
  size_t stride = size * sizeof(char);

  if (n > 1) {
    size_t i;
    for (i = 0; i < n - 1; ++i) {
      size_t rnd = (size_t) rand();
      size_t j = i + rnd / (RAND_MAX / (n - i) + 1);

      memcpy(tmp, arr + j * stride, size);
      memcpy(arr + j * stride, arr + i * stride, size);
      memcpy(arr + i * stride, tmp, size);
    }
  }
}

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
	.size = 20,
	.class = sigmoid,
      },
      (t_layer){
	.size = 1,
	.class = sigmoid,
      }
    }
  };
  srand(42);
  alloc_network(&net);
  //random_weights(&net);
  net.layers[0].weights = weights0;
  net.layers[1].weights = weights1;

  const size_t test_count = sizeof(tests)/sizeof(tests[0]);
  puts("=> initial weights");
  print_weights(&net);

  size_t ratio = 0.1;
  for(size_t i = 0; i < 1; i++)
  {
    double error = 0.0;
    shuffle(tests, 4, sizeof(t_testcase));
    for(size_t test_i = 0; test_i < 1/*test_count*/; test_i++)
    {
      t_testcase *test = &tests[test_i];
      forward_init(&net, &test->inputs[0]);
      forward(&net);
      double er = compute_error(&net, &(double){test->result});
      printf("%f\n", er);
      error += 0.5*er*er;
      backward(&net);
      //print_weights(&net);
    }
    error /= (double)4;
    apply_delta(&net, ratio);

    //ratio *= 0.95;

    printf("error: %f\n", error);
    /*if (error < 0.25)
    {
      break;
      }*/
  }

  for(size_t test_i = 0; test_i < test_count; test_i++)
  {
    t_testcase *test = &tests[test_i];
    printf("inputs: %f %f\n", test->inputs[0], test->inputs[1]);
    forward_init(&net, &test->inputs[0]);
    forward(&net);
    printf("output: %f\n", *net.layers[2].out);
  }
}
