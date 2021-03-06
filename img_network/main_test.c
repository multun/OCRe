#define _DEFAULT_SOURCE
#define _XOPEN_SOURCE 500
#define _POSIX_C_SOURCE 201610L

#include "neurons.h"
#include "sets.h"
#include "../tdefs.h"
#include "../error.h"
#ifndef FUCKIT
#include "training.h"
#include "../signal_running.h"
#else
#include "training.c"
#include "../signal_running.c"
#include "../base_structs/vector.c"
#include "sets.c"
#endif

#include <stdio.h>
#include <stdbool.h>

#define ERROR_TRESHOLD CF(0.01)
#define LEARNING_RATE  CF(0.006)
#define EVAL_SIZE 100

#include "network.h"

static nfloat run_net(t_network *net, t_training_case *cas, bool is_correct)
{
  nfloat eres = (is_correct) ? CF(1.0) : CF(0.0);
  return cycle(net, &AT(cas->data, 0, 0), &eres);
}

int main(int argc, char *argv[])
{
  if(argc < 3)
    FAIL("usage: %s [char] [case]", argv[0]);

  const char *name = argv[1];
  t_training_case *cas = get_training_case(argv[2]);

  t_network net = NETWORK(name);

  srand(42);
  if(load_network(&net, true))
    FAIL0("couldn't load the network");

  nfloat err = run_net(&net, cas, false);
  printf("output is %f\n", err);
  free_network(&net);
}
