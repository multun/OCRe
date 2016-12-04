#ifndef SETS_H
#define SETS_H

#include "training.h"

typedef enum
{
  MATCH,
  INVERTED,
} t_case_request_type;

t_training_case *get_case(t_training_cat *cat,
			  t_training_set *set,
			  t_case_request_type type);

#endif
