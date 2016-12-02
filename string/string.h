#ifndef STRING_H
#define STRING_H

#include "../base_structs/vector.h"

#include <stdlib.h>

DECL_NAMED_VECTOR(char, char);

typedef t_char_vect t_string;


t_string *alloc_string(const char *c_str);
void string_append(t_string *dst, const t_string *src);
void string_c_append(t_string *dst, const char *src);
size_t string_len(t_string *str);

#endif
