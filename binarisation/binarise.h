#ifndef BINARISE_H
# define BINARISE_H

#include "../libbitmap/img.h"

typedef enum e_bintype
{
  FIXED,
  MEDIAN,
  WOLF,
} t_bintype;

typedef t_bw_img* (*t_binarisator)(const t_color_img *);

t_bw_img *binarise(t_bintype bintype, const t_color_img *orig_img);
unsigned char intensity(t_color_pix pix);

#endif
