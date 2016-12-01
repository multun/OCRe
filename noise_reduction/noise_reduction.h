#ifndef NOISE_REDUCTION_H
#define NOISE_REDUCTION_H

#include <stdio.h>
#include "../bitmap/bmp.h"
#include "../bitmap/img.h"

int ibw(int a, int b, int c);
t_bw_img* reduce_noise(t_bw_img *img);
