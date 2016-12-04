#ifndef RECOGNISE_H
#define RECOGNISE_H

#include "../string/string.h"
#include "../neural_network/neurons.h"
#include "../gtk/vectors.h"
#include "char_recon.h"

typedef t_sub_bw_img t_block;
typedef t_sub_bw_img t_line;
typedef t_l_bw_img t_char;

t_string *recognise_bw_img(t_w_network_vect *context, t_bw_img *img);

#endif
