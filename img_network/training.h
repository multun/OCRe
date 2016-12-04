#ifndef TRAINING_H
#define TRAINING_H

#include "../base_structs/vector.h"
#include "../bitmap/img.h"
#include "../neural_network/neurons.h"
#include <stdbool.h>

IMAGE_DECLARE(nfloat, nfloat_mat)
ALLOC_IMAGE_DECLARE(nfloat, nfloat_mat);

typedef struct s_training_case
{
  int fd;
  t_nfloat_mat *data;
} t_training_case;

DECL_NAMED_VECTOR(t_training_case*, training_case);

typedef struct s_training_cat
{
  char *name;
  char *dirname;
  t_training_case_vect *cases;
} t_training_cat;

DECL_NAMED_VECTOR(t_training_cat*, training_cat);

typedef struct s_training_set
{
  t_training_cat_vect *cats;
  size_t count;
} t_training_set;


typedef enum e_CAT_TYPE
{
  CAT_HIGH,
  CAT_REGULAR,
  CAT_LOW,
} t_CAT_TYPE;

typedef struct s_cat_binding
{
  char *dirname;
  char *truename;
  t_CAT_TYPE type;
} t_cat_binding;

t_training_case *get_training_case(char *path);
t_training_cat *get_training_cat(char *path, char *dirname, char *name);
t_training_set *get_training_set(char *path,
				 size_t cats_count,
				 t_cat_binding *bindings);
t_training_cat *find_cat(t_training_set *set,
			 const char *name,
			 bool is_plaintext);

#endif
