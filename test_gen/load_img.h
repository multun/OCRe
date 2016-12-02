#ifndef SDL_LOAD_IMG_H
#define SDL_LOAD_IMG_H

#include <SDL2/SDL.h>
#include "../bitmap/img.h"

typedef union u_color_pix_mask
{
  t_color_pix	pix;
  Uint32	mask;
} t_color_pix_mask;

SDL_Surface *load_color_img(t_color_img *img);
SDL_Surface *load_bw_img(t_bw_img *img);

#endif
