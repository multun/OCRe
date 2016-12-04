#include <stdlib.h>

#include "sets.h"

t_training_case *get_case(t_training_cat *cat,
			  t_training_set *set,
			  t_case_request_type type)
{
  t_training_cat *chosen_cat;
  do
    chosen_cat = VECT_RAND(set->cats);
  while((!!(chosen_cat == cat))^type); // <3
  return VECT_RAND(chosen_cat->cases);
}
