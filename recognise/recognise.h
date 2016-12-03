#ifndef RECOGNISE_H
#define RECOGNISE_H

#include "../string/string.h"
#include "../neural_network/neurons.h"
#include "../gtk/vectors.h"

typedef t_sub_bw_img t_block;
typedef t_sub_bw_img t_line;
typedef t_l_bw_img t_char;

t_string *recognise_bw_img(t_network *net, t_bw_img *img);

#endif
