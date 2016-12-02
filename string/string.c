#include "string.h"
#include <stdlib.h>
#include <string.h>

t_string *alloc_string(const char *c_str)
{
  const size_t len = strlen(c_str);
  t_string *res = VECT_ALLOC(char, len + 2);
  for(size_t i = 0; i < len + 1; i++)
    VECT_PUSH(res, c_str[i]);
  return res;
}


void string_append(t_string *dst, const t_string *src)
{
  VECT_POP(dst);
  for(size_t i = 0; i < VECT_GET_SIZE(src); i++)
    VECT_PUSH(dst, VECT_GET(src, i));
}


void string_c_append(t_string *dst, const char *c_str)
{
  VECT_POP(dst);
  for(size_t i = 0; i < strlen(c_str) + 1; i++)
    VECT_PUSH(dst, c_str[i]);
}

size_t string_len(t_string *str)
{
  return VECT_GET_SIZE(str) - 1;
}
