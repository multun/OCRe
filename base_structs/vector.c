#include <stdlib.h>
#include <string.h>
#include "vector.h"

#ifdef VECTOR_DEBUG
#include <stdio.h>
#endif
int generic_vector_resize(size_t *size, size_t nsize, void **data)
{
  void *ndata;
#ifdef VECTOR_DEBUG
  printf("resizing from %lu to %lu\n", *size, nsize);
#endif
  if ((ndata = realloc(*data, nsize)) == NULL)
  {
    void *former = *data;
    if ((*data = malloc(nsize)) == NULL)
      return -1;
    else
    {
      memcpy(*data, former, *size);
      free(former);
    }
  }
  else
    *data = ndata;
  *size = nsize;
  return 0;
}

DECL_VECTOR(void);

void	*generic_vector_alloc(size_t size)
{
#ifdef VECTOR_DEBUG
  printf("allocating %lu\n", size);
#endif
  t_void_vect *ret;
  if ((ret = malloc(sizeof(t_void_vect))) == NULL
      || ((ret->data.gen = malloc(size)) == NULL))
  {
    if (ret == NULL)
      free(ret);
    return NULL;
  }
  ret->size = size;
  ret->i = 0;
  return (void*)ret;
}
