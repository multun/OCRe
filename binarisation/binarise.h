#ifndef BINARISE_H
# define BINARISE_H

#include "../bitmap/img.h"

typedef enum e_bintype
{
  FIXED   = 0,
  RATIO   = 1,
  SAUVOLA = 2,
//  WOLF,
} t_bintype;

typedef t_bw_img* (*t_binarisator)(const t_color_img *, void *);

t_bw_img *binarise(t_bintype, const t_color_img *, void *);
unsigned char intensity(t_color_pix pix);

#endif
