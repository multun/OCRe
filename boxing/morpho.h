#ifndef MORPHO_H_
#define MORPHO_H_

#include "../bitmap/img.h"
#include "../error.h"
#include "../tdefs.h"

t_bw_img *dilate(t_bw_img *input);

t_bw_img *erode(t_bw_img *input);

uint pix_diff(t_bw_img *input1, t_bw_img *input2);

void close(t_bw_img *img);

void open(t_bw_img *img);

void xor(t_bw_img *output, t_bw_img *input);

void border(t_bw_img *img);

#endif
