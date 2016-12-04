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
#include "charset.c"
#endif

#include <stdio.h>
#include <stdbool.h>
#include "charset.h"
#include "network.h"

#define ERROR_TRESHOLD CF(0.01)
#define LEARNING_RATE  CF(0.006)
#define EVAL_SIZE 100

static nfloat run_net(t_network *net, t_training_case *cas, bool is_correct)
{
  nfloat eres = (is_correct) ? CF(1.0) : CF(0.0);
  nfloat ret = cycle(net, &AT(cas->data, 0, 0), &eres);
  return ret;
}


nfloat evaluate(t_network *net, t_training_set *set, t_training_cat *cat)
{
  nfloat err = CF(0.0);
  for(uint i = 0; i < EVAL_SIZE; i++)
    err += run_net(net, get_case(cat, set, !(i < 50)), i < 50);
  return err / (nfloat)EVAL_SIZE;
}


// only meant to demangle the == padding of the base64 IDs
static void demangle_name(char *str)
{
  char *bi = str, *ei = str;
  for(; *ei; ei++, bi++)
    if(*ei == '\\')
    {
      *bi='=';
      if(*(ei+1) != 'x' || !*(ei+2))
	 FAIL0("not a systemd mangled name");
      ei+=3;
    }
    else
      *bi = *ei;
  *bi='\0';
}

int main(int argc, char *argv[])
{
  if(argc < 3)
    FAIL("usage: %s [char] [training_set]", argv[0]);
  char *name = argv[1];

  t_training_set *set = get_training_set(argv[2],
					 cats_bindings_size,
					 cats_bindings);

  t_network net = NETWORK(name);

  demangle_name(name);
  t_training_cat *cat = find_cat(set, name, false);
  if(!cat)
    FAIL("cannot find category `%s`", name);

  setup_signal();
  srand(42);
  if(load_network(&net, true))
    random_weights(&net, -1., 1., -1.5, 1.5);

  size_t epoch = 0;
  for(; running; epoch++)
  {
    nfloat err = CF(0.0);

    for(uint i = 0; i < 100; i++)
    {
      err += run_net(&net, get_case(cat, set, !!(i % 2)), (i % 2));
      apply_delta(&net, LEARNING_RATE);
    }

    if(epoch % 400 == 0)
      fprintf(stderr, "err: %f\n", err / 100);
  }
  fprintf(stderr, "trained for %lu cycles.\n", epoch);
  free_network(&net);
}
