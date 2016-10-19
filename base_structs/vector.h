#ifndef VECTOR_H
#define VECTOR_H
#include <stdlib.h>

#define VECT_NAME(PREFIX, NAME) PREFIX ## _ ## NAME ## _vect

#define DECL_VECTOR(CHILD_TYPE)				\
  DECL_NAMED_VECTOR(CHILD_TYPE, CHILD_TYPE)

#define DECL_NAMED_VECTOR(CHILD_TYPE, NAME)		\
  typedef struct VECT_NAME(s, NAME)			\
  {							\
    size_t	size, i;				\
    union						\
    {							\
      CHILD_TYPE	*spec;				\
      void		*gen;				\
    } data;						\
  } VECT_NAME(t, NAME)


int	generic_vector_resize(size_t *size, size_t nsize, void **data);
void	*generic_vector_alloc(size_t size);

#define VECT_RESIZE_ABS(VECT, SIZE)					\
  ((SIZE > VECT->size)							\
   ? generic_vector_resize(&(VECT->size), SIZE, &(VECT->data.gen))	\
   : 0)
#define VECT_EXPAND(VECT)						\
  VECT_RESIZE_ABS(VECT, (VECT->size) * 2)
#define VECT_PUSH(VECT, VAL)					\
  (((++(VECT->i) < VECT->size) ? (VECT_EXPAND(VECT)) : 0)	\
  || (VECT_GET(VECT, VECT->i) = VAL, 0));

#define VECT_RESIZE(VECT, SIZE)					\
  VECT_RESIZE_ABS(VECT, SIZE * sizeof(*(VECT->data.spec)))

#define VECT_GET(VECT, INDEX) (VECT->data.spec[INDEX])
#define VECT_GET_LAST(VECT) VECT_GET(VECT, VECT->i)
#define VECT_GETP(VECT, INDEX) (&(VECT_GET(VECT, INDEX)))
#define VECT_ALLOC(TYPE, SIZE)						\
  (VECT_NAME(t, TYPE)*)generic_vector_alloc(				\
    SIZE * sizeof(((VECT_NAME(t, TYPE)*)0)->data.spec))
#define VECT_GET_SIZE(VECT) (VECT->size / sizeof(*(VECT->data.spec)))
#define VECT_FREE(VECT) (free(VECT->data.gen), free(VECT))

#endif
