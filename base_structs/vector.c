#include <stdlib.h>
#include <string.h>
#include "vector.h"

int generic_vector_resize(size_t *size,
			  size_t nsize,
			  void **data,
			  size_t e_size)
{
  void *ndata;
  size_t alloc_size = nsize * e_size;
  if ((ndata = realloc(*data, alloc_size)) == NULL)
    {
      void *former = *data;
      if ((*data = malloc(alloc_size)) == NULL)
	return -1;
      else
      {
	memcpy(*data, former, *size * e_size);
	free(former);
      }
    }
  else
    *data = ndata;
  *size = nsize;
  return 0;
}

DECL_VECTOR(void);

void	*generic_vector_alloc(size_t size, size_t e_size)
{
  t_void_vect *ret;
  if ((ret = malloc(sizeof(t_void_vect))) == NULL
      || ((ret->data.gen = malloc(size * e_size)) == NULL))
  {
    if (ret == NULL)
      free(ret);
    return NULL;
  }
  ret->size = size;
  ret->i = 0;
  return (void*)ret;
}
